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
 * @file    io_package_cfg.h
 * @author  foxBMS Team
 * @date    07.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  IO
 *
 * @brief   configuration header of the io-module
 *
 * ==============================================================================
 *                 Configuration of the GPIOs
 * ==============================================================================
 *
 * The io_package_cfg.h assigns a name of each GPIO available on the specific
 * package to a unique, ascending number starting 0, e.g. the first pin at the
 * port A gets the name IO_PA_0 = 0, the second IO_PA_1 = 1, ... With this unique
 * number for each pin it is possible to calculate from every pin name the
 * corresponding port (by integer division) and corresponding pin (by modulo
 * division).
 */

#ifndef IO_PACKAGE_CFG_H_
#define IO_PACKAGE_CFG_H_

/*================== Includes =============================================*/

/*================== Macros and Definitions ===============================*/
/**
 * @ingroup CONFIG_IO
 * Nr. of pins per port must always be set to 16
 * \par Type:
 * int
 * \par Default:
 * 16
*/

/**
 * Number of pins per port at the microcontroller. This number is fixed.
 *
 */
#define IO_NR_OF_PINS_PER_PORT 16

/**
 * @ingroup CONFIG_IO
 * specify the type of microcontroller used
 *
 * \par Type:
 * switch(3)
 * \par Default:
 * 2
*/

/**
 * Defines the used microcontroller package. The user can choose between
 * - IO_PACKAGE_LQFP100
 * - IO_PACKAGE_LQFP144
 * - IO_PACKAGE_LQFP176
 *
 * by using foxygen.
 *
 */

/* #define IO_PACKAGE_LQFP100 */
/* #define IO_PACKAGE_LQFP144 */
#define IO_PACKAGE_LQFP176

/*================== Constant and Variable Definitions ====================*/
#if defined(IO_PACKAGE_LQFP100) || defined(IO_PACKAGE_LQFP144) || defined(IO_PACKAGE_LQFP176)
/**
 * symbolic names of the pins at the microcontroller
 */
typedef enum {
    IO_PA_0     =   0,      /*!< Port A, Pin 0  */
    IO_PA_1     =   1,      /*!< Port A, Pin 1  */
    IO_PA_2     =   2,      /*!< Port A, Pin 2  */
    IO_PA_3     =   3,      /*!< Port A, Pin 3  */
    IO_PA_4     =   4,      /*!< Port A, Pin 4  */
    IO_PA_5     =   5,      /*!< Port A, Pin 5  */
    IO_PA_6     =   6,      /*!< Port A, Pin 6  */
    IO_PA_7     =   7,      /*!< Port A, Pin 7  */
    IO_PA_8     =   8,      /*!< Port A, Pin 8  */
    IO_PA_9     =   9,      /*!< Port A, Pin 9  */
    IO_PA_10    =   10,     /*!< Port A, Pin 10 */
    IO_PA_11    =   11,     /*!< Port A, Pin 11 */
    IO_PA_12    =   12,     /*!< Port A, Pin 12 */
    IO_PA_13    =   13,     /*!< Port A, Pin 13 */
    IO_PA_14    =   14,     /*!< Port A, Pin 14 */
    IO_PA_15    =   15,     /*!< Port A, Pin 15 */

    IO_PB_0     =   16,     /*!< Port B, Pin 0  */
    IO_PB_1     =   17,     /*!< Port B, Pin 1  */
    IO_PB_2     =   18,     /*!< Port B, Pin 2  */
    IO_PB_3     =   19,     /*!< Port B, Pin 3  */
    IO_PB_4     =   20,     /*!< Port B, Pin 4  */
    IO_PB_5     =   21,     /*!< Port B, Pin 5  */
    IO_PB_6     =   22,     /*!< Port B, Pin 6  */
    IO_PB_7     =   23,     /*!< Port B, Pin 7  */
    IO_PB_8     =   24,     /*!< Port B, Pin 8  */
    IO_PB_9     =   25,     /*!< Port B, Pin 9  */
    IO_PB_10    =   26,     /*!< Port B, Pin 10 */
    IO_PB_11    =   27,     /*!< Port B, Pin 11 */
    IO_PB_12    =   28,     /*!< Port B, Pin 12 */
    IO_PB_13    =   29,     /*!< Port B, Pin 13 */
    IO_PB_14    =   30,     /*!< Port B, Pin 14 */
    IO_PB_15    =   31,     /*!< Port B, Pin 15 */

    IO_PC_0     =   32,     /*!< Port C, Pin 0  */
    IO_PC_1     =   33,     /*!< Port C, Pin 1  */
    IO_PC_2     =   34,     /*!< Port C, Pin 2  */
    IO_PC_3     =   35,     /*!< Port C, Pin 3  */
    IO_PC_4     =   36,     /*!< Port C, Pin 4  */
    IO_PC_5     =   37,     /*!< Port C, Pin 5  */
    IO_PC_6     =   38,     /*!< Port C, Pin 6  */
    IO_PC_7     =   39,     /*!< Port C, Pin 7  */
    IO_PC_8     =   40,     /*!< Port C, Pin 8  */
    IO_PC_9     =   41,     /*!< Port C, Pin 9  */
    IO_PC_10    =   42,     /*!< Port C, Pin 10 */
    IO_PC_11    =   43,     /*!< Port C, Pin 11 */
    IO_PC_12    =   44,     /*!< Port C, Pin 12 */
    IO_PC_13    =   45,     /*!< Port C, Pin 13 */
    IO_PC_14    =   46,     /*!< Port C, Pin 14 */
    IO_PC_15    =   47,     /*!< Port C, Pin 15 */

    IO_PD_0     =   48,     /*!< Port D, Pin 0  */
    IO_PD_1     =   49,     /*!< Port D, Pin 1  */
    IO_PD_2     =   50,     /*!< Port D, Pin 2  */
    IO_PD_3     =   51,     /*!< Port D, Pin 3  */
    IO_PD_4     =   52,     /*!< Port D, Pin 4  */
    IO_PD_5     =   53,     /*!< Port D, Pin 5  */
    IO_PD_6     =   54,     /*!< Port D, Pin 6  */
    IO_PD_7     =   55,     /*!< Port D, Pin 7  */
    IO_PD_8     =   56,     /*!< Port D, Pin 8  */
    IO_PD_9     =   57,     /*!< Port D, Pin 9  */
    IO_PD_10    =   58,     /*!< Port D, Pin 10 */
    IO_PD_11    =   59,     /*!< Port D, Pin 11 */
    IO_PD_12    =   60,     /*!< Port D, Pin 12 */
    IO_PD_13    =   61,     /*!< Port D, Pin 13 */
    IO_PD_14    =   62,     /*!< Port D, Pin 14 */
    IO_PD_15    =   63,     /*!< Port D, Pin 15 */

    IO_PE_0     =   64,     /*!< Port E, Pin 0  */
    IO_PE_1     =   65,     /*!< Port E, Pin 1  */
    IO_PE_2     =   66,     /*!< Port E, Pin 2  */
    IO_PE_3     =   67,     /*!< Port E, Pin 3  */
    IO_PE_4     =   68,     /*!< Port E, Pin 4  */
    IO_PE_5     =   69,     /*!< Port E, Pin 5  */
    IO_PE_6     =   70,     /*!< Port E, Pin 6  */
    IO_PE_7     =   71,     /*!< Port E, Pin 7  */
    IO_PE_8     =   72,     /*!< Port E, Pin 8  */
    IO_PE_9     =   73,     /*!< Port E, Pin 9  */
    IO_PE_10    =   74,     /*!< Port E, Pin 10 */
    IO_PE_11    =   75,     /*!< Port E, Pin 11 */
    IO_PE_12    =   76,     /*!< Port E, Pin 12 */
    IO_PE_13    =   77,     /*!< Port E, Pin 13 */
    IO_PE_14    =   78,     /*!< Port E, Pin 14 */
    IO_PE_15    =   79      /*!< Port E, Pin 15 */
#endif /* IO_PACKAGE_LQFP100 */

#if defined(IO_PACKAGE_LQFP144) || defined(IO_PACKAGE_LQFP176)
    ,
    IO_PF_0     =   80,     /*!< Port F, Pin 0  */
    IO_PF_1     =   81,     /*!< Port F, Pin 1  */
    IO_PF_2     =   82,     /*!< Port F, Pin 2  */
    IO_PF_3     =   83,     /*!< Port F, Pin 3  */
    IO_PF_4     =   84,     /*!< Port F, Pin 4  */
    IO_PF_5     =   85,     /*!< Port F, Pin 5  */
    IO_PF_6     =   86,     /*!< Port F, Pin 6  */
    IO_PF_7     =   87,     /*!< Port F, Pin 7  */
    IO_PF_8     =   88,     /*!< Port F, Pin 8  */
    IO_PF_9     =   89,     /*!< Port F, Pin 9  */
    IO_PF_10    =   90,     /*!< Port F, Pin 10 */
    IO_PF_11    =   91,     /*!< Port F, Pin 11 */
    IO_PF_12    =   92,     /*!< Port F, Pin 12 */
    IO_PF_13    =   93,     /*!< Port F, Pin 13 */
    IO_PF_14    =   94,     /*!< Port F, Pin 14 */
    IO_PF_15    =   95,     /*!< Port F, Pin 15 */

    IO_PG_0     =   96,     /*!< Port G, Pin 0  */
    IO_PG_1     =   97,     /*!< Port G, Pin 1  */
    IO_PG_2     =   98,     /*!< Port G, Pin 2  */
    IO_PG_3     =   99,     /*!< Port G, Pin 3  */
    IO_PG_4     =   100,    /*!< Port G, Pin 4  */
    IO_PG_5     =   101,    /*!< Port G, Pin 5  */
    IO_PG_6     =   102,    /*!< Port G, Pin 6  */
    IO_PG_7     =   103,    /*!< Port G, Pin 7  */
    IO_PG_8     =   104,    /*!< Port G, Pin 8  */
    IO_PG_9     =   105,    /*!< Port G, Pin 9  */
    IO_PG_10    =   106,    /*!< Port G, Pin 10 */
    IO_PG_11    =   107,    /*!< Port G, Pin 11 */
    IO_PG_12    =   108,    /*!< Port G, Pin 12 */
    IO_PG_13    =   109,    /*!< Port G, Pin 13 */
    IO_PG_14    =   110,    /*!< Port G, Pin 14 */
    IO_PG_15    =   111,    /*!< Port G, Pin 15 */

    IO_PH_0     =   112,    /*!< Port H, Pin 0  */
    IO_PH_1     =   113     /*!< Port H, Pin 1  */
#endif /* IO_PACKAGE_LQFP144 */

#if defined(IO_PACKAGE_LQFP176)
    ,
    IO_PH_2     =   114,    /*!< Port H, Pin 2  */
    IO_PH_3     =   115,    /*!< Port H, Pin 3  */
    IO_PH_4     =   116,    /*!< Port H, Pin 4  */
    IO_PH_5     =   117,    /*!< Port H, Pin 5  */
    IO_PH_6     =   118,    /*!< Port H, Pin 6  */
    IO_PH_7     =   119,    /*!< Port H, Pin 7  */
    IO_PH_8     =   120,    /*!< Port H, Pin 8  */
    IO_PH_9     =   121,    /*!< Port H, Pin 9  */
    IO_PH_10    =   122,    /*!< Port H, Pin 10 */
    IO_PH_11    =   123,    /*!< Port H, Pin 11 */
    IO_PH_12    =   124,    /*!< Port H, Pin 12 */
    IO_PH_13    =   125,    /*!< Port H, Pin 13 */
    IO_PH_14    =   126,    /*!< Port H, Pin 14 */
    IO_PH_15    =   127,    /*!< Port H, Pin 15 */

    IO_PI_0     =   128,    /*!< Port I, Pin 0  */
    IO_PI_1     =   129,    /*!< Port I, Pin 1  */
    IO_PI_2     =   130,    /*!< Port I, Pin 2  */
    IO_PI_3     =   131,    /*!< Port I, Pin 3  */
    IO_PI_4     =   132,    /*!< Port I, Pin 4  */
    IO_PI_5     =   133,    /*!< Port I, Pin 5  */
    IO_PI_6     =   134,    /*!< Port I, Pin 6  */
    IO_PI_7     =   135,    /*!< Port I, Pin 7  */
    IO_PI_8     =   136,    /*!< Port I, Pin 8  */
    IO_PI_9     =   137,    /*!< Port I, Pin 9  */
    IO_PI_10    =   138,    /*!< Port I, Pin 10 */
    IO_PI_11    =   139     /*!< Port I, Pin 11 */
#endif /* IO_PACKAGE_LQFP176 */
} IO_PORTS_e;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* IO_PACKAGE_CFG_H_ */
