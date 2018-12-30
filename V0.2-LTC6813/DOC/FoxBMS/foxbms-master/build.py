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
Helper script for building binaries and documentation of foxBMS
"""
import os
import sys
import argparse
import logging
import subprocess

__version__ = 0.1
__date__ = '2017-11-29'
__updated__ = '2018-01-25'


def create_waf_run_string(waf_version, *args):
    arg_string = ' '.join(args)
    return [sys.executable, waf_version, arg_string]


def start_process(cmd, supress_output=False):
    """Starts the build process by passing the command string to the
    command line

    Args:
        cmd (string): command for the build process.
        supress_output (bool): Indicates if logging is active for the build .
    """
    logging.debug(cmd)
    proc = subprocess.Popen(cmd, stdout=None, stderr=subprocess.PIPE)
    out, err = proc.communicate()
    rtn_code = proc.returncode

    if supress_output is False:
        if out:
            logging.info(out)
        if err:
            logging.error(err)

    if rtn_code == 0 or rtn_code is None:
        logging.info('Success: Process return code %s', str(rtn_code))
    else:
        logging.error('Error: Process return code %s', str(rtn_code))
        sys.exit(1)


def generate_cmd(variant, build=True, clean=False, doxygen=False):
    jobs = []
    if clean:
        jobs.append('clean_{}'.format(variant))
    if build:
        jobs.append('build_{}'.format(variant))
    if doxygen:
        jobs.append('doxygen_{}'.format(variant))
    return jobs


def main(args=None):
    """Based on the input form command line the build/clean string is generated
    and passed to waf.
    """
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

    bld_args = parser.add_argument_group('build arguments')

    bld_args.add_argument(
        '-c',
        '--conf',
        action='store_true',
        required=False,
        help='configures the project')

    build_tgt = {'-p': '--primary',
                 '-s': '--secondary'}
    for _short, _long in build_tgt.items():
        bld_args.add_argument(_short, _long,
                              action='store_true',
                              required=False,
                              default=False,
                              help='builds {} binaries'.format(_long[2:]))

    bld_args.add_argument(
        '-dox',
        '--doxygen',
        action='store_true',
        required=False,
        help='builds the software documentation for the specified mcu {}\
'.format(build_tgt.items()))

    bld_args.add_argument(
        '-sphi',
        '--sphinx',
        action='store_true',
        required=False,
        help='builds the general sphinx documenation')

    bld_args.add_argument(
        '-a',
        '--all',
        action='store_true',
        required=False,
        help='generates all of the above mentioned')

    bld_args.add_argument(
        '--clean',
        action='store_true',
        required=False,
        help='cleans the specified mcu binaries and documentation {} or \
general sphinx documentation'.format(build_tgt.items()))

    bld_args.add_argument(
        '-dc',
        '--distclean',
        action='store_true',
        required=False,
        help='Removes the build directory and lock file. The project is \
reconfigured afterwards')

    bld_args.add_argument(
        '-nobld',
        '--nobuild',
        default=False,
        action='store_true',
        required=False,
        help='If specified, then no rebuild after cleaning will be triggered.')

    args = parser.parse_args(args)

    if args.verbosity == 1:
        logging.basicConfig(level=logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.ERROR)

    rebuild = True
    if args.nobuild:
        rebuild = False

    waf_path = os.path.join('tools', 'waf')

    run_list = []
    if args.distclean:
        run_list.append('distclean')
    run_list.append('configure')

    if args.all:
        args.primary = True
        args.secondary = True
        args.doxygen = True
        args.sphinx = True

    if args.primary:
        run_list.extend(
            generate_cmd(
                'primary',
                build=rebuild,
                clean=args.clean,
                doxygen=args.doxygen))
    if args.secondary:
        run_list.extend(
            generate_cmd(
                'secondary',
                build=rebuild,
                clean=args.clean,
                doxygen=args.doxygen))

    if args.sphinx:
        run_list.append('sphinx')
    logging.info('Building following targets:')
    for targ in run_list[1:]:
        logging.info('  - {}'.format(targ))

    exec_cmd = [create_waf_run_string(waf_path, targ) for targ in run_list]
    logging.info('Created run string for following targets:')
    for _cmd in exec_cmd:
        logging.info('  - {}'.format(_cmd))

    for _cmd in exec_cmd:
        start_process(_cmd)


if __name__ == '__main__':
    main()
