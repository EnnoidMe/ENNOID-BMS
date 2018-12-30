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

import argparse
import logging
import os
import re
import subprocess
import sys
# import time

import yaml

import detect


try:
    ()
    # from configparser import ConfigParser
except ImportError:
    ()
    # from ConfigParser import ConfigParser

MCU_CHOICE = {0: 'primary-mcu', 1: 'secondary-mcu'}


class flashProcess(object):
    def __init__(self, COMport, actions, binary, address):
        script = 'foxflasher.py'
        self.script = os.path.join(os.path.dirname(sys.argv[0]), script)
        self.COMport = COMport
        self.actions = actions
        self.binary = binary
        self.address = address

    def create_run_str(self):
        actions_string = '--' + ' --'.join(self.actions)
        run_str = '''{} {} -p {} {} {} --address {} -v'''.format(sys.executable, self.script,
                                                                 self.COMport, actions_string,
                                                                 self.binary, self.address)
        return run_str


def main():
    parser = argparse.ArgumentParser(description='Wrapper for the foxBMS-STM32 flash tool',
                                     formatter_class=argparse.RawDescriptionHelpFormatter,
                                     epilog="""\
Example:
%s <mcu> <firmwarefile> [--flashconf]

Wrapps the foxBMS-STM32 flash tool

The mcu to be flashed and the path to firmware must be specified.
The actual flash configuration is default in flashconfig.yml and works for foxBMS.
There is no need to change it if you are working with a normal foxBMS.

Copyright (c) 2017 Fraunhofer IISB.
All rights reserved.
This program has been released under the conditions of the BSD 3-Clause License.

""" % sys.argv[0])

    script_dir = os.path.abspath(os.path.dirname(sys.argv[0]))
    _flashconfigfile = os.path.join(script_dir, 'flashconfig.yml')
    parser.add_argument('mcu', type=int, choices=[0, 1],
                        help='choice between primary and secondary')
    parser.add_argument('-f', '--flashconf', default=_flashconfigfile,
                        help='path to configuration file (default: {})'.format(_flashconfigfile))
    parser.add_argument('-v', '--verbosity', action='count',
                        default=0, help="increase output verbosity")
    args = parser.parse_args()

    if args.verbosity == 1:
        logging.basicConfig(level=logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.ERROR)

    if not os.path.isfile(args.flashconf):
        raise RuntimeError('Required configuration file {} not found.'.format(args.flashconf))
    if args.mcu is None:
        raise RuntimeError('The mcu must be specified by --mcu [0,1]')

    with open(args.flashconf, 'r') as stream:
        yaml_data = yaml.load(stream)

    port = None
    selected_mcu = MCU_CHOICE[args.mcu]
    if yaml_data[selected_mcu][0]['COM'] == 'AUTODETECT':
        logging.info('Using \'AUTODETECT\'')
        foxBMS_hardware = detect.foxBMS()
        port = foxBMS_hardware.getCOMport()
    else:
        port = yaml_data[selected_mcu][0]['COM']
        logging.info('Using \'%s\'', port)
        m = re.match(r'COM[0-9]{1,2}', port)
        if not m:
            logging.error('Specify a valid COM port, \'COMx\' where x is the number of the COM port.')
            sys.exit(1)
    for _tar in yaml_data[selected_mcu][1]['targ']:
        _tar_counter = list(_tar.keys())[0]
        _actions = _tar[_tar_counter][0]['actions']
        _address = hex(_tar[_tar_counter][1]['address'])
        _binary = _tar[_tar_counter][2]['binary']
        print('Create run string for \'{}\' and \'{}\''.format(selected_mcu, _binary))
        r = flashProcess(port, _actions, _binary, _address).create_run_str()
        print('Staring flash process: {}'.format(r))
        subprocess.call(r)


if __name__ == "__main__":
    main()
