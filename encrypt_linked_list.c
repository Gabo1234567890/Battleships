#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Define Point and Ship structs
typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Ship {
    Point p;
    char direction;
} Ship;

// Define ReplayNode and ReplayList structs
struct ReplayNode {
    int player;
    bool isHit;
    Point point;
    struct ReplayNode *next;
};

typedef struct {
    struct ReplayNode *head;
    int size;
} ReplayList;

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
    if (num <= 1) return 2;
    while (!is_prime(num)) {
        num++;
    }
    return num;
}

// Struct to hold public and private keys
typedef struct {
    unsigned long pub_key;
    unsigned long priv_key;
} KeysPair;

// Function to generate public and private keys from a word
KeysPair generate_public_and_private_keys(const char *word) {
    long hash1 = 0;
    long hash2 = 0;
    int word_length = strlen(word);
    int half_length = word_length / 2;
    
    for (int i = 0; i < half_length; i++) {
        hash1 += word[i];
    }

    for (int i = half_length; i < word_length; i++) {
        hash2 += word[i];
    }

    KeysPair result;
    result.pub_key = next_prime(hash1);
    result.priv_key = next_prime(hash2);

    return result;
}

// Function to compute the greatest common divisor
long gcd(long a, long b) {
    while (b != 0) {
        long temp = b;
        b = a % b;
        a = temp;
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

// Function to encrypt a string using RSA
long *rsa_encrypt_string(const char *word, KeysPair keys_pair) {
    int word_length = strlen(word);
    unsigned long p = keys_pair.pub_key;
    unsigned long q = keys_pair.priv_key;
    
    unsigned long n = p * q;
    unsigned long phi = (p - 1) * (q - 1);

    unsigned long e = generate_e(phi);
    unsigned long d = mod_inverse(e, phi);

    long *encrypted_numbers = malloc(sizeof(long) * word_length);
    if (encrypted_numbers == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < word_length; i++) {
        long number_representation = (long)word[i]; // Convert character to its ASCII value
        encrypted_numbers[i] = mod_exp(number_representation, e, n);
    }
    return encrypted_numbers;
}

// Function to find the length of an array
int find_array_length(long numbers[]) {
    int length = 0;
    while (numbers[length] != '\0') {
        length++;
    }
    return length;
}

// Function to decrypt an array of RSA encrypted numbers
char *rsa_decrypt_string(long numbers[], KeysPair keys_pair) {
    unsigned long p = keys_pair.pub_key;
    unsigned long q = keys_pair.priv_key;

    unsigned long n = p * q;
    unsigned long phi = (p - 1) * (q - 1);
    unsigned long e = generate_e(phi);
    unsigned long d = mod_inverse(e, phi);

    int num_length = find_array_length(numbers); // Find the length of the numbers array

    char *decrypted_message = malloc(num_length + 1); // +1 for null terminator
    if (decrypted_message == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < num_length; i++) {
        long decrypted_number = mod_exp(numbers[i], d, n);
        decrypted_message[i] = (char)decrypted_number;
    }
    decrypted_message[num_length] = '\0'; // Null terminate the string

    return decrypted_message;
}

// Create a new ReplayNode
static struct ReplayNode *createnode(int player, bool isHit, Point point) {
    struct ReplayNode *newnode = (struct ReplayNode *)malloc(sizeof(struct ReplayNode));
    if (newnode == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    newnode->player = player;
    newnode->isHit = isHit;
    newnode->point = point;
    newnode->next = NULL;
    return newnode;
}

// Initialize a ReplayList
ReplayList init() {
    ReplayList list = { .head = NULL, .size = 0 };
    return list;
}

// Get the node at a specific index
struct ReplayNode *get(ReplayList *list, int index) {
    if (index < 0 || index >= list->size) {
        return NULL;
    }
    struct ReplayNode *currentnode = list->head;
    for (int i = 0; i < index; i++) {
        currentnode = currentnode->next;
    }
    return currentnode;
}

// Insert a new node at the front of the list
void pushfront(ReplayList *list, int player, bool isHit, Point point) {
    struct ReplayNode *newnode = createnode(player, isHit, point);
    newnode->next = list->head;
    list->head = newnode;
    list->size++;
}

// Insert a new node at a specific index
void push(ReplayList *list, int index, int player, bool isHit, Point point) {
    if (index == 0) {
        pushfront(list, player, isHit, point);
        return;
    }
    struct ReplayNode *prev = get(list, index - 1);
    if (prev == NULL) {
        return;
    }
    struct ReplayNode *newnode = createnode(player, isHit, point);
    newnode->next = prev->next;
    prev->next = newnode;
    list->size++;
}

// Insert a new node at the back of the list
void pushback(ReplayList *list, int player, bool isHit, Point point) {
    push(list, list->size, player, isHit, point);
}

// Print the list
void printReplayList(ReplayList *list) {
    struct ReplayNode *currentnode = list->head;
    while (currentnode != NULL) {
        printf("Player %d shoots at X:%d and Y:%d\n", currentnode->player, currentnode->point.x, currentnode->point.y);
        currentnode = currentnode->next;
    }
    printf("\n");
}

// Copy one list to another
void copyReplayList(ReplayList *dest, ReplayList *src) {
    struct ReplayNode *currentnode = src->head;
    while (currentnode != NULL) {
        pushback(dest, currentnode->player, currentnode->isHit, currentnode->point);
        currentnode = currentnode->next;
    }
}

// Create a token from ReplayNode data
char *create_token(struct ReplayNode *node) {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d", node->player, node->isHit, node->point.x, node->point.y);
    return strdup(buffer);
}

// Convert a linked list to string representation
char *list_to_string(struct ReplayNode *head) {
    if (head == NULL) {
        return NULL;
    }

    size_t initial_size = 100;
    char *str = (char *)malloc(initial_size * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    str[0] = '\0';

    struct ReplayNode *temp = head;
    while (temp != NULL) {
        char *token = create_token(temp);
        size_t current_length = strlen(str);
        size_t token_length = strlen(token);
        if (current_length + token_length + 2 >= initial_size) {
            initial_size *= 2;
            str = realloc(str, initial_size * sizeof(char));
            if (str == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(1);
            }
        }
        strcat(str, token);
        strcat(str, ";");
        free(token);
        temp = temp->next;
    }

    if (strlen(str) > 0) {
        str[strlen(str) - 1] = '\0';
    }

    return str;
}

// Convert string representation back to linked list
struct ReplayNode *string_to_list(const char *str) {
    if (str == NULL || strlen(str) == 0) {
        return NULL;
    }

    struct ReplayNode *head = NULL;
    struct ReplayNode *tail = NULL;
    const char *delimiter = ";";
    char *copy = strdup(str);
    char *token = strtok(copy, delimiter);
    while (token != NULL) {
        struct ReplayNode *new_node = (struct ReplayNode *)malloc(sizeof(struct ReplayNode));
        if (new_node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        sscanf(token, "%d,%d,%d,%d", &new_node->player, &new_node->isHit, &new_node->point.x, &new_node->point.y);
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        token = strtok(NULL, delimiter);
    }
    free(copy);

    return head;
}

// Free the ReplayList
void free_replay_list(ReplayList *list) {
    struct ReplayNode *current = list->head;
    while (current != NULL) {
        struct ReplayNode *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}

long * encrypt_linked_list(ReplayList list,KeysPair keys_pair){
    char *str = list_to_string(list.head);
    
        long *encrypted_numbers = rsa_encrypt_string(str, keys_pair);
        if (encrypted_numbers == NULL) {
            fprintf(stderr, "Encryption failed.\n");
            free(str);
            free_replay_list(&list);
            exit( 1 );
        }

}


ReplayList decrypt_linked_list(long encrypted_numbers[], KeysPair keys_pair){
    char *decrypted_message = rsa_decrypt_string(encrypted_numbers, keys_pair);
        if (decrypted_message == NULL) {
            fprintf(stderr, "Decryption failed.\n");
            exit(1);
        }

        // Convert the decrypted string back to ReplayList
        struct ReplayNode *new_head = string_to_list(decrypted_message);
        printf("Reconstructed ReplayList:\n");
        ReplayList new_list = init();
        new_list.head = new_head;
        new_list.size = 0; // FIX
        return new_list;
}


void encrypt_replay(ReplayList list){ // save the string inside a file

    char password[100];

    printf("enter password (must be below 100 in length):");

    scanf("%s",password);
    KeysPair keys_pair = generate_public_and_private_keys(password);

    long * encrypted_ascii_representations = encrypt_linked_list(list,keys_pair);
    int array_length = sizeof(encrypted_ascii_representations) / sizeof(encrypted_ascii_representations[0]);
    FILE *file = fopen("replay.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < array_length; i++) {
        print("%ld\n", encrypted_ascii_representations[i]);
        fprintf(file, "%ld\n", encrypted_ascii_representations[i]);
    }
    fclose(file);


    printf("here are your keys: public -> %d private -> %d", keys_pair.pub_key, keys_pair.priv_key);


}


ReplayList decrypt_replay(KeysPair keys_pair){

}





int main() {
    // Initialize and populate the ReplayList
    ReplayList list = init();
    Point p1 = {0, 0};
    Point p2 = {1, 3};
    Point p3 = {2, 5};

    pushback(&list, 1, true, p1);
    pushback(&list, 2, false, p2);
    pushback(&list, 1, true, p3);

    printf("Original ReplayList:\n");
    printReplayList(&list);
    KeysPair keys_pair = generate_public_and_private_keys("kolko");
    // Convert the ReplayList to string representation
    char *str = list_to_string(list.head);
    if (str != NULL) {
        printf("String representation: %s\n", str);
        long * nums = encrypt_linked_list(list,keys_pair);
        ReplayList new_list = decrypt_linked_list(nums,keys_pair);
        encrypt_replay(list);

        // Decrypt the encrypted numbers back to string
        
        printReplayList(&new_list);

        // Free allocated memory
    } else {
        fprintf(stderr, "Conversion to string failed\n");
    }

    free_replay_list(&list);

    return 0;
}


