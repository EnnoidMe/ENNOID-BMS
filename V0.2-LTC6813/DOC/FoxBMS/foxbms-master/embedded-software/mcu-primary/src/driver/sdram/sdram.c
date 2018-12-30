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
 * @file    sdram.c
 * @author  foxBMS Team
 * @date    13.11.2015 (date of creation)
 * @ingroup UTIL
 * @prefix  SDRAM
 *
 * @brief   Driver for the volatile SDRAM.
 *
 */

/*================== Includes =============================================*/
#include "sdram.h"
#include "cpu_cfg.h"


#ifdef HAL_SDRAM_MODULE_ENABLED

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

void SDRAM_Init(void) {

    SDRAM_HandleTypeDef      sdram_handle;
    FMC_SDRAM_TimingTypeDef  sdram_timing;

    sdram_handle.Instance = FMC_SDRAM_DEVICE;   /*!< Register base address                 */
    sdram_handle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;     /*!< SDRAM device configuration parameters */
    sdram_handle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
    sdram_handle.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
    sdram_handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    sdram_handle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
    sdram_handle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    sdram_handle.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
    sdram_handle.Init.ReadBurst          = FMC_SDRAM_RBURST_DISABLE;
    sdram_handle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;
    sdram_handle.Init.SDBank             = FMC_SDRAM_BANK2;

    sdram_handle.State = HAL_SDRAM_STATE_RESET;     /*!< SDRAM access state                    */

    /* SDRAM Bank Timing Configuration based on frequency of 180/2 MHz */
    sdram_timing.LoadToActiveDelay    = 2;  /*!< Defines the delay between a Load Mode Register command and
                                                  an active or Refresh command in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.ExitSelfRefreshDelay = 7;  /*!< Defines the delay from releasing the self refresh command to
                                                  issuing the Activate command in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.SelfRefreshTime      = 4;  /*!< Defines the minimum Self Refresh period in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.RowCycleDelay        = 7;  /*!< Defines the delay between the Refresh command and the Activate command
                                                  and the delay between two consecutive Refresh commands in number of
                                                  memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.WriteRecoveryTime    = 2;  /*!< Defines the Write recovery Time in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.RPDelay              = 2;  /*!< Defines the delay between a Precharge Command and an other command
                                                  in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */
    sdram_timing.RCDDelay             = 2;  /*!< Defines the delay between the Activate Command and a Read/Write
                                                  command in number of memory clock cycles.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16  */

    HAL_SDRAM_Init(&sdram_handle, &sdram_timing);

    FMC_SDRAM_CommandTypeDef   sdram_cmd;

    /* Send Command: Clock Configuration */
    sdram_cmd.CommandMode     = FMC_SDRAM_CMD_CLK_ENABLE;  /*!< Defines the command issued to the SDRAM device.
                                                  This parameter can be a value of @ref FMC_SDRAM_Command_Mode.          */
    sdram_cmd.CommandTarget   = FMC_SDCMR_CTB2;  /*!< Defines which device (1 or 2) the command will be issued to.
                                                  This parameter can be a value of @ref FMC_SDRAM_Command_Target.        */
    sdram_cmd.AutoRefreshNumber = 1;  /*!< Defines the number of consecutive auto refresh command issued
                                                  in auto refresh mode.
                                                  This parameter can be a value between Min_Data = 1 and Max_Data = 16   */
    sdram_cmd.ModeRegisterDefinition = 0;  /*!< Defines the SDRAM Mode register content                                */
    /* wait if SDRAM is busy */
    /* HAL_IS_BIT_SET(Device->SDSR, FMC_SDSR_BUSY) */
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);

    HAL_Delay(10);      /* @todo 10 or 100ms? */

    /* Send Command: PreCharge all */
    sdram_cmd.CommandMode     = FMC_SDRAM_CMD_PALL;
    sdram_cmd.CommandTarget   = FMC_SDCMR_CTB2;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = 0;
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);      /* @todo 10 or 100ms? */

    /* Send Command: Autorefresh */
    sdram_cmd.CommandMode     = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    sdram_cmd.CommandTarget   = FMC_SDCMR_CTB2;
    sdram_cmd.AutoRefreshNumber = 4;
    sdram_cmd.ModeRegisterDefinition = 0;
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);      /* @todo 10 or 100ms? */

    /* Send Command: Autorefresh first Command */
    sdram_cmd.CommandMode     = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    sdram_cmd.CommandTarget   = FMC_SDCMR_CTB2;
    sdram_cmd.AutoRefreshNumber = 4;
    sdram_cmd.ModeRegisterDefinition = 0;
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);      /* @todo 10 or 100ms? */

    /* Send Command: second Command */
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);      /* @todo 10 or 100ms? */

    /* Send Command: external memory mode */
    sdram_cmd.CommandMode     = FMC_SDRAM_CMD_LOAD_MODE;
    sdram_cmd.CommandTarget   = FMC_SDCMR_CTB2;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = 0x231;  /* CAS LATENCY 3, BURST LENGTH 2, single WRITEBURST mode */
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}
    HAL_SDRAM_SendCommand(&sdram_handle, &sdram_cmd, 1);      /* @todo 10 or 100ms? */


    HAL_SDRAM_ProgramRefreshRate(&sdram_handle,1292);          /* refreshrate 15,62us x 84Mhz -20 */
    while(__FMC_SDRAM_GET_FLAG(sdram_handle.Instance, FMC_SDSR_BUSY))
    {}


    SDRAM_c_init();

}

#endif

    /*@todo implement ram test instead of the following verification of linker */
