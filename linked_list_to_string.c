#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked_list_to_string.h"

// Create a new ReplayNode
static struct ReplayNode *createnode(int player, bool isHit, Point point)
{
    struct ReplayNode *newnode = (struct ReplayNode *)malloc(sizeof(struct ReplayNode));
    if (newnode == NULL)
    {
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
ReplayList init()
{
    ReplayList list = {.head = NULL, .size = 0};
    return list;
}

// Get the node at a specific index
struct ReplayNode *get(ReplayList *list, int index)
{
    if (index < 0 || index >= list->size)
    {
        return NULL;
    }
    struct ReplayNode *currentnode = list->head;
    for (int i = 0; i < index; i++)
    {
        currentnode = currentnode->next;
    }
    return currentnode;
}

// Insert a new node at the front of the list
void pushfront(ReplayList *list, int player, bool isHit, Point point)
{
    struct ReplayNode *newnode = createnode(player, isHit, point);
    newnode->next = list->head;
    list->head = newnode;
    list->size++;
}

// Insert a new node at a specific index
void push(ReplayList *list, int index, int player, bool isHit, Point point)
{
    if (index == 0)
    {
        pushfront(list, player, isHit, point);
        return;
    }
    struct ReplayNode *prev = get(list, index - 1);
    if (prev == NULL)
    {
        return;
    }
    struct ReplayNode *newnode = createnode(player, isHit, point);
    newnode->next = prev->next;
    prev->next = newnode;
    list->size++;
}

// Insert a new node at the back of the list
void pushback(ReplayList *list, int player, bool isHit, Point point)
{
    push(list, list->size, player, isHit, point);
}

// Print the list
void printReplayList(ReplayList *list)
{
    struct ReplayNode *currentnode = list->head;
    while (currentnode != NULL)
    {
        printf("Player %d shoots at X:%d and Y:%d\n", currentnode->player, currentnode->point.x, currentnode->point.y);
        currentnode = currentnode->next;
    }
    printf("\n");
}

// Copy one list to another
void copyReplayList(ReplayList *dest, ReplayList *src)
{
    struct ReplayNode *currentnode = src->head;
    while (currentnode != NULL)
    {
        pushback(dest, currentnode->player, currentnode->isHit, currentnode->point);
        currentnode = currentnode->next;
    }
}

// Create a token from ReplayNode data
char *create_token(struct ReplayNode *node)
{
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d", node->player, node->isHit, node->point.x, node->point.y);
    return strdup(buffer);
}

// Convert a linked list to string representation
char *list_to_string(struct ReplayNode *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    size_t initial_size = 100;
    char *str = (char *)malloc(initial_size * sizeof(char));
    if (str == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    str[0] = '\0';

    struct ReplayNode *temp = head;
    while (temp != NULL)
    {
        char *token = create_token(temp);
        size_t current_length = strlen(str);
        size_t token_length = strlen(token);
        if (current_length + token_length + 2 >= initial_size)
        {
            initial_size *= 2;
            str = realloc(str, initial_size * sizeof(char));
            if (str == NULL)
            {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(1);
            }
        }
        strcat(str, token);
        strcat(str, ";");
        free(token);
        temp = temp->next;
    }

    if (strlen(str) > 0)
    {
        str[strlen(str) - 1] = '\0';
    }

    return str;
}

// Insert data at the end of the generic linked list
void insert_at_end(Node **head, const char *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->data = strdup(data);
    new_node->next = NULL;
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        Node *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Convert string representation back to linked list
struct ReplayNode *string_to_list(const char *str)
{
    if (str == NULL || strlen(str) == 0)
    {
        return NULL;
    }

    struct ReplayNode *head = NULL;
    struct ReplayNode *tail = NULL;
    const char *delimiter = ";";
    char *copy = strdup(str);
    char *token = strtok(copy, delimiter);
    while (token != NULL)
    {
        struct ReplayNode *new_node = (struct ReplayNode *)malloc(sizeof(struct ReplayNode));
        if (new_node == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        sscanf(token, "%d,%d,%d,%d", &new_node->player, &new_node->isHit, &new_node->point.x, &new_node->point.y);
        new_node->next = NULL;
        if (head == NULL)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
        token = strtok(NULL, delimiter);
    }
    free(copy);

    return head;
}

// Free the generic linked list
void free_list(Node *head)
{
    Node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

// Free the ReplayList
void free_replay_list(ReplayList *list)
{
    struct ReplayNode *current = list->head;
    while (current != NULL)
    {
        struct ReplayNode *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}

char *encrypt_linked_list(ReplayList *list)
{
    char *string_to_be_encrypted = list_to_string(list->head);
}

int main()
{
    ReplayList list = init();
    Point p1 = {0, 0};
    Point p2 = {1, 3};
    Point p3 = {2, 5};

    pushback(&list, 1, true, p1);
    pushback(&list, 2, false, p2);
    pushback(&list, 1, true, p3);

    printf("Original ReplayList:\n");
    printReplayList(&list);

    char *str = list_to_string(list.head);
    if (str != NULL)
    {
        printf("String representation: %s\n", str);

        struct ReplayNode *new_head = string_to_list(str);

        printf("Reconstructed ReplayList:\n");
        ReplayList new_list = init();
        new_list.head = new_head;
        new_list.size = list.size;
        printReplayList(&new_list);

        free(str);
        free_replay_list(&new_list);
    }
    else
    {
        fprintf(stderr, "Conversion to string failed\n");
    }

    free_replay_list(&list);

    return 0;
}
