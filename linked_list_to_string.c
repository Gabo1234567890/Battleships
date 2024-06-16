#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

Node* create_node(const char* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->data = strdup(data);
    new_node->next = NULL;
    return new_node;
}

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

void print_list(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%s ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

char* create_token(Node* node) {
    return strdup(node->data);
}

char* list_to_string(Node* head) {
    if (head == NULL) {
        return NULL;
    }

    size_t initial_size = 100;
    char* str = (char*)malloc(initial_size * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    str[0] = '\0'; 

    Node* temp = head;
    while (temp != NULL) {
        char* token = create_token(temp);
        size_t current_length = strlen(str);
        size_t token_length = strlen(token);
        if (current_length + token_length + 1 >= initial_size) {
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

Node* string_to_list(const char* str) {
    if (str == NULL || strlen(str) == 0) {
        return NULL;
    }

    Node* head = NULL;
    Node* tail = NULL;
    const char* delimiter = ";";
    char* token = strtok((char*)str, delimiter);
    while (token != NULL) {
        insert_at_end(&head, token);
        token = strtok(NULL, delimiter);
    }

    return head;
}

void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data); 
        free(temp);       
    }
}

int main() {
    Node* head = NULL;
    insert_at_end(&head, "Hello");
    insert_at_end(&head, "world");
    insert_at_end(&head, "of");
    insert_at_end(&head, "strings");

    printf("Original Linked List: ");
    print_list(head);

    char* str = list_to_string(head);
    if (str != NULL) {
        printf("String representation: %s\n", str);

        Node* new_head = string_to_list(str);

        printf("Reconstructed Linked List: ");
        print_list(new_head);

        free(str);
        free_list(new_head);
    } else {
        fprintf(stderr, "Conversion to string failed\n");
    }

    free_list(head);

    return 0;
}
