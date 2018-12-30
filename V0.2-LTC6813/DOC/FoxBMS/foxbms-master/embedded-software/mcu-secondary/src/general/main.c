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
 * @file    main.c
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup GENERAL
 * @prefix  none
 *
 * @brief   Main function
 *
 */


/*================== Includes =============================================*/
#include "main.h"
#include "cpu_cfg.h"
#include "os.h"
#include "version.h"
#include "vic.h"
#include "io.h"
#include "rcc.h"
#include "dma.h"
#include "spi.h"
#include "led.h"
#include "bkpsram.h"
#include "chksum.h"
#include "diag.h"
#include "mcu.h"
#include "wdg.h"
#include "adc.h"
#include "rtc.h"
#include "FreeRTOS.h"
#include "task.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
RTC_STATUS_s main_state;


/*================== Function Prototypes ==================================*/
void SystemClock_Config(void);
void BOOT_Init(void);

/*================== Function Implementations =============================*/
/**
  * @brief  Main program
  *
  * @return int
  *
  */
int main(void)
{
    uint32_t retErrorCode = 0;
    STD_RETURN_TYPE_e retval = E_NOT_OK;

    HAL_Init();         /* STM32F4xx HAL library initialization */
    RTC_Init();
#if BUILD_MODULE_ENABLE_WATCHDOG
    WDG_Init();        /* initialize and start watchdog*/
#endif
    BKP_SRAM_Init();    /* at this point diagnosis event memory in BKP_SRAM will be available */
    SystemClock_Config();

    retval = DIAG_Init(&diag_dev, RTC_getRegisterValueBKPSRAM(BKPREGISTER_BKPSRAM_DIAG_VALID));
    if (retval == E_OK) {
        RTC_setRegisterValueBKPSRAM(BKPREGISTER_BKPSRAM_DIAG_VALID, BKPREGISTER_VALID);
    }
    BOOT_Init();
    IO_Init(&io_cfg[0]);

    IO_TogglePin(LED_DEBUG_LED_1);

    DMA_Init(&dma_devices[0]);
    SPI_Init(&spi_devices[0]);

    VIC_PreOsInterruptInit();
    LED_Init();
    ADC_Init(&adc_devices[0]);

    /* Initialize mutexes, events and tasks */
    OS_TaskInit();

    os_schedulerstarttime = OS_getOSSysTick();

    /* Initialize interrupts */
    retErrorCode = VIC_PreOsInterruptInit();
    if (retErrorCode != 0) {
        /* error event in vic init */
        DIAG_Handler(DIAG_CH_VIC_INIT_FAILURE, DIAG_EVENT_NOK, retErrorCode, NULL);
    }

    if(CHK_Flashchecksum(&ver_sw_validation)==E_OK)
    {
        chk_status.checksumstatus = CHK_CHECKSUM_PASSED;
    }
    else
    {
        chk_status.checksumstatus = CHK_CHECKSUM_FAILED;
        if(DIAG_HANDLER_RETURN_OK != DIAG_Handler(DIAG_CH_FLASHCHECKSUM, DIAG_EVENT_NOK, 0, NULL))
        {
            while (1);
        }
    }

    os_boot = OS_INIT_OSSTARTKERNEL;    /* start scheduler */
    vTaskStartScheduler();              /* vTaskStartScheduler() should never return */

    while(1)
    {
        ;
    }
    return 0;
}


void SystemClock_Config(void)
{
    RCC_ClockConfig();
}

/**
 * Set boot reset date, time and status
 */
void BOOT_Init(void)
{
    uint32_t errCode = 0;
    RTC_Time_s currTime;
    RTC_Date_s currDate;

    errCode = MCU_SystemResetStatus(&main_state.CSR);
    /* Unexpected reset occurred */
    if (errCode != 0) {
        DIAG_Handler(DIAG_CH_WATCHDOGRESET_FAILURE, DIAG_EVENT_NOK, errCode, NULL_PTR);
    }

    RTC_getTime(&currTime);
    RTC_getDate(&currDate);
    main_state.boot_rtcdate = currDate;      /* set boot date and time */
    main_state.boot_rtctime = currTime;
    RTC_WDG_RESETCOUNTER++;

    MCU_GetDeviceID(&mcu_unique_deviceID);

    /* @todo akdere: check diagnosis memory */
}
