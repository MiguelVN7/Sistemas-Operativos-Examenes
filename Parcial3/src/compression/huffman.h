#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>
#include <sys/types.h>

ssize_t huffman_compress(const unsigned char* input, size_t input_size,
                        unsigned char* output, size_t output_capacity);
ssize_t huffman_decompress(const unsigned char* input, size_t input_size,
                          unsigned char* output, size_t output_capacity);

#endif // HUFFMAN_H
