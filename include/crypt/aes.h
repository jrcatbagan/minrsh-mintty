/*
 * File: aes.h
 * 
 * 
 * Description: AES-128 encryption and decryption adapted for minrsh/minrshd.
 */

#ifndef AES_H
#define AES_H

void aes_encrypt(unsigned char *state, unsigned char *key);
void aes_decrypt(unsigned char *state, unsigned char *key);

#endif /* AES_H */
