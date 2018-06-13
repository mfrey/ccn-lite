// base64.c
// from stackoverflow.com, user RYYST

/**
 * @addtogroup CCNL-utils
 * @{
 */

#ifndef BASE64_H
#define BASE64_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Initializes the base64 decoding table
 */
void base64_build_decoding_table(void);

/**
 * @brief Encodes a string \p data into a base64 string
 *
 * @param[in] data The data to encode
 * @param[in] input_length The size of the data to encode
 * @param[out] output_length The size of the encoded date
 *
 * @return Upon success, a base64 encoded representation of \p data 
 */
char *base64_encode(const char *data,
               size_t input_length,
               size_t *output_length);


/**
 * @brief Decodes a string \p data into a base64 string
 *
 * @param[in] data The data to decode
 * @param[in] input_length The size of the data to decode
 * @param[out] output_length The size of the decoded date
 * 
 * @return Upon success, the base64 decoded \p data 
 */
unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);

/**
 *
 *
 */
void base64_cleanup(void);

#endif // EOF
/** @} */
