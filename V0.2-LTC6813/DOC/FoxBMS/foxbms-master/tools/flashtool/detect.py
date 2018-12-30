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

import logging
import sys

import serial.tools
import serial.tools.list_ports


class foxBMS(object):
    def __init__(self):
        self.__VID__ = '0x403'
        self.__PID__ = '0x6015'
        self.__HWNAME__ = 'foxBMS'
        foxbms_hardware_com_dict = self.autodetect(self.__VID__, self.__PID__, self.__HWNAME__)
        for key in foxbms_hardware_com_dict:
            setattr(self, key, foxbms_hardware_com_dict[key])

    def getCOMport(self):
        return self.__dict__['device']

    @staticmethod
    def autodetect(vid='0x403', pid='0x6015', hw_name='foxBMS'):
        """
        Args:
            vid ():
            pid ():
            hw_name ():
        Returns:

        Raises:

        """
        dev_found = None
        dev_return = None
        device_list = [port for port in serial.tools.list_ports.comports() if port[2] != 'n/a']
        try:
            for _device in device_list:
                logging.info("_device.name %s, %s", _device.vid, _device.pid)
                try:
                    if hex(_device.vid) == vid and hex(_device.pid) == pid:
                        if dev_found is None:
                            dev_found = _device
                            print('{} hardware detected @ {}'.format(hw_name, dev_found).strip())
                            dev_return = dict((k, v) for k,
                                              v in _device.__dict__.items() if not k.startswith('__'))
                        else:
                            raise IOError('''Error, detected second USB which behaves like \
{} @ {}
Exiting...'''.format(hw_name, _device.device))
                            sys.exit(1)
                except TypeError as e:
                    pass
        except Exception as e:
            print(e)
            sys.exit(1)
        if dev_found is None:
            raise IOError('Could not find any {} hardware\nExiting...'.format(hw_name))
        return dev_return


def main():
    foxBMS_hardware = foxBMS()
    print(foxBMS_hardware.getCOMport())


if __name__ == '__main__':
    main()
