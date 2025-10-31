#ifndef RLE_H
#define RLE_H

#include <stddef.h>
#include <sys/types.h>

ssize_t rle_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity);
ssize_t rle_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity);

#endif // RLE_H
