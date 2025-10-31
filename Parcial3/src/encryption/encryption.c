#include "encryption.h"
#include "vigenere.h"
#include "des.h"
#include "aes_simple.h"
#include "../utils/error_handler.h"
#include <string.h>

ssize_t encrypt_data(const unsigned char* input, size_t input_size,
                    FileBuffer* output, EncryptionAlgorithm algorithm,
                    const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL) {
        return -1;
    }

    if (output->capacity < input_size + 256) {
        if (resize_file_buffer(output, input_size + 256) < 0) {
            return -1;
        }
    }

    ssize_t result = -1;

    switch (algorithm) {
        case ENC_VIGENERE:
            result = vigenere_encrypt(input, input_size, output->data, key, key_len);
            break;
        case ENC_DES:
            result = des_encrypt(input, input_size, output->data, key, key_len);
            break;
        case ENC_AES:
            result = aes_encrypt(input, input_size, output->data, key, key_len);
            break;
        default:
            LOG_ERROR(ERROR_ENCRYPTION_FAILED, SEVERITY_ERROR, "Unknown algorithm");
            return -1;
    }

    return result;
}

ssize_t decrypt_data(const unsigned char* input, size_t input_size,
                    FileBuffer* output, EncryptionAlgorithm algorithm,
                    const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL) {
        return -1;
    }

    if (output->capacity < input_size) {
        if (resize_file_buffer(output, input_size) < 0) {
            return -1;
        }
    }

    ssize_t result = -1;

    switch (algorithm) {
        case ENC_VIGENERE:
            result = vigenere_decrypt(input, input_size, output->data, key, key_len);
            break;
        case ENC_DES:
            result = des_decrypt(input, input_size, output->data, key, key_len);
            break;
        case ENC_AES:
            result = aes_decrypt(input, input_size, output->data, key, key_len);
            break;
        default:
            LOG_ERROR(ERROR_DECRYPTION_FAILED, SEVERITY_ERROR, "Unknown algorithm");
            return -1;
    }

    return result;
}
