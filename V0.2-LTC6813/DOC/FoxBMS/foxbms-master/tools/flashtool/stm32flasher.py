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

import stm32interface
import argparse
import sys
import logging

"""
flash tool implementation to the STM32F4 microcontroller

- for detailed insight to the USART protocol refer to STM32 appnote AN3155
- for detailed insight to the device bootloader" refer to STM32 appnote AN2606
"""

class STM32Flasher(stm32interface.STM32Interface):
    def __init__(self, port = None, file = None, baudrate=115200,
                 address = 0x08000000, goaddress = -1, bytes = 256,**kwargs):

        stm32interface.STM32Interface.__init__(self, port, baudrate)
        self._file = file
        self.bytes = bytes
        self.address = address

        self._doBeforeInit()
        self.init()

    def _doBeforeInit(self):
        ''' abstract method to optionally reset microcontroller or toggle boot pins '''
        pass

    def __enter__(self):
        return self

    def read(self):
        data = []
        length = self.bytes
        address = self.address
        logging.debug("Flash Read Start, Length: {0}, Address: {1:#x} ".format(length, address))

        while length > 256:
            logging.debug("Read {0} bytes at {1:#x}".format(256, address))
            data_block = self.readMemory(address, 256)
            data.extend(data_block)
            address += 256
            length -= 256
            logging.info("[{0}/{1}] read ".format(self.bytes-length, self.bytes))
        logging.debug("Read {0} bytes at {1:#x}".format(length, address))
        data.extend(self.readMemory(address, length))
        logging.info("[{0}/{1}] read".format(self.bytes, self.bytes))
        return data

    def write(self, data):
        logging.debug("Flash Write Start")
        length = len(data)
        alllng = len(data)
        address = self.address
        offset = 0
        while length > 256:
            logging.debug("Write {0} bytes at {1:#x}".format(256, address))
            self.writeMemory(address, data[offset:offset+256])
            offset += 256
            address += 256
            length -= 256
            logging.info("[{0}/{1}] written".format(alllng-length, alllng))
        logging.debug("Write {0} bytes at {1:#x}".format(length, address))
        self.writeMemory(address, data[offset:offset+length] )
        logging.info("[{0}/{1}] written".format(alllng, alllng))
        #logging.info("Flash Write End")

    def erase(self):
        logging.info("Flash Erase Start")
        super(STM32Flasher, self).erase()
        logging.info("Flash Erase End")


    def verify(self, data):
        logging.info("Flash verify")
        self.bytes = len(data)
        verify = self.read()
        if data == verify:
            logging.info("Verify successful")
            return True
        else:
            self.veriFail = str(len(data)) + ' vs ' + str(len(verify)) + '\n'
            for i in range(0, len(data)):
                if data[i] != verify[i]:
                    self.veriFail += hex(i) + ': ' + hex(data[i]) + ' vs ' + hex(verify[i]) + '\n'
            logging.error(self.veriFail)
            return False

    def __str__(self):
        id =  self.getId()[1:3] # id without length byte and ack byte
        version = self.getVersion()
        return "ID: %s Bootloader version: %x" % (hex(reduce(lambda x, y: x*0x100+y, id)), version[0])

def auto_int(x):
    return int(x, 0)

def main():
    parser = argparse.ArgumentParser(description='STM32 flash tool',
            formatter_class=argparse.RawDescriptionHelpFormatter,
            epilog = '''\
Example:
%s --port COM3 --erase --write --verify build/src/general/foxbms_flash.bin

Copyright (c) 2015, 2016 Fraunhofer IISB.
All rights reserved.
This program has been released under the conditions of the BSD 3-Clause License.
''' % sys.argv[0])

    parser.add_argument('-v', '--verbosity', action='count', default=0, help="increase output verbosity")
    parser.add_argument('--erase', '-e', action='store_true', help='erase firmware')
    parser.add_argument('--read',  '-r', action='store_true', help='read and store firmware')
    parser.add_argument('--write',  '-w', action='store_true', help='writes firmware')
    parser.add_argument('--verify', '-y', action='store_true', help='verify the firmware')
    parser.add_argument('--bytes', '-s', type=int, default = 256, help='bytes to read from the firmware')
    parser.add_argument('--bauds', '-b', type=int, default=115200, help='transfer speed (bauds)')
    parser.add_argument('--port', '-p', type=str, default='/dev/tty.usbserial-ftCYPMYJ', help='ttyUSB port')
    parser.add_argument('--address', '-a', type=auto_int, default=0x08000000, help='target address')
    parser.add_argument('--goaddress', '-g', type=auto_int, default=-1, help='start address (use -1 for default)')
    parser.add_argument('firmware', metavar = 'FIRMWARE FILE', help='firmware binary')

    args = parser.parse_args()

    if args.verbosity == 1:
        logging.basicConfig(level = logging.INFO)
    elif args.verbosity > 1:
        logging.basicConfig(level = logging.DEBUG)
    else:
        logging.basicConfig(level = logging.ERROR)

    if args.read:
        if args.erase:
            parser.error('Cannot use --erase together with --read')
        if args.write:
            parser.error('Cannot use --write together with --read')
        if args.bytes == None:
            parser.error('Please give a length (in bytes) to read')

    with STM32Flasher(**vars(args)) as flasher:
        if args.write or args.verify:
            with open(args.firmware, 'rb') as f:
                data = map(lambda c: ord(c), f.read())

        if args.erase:
            flasher.erase()

        if args.write:
            flasher.write(data)

        if args.verify:
            flasher.verify(data)

        if args.read:
            rdata = flasher.read()
            with open(args.firmware, 'wb') as f:
                f.write(''.join(map(chr,rdata)))

        if args.goaddress > -1:
            flasher.go(args.goaddress)



if __name__ == "__main__":
    main()
