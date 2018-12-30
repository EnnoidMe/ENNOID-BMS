/**
 * @author  STMicroelectronics
 * @date    2016
 * @ingroup GENERAL_CONF
 * @brief HAL IT Module, definition of IRQ handlers for hardware units,
 *        based on project examples Templates\Src\stm32f4xx_it.c    
 *
 */

/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    06-May-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*================== Includes =============================================*/

#include "stm32f4xx_it.h"
#include "os.h"
#include "dma.h"
#include "spi.h"
#include "can.h"
#include "uart.h"
#include "diag.h"
#include "adc.h"
#include "timer.h"
#include "mcu.h"
#include "io.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  *
  * @retval void
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  *
  * @retval void
  */
void HardFault_Handler(void)
{
    uint32_t faultaddress;        /* note: MSP Stack will be changed */
    uint32_t lr_register;
    uint32_t sp_register;
    uint32_t caller_addr_at_stack;
  /* Go to infinite loop when Hard Fault exception occurs */
/*   while (1) */
/*   { */
/*   } */

#ifdef STM32F4

    __ASM volatile ("mov %0, r14" : "=r" (lr_register) );

/*     VOID_FUNC_VOID fuptr; */
/*     fuptr = (VOID_FUNC_VOID)(0xB0000020); */
/*     fuptr(); */

    /* Check EXC_RETURN at exception entry to identify the used stack (MSP or PSP) */
    if(lr_register== 0xFFFFFFFD)   /* Return to Thread mode, exception return uses non-floating-point state from */
    {                              /* the PSP and execution uses PSP after return */
        sp_register = __get_PSP();
    }
    else if(lr_register== 0xFFFFFFF1)  /* Return to Handler mode, exception return uses non-floating-point state from */
    {                                   /* MSP and execution uses MSP after return */
        sp_register = __get_MSP() + 0x20;    /* because of function local variables stack will be changed in MSP */
    }

    diag_fc.Val0 = SCB->CFSR;
    /* Check if division by zero exception occured */
    if(SCB->CFSR &= 0x02000000) /* FLAG: DIVBYZERO */
    {
        caller_addr_at_stack = sp_register + 0x18;

        diag_fc.Val1 = *(uint32_t*)caller_addr_at_stack;  /* report instruction address where division has occured */
        DIAG_Handler(DIAG_CH_DIV_BY_ZERO_FAILURE,DIAG_EVENT_NOK,0, NULL);

    }

    else if(SCB->CFSR &= 0x00010000) /* FLAG: UNDEFINSTR */
    {   /* tested by undefined instruction in memory: "0xF7F0A000" */
        caller_addr_at_stack = sp_register+0x18;

        diag_fc.Val1 = *(uint32_t*)caller_addr_at_stack;  /*  report instruction address with undefined instruction */
        DIAG_Handler(DIAG_CH_UNDEF_INSTRUCTION_FAILURE,DIAG_EVENT_NOK,0, NULL);
    }


    /* Check if data bus error occured or data access violation */
    else if(SCB->CFSR &= 0x00000202) /* FLAG: PRECIS ERR or DACC VIOL */
    {   /* tested by data pointer to address area 0x04000000, 0xb0000000... */
        caller_addr_at_stack = sp_register+0x18;

        diag_fc.Val1 = *(uint32_t*)(caller_addr_at_stack);    /*  report instruction address where data bus error has occured */

        faultaddress = SCB->BFAR;
        if(SCB->CFSR &= 0x00008000) /* check if BFAR (reported Bus Fault Address) is valid */
        {
            diag_fc.Val2 = faultaddress;                    /* report bus address being accessed */
        }
        DIAG_Handler(DIAG_CH_DATA_BUS_FAILURE,DIAG_EVENT_NOK,0, NULL);

    }

    /* Check if instruction bus error occured or instruction access violation */
    else if(SCB->CFSR &= 0x00000101) /* FLAG: IBUS ERR or IACC VIOL */
    {   /* tested by using function pointer to address area 0x04000000, 0xb0000000... */
        caller_addr_at_stack = (sp_register+0x18);

        diag_fc.Val1 = *(uint32_t*)(caller_addr_at_stack);    /* report instruction address where instruction bus error has occured */

        faultaddress = SCB->BFAR;
        if(SCB->CFSR &= 0x00008000) /* check if BFAR (reported Bus Fault Address) is valid */
        {
            diag_fc.Val2 = faultaddress;                        /* report bus address being accessed */
        }
        DIAG_Handler(DIAG_CH_INSTRUCTION_BUS_FAILURE,DIAG_EVENT_NOK, 0, NULL);

    }

    else
    {
        DIAG_Handler(DIAG_CH_HARDFAULT_NOTHANDLED, DIAG_EVENT_NOK, 0, NULL);
    }
#endif

    while(1) {
        ;
    }

}

/**
  * @brief  This function handles Memory Manage exception.
  *
  * @retval void
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  *
  * @retval void
  *
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  *
  * @retval void
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

#if 0
/* this function is implemented by FreeRTOS in port.c: macro vPortSVCHandler */
/**
  * @brief  This function handles SVCall exception.
  * 
  * @retval void
  */
void SVC_Handler(void)
{
}
#endif
/**
  * @brief  This function handles Debug Monitor exception.
  *
  * @retval void
  */
void DebugMon_Handler(void)
{
}

#if 0
/* this function is implemented by FreeRTOS in port.c: macro xPortPendSVHandler */
/**
  * @brief  This function handles PendSVC exception.
  * 
  * @retval void
  */
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  *
  * @retval void
  */
void SysTick_Handler(void) {
    OS_SysTickHandler();
    HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  *
  * @retval void
  */
/*void PPP_IRQHandler(void)
{
}*/


void CAN1_TX_IRQHandler(void)
{
    CAN_TX_IRQHandler(&hcan1);
}

void CAN1_RX0_IRQHandler(void)
{
    CAN_RX_IRQHandler(CAN_NODE1, &hcan1);
}

void CAN1_RX1_IRQHandler(void)
{
    CAN_RX_IRQHandler(CAN_NODE1, &hcan1);
}

void CAN1_SCE_IRQHandler(void)
{
    CAN_Error_IRQHandler(CAN_NODE1, &hcan1);
}

void CAN0_TX_IRQHandler(void)
{
    CAN_TX_IRQHandler(&hcan0);
}

void CAN0_RX0_IRQHandler(void)
{
    CAN_RX_IRQHandler(CAN_NODE0, &hcan0);
}

void CAN0_RX1_IRQHandler(void)
{
    CAN_RX_IRQHandler(CAN_NODE0, &hcan0);
}

void CAN0_SCE_IRQHandler(void)
{
    CAN_Error_IRQHandler(CAN_NODE0, &hcan0);
}

#if 1   /* FIXME nötig? */
/**
* @brief This function handles SPI6 global interrupt.
*/
void SPI6_IRQHandler(void)
{
  /* USER CODE BEGIN SPI6_IRQn 0 */

  /* USER CODE END SPI6_IRQn 0 */
  HAL_NVIC_ClearPendingIRQ(SPI6_IRQn);
  HAL_SPI_IRQHandler(&spi_devices[1]);

}
#endif

#if 0   /* FIXME kann gelöscht werden? */
/**
* @brief This function handles SPI4 global interrupt.
*/
void SPI4_IRQHandler(void)
{
  /* USER CODE BEGIN SPI4_IRQn 0 */

  /* USER CODE END SPI4_IRQn 0 */
  HAL_NVIC_ClearPendingIRQ(SPI4_IRQn);
  HAL_SPI_IRQHandler(&spi_devices[0]);
  /* USER CODE BEGIN SPI4_IRQn 1 */

  /* USER CODE END SPI4_IRQn 1 */
}
#endif


/**
 * interrupt-handler for DMA2
 *
 * @ingroup HAL
 */
void DMA2_Stream0_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream0_IRQn);
    HAL_DMA_IRQHandler(&dma_devices[0]);
}

/**
 * interrupt-handler for DMA2
 *
 * @ingroup HAL
 */
void DMA2_Stream1_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream1_IRQn);
    HAL_DMA_IRQHandler(&dma_devices[1]);

}


/**
 * interrupt-handler for DMA2
 *
 * @ingroup HAL
 */
void DMA2_Stream2_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
    HAL_DMA_IRQHandler(&dma_devices[0]);
}

/**
 * interrupt-handler for DMA2
 *
 * @ingroup HAL
 */
void DMA2_Stream3_IRQHandler(void)
{
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream3_IRQn);
    HAL_DMA_IRQHandler(&dma_devices[1]);

}

/**
 * interrupt-handler for USART3
 *
 * @ingroup HAL
 */
void USART3_IRQHandler(void)
{
#if BUILD_MODULE_ENABLE_UART
    HAL_UART_CustomIRQHandler(&uart_cfg[0]);

#endif
}

/**
 * interrupt-handler for USART2
 *
 * @ingroup HAL
 */
void USART2_IRQHandler(void)
{
#if BUILD_MODULE_ENABLE_RS485
    /* @todo akdere */
    /* HAL_UART_CustomIRQHandler(&huart2); */
#endif
}



void ADC_IRQHandler(void)
{

  HAL_NVIC_ClearPendingIRQ(ADC_IRQn);
  HAL_ADC_IRQHandler(&adc_devices[0]);
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
