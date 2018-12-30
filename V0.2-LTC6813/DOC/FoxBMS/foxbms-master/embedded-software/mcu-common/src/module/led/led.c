/**
 *
 * @copyright &copy; 2010 - 2018, Fraunhofer-Gesellschaft zur Foerderung der
 *  angewandten Forschung e.V. All rights reserved.
 *
 * BSD 3-Clause License
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * We kindly request you to use one or more of the following phrases to refer
 * to foxBMS in your hardware, software, documentation or advertising
 * materials:
 *
 * &Prime;This product uses parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product includes parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product is derived from foxBMS&reg;&Prime;
 *
 */

/**
 * @file    led.c
 * @author  foxBMS Team
 * @date    12.11.2015 (date of creation)
 * @ingroup UTIL
 * @prefix  LED
 *
 * @brief   Driver to control the blinking of the display LEDs.
 *
 */

/*================== Includes =============================================*/
#include "led.h"

#include "io.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/


void LED_Init(void) {
    IO_WritePin(LED_DEBUG_LED_0, GPIO_PIN_RESET);
    IO_WritePin(LED_DEBUG_LED_1, GPIO_PIN_RESET);
}



void LED_Ctrl(void) {
    static uint16_t led_counter = 0;
    static uint8_t led_0_nbr_of_blink = 1;
    static uint8_t led_1_nbr_of_blink = 1;  /* 1 -> ON/OFF; >1 -> blinking */
    static uint16_t led_blink_time = 3000;  /* in [ms] */
    static uint8_t led_state = 0x10;

    static uint8_t led_0_duty_cycle = 0;
    static uint8_t led_1_duty_cycle = 0;
    static uint16_t cycle_time = 0;


    if (led_counter == 0) {

        /* get number of blink events from sys() state */
        led_0_nbr_of_blink = 1;
        led_1_nbr_of_blink = 1;
        led_blink_time = 3000;      /* in [ms] */

        /* calculate number of pin toggle events to get desired number of blinking */
        if ( led_0_nbr_of_blink > 1 ) {
            led_0_duty_cycle = (led_blink_time/LED_BLINK_TASK_DURATION_MS)/(led_0_nbr_of_blink*2);
        }
        if ( led_1_nbr_of_blink > 1 ) {
            led_1_duty_cycle = (led_blink_time/LED_BLINK_TASK_DURATION_MS)/(led_1_nbr_of_blink*2);
        }

        /* If one LED has to blink, set blinking time, otherwise set OFF time */
        if ( led_state == 0x10 ) {
            cycle_time = led_blink_time/LED_BLINK_TASK_DURATION_MS;
            /* if not blinking requested: simply switch ON/OFF the LEDs */

            IO_WritePin(LED_DEBUG_LED_0, GPIO_PIN_SET);
            IO_WritePin(LED_DEBUG_LED_1, GPIO_PIN_RESET);
        }
        else if ( led_state == 0x01 ) {
            cycle_time = led_blink_time/LED_BLINK_TASK_DURATION_MS;
            /* if not blinking requested: simply switch ON/OFF the LEDs */

            IO_WritePin(LED_DEBUG_LED_0, GPIO_PIN_RESET);
            IO_WritePin(LED_DEBUG_LED_1, GPIO_PIN_SET);
        }
        else {
            cycle_time = LED_OFF_TIME_MS/LED_BLINK_TASK_DURATION_MS;
            /* OFF period: switch OFF the LEDs */
            IO_WritePin(LED_DEBUG_LED_0, GPIO_PIN_RESET);
            IO_WritePin(LED_DEBUG_LED_1, GPIO_PIN_RESET);
        }
    }

    /* effectively toggle the LED pins */
    if ( led_state == 0x10 && led_0_nbr_of_blink > 1 && led_0_duty_cycle > 0 ) {
        if ( led_counter%led_0_duty_cycle == 0 )
            IO_TogglePin(LED_DEBUG_LED_0);
    }
    if ( led_state == 0x01 && led_1_nbr_of_blink > 1 && led_1_duty_cycle > 0 ) {
        if ( led_counter%led_1_duty_cycle == 0 )
            IO_TogglePin(LED_DEBUG_LED_1);
    }


    /* counter update */
    if ( led_counter < cycle_time ) {
        led_counter++;
    }
    else {
        if (led_state == 0x10) {
            led_state = 0x00;
        }
        else if ( led_state == 0x00 ) {
            led_state = 0x01;
        }
        else if ( led_state == 0x01 ) {
            led_state = 0x11;
        }
        else if ( led_state == 0x11 ) {
            led_state = 0x10;
        }
        else {
            led_state = 0x10;
        }

        led_counter = 0;
    }

}
