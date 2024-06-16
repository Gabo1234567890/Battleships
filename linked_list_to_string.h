#ifndef L_L_T_S_H
#define L_L_T_S_H

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct Ship
{
    Point p;
    char direction;
} Ship;

struct ReplayNode
{
    int player;
    bool isHit;
    Point point;
    struct ReplayNode *next;
};

typedef struct
{
    struct ReplayNode *head;
    int size;
} ReplayList;

// Node structure for generic linked list
typedef struct Node
{
    char *data;
    struct Node *next;
} Node;

ReplayList init();
struct ReplayNode *get(ReplayList *list, int index);
void pushfront(ReplayList *list, int player, bool isHit, Point point);
void push(ReplayList *list, int index, int player, bool isHit, Point point);
void pushback(ReplayList *list, int player, bool isHit, Point point);
void printReplayList(ReplayList *list);
void copyReplayList(ReplayList *dest, ReplayList *src);
char *create_token(struct ReplayNode *node);
char *list_to_string(struct ReplayNode *head);
void insert_at_end(Node **head, const char *data);
struct ReplayNode *string_to_list(const char *str);
void free_list(Node *head);
void free_replay_list(ReplayList *list);

#endif