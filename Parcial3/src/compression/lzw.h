#ifndef LZW_H
#define LZW_H

#include <stddef.h>
#include <sys/types.h>

ssize_t lzw_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity);
ssize_t lzw_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity);

#endif // LZW_H
