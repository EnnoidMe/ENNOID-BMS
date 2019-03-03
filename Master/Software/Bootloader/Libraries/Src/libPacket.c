/*
	Copyright 2016 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include <string.h>
#include "libPacket.h"
#include "libCRC.h"

typedef struct {
	volatile unsigned char rx_state;
	volatile unsigned char rx_timeout;
	void(*send_func)(unsigned char *data, unsigned int len);
	void(*process_func)(unsigned char *data, unsigned int len);
	unsigned int payload_length;
	unsigned char rx_buffer[PACKET_MAX_PL_LEN];
	unsigned char tx_buffer[PACKET_MAX_PL_LEN + 6];
	unsigned int rx_data_ptr;
	unsigned char crc_low;
	unsigned char crc_high;
} PACKET_STATE_t;

static PACKET_STATE_t libPacketHandlerStates[PACKET_HANDLERS];

void libPacketInit(void (*s_func)(unsigned char *data, unsigned int len),
		void (*p_func)(unsigned char *data, unsigned int len), int handler_num) {
	libPacketHandlerStates[handler_num].send_func = s_func;
	libPacketHandlerStates[handler_num].process_func = p_func;
}

void libPacketSendPacket(unsigned char *data, unsigned int len, int handler_num) {
	if (len > PACKET_MAX_PL_LEN) {
		return;
	}

	int b_ind = 0;

	if (len <= 256) {
		libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = 2;
		libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = len;
	} else {
		libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = 3;
		libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = len >> 8;
		libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = len & 0xFF;
	}

	memcpy(libPacketHandlerStates[handler_num].tx_buffer + b_ind, data, len);
	b_ind += len;

	unsigned short crc = libCRCCalcCRC16(data, len);
	libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = (uint8_t)(crc >> 8);
	libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = (uint8_t)(crc & 0xFF);
	libPacketHandlerStates[handler_num].tx_buffer[b_ind++] = 3;

	if (libPacketHandlerStates[handler_num].send_func) {
		libPacketHandlerStates[handler_num].send_func(libPacketHandlerStates[handler_num].tx_buffer, b_ind);
	}
}

/**
 * Call this function every millisecond.
 */
void libPacketTimerFunc(void) {
	for (int i = 0;i < PACKET_HANDLERS;i++) {
		if (libPacketHandlerStates[i].rx_timeout) {
			libPacketHandlerStates[i].rx_timeout--;
		} else {
			libPacketHandlerStates[i].rx_state = 0;
		}
	}
}

void libPacketProcessByte(uint8_t rx_data, int handler_num) {
	switch (libPacketHandlerStates[handler_num].rx_state) {
	case 0:
		if (rx_data == 2) {
			// 1 byte PL len
			libPacketHandlerStates[handler_num].rx_state += 2;
			libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
			libPacketHandlerStates[handler_num].rx_data_ptr = 0;
			libPacketHandlerStates[handler_num].payload_length = 0;
		} else if (rx_data == 3) {
			// 2 byte PL len
			libPacketHandlerStates[handler_num].rx_state++;
			libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
			libPacketHandlerStates[handler_num].rx_data_ptr = 0;
			libPacketHandlerStates[handler_num].payload_length = 0;
		} else {
			libPacketHandlerStates[handler_num].rx_state = 0;
		}
		break;

	case 1:
		libPacketHandlerStates[handler_num].payload_length = (unsigned int)rx_data << 8;
		libPacketHandlerStates[handler_num].rx_state++;
		libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 2:
		libPacketHandlerStates[handler_num].payload_length |= (unsigned int)rx_data;
		if (libPacketHandlerStates[handler_num].payload_length > 0 &&
				libPacketHandlerStates[handler_num].payload_length <= PACKET_MAX_PL_LEN) {
			libPacketHandlerStates[handler_num].rx_state++;
			libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
		} else {
			libPacketHandlerStates[handler_num].rx_state = 0;
		}
		break;

	case 3:
		libPacketHandlerStates[handler_num].rx_buffer[libPacketHandlerStates[handler_num].rx_data_ptr++] = rx_data;
		if (libPacketHandlerStates[handler_num].rx_data_ptr == libPacketHandlerStates[handler_num].payload_length) {
			libPacketHandlerStates[handler_num].rx_state++;
		}
		libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 4:
		libPacketHandlerStates[handler_num].crc_high = rx_data;
		libPacketHandlerStates[handler_num].rx_state++;
		libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 5:
		libPacketHandlerStates[handler_num].crc_low = rx_data;
		libPacketHandlerStates[handler_num].rx_state++;
		libPacketHandlerStates[handler_num].rx_timeout = PACKET_RX_TIMEOUT;
		break;

	case 6:
		if (rx_data == 3) {
			if (libCRCCalcCRC16(libPacketHandlerStates[handler_num].rx_buffer, libPacketHandlerStates[handler_num].payload_length)
					== ((unsigned short)libPacketHandlerStates[handler_num].crc_high << 8
							| (unsigned short)libPacketHandlerStates[handler_num].crc_low)) {
				// Packet received!
				if (libPacketHandlerStates[handler_num].process_func) {
					libPacketHandlerStates[handler_num].process_func(libPacketHandlerStates[handler_num].rx_buffer,
							libPacketHandlerStates[handler_num].payload_length);
				}
			}
		}
		libPacketHandlerStates[handler_num].rx_state = 0;
		break;

	default:
		libPacketHandlerStates[handler_num].rx_state = 0;
		break;
	}
}
