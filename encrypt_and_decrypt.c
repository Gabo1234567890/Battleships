#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "encrypt_and_decrypt.h"

// Function to check if a number is prime
bool is_prime(unsigned long num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (unsigned long i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

// Function to find the next prime number greater than or equal to num
unsigned long next_prime(unsigned long num)
{
    if (num <= 1)
        return 2;
    while (!is_prime(num))
    {
        num++;
    }
    return num;
}

// Function to generate public and private keys from a word
KeysPair generate_public_and_private_keys(const char *word)
{
    long hash1 = 0;
    long hash2 = 0;
    int word_length = strlen(word);
    int half_length = word_length / 2;

    // Calculate hash1 from the first half of the word
    for (int i = 0; i < half_length; i++)
    {
        hash1 += word[i];
    }

    // Calculate hash2 from the second half of the word
    for (int i = half_length; i < word_length; i++)
    {
        hash2 += word[i];
    }

    KeysPair result;
    result.pub_key = next_prime(hash1);
    result.priv_key = next_prime(hash2);

    return result;
}

// Function to compute the greatest common divisor
long gcd(long a, long b)
{
    while (b != 0)
    {
        long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to perform modular exponentiation
long mod_exp(long base, long exp, long mod)
{
    long result = 1;
    base = base % mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to generate a public exponent e
long generate_e(long phi)
{
    long e;
    srand(time(NULL));
    do
    {
        e = rand() % (phi - 2) + 2; // Ensure e is in range 2 to phi-1
    } while (gcd(e, phi) != 1);
    return e;
}

// Function to compute the modular inverse of e modulo phi
long mod_inverse(long e, long phi)
{
    long m0 = phi, t, q;
    long x0 = 0, x1 = 1;
    if (phi == 1)
    {
        return 0;
    }
    while (e > 1)
    {
        q = e / phi;
        t = phi;
        phi = e % phi, e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
    {
        x1 += m0;
    }
    return x1;
}

// Function to encrypt a string using RSA
long *rsa_encrypt_string(const char *word, KeysPair keys_pair)
{
    int word_length = strlen(word);
    unsigned long p = keys_pair.pub_key;
    unsigned long q = keys_pair.priv_key;

    unsigned long n = p * q;
    unsigned long phi = (p - 1) * (q - 1);

    unsigned long e = generate_e(phi);
    unsigned long d = mod_inverse(e, phi);

    long *encrypted_numbers = malloc(sizeof(long) * word_length);
    if (encrypted_numbers == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < word_length; i++)
    {
        long number_representation = (long)word[i]; // Convert character to its ASCII value
        printf("%d/n", number_representation);
        encrypted_numbers[i] = mod_exp(number_representation, e, n);
    }
    return encrypted_numbers;
}

int find_array_length(long numbers[])
{
    int length = 0;
    while (numbers[length] != '\0')
    {
        length++;
    }
    return length;
}

// Function to decrypt an array of RSA encrypted numbers
char *rsa_decrypt_string(long numbers[], KeysPair keys_pair)
{
    unsigned long p = keys_pair.pub_key;
    unsigned long q = keys_pair.priv_key;

    unsigned long n = p * q;
    unsigned long phi = (p - 1) * (q - 1);
    unsigned long e = generate_e(phi);
    unsigned long d = mod_inverse(e, phi);

    int num_length = find_array_length(numbers); // Find the length of the numbers array

    char *decrypted_message = malloc(num_length + 1); // +1 for null terminator
    if (decrypted_message == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < num_length; i++)
    {
        long decrypted_number = mod_exp(numbers[i], d, n);
        printf("%d", decrypted_message);
        decrypted_message[i] = (char)decrypted_number;
    }
    decrypted_message[num_length] = '\0'; // Null terminate the string

    return decrypted_message;
}

// int main()
// {
//     char word[100] = "kolko";

//     KeysPair keys_pair = generate_public_and_private_keys(word);
//     printf("%d %d lll         ", keys_pair.priv_key, keys_pair.pub_key);
//     char word2[] = "1,1,0,0;2,0,1,3;1,1,2,5";
//     long *encrypted_numbers = rsa_encrypt_string(word2, keys_pair);

//     if (encrypted_numbers == NULL)
//     {
//         fprintf(stderr, "Encryption failed.\n");
//         return 1;
//     }

//     // Print encrypted numbers
//     printf("Encrypted numbers:\n");
//     for (int i = 0; i < strlen(word2); i++)
//     {
//         printf("%ld  ", encrypted_numbers[i]);
//     }
//     printf("\n");

//     char *decrypted_message = rsa_decrypt_string(encrypted_numbers, keys_pair);
//     if (decrypted_message == NULL)
//     {
//         fprintf(stderr, "Decryption failed.\n");
//         return 1;
//     }

//     printf("Decrypted Message: %s\n", decrypted_message);

//     free(encrypted_numbers); // Free memory allocated in rsa_encrypt_string
//     free(decrypted_message); // Free memory allocated in rsa_decrypt_string

//     return 0;
// }
