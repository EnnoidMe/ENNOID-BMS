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

"""Post-Build Program for checksum generation of binary.
In the first build the elf-file will contain a wrong checksum value,
only *.hex-file will be modified with a valid checksum.
This script will be invoked in a post-build step to generate either a STM32
compatible CRC32 checksum or a simple Modulo32BitAddition of the provided
binary. For this purpose the generated .hex file will be loaded from disc and
its content is being parsed. In the next step a buffer file will be created,
initialized with 0xFF to match the pattern during flashand parsed content from
the hex file is stored into this file buffer. Each line gets its own line
checksum and desired checksum type is calculated and written into a header file
in the final step.
"""

import sys
import os
import yaml
import binascii
import struct
import argparse
import ntpath
import logging

__version__ = 0.3
__date__ = '2017-12-05'
__updated__ = '2018-08-10'


class CheckSumHandler(object):
    """ Read the project configuration of checksum project"""
    def __init__(self, file_path, variant, bin_flash_file,
                 bin_flash_header_file):
        for _fp in [file_path, bin_flash_file, bin_flash_header_file]:
            if not os.path.isfile(_fp):
                print('Error: file \'{}\' not found.'.format(file_path))
                sys.exit(1)

        with open(file_path, 'r') as stream:
            try:
                mcu_config = yaml.load(stream)
            except yaml.YAMLError as exc:
                print(exc)
                sys.exit(1)

        project = mcu_config[variant]
        self.bin_flash_file_path = bin_flash_file
        self.bin_flash_header_file_path = bin_flash_header_file
        self.flashsize = int(project['flashsize'], 16)
        self.flash_header_address = int(project['flashheader'], 16)
        self.flash_header_address &= 0x00ffffff
        self.begin_address = int(project['beginaddress'], 16)
        self.begin_address &= 0x00ffffff
        self.end_address = int(project['endaddress'], 16)
        self.end_address &= 0x00ffffff
        self.chksum_address = int(project['chksumaddress'], 16)
        self.chksum_address &= 0x00ffffff

    def __str__(self):
        """helper function for pretty printing the configuration"""
        conf_string = '''* Configuration
* Bin flash file path:          {}
* Bin flash header file path:   {}
* Flashsize:                    {} ({})
* Flash header address:         {} ({})
* Begin address:                {} ({})
* End address:                  {} ({})
* Checksum address:             {} ({})\
'''.format(self.bin_flash_file_path,
           self.bin_flash_header_file_path,
           hex(self.flashsize), self.flashsize,
           hex(self.flash_header_address), self.flash_header_address,
           hex(self.begin_address), self.begin_address,
           hex(self.end_address), self.end_address,
           hex(self.chksum_address), self.chksum_address
           )
        max_len = 0
        for l in conf_string.split('\n'):
            if len(l) > max_len:
                max_len = len(l)
        max_len = max_len + 1
        a = ''
        for i, l in enumerate(conf_string.split('\n')):
            a += l + ' ' * (max_len - len(l)) + '*'
            if not i == len(conf_string.split('\n')) - 1:
                a += '\n'

        final_conf = '''
{}
{}
{}'''.format('*' * (max_len + 1), a, '*' * (max_len + 1))
        return final_conf

    def checksum_result(self, result_input):
        config_reps = self.__str__()
        max_len = len(config_reps.splitlines()[1])
        result_input = '* {}'.format(result_input)
        add_ws_len = max_len - len(result_input.strip()) - 2
        result_input = '{} {}*'.format(result_input, ' ' * add_ws_len)
        result = '{}\n{}\n{}'.format(config_reps, result_input, '*' * max_len)
        return result


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
    Copyright 2018 foxBMS. All rights reserved.

    Licensed under the BSD 3-Clause License.

    Distributed on an "AS IS" basis without warranties
    or conditions of any kind, either express or implied.

USAGE
'''.format(program_name, program_shortdesc, str(__date__))
    parser = argparse.ArgumentParser(
        description=program_license,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('checksum_config_file', help='ini-file')
    parser.add_argument('variant', help='Variant of the build')
    parser.add_argument('-csof', '--checksum-out-file',
                        help='specify checksum output directory')
    parser.add_argument('-bf', '--bin-flash', help='Specify binfile \
for that the checksum must be calculated.')
    parser.add_argument('-bfh', '--bin-flash-header', help='Specify \
flashheaderbinfile for that the checksum must be calculated.')
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
    args = parser.parse_args()

    if args.verbosity == 1:
        logging.basicConfig(level=logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.ERROR)

    dir_path, file_path = ntpath.split(args.checksum_out_file)

    if args.checksum_out_file:
        temp_dir = [os.path.normpath(dir_path)]
    else:
        temp_dir = ['chk5um']

    try:
        os.makedirs(*temp_dir)
    except OSError:
        if not os.path.isdir(dir_path):
            raise

    chksum_cfg = CheckSumHandler(args.checksum_config_file, args.variant, args.bin_flash, args.bin_flash_header)

    output = ''
    final_data_chksum = 0

    with open(chksum_cfg.bin_flash_file_path, 'rb') as p_buffer_file, open(chksum_cfg.bin_flash_header_file_path, 'rb') as pheader_buffer_file:
        # calculate expected binary size from flasheader credentials
        pheader_buffer_file.seek(chksum_cfg.begin_address ^ chksum_cfg.flash_header_address)
        checksum_start_address = struct.unpack("i", pheader_buffer_file.read(4))[0]
        pheader_buffer_file.seek(chksum_cfg.end_address ^ chksum_cfg.flash_header_address)
        checksum_end_address = struct.unpack("i",
                                             pheader_buffer_file.read(4))[0]
        chksum_calc_length = checksum_end_address - checksum_start_address + 1

        # get binary size from filesize
        p_buffer_file.seek(0, 2)  # move file pointer to end of file
        p_buffer_filesize = p_buffer_file.tell()

        if chksum_calc_length == p_buffer_filesize:
            p_buffer_file.seek(0)
            # calc crc32, using python binascii module, use mask to ensure compatible output
            final_data_chksum = (binascii.crc32(p_buffer_file.read(chksum_calc_length), 0) &
                                 0xFFFFFFFF)

            chksum_output_string = '32-bit SW-Chksum:             0x%08X' % (final_data_chksum)
            output += chksum_output_string

            p_buffer_file.seek(chksum_cfg.flash_header_address + 24)
        else:
            print('Checksum calculation error')
            sys.exit(1)

    output = chksum_cfg.checksum_result(output)
    print(output)
    with open(os.path.normpath(args.checksum_out_file), 'w+') as f:
        p = ''
        for i in output.split('\n'):
            if not i:
                continue
            p += '# {}\n'.format(i)
        as_upper_hex = '0x' + ('{:08x}'.format(final_data_chksum)).upper()
        p += 'checksum: {}\n'.format(as_upper_hex)
        f.write(p)


if __name__ == '__main__':
    main()
