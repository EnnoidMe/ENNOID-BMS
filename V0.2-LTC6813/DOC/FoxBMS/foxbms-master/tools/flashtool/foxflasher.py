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
import sys
import time
import yaml
import stm32flasher


class FoxFlasher(stm32flasher.STM32Flasher):
    def __init__(self, port=None, file=None, **kwargs):
        stm32flasher.STM32Flasher.__init__(self, port, file, **kwargs)

    def _doBeforeInit(self):
        self.enterBootmode()
        self.reset()

    def reset(self):
        ''' resets the microcontroller by giving a pulse to RESET pin '''
        self._port.setRTS(1)
        time.sleep(0.5)
        self._port.setRTS(0)
        time.sleep(0.5)

    def enterBootmode(self):
        ''' sets DTR pin, which is connected to microcontroller BOOT pin '''
        self._port.setDTR(1)

    def exitBootmode(self):
        ''' resets DTR pin, which is connected to microcontroller BOOT pin '''
        self._port.setDTR(0)


def get_section_list(start_address, length, yaml_data):
        ''' Calculates the sections for the erase and puts them into a list  as lists a 8 byte.
            Args:
                start_address (hex): Start address for the write command
                length (int): length of the data to write
                yaml_data (obj): object which contains the specific config data of the u-Controller
            Returns:
                section_list (list): list with the lists of sections inside
        '''
        start_section = 0
        end_section = 0
        sections_startaddress = yaml_data['Controller'][3]['mcu_sections_startaddress']
        sections_endaddress = yaml_data['Controller'][4]['mcu_sections_endaddress']
        numberFlashSections = yaml_data['Controller'][2]['mcu_number_of_sections']

        for i in range(numberFlashSections):
            if (start_address >= sections_startaddress[i] and start_address <= sections_endaddress[i]):
                start_section = i

        for i in range(numberFlashSections):
            if ((start_address + length - 1) >= sections_startaddress[i] and (start_address + length - 1) <= sections_endaddress[i]):
                # -1 because address starts at 0
                end_section = i

        sectionsToErase = range(start_section, (end_section + 1))
        length = len(sectionsToErase)
        numberOfLists = (length + 7) // 8
        section_list = [[] for x in range(numberOfLists)]
        offset = 0

        for liste in section_list:

            if length > 8:
                for section in sectionsToErase[offset:offset + 8]:
                        liste.append(section)
                offset += 8
                length -= 8
            if length <= 8 and len(liste) == 0:
                for section in sectionsToErase[offset:offset + length]:
                    liste.append(section)

        return section_list, length - 1


def auto_int(x):
    return int(x, 0)


def main():
    parser = argparse.ArgumentParser(description='foxBMS---STM32 flash tool',
                                     formatter_class=argparse.RawDescriptionHelpFormatter,
                                     epilog='''\
Example:
%s --port COM3 --erase --write --verify --address 0x08008000 build/src/general/foxbms_flash.bin

Erases the complete flash but the bootloader section and afterwards
writes the foxbms_flash.bin at address 0x08008000 upwards

Copyright (c) 2015 - 2017 Fraunhofer IISB.
All rights reserved.
This program has been released under the conditions of the BSD 3-Clause License.

''' % sys.argv[0])

    # --port COM3 --write --verify --address 0x08004000 build/src/general/foxbms_flash.bin
    # Only erases the sections that are needed to
    # write new bin file to flash memory and afterwards flashes bin file

    _configfile = os.path.join(os.path.dirname(sys.argv[0]), 'mcuconfig.yml')
    parser.add_argument('-v', '--verbosity', action='count', default=0, help="increase output verbosity")
    parser.add_argument('--erase', '-e', action='store_true', help='erase only needed flash depending on startaddress and binary length')
    parser.add_argument('--read', '-r', action='store_true', help='read and store firmware')
    parser.add_argument('--write', '-w', action='store_true', help='writes firmware')
    parser.add_argument('--verify', '-y', action='store_true', help='verify the firmware')
    parser.add_argument('--bytes', '-s', nargs='?', type=int, help='bytes to read from the firmware')
    parser.add_argument('--bauds', '-b', type=int, default=115200, help='transfer speed (bauds)')
    parser.add_argument('--port', '-p', type=str, default='/dev/tty.usbserial-ftCYPMYJ', help='ttyUSB port')
    parser.add_argument('--address', '-a', type=auto_int, default=0x08000000, help='target address')
    parser.add_argument('--goaddress', '-g', type=auto_int, default=-1, help='start address (use -1 for default)')
    parser.add_argument('--config', '-c', default=_configfile, help='path to configuration file (default: {})'.format(_configfile))
    parser.add_argument('firmware', nargs='?', metavar='FIRMWARE FILE', help='firmware binary')
    parser.add_argument('--extendederase', '-ee', action='store_true', help='erase complete flash but the bootloader memory section')
    parser.add_argument('--fullerase', '-fe', action='store_true', help='erases complete flash')

    args = parser.parse_args()

    if args.verbosity == 1:
        logging.basicConfig(level=logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.ERROR)

    if not os.path.isfile(args.config):
        raise RuntimeError(
            'Required configuration file {} not found.'.format(args.config))

    # Load mcu config file
    '''Create mcuconfig object, read the specific configfile from argparse
        and creates the variables with the values from the configfile
    '''
    with open(args.config, 'r') as stream:
        yaml_data = yaml.load(stream)

    start_address_flashmemory = \
        yaml_data['Controller'][0]['start_address_flashmemory']
    end_address_flashmemory = \
        yaml_data['Controller'][1]['end_address_flashmemory']
    sectionNames = \
        yaml_data['Controller'][5]['mcu_section_names']

    logging.debug(sectionNames)

    # Get size of binfile to flash
    binsize = os.path.getsize(args.firmware)

    # If invalid flash address or file too big: exit
    if ((args.address < start_address_flashmemory) or
            (args.address > end_address_flashmemory) or
            (args.address + binsize > end_address_flashmemory)):
        sys.exit('Params are not legit. Please check your config and start the script again!')

    # Calculate sections that are to be deleted
    sections, erase_sections_no = get_section_list(args.address,
                                                   binsize, yaml_data)

    if args.read:
        if args.erase:
            parser.error('Cannot use --erase together with --read')
        if args.write:
            parser.error('Cannot use --write together with --read')
        if args.bytes is None:
            parser.error('Please give a length (in bytes) to read')

    with FoxFlasher(**vars(args)) as ff:
        if args.write or args.verify:
            with open(args.firmware, 'rb') as f:
                data = [d for d in f.read()]

        if args.erase:
            for element in sections:
                for section in element:
                    ff.extendedErase(sectionNames[section])

        if args.extendederase:
            ff.extendedErase("AllButBootloader")

        if args.fullerase:
            ff.erase()

        if args.write:
            ff.write(data)

        if args.verify:
            ff.verify(data)

        if args.read:
            rdata = ff.read()
            with open(args.firmware, 'wb') as f:
                f.write(''.join(map(chr, rdata)))

        if args.goaddress > -1:
            ff.go(args.goaddress)

        ff.exitBootmode()
        ff.reset()


if __name__ == "__main__":
    main()
