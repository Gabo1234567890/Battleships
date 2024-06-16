#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct for the linked list node
typedef struct Node {
    char* data; // Change to char* for strings
    struct Node* next;
} Node;

// Function to create a new node
Node* create_node(const char* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    // Allocate memory for data and copy string
    new_node->data = strdup(data);
    new_node->next = NULL;
    return new_node;
}

// Function to insert a node at the end of the linked list
void insert_at_end(Node** head, const char* data) {
    Node* new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Function to print the linked list
void print_list(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%s ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

// Function to convert linked list to string
char* list_to_string(Node* head) {
    if (head == NULL) {
        return NULL;
    }

    // Calculate length of the linked list
    int length = 0;
    Node* temp = head;
    while (temp != NULL) {
        length += strlen(temp->data) + 1; // Add 1 for semicolon
        temp = temp->next;
    }

    // Allocate memory for the string representation (+1 for null terminator)
    char* str = (char*)malloc(length * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    str[0] = '\0'; // Initialize empty string

    // Copy elements of the linked list to the string
    temp = head;
    while (temp != NULL) {
        strcat(str, temp->data);
        strcat(str, ";"); // Add semicolon after each node's data
        temp = temp->next;
    }

    // Remove trailing semicolon if there is at least one node
    if (strlen(str) > 0) {
        str[strlen(str) - 1] = '\0';
    }

    return str;
}

// Function to construct linked list from string
Node* string_to_list(const char* str) {
    if (str == NULL || strlen(str) == 0) {
        return NULL;
    }

    Node* head = NULL;
    Node* tail = NULL;
    const char* delimiter = ";";

    // Tokenize the string based on delimiter
    char* token = strtok((char*)str, delimiter);
    while (token != NULL) {
        insert_at_end(&head, token);
        token = strtok(NULL, delimiter);
    }

    return head;
}

// Function to free memory allocated for linked list
void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data); // Free data allocated for each node
        free(temp);       // Free node itself
    }
}

int main() {
    // Example linked list creation
    Node* head = NULL;
    insert_at_end(&head, "Hello");
    insert_at_end(&head, "world");
    insert_at_end(&head, "of");
    insert_at_end(&head, "strings");

    // Print original linked list
    printf("Original Linked List: ");
    print_list(head);

    // Convert linked list to string with semicolon separator
    char* str = list_to_string(head);
    if (str != NULL) {
        printf("String representation: %s\n", str);

        // Construct linked list from string (removing semicolons)
        Node* new_head = string_to_list(str);

        // Print reconstructed linked list
        printf("Reconstructed Linked List: ");
        print_list(new_head);

        // Free memory allocated for string and reconstructed list
        free(str);
        free_list(new_head);
    } else {
        fprintf(stderr, "Conversion to string failed\n");
    }

    // Free memory allocated for original linked list
    free_list(head);

    return 0;
}
