#ifndef DES_H
#define DES_H

#include <stddef.h>
#include <sys/types.h>

ssize_t des_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);
ssize_t des_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);

#endif // DES_H
