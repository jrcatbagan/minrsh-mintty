/*
 * File: aes.h
 * 
 * 
 * Description: AES-128 encryption and decryption adapted for minrsh/minrshd.
 */

#ifndef AES_H
#define AES_H

/* 'state' is the data to encrypt
 */
extern "C"
{
void aes_encrypt(unsigned char *state, unsigned char *key);

/* 'state' is the data to decrypt
 */
void aes_decrypt(unsigned char *state, unsigned char *key);
}
#endif /* AES_H */
