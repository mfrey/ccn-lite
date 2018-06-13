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

void base64_build_decoding_table(void);

char *base64_encode(const char *data,
               size_t input_length,
               size_t *output_length);


unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);

void base64_cleanup(void);

#endif // EOF
/** @} */
