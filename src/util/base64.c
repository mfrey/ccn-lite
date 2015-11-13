// base64.c
// from stackoverflow.com, user RYYST

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static char
encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                    'w', 'x', 'y', 'z', '0', '1', '2', '3',
                    '4', '5', '6', '7', '8', '9', '+', '/'};

static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void
base64_build_decoding_table(void) {

    decoding_table = (char *) malloc(256);
    int i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


char
*base64_encode(const char *data,
               size_t input_length,
               size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3) + 1;

    char *encoded_data = (char *) malloc(*output_length);
    memset(encoded_data, '\0', *output_length);
    if (encoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; (unsigned int) i < input_length;) {

        uint32_t octet_a = (unsigned int) i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = (unsigned int) i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = (unsigned int) i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 2 - i] = '=';

    return encoded_data;
}


unsigned char
*base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    uint32_t i, j;
    if (decoding_table == NULL)
        base64_build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = (unsigned char *) malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : (uint32_t) decoding_table[(uint32_t) data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : (uint32_t) decoding_table[(uint32_t) data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : (uint32_t) decoding_table[(uint32_t) data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : (uint32_t) decoding_table[(uint32_t) data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

void
base64_cleanup(void) {
    free(decoding_table);
}

// eof
