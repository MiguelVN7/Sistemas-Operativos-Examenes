#ifndef VIGENERE_H
#define VIGENERE_H

#include <stddef.h>
#include <sys/types.h>

ssize_t vigenere_encrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len);
ssize_t vigenere_decrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len);

#endif // VIGENERE_H
