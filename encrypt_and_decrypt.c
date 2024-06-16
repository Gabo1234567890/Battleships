#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Struct to hold public and private keys
typedef struct {
    unsigned long pub_key;
    unsigned long priv_key;
} KeysPair;

// Function to check if a number is prime
bool is_prime(unsigned long num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (unsigned long i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

// Function to find the next prime number greater than or equal to num
unsigned long next_prime(unsigned long num) {
    while (!is_prime(num)) {
        num++;
    }
    return num;
}





// Function to generate public and private keys from a word
KeysPair generate_public_and_private_keys(const char *word) {
     long hash1 = 0;
     long hash2 = 0;
    int word_length = strlen(word);
    int half_length = word_length / 2;
    
    // Calculate hash1 from the first half of the word
    for (int i = 0; i < half_length; i++) {
        hash1 = hash1  + word[i];
    }

    // Calculate hash2 from the second half of the word
    printf("%d",half_length);
    for (int i = half_length - 1; i < word_length; i++) {
        hash2 = hash2  + word[i];
    }

    KeysPair result;
    result.pub_key = next_prime(hash1);
    result.priv_key = next_prime(hash2);

    return result;
}

// Function to compute the greatest common divisor
long gcd(long a, long b) {
    while (b != 0) {
        long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to perform modular exponentiation
long mod_exp(long base, long exp, long mod) {
    long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to generate a public exponent e
long generate_e(long phi) {
    long e;
    srand(time(NULL));
    do {
        e = rand() % (phi - 2) + 2; // Ensure e is in range 2 to phi-1
    } while (gcd(e, phi) != 1);
    return e;
}

// Function to compute the modular inverse of e modulo phi
long mod_inverse(long e, long phi) {
    long m0 = phi, t, q;
    long x0 = 0, x1 = 1;
    if (phi == 1) {
        return 0;
    }
    while (e > 1) {
        q = e / phi;
        t = phi;
        phi = e % phi, e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) {
        x1 += m0;
    }
    return x1;
}

int main() {
    char word[100];
    printf("Enter a word to generate keys: ");
    scanf("%99s", word);

    KeysPair keys_pair = generate_public_and_private_keys(word);

    unsigned long p = keys_pair.pub_key;
    unsigned long q = keys_pair.priv_key;

    unsigned long n = p * q;
    unsigned long phi = (p - 1) * (q - 1);

    unsigned long e = generate_e(phi);
    unsigned long d = mod_inverse(e, phi);

    printf("Public Key: (%lu, %lu)\n", e, n);
    printf("Private Key: (%lu, %lu)\n", d, n);

    unsigned long message = 65; // Example message
    unsigned long encrypted = mod_exp(message, e, n);
    unsigned long decrypted = mod_exp(encrypted, d, n);

    printf("Original Message: %lu\n", message);
    printf("Encrypted Message: %lu\n", encrypted);
    printf("Decrypted Message: %lu\n", decrypted);

    return 0;
}
