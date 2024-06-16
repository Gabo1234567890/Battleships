#ifndef STRUCTURES_H
#define STRUCTURES_H

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

#endif