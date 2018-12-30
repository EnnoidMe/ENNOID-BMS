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
 * @file    foxmath.h
 * @author  foxBMS Team
 * @date    18.01.2018 (date of creation)
 * @ingroup DRIVERS
 * @prefix  MATH
 *
 * @brief   math library for often used math functions
 *
 * @details This header file contains the different math function declarations.
 *          Currently the following functions are supported:
 *          - Min
 *          - Max
 *          - Abs
 *          - Slope
 *          - Linear interpolation
 *
 */

#ifndef FOXMATH_H_
#define FOXMATH_H_

/*================== Includes ===============================================*/
#include "general.h"

/*================== Macros and Definitions =================================*/
#define MINDIFF     2.25e-308
#define LN10        2.30258509299f
#define MATH_PI     3.14159265359f
/*================== Extern Constant and Variable Declarations ==============*/

/*================== Extern Function Prototypes =============================*/
/**
 * @brief   returns the absolute value
 */
extern float MATH_Abs(float value);

/**
 * @brief   calculate the tangent for the provided value(in degrees)
 * @param   value (type: float)
 * @return  float
 */
extern float MATH_tan(float value);

/**
 * @brief   calculate the sine for the provided value(in degrees)
 * @param   value (type: float)
 * @return  float
 */
extern float MATH_sin(float value);

/**
 * @brief   calculate the cosine for the provided value(in degrees)
 * @param   value (type: float)
 * @return  float
 */
extern float MATH_cos(float value);

/**
 * @brief   calculate the Natural logarithm for the provided value
 * @param   value (type: float)
 * @return  float
 */
extern float MATH_ln(float value);

/**
 * @brief   calculate the base 10 logarithm for the provided value
 * @param   value (type: float)
 * @return  float
 */
extern float MATH_log10(float value);

/**
 * @brief   calculate the power to the exponent for the provided base
 * @param   base (type: float)
 * @param   exponent (type: int)
 * @return  float
 */
extern float MATH_Pow(float base,int exponent);

/**
 * @brief   calculate the exponential of provided value
 * @param   base (type: float)
 * @return  float
 */
extern float MATH_exp(float value);

/**
 * @brief   calculate the square root of the provided value
 * @param   base (type: float)
 * @return  float
 */
extern float MATH_sqroot(float value);


#endif /* FOXMATH_H_ */
