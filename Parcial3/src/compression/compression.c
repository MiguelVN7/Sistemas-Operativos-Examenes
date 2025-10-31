#include "compression.h"
#include "rle.h"
#include "huffman.h"
#include "lzw.h"
#include "../utils/error_handler.h"
#include <string.h>

ssize_t compress_data(const unsigned char* input, size_t input_size,
                     FileBuffer* output, CompressionAlgorithm algorithm) {
    if (input == NULL || output == NULL) {
        return -1;
    }

    if (output->capacity < input_size * 2) {
        if (resize_file_buffer(output, input_size * 2) < 0) {
            return -1;
        }
    }

    ssize_t result = -1;

    switch (algorithm) {
        case COMP_RLE:
            result = rle_compress(input, input_size, output->data, output->capacity);
            break;
        case COMP_HUFFMAN:
            result = huffman_compress(input, input_size, output->data, output->capacity);
            break;
        case COMP_LZW:
            result = lzw_compress(input, input_size, output->data, output->capacity);
            break;
        default:
            LOG_ERROR(ERROR_COMPRESSION_FAILED, SEVERITY_ERROR, "Unknown algorithm");
            return -1;
    }

    return result;
}

ssize_t decompress_data(const unsigned char* input, size_t input_size,
                       FileBuffer* output, CompressionAlgorithm algorithm) {
    if (input == NULL || output == NULL) {
        return -1;
    }

    if (output->capacity < input_size * 4) {
        if (resize_file_buffer(output, input_size * 4) < 0) {
            return -1;
        }
    }

    ssize_t result = -1;

    switch (algorithm) {
        case COMP_RLE:
            result = rle_decompress(input, input_size, output->data, output->capacity);
            break;
        case COMP_HUFFMAN:
            result = huffman_decompress(input, input_size, output->data, output->capacity);
            break;
        case COMP_LZW:
            result = lzw_decompress(input, input_size, output->data, output->capacity);
            break;
        default:
            LOG_ERROR(ERROR_DECOMPRESSION_FAILED, SEVERITY_ERROR, "Unknown algorithm");
            return -1;
    }

    return result;
}
