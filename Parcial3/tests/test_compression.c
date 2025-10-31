#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/compression/rle.h"
#include "../src/compression/huffman.h"
#include "../src/compression/lzw.h"
#include "../src/utils/error_handler.h"

/**
 * @file test_compression.c
 * @brief Pruebas unitarias para módulos de compresión
 */

void test_rle_basic() {
    printf("Testing RLE basic compression...\n");

    const char* input = "AAABBBCCCDDDD";
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* compressed = rle_compress((unsigned char*)input, input_size, &output_size);
    assert(compressed != NULL);
    assert(output_size > 0);

    printf("  Input size: %zu bytes\n", input_size);
    printf("  Compressed size: %zu bytes\n", output_size);

    size_t decompressed_size = 0;
    unsigned char* decompressed = rle_decompress(compressed, output_size, &decompressed_size);
    assert(decompressed != NULL);
    assert(decompressed_size == input_size);
    assert(memcmp(decompressed, input, input_size) == 0);

    printf("  ✓ RLE basic test passed\n");

    free(compressed);
    free(decompressed);
}

void test_rle_no_repetition() {
    printf("Testing RLE with no repetition...\n");

    const char* input = "ABCDEFGH";
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* compressed = rle_compress((unsigned char*)input, input_size, &output_size);
    assert(compressed != NULL);

    // En el peor caso, cada byte diferente da 2 bytes (valor + count=1)
    printf("  Input size: %zu bytes\n", input_size);
    printf("  Compressed size: %zu bytes\n", output_size);

    size_t decompressed_size = 0;
    unsigned char* decompressed = rle_decompress(compressed, output_size, &decompressed_size);
    assert(decompressed != NULL);
    assert(decompressed_size == input_size);
    assert(memcmp(decompressed, input, input_size) == 0);

    printf("  ✓ RLE no repetition test passed\n");

    free(compressed);
    free(decompressed);
}

void test_huffman_placeholder() {
    printf("Testing Huffman (placeholder)...\n");

    const char* input = "This is a test string for Huffman encoding";
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* compressed = huffman_compress((unsigned char*)input, input_size, &output_size);

    if (compressed != NULL) {
        printf("  Input size: %zu bytes\n", input_size);
        printf("  Compressed size: %zu bytes\n", output_size);
        printf("  ⚠ Huffman using placeholder implementation\n");
        free(compressed);
    } else {
        printf("  ⚠ Huffman compression returned NULL\n");
    }
}

void test_lzw_placeholder() {
    printf("Testing LZW (placeholder)...\n");

    const char* input = "TOBEORNOTTOBEORTOBEORNOT";
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* compressed = lzw_compress((unsigned char*)input, input_size, &output_size);

    if (compressed != NULL) {
        printf("  Input size: %zu bytes\n", input_size);
        printf("  Compressed size: %zu bytes\n", output_size);
        printf("  ⚠ LZW using placeholder implementation\n");
        free(compressed);
    } else {
        printf("  ⚠ LZW compression returned NULL\n");
    }
}

int main() {
    printf("\n=== Compression Module Tests ===\n\n");

    test_rle_basic();
    test_rle_no_repetition();
    test_huffman_placeholder();
    test_lzw_placeholder();

    printf("\n=== All compression tests completed ===\n\n");

    return 0;
}
