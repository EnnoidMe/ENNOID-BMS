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
 * @file    foxmath.c
 * @author  foxBMS Team
 * @date    18.01.2018 (date of creation)
 * @ingroup DRIVERS
 * @prefix  MATH
 *
 * @brief   mathlib function implementations
 *
 */

/*================== Includes ===============================================*/
#include "foxmath.h"

/*================== Macros and Definitions =================================*/

/*================== Static Constant and Variable Definitions ===============*/

/*================== Extern Constant and Variable Definitions ===============*/

/*================== Static Function Prototypes =============================*/

/*================== Static Function Implementations ========================*/

/*================== Extern Function Implementations ========================*/
float MATH_Abs(float value) {
    if (value >= 0)
        return value;
    else
        return -value;
}


float MATH_Pow(float base,int exponent) {
    float value = 1.0;
    if (exponent > 0) {
        while (exponent != 0) {
                value *= base;
                --exponent;
        }
        return value;
    } else if (exponent < 0) {
        while (exponent != 0) {
                value *= base;
                ++exponent;
        }
        return 1.0/value;
    } else if (exponent == 0) {
        return value;
    } else {
        return 1.0;
    }
}


float MATH_exp(float value) {
    float temp = 1.0;
    float fact = 1.0;
    float result = 1.0;
    for(int i=1; i<=100; i++) {
        fact = fact*i;
        temp = temp * value;
        result = result + ((float)temp/(float)fact);
    }
    return result;
}


float MATH_sin(float value) {
    float degree = 0.0;
    float sinx = 0.0;
    float powerseven = 0.0;
    float powerfive = 0.0;
    float powerthree = 0.0;
    degree = (value * MATH_PI)/180.0;
    powerthree = (degree * degree * degree);
    powerfive = (powerthree * degree * degree);
    powerseven = (powerfive * degree * degree);

    sinx = (degree - (powerthree/6.0) + (powerfive/120.0) - (powerseven/5040.0));
    return sinx;
}


float MATH_cos(float value) {
    float degree = 0;
    float cosx = 0;
    float powertwo = 0;
    float powerfour = 0;
    float powersix = 0;

    degree = (value * MATH_PI)/180.0;
    powertwo = (degree * degree);
    powerfour = (powertwo * degree * degree);
    powersix = (powerfour * degree * degree);

    cosx = (1.0 - (powertwo/2.0) + (powerfour/24.0) - (powersix/720.0));
    return cosx;
}


float MATH_sqroot(float value) {

    float root = 0;
    float last = 0;
    float diff = 1;
    root = value/3;

    if (value <= 0)
        return 0;

    while (diff > MINDIFF || diff < -MINDIFF) {
        last = root;
        root = (root + (value/root))/2.0;
        diff = root - last;
    }
    return root;
}


float MATH_tan(float x) {
    float result;
    float tmpSin = MATH_sin(x);
    float tmpPow = MATH_Pow(tmpSin,2);
    float tmp = MATH_sqroot(1 - tmpPow);
    result = MATH_sin(x)/tmp;
    return result;
}


float MATH_ln(float value) {
    float old_sum = 0.0;
    float xmlxpl = (value - 1) / (value + 1);
    float xmlxpl_2 = xmlxpl * xmlxpl;
    float denom = 1.0;
    float frac = xmlxpl;
    float term = frac;
    float sum = term;
    if (value > 0) {
        while (sum != old_sum) {
            old_sum = sum;
            denom += 2.0;
            frac *= xmlxpl_2;
            sum += frac / denom;
        }
        return 2.0 * sum;
    } else {
        return 0;
    }
}


float MATH_log10( float value ) {
    return MATH_ln(value) / LN10;
}
