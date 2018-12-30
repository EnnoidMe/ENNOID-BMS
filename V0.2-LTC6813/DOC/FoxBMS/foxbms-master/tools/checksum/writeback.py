#!/usr/bin/env python
# -*- coding: utf-8 -*-

# @copyright &copy; 2010 - 2018, Fraunhofer-Gesellschaft zur Foerderung der
#   angewandten Forschung e.V. All rights reserved.
#
# BSD 3-Clause License
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1.  Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
# 3.  Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived from this
#     software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# We kindly request you to use one or more of the following phrases to refer to
# foxBMS in your hardware, software, documentation or advertising materials:
#
# &Prime;This product uses parts of foxBMS&reg;&Prime;
#
# &Prime;This product includes parts of foxBMS&reg;&Prime;
#
# &Prime;This product is derived from foxBMS&reg;&Prime;

"""
@file       writeback.py
@date       30.11.2017 (date of creation)
@author     foxBMS Team
@ingroup    tools
@prefix     none
@brief      Writing a checksum back into a binary file
"""

import sys
import os
import argparse
import logging
import subprocess
import struct
import yaml
import datetime

__version__ = 0.2
__date__ = '2017-11-30'
__updated__ = '2018-08-10'


def main():
    program_name = os.path.basename(sys.argv[0])
    program_version = '{}'.format(__version__)
    program_build_date = str(__updated__)
    program_version_message = '{} {}'.format(
        program_version, program_build_date)
    program_shortdesc = __import__('__main__').__doc__.split('\n')[1]
    program_license = '''{}
    {}

    Created by the foxBMS Team on {}.
    Copyright 2017 foxBMS. All rights reserved.

    Licensed under the BSD 3-Clause License.

    Distributed on an "AS IS" basis without warranties
    or conditions of any kind, either express or implied.

    Reads the ouput file of the checksum tool, and writes the \
    checksum back into the specified elf file using the program passed by
    command line argument.

USAGE
'''.format(program_name, program_shortdesc, str(__date__))

    parser = argparse.ArgumentParser(
        description=program_license,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        '-v',
        '--verbosity',
        dest='verbosity',
        action='count',
        default=0,
        help='set verbosity level')
    parser.add_argument(
        '-V',
        '--version',
        action='version',
        version=program_version_message)

    parser.add_argument('-c', '--conffile', help='directory where the \
checksum ouput is stored')
    parser.add_argument('-e', '--elffile', help='elffile the checksum should \
be written into')
    parser.add_argument('-t', '--tool', default='arm-none-eabi-objdump',
                        help='tool used to write in the elf file')

    args = parser.parse_args()

    if args.verbosity == 1:
        logging.basicConfig(level=logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.ERROR)

    conffile = args.conffile
    elffile = args.elffile
    tool = args.tool

    checksum_struct_name = "ver_sw_validation"
    checksum_position_in_struct = 0x10
    date_position_in_struct = 0xA0
    time_position_in_struct = 0xAC

    for _fr in [conffile, elffile, tool]:
        if not os.path.isfile(_fr):
            raise RuntimeError('{} is missing'.format(_fr))

    with open(conffile, 'r') as fcs_conf:
        try:
            checksum_data = yaml.load(fcs_conf)
        except yaml.YAMLError as exc:
            print(exc)
            sys.exit(1)
    checksum = hex(checksum_data['checksum'])
    if checksum.endswith('L'):
        checksum = checksum[:-1]
    print('\nWriting checksum {} into {}'.format(checksum, elffile))

    # Get information on .flashheader section in ELF file
    cmd = [tool, '--section=.flashheader', '-h', elffile]
    print(cmd)
    proc_write_to_elf = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE)
    std_out, std_err = proc_write_to_elf.communicate()
    std_out = std_out.decode('UTF-8')
    std_err = std_err.decode('UTF-8')
    rtn_code = proc_write_to_elf.returncode

    if std_err:
        print(std_err)
    if rtn_code != 0:
        sys.exit(rtn_code)

    sectionAttributes = dict(zip(std_out.splitlines()[4].split(),
                             std_out.splitlines()[5].split()))

    # Get location of struct ver_sw_validation in .flasheader section
    cmd = [tool, '--section=.flashheader', '-t', elffile]
    print(cmd)
    proc_write_to_elf = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE)
    std_out, std_err = proc_write_to_elf.communicate()
    rtn_code = proc_write_to_elf.returncode
    std_out = std_out.decode('UTF-8')
    std_err = std_err.decode('UTF-8')
    if std_err:
        print(std_err)
    if rtn_code != 0:
        sys.exit(rtn_code)

    std_out = [line for line in std_out.splitlines() if checksum_struct_name in line]
    symbolAttributes = std_out[0].split()

    positionInSection = int(symbolAttributes[0], 16) - int(sectionAttributes["LMA"], 16)

    # calculate offset of checksum in ELF file
    offset = int(sectionAttributes["File"], 16) + positionInSection + checksum_position_in_struct

    # create single bytes from checksum string
    bytes = struct.pack('I', int(checksum, 16))

    # write checksum bytes to calculated offset in ELF file
    with open(elffile, "r+b") as fh:
        fh.seek(offset)
        fh.write(bytes)

    # calculate offset of date in ELF file
    offset = int(sectionAttributes["File"], 16) + positionInSection + date_position_in_struct

    # create single bytes from date string
    d = datetime.datetime.now()
    bytes = d.strftime("%b %d %Y").encode()

    # write date bytes to calculated offset in ELF file
    with open(elffile, "r+b") as fh:
        fh.seek(offset)
        fh.write(bytes)

    # calculate offset of time in ELF file
    offset = int(sectionAttributes["File"], 16) + positionInSection + time_position_in_struct

    # create single bytes from time string
    bytes = d.strftime("%H:%M:%S").encode()

    # write time bytes to calculated offset in ELF file
    with open(elffile, "r+b") as fh:
        fh.seek(offset)
        fh.write(bytes)

    logging.info('done...')


if __name__ == '__main__':
    main()
