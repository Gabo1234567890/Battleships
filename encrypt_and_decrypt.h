#ifndef E_A_D_H
#define E_A_D_H

// Struct to hold public and private keys
typedef struct
{
    unsigned long pub_key;
    unsigned long priv_key;
} KeysPair;

bool is_prime(unsigned long num);
unsigned long next_prime(unsigned long num);
KeysPair generate_public_and_private_keys(const char *word);
long gcd(long a, long b);
long mod_exp(long base, long exp, long mod);
long generate_e(long phi);
long mod_inverse(long e, long phi);
long *rsa_encrypt_string(const char *word, KeysPair keys_pair);
int find_array_length(long numbers[]);
char *rsa_decrypt_string(long numbers[], KeysPair keys_pair);

#endif