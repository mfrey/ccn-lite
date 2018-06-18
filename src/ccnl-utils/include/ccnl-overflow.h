/**
 * @addtogroup CCNL-utils
 * @{
 *
 * @file ccnl-overflow.h
 * @brief Provides macros for detecting integer overflows
 *
 * Copyright (C) 2018 Michael Frey, MSA Safety
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef CCNL_OVERFLOW_H
#define CCNL_OVERFLOW_H

/**
 * @brief Checks if two integers can be multiplied without causing an 
 * integer overflow.
 * 
 * @note This macro definition makes use of GCCs/CLANGs builtin functions
 * for detecting integer overflows.
 *
 * @param[in] a The first operand of the operation
 * @param[in] b The second operand of the operation
 *
 * @return True if an overflow would be triggered, false otherwise
 */
#define INT_MULT_OVERFLOW_P(a, b) \
   __builtin_mul_overflow_p (a, b, (__typeof__ ((a) * (b))) 0)

/**
 * @brief Checks if two integers can be added without causing an 
 * integer overflow.
 * 
 * @note This macro definition makes use of GCCs/CLANGs builtin functions
 * for detecting integer overflows.
 *
 * @param[in] a The first operand of the operation
 * @param[in] b The second operand of the operation
 *
 * @return True if an overflow would be triggered, false otherwise
 */
#define INT_ADD_OVERFLOW_P(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)

#endif 
/** @} */
