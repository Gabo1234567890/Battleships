#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
// #include <windows.h>
//   SYSTEM
//   SLEEP

#define BOARD_SIDE_SIZE 10

#define NUMBER_OF_SMALL_SHIPS 4
#define SMALL_SHIP_LENGTH 2

#define NUMBER_OF_MID_SHIPS 3
#define MID_SHIP_LENGTH 3

#define NUMBER_OF_BIG_SHIPS 2
#define BIG_SHIP_LENGTH 4

#define NUMBER_OF_GIGA_SHIPS 1
#define GIGA_SHIP_LENGTH 6

#define SHIP_SIGN '#'
#define SEA_SIGN '~'
#define MISSED_SHIP_SIGN 'O'
#define HIT_SHIP_SIGN 'X'
#define DESTROYED_SHIP_SIGN '@'

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
    int x;
    int y;
    char aftermath[10];
    struct ReplayNode *next;
};

typedef struct
{
    struct ReplayNode *head;
    int size;
} ReplayList;

// WORKS
static struct ReplayNode *createnode(int player, int x, int y, char *aftermath)
{
    struct ReplayNode *newnode = (struct ReplayNode *)malloc(sizeof(struct ReplayNode));
    if (newnode == NULL)
    {
        printf("Error allocating memory \n");
        exit(1);
    }
    newnode->player = player;
    newnode->x = x;
    newnode->y = y;
    strcpy(newnode->aftermath, aftermath);

    return newnode;
}

// WORKS
ReplayList init()
{
    ReplayList list = {size : 0, head : NULL};
    return list;
}

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

// WORKS
void pushfront(ReplayList *list, int player, int x, int y, char *aftermath)
{
    struct ReplayNode *newnode = createnode(player, x, y, aftermath);
    newnode->next = list->head;
    list->head = newnode;
    list->size++;
}

// WORKS
void push(ReplayList *list, int index, int player, int x, int y, char *aftermath)
{
    if (index == 0)
    {
        pushfront(list, player, x, y, aftermath);
        return;
    }

    struct ReplayNode *prev = get(list, index - 1);
    struct ReplayNode *newnode = createnode(player, x, y, aftermath);
    newnode->next = prev->next;
    prev->next = newnode;
    list->size++;
}

// WORKS
void pushback(ReplayList *list, int player, int x, int y, char *aftermath)
{
    push(list, list->size, player, x, y, aftermath);
}

// WORKS
void printReplayList(ReplayList *list)
{
    struct ReplayNode *currentnode = list->head;
    while (currentnode != NULL)
    {
        printf("Player %d shoots at X:%d and Y:%d and %s \n", currentnode->player, currentnode->x, currentnode->y, currentnode->aftermath);
        currentnode = currentnode->next;
    }
    printf("\n");
}

void copyReplayList(ReplayList *dest, ReplayList *src)
{
    struct ReplayNode *currentnode = src->head;
    while (currentnode != NULL)
    {
        pushback(dest, currentnode->player, currentnode->x, currentnode->y, currentnode->aftermath);
        currentnode = currentnode->next;
    }
}

// WORKS
char **setSea()
{
    char **board = (char **)malloc(BOARD_SIDE_SIZE * sizeof(char *));
    for (char i = 0; i < BOARD_SIDE_SIZE; ++i)
    {
        board[i] = (char *)malloc(BOARD_SIDE_SIZE * sizeof(char));
    }
    for (int i = 0; i < BOARD_SIDE_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIDE_SIZE; ++j)
        {
            board[i][j] = SEA_SIGN;
        }
    }
    return board;
}

// WORKS
void printBoard(char **board)
{
    bool flag = false;
    for (int i = -1; i < BOARD_SIDE_SIZE; i++)
    {
        if (i >= 0)
        {
            printf(" %d ", i);
        }
        else
        {
            printf("x/y");
        }
        for (int j = 0; j < BOARD_SIDE_SIZE; j++)
        {
            if (i == -1)
            {
                printf(" %d ", j);
            }
            else
            {
                printf(" %c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// WORKS
bool isWithinBoard(Point p)
{
    return (p.x >= 0 && p.x < BOARD_SIDE_SIZE && p.y >= 0 && p.y < BOARD_SIDE_SIZE);
}

// WORKS
bool isWithinBoardCoordinate(int coord)
{
    return (coord >= 0 && coord < BOARD_SIDE_SIZE);
}

// WORKS
bool isShipAdjacent(char **board, Point p, int shipLength, bool horizontal)
{
    int xOffset[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int yOffset[] = {0, 0, 1, -1, 1, -1, 1, -1};
    for (int i = 0; i < shipLength; i++)
    {
        Point current;
        current.x = horizontal ? p.x : p.x + i;
        current.y = horizontal ? p.y + i : p.y;

        if (isWithinBoard(current) && board[current.x][current.y] == SHIP_SIGN)
        {
            return false;
        }

        for (int j = 0; j < 8; j++)
        {
            Point adjacent;
            adjacent.x = current.x + xOffset[j];
            adjacent.y = current.y + yOffset[j];

            if (isWithinBoard(adjacent) && board[adjacent.x][adjacent.y] == SHIP_SIGN)
            {
                return false;
            }
        }
    }
    return true;
}

// WORKS
int isShipValid(char **board, Ship s, int shipLength)
{
    bool res = true;
    if (s.direction == 'u' && isWithinBoardCoordinate(s.p.x - shipLength + 1))
    {
        while (s.p.x >= 0)
        {
            res = isShipAdjacent(board, s.p, shipLength, false) && res;
            s.p.x--;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    if (s.direction == 'd' && isWithinBoardCoordinate(s.p.x + shipLength - 1))
    {
        while (s.p.x < BOARD_SIDE_SIZE)
        {
            res = isShipAdjacent(board, s.p, shipLength, false) && res;
            s.p.x++;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    if (s.direction == 'l' && isWithinBoardCoordinate(s.p.y - shipLength + 1))
    {
        while (s.p.y >= 0)
        {
            res = isShipAdjacent(board, s.p, shipLength, true) && res;
            s.p.y--;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    if (s.direction == 'r' && isWithinBoardCoordinate(s.p.y + shipLength - 1))
    {
        while (s.p.y < BOARD_SIDE_SIZE)
        {
            res = isShipAdjacent(board, s.p, shipLength, true) && res;
            s.p.y++;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }

    return -1;
}

// WORKS
int setShip(char **board, Ship s, int shipLength)
{
    int res = isShipValid(board, s, shipLength);
    if (res < 0)
    {
        return res;
    }
    printf("Successfully put ship!\n");
    if (s.direction == 'u')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.x--;
        }
        return 1;
    }
    if (s.direction == 'd')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.x++;
        }
        return 1;
    }
    if (s.direction == 'r')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.y++;
        }
        return 1;
    }
    if (s.direction == 'l')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.y--;
        }
        return 1;
    }
}

// WORKS
void enterCoordinates(Point *p)
{
    p->x = -1;
    p->y = -1;
    while (!isWithinBoardCoordinate(p->x))
    {
        printf("Enter x:\n");
        printf("x must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
        printf("x: ");
        scanf("%d", &p->x);
        getchar();
    }
    while (!isWithinBoardCoordinate(p->y))
    {
        printf("Enter y:\n");
        printf("y must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
        printf("y: ");
        scanf("%d", &p->y);
        getchar();
    }
}

// WORKS
int removeShip(char **board, Point p)
{
    int shipLength = 0;
    Point newP;
    newP.x = p.x;
    newP.y = p.y;
    if (isWithinBoard(newP) && board[newP.x][newP.y] == SHIP_SIGN)
    {
        board[newP.x][newP.y] = SEA_SIGN;
        shipLength++;
    }

    newP.x = p.x - 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            board[newP.x][newP.y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        newP.x--;
    }

    newP.x = p.x + 1;
    newP.y = p.y;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            board[newP.x][newP.y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        newP.x++;
    }

    newP.x = p.x;
    newP.y = p.y + 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            board[newP.x][newP.y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        newP.y++;
    }

    newP.x = p.x;
    newP.y = p.y - 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            board[newP.x][newP.y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        newP.y--;
    }

    return shipLength;
}

// WORKS
bool moveShip(char **board, Ship s1, Ship s2)
{
    int shipLength = removeShip(board, s1.p);
    if (shipLength == 0)
    {
        return false;
    }

    int dRes = isShipValid(board, s2, shipLength);
    if (dRes == -1)
    {
        printf("Can't put ship there! You are out of the board!\n");
        return false;
    }
    else if (dRes == -2)
    {
        printf("\nThere is another ship next to this one! You can't put it here!\n");
        return false;
    }

    setShip(board, s2, shipLength);

    return true;
}

// WORKS
void **setOneKindShips(char **board, int numberOfShips, int shipLength, bool first, bool last)
{
    Ship s;
    char move = '-';
    for (int i = 0; i < numberOfShips; i++)
    {
        printf("\nShip %d:\n", i + 1);
        s.p.x = -1;
        s.p.y = -1;
        s.direction = '-';
        int choice = 0;
        printBoard(board);
        while (choice != 1 && choice != 2)
        {
            if (first)
            {
                choice = 1;
                first = false;
            }
            else if(move == 'y')
            {
                choice = 2;
            }
            else
            {
                printf("1. Put ship\n");
                printf("2. Move ship\n");
                printf("Choice: ");
                scanf("%d", &choice);
                getchar();
            }

            switch (choice)
            {
            case 1:
                enterCoordinates(&s.p);
                while (s.direction != 'u' && s.direction != 'd' && s.direction != 'l' && s.direction != 'r')
                {
                    printf("The direction must be up - u, down - d, left - l, right - r!\n");
                    printf("Enter direction: ");
                    scanf("%c", &s.direction);
                    getchar();
                }

                int validShip = isShipValid(board, s, shipLength);

                if (validShip == -1)
                {
                    printf("Can't put ship there! You are out of the board!\n");
                    i--;
                    // SLEEP
                    sleep(2);
                    // Sleep(2000);
                }
                else if (validShip == -2)
                {
                    printf("\nThere is another ship next to this one! You can't put it here!\n");
                    i--;
                    // SLEEP
                    sleep(2);
                    // Sleep(2000);
                }
                else
                {
                    setShip(board, s, shipLength);
                }
                if(last && i == numberOfShips - 1)
                {
                    // SYSTEM
                    system("clear");
                    // system("cls");
                    printBoard(board);
                    move = '-';
                    while(move != 'y' && move != 'n')
                    {
                        printf("Do you want to move a ship? (yes - y, no - n)\n");
                        scanf("%c", &move);
                        getchar();
                    }
                    if(move == 'y')
                    {
                        i--;
                    }
                }
                break;

            case 2:
                // SYSTEM
                system("clear");
                // system("cls");
                printBoard(board);
                Ship s1;
                while (1)
                {
                    printf("The ship you want to move:\n");
                    enterCoordinates(&s1.p);
                    if (board[s1.p.x][s1.p.y] != SHIP_SIGN)
                    {
                        printf("\nThere is no ship!\n\n");
                    }
                    else
                    {
                        break;
                    }
                }
                while (1)
                {
                    printf("New place of this ship:\n");
                    Ship s2;
                    enterCoordinates(&s2.p);
                    printf("The direction must be up - u, down - d, left - l, right - r!\n");
                    printf("Enter direction: ");
                    scanf("%c", &s2.direction);
                    getchar();
                    if (moveShip(board, s1, s2) == true)
                    {
                        if(move != 'y')
                        {
                            i--;
                        }
                        break;
                    }
                }
                // SYSTEM
                system("clear");
                // system("cls");
                break;

            default:
                break;
            }
        }
    }
}

// WORKS
void setAllShips(char **board)
{
    printf("\nSET GIGA SHIPS (%d)\n", NUMBER_OF_GIGA_SHIPS);
    setOneKindShips(board, NUMBER_OF_GIGA_SHIPS, GIGA_SHIP_LENGTH, true, true);

    // printf("\nSET BIG SHIPS (%d)\n", NUMBER_OF_BIG_SHIPS);
    // setOneKindShips(board, NUMBER_OF_BIG_SHIPS, BIG_SHIP_LENGTH, false, false);

    // printf("\nSET MID SHIPS (%d)\n", NUMBER_OF_MID_SHIPS);
    // setOneKindShips(board, NUMBER_OF_MID_SHIPS, MID_SHIP_LENGTH, false, false);

    // printf("\nSET SMALL SHIPS (%d)\n", NUMBER_OF_SMALL_SHIPS);
    // setOneKindShips(board, NUMBER_OF_SMALL_SHIPS, SMALL_SHIP_LENGTH, false, true);
}

// WORKS
bool isShipDestroyed(char **sea, char **board, Point p)
{
    Point *wholeShipCoordinates = (Point *)malloc(sizeof(Point));
    int size = 1;
    Point newP;
    newP.x = p.x;
    newP.y = p.y;
    bool destroyed = false;
    if (isWithinBoard(newP) && board[newP.x][newP.y] == HIT_SHIP_SIGN)
    {
        destroyed = true;
        wholeShipCoordinates[0].x = newP.x;
        wholeShipCoordinates[0].y = newP.y;
    }

    newP.x = p.x - 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            destroyed = false;
            break;
        }
        else if (board[newP.x][newP.y] == HIT_SHIP_SIGN)
        {
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));

            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            destroyed = true && destroyed;
        }
        else
        {
            break;
        }
        newP.x--;
    }

    newP.x = p.x + 1;
    newP.y = p.y;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            destroyed = false;
            break;
        }
        else if (board[newP.x][newP.y] == HIT_SHIP_SIGN)
        {
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));

            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            destroyed = true && destroyed;
        }
        else
        {
            break;
        }
        newP.x++;
    }

    newP.x = p.x;
    newP.y = p.y + 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            destroyed = false;
            break;
        }
        else if (board[newP.x][newP.y] == HIT_SHIP_SIGN)
        {
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));

            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            destroyed = true && destroyed;
        }
        else
        {
            break;
        }
        newP.y++;
    }

    newP.x = p.x;
    newP.y = p.y - 1;
    while (isWithinBoard(newP))
    {
        if (board[newP.x][newP.y] == SHIP_SIGN)
        {
            destroyed = false;
            break;
        }
        else if (board[newP.x][newP.y] == HIT_SHIP_SIGN)
        {
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));

            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            destroyed = true && destroyed;
        }
        else
        {
            break;
        }
        newP.y--;
    }

    if (destroyed == true)
    {
        for (int i = 0; i < size; i++)
        {
            sea[wholeShipCoordinates[i].x][wholeShipCoordinates[i].y] = DESTROYED_SHIP_SIGN;
            board[wholeShipCoordinates[i].x][wholeShipCoordinates[i].y] = DESTROYED_SHIP_SIGN;
        }
    }

    return destroyed;
}

// WORKS
int isShipHit(char **sea, char **board, Point p, int hits, int *i)
{
    if (board[p.x][p.y] == SHIP_SIGN)
    {
        sea[p.x][p.y] = HIT_SHIP_SIGN;
        board[p.x][p.y] = HIT_SHIP_SIGN;
        (*i)--;
        hits++;
        if (isShipDestroyed(sea, board, p))
        {
            printf("DESTROYED SHIP\n");
        }
    }
    else if(board[p.x][p.y] == HIT_SHIP_SIGN || board[p.x][p.y] == MISSED_SHIP_SIGN || board[p.x][p.y] == DESTROYED_SHIP_SIGN)
    {
        (*i)--;
        return -1;
    }
    else
    {
        sea[p.x][p.y] = MISSED_SHIP_SIGN;
        board[p.x][p.y] = MISSED_SHIP_SIGN;
    }

    return hits;
}

// WORKS
int countShipSigns()
{
    return NUMBER_OF_SMALL_SHIPS * SMALL_SHIP_LENGTH + NUMBER_OF_MID_SHIPS * MID_SHIP_LENGTH + NUMBER_OF_BIG_SHIPS * BIG_SHIP_LENGTH + NUMBER_OF_GIGA_SHIPS * GIGA_SHIP_LENGTH;
}

// 
Point enterAdjacentHit(Point previousHit)
{
    Point newHit;
    newHit.x = previousHit.x;
    newHit.y = previousHit.y;
    char direction = '-';
    while(direction != 'u' && direction != 'd' && direction != 'l' && direction != 'r')
    {
        printf("Chooce direction (up - u, down - d, left - l, right - r): ");
        scanf("%c", &direction);
        getchar();
    }
    if(direction == 'u')
    {
        newHit.x = previousHit.x - 1;
    }
    if(direction == 'd')
    {
        newHit.x = previousHit.x + 1;
    }
    if(direction == 'l')
    {
        newHit.y = previousHit.y - 1;
    }
    if(direction == 'r')
    {
        newHit.y = previousHit.y + 1;
    }

    return newHit;
}

// IN PROCESS
ReplayList gamePvsP(char **board1, char **board2)
{
    char **sea1 = setSea();
    char **sea2 = setSea();
    ReplayList rlist = init();
    Point p;
    int i = 1;
    int hits1 = 0;
    int hits2 = 0;
    bool hit = false;
    printf("Let the game begin!\n");
    while (1)
    {
        if (i % 2 == 1)
        {
            printf("\n------------PLAYER 1-----------\n");
            printf("Enemy's board:\n");
            printBoard(sea1);
            printf("---------------------------------\n");
            printf("Your board:\n");
            printBoard(board1);
            if(!hit)
            {
                enterCoordinates(&p);
            }
            else
            {
                int choice = 0;
                while(choice != 1 && choice != 2)
                {
                    printf("1. Enter coordinates\n");
                    printf("2. Hit adjacent sell\n");
                    printf("Choice: ");
                    scanf("%d", &choice);
                    getchar();
                }
                switch (choice)
                {
                case 1:
                    enterCoordinates(&p);
                    break;

                case 2:
                    p = enterAdjacentHit(p);
                    break;
                
                default:
                    break;
                }
            }
            int newHits1 = isShipHit(sea1, board2, p, hits1, &i);
            if(newHits1 == -1)
            {
                printf("You already hit there!\n");
                hit = false;
            }
            else if (hits1 < newHits1)
            {
                pushback(&rlist, 1, p.x, p.y, "Hits");
                printf("You hit a ship!\n");
                hits1 = newHits1;
                hit = true;
            }
            else
            {
                pushback(&rlist, 1, p.x, p.y, "Misses");
                printf("You missed it!\n");
                hit = false;
            }
        }
        else
        {
            printf("\n-------------PLAYER 2-----------\n");
            printf("Enemy's board:\n");
            printBoard(sea2);
            printf("---------------------------------\n");
            printf("Your board:\n");
            printBoard(board2);
            if(!hit)
            {
                enterCoordinates(&p);
            }
            else
            {
                int choice = 0;
                while(choice != 1 && choice != 2)
                {
                    printf("1. Enter coordinates\n");
                    printf("2. Hit adjacent sell\n");
                    printf("Choice: ");
                    scanf("%d", &choice);
                    getchar();
                }
                switch (choice)
                {
                case 1:
                    enterCoordinates(&p);
                    break;

                case 2:
                    p = enterAdjacentHit(p);
                    break;
                
                default:
                    break;
                }
            }
            int newHits2 = isShipHit(sea2, board1, p, hits2, &i);
            if(newHits2 == -1)
            {
                printf("You already hit there!\n");
                hit = false;
            }
            else if (hits2 < newHits2)
            {
                pushback(&rlist, 2, p.x, p.y, "Hits");
                printf("You hit a ship!\n");
                hits2 = newHits2;
                hit = true;
            }
            else
            {
                pushback(&rlist, 2, p.x, p.y, "Misses");
                printf("You missed it!\n");
                hit = false;
            }
        }
        if (hits1 == /*countShipSigns()*/ 6)
        {
            printf("\n---------PLAYER 1 WINS----------\n");
            return rlist;
        }
        if (hits2 == /*countShipSigns()*/ 6)
        {
            printf("\n---------PLAYER 2 WINS---------\n");
            return rlist;
        }
        i++;
        // SLEEP
        sleep(1);
        // Sleep(1000);
    }
}

char intDirToCharDir(int direction)
{
    if (direction == 1)
    {
        return 'u';
    }
    if (direction == 2)
    {
        return 'd';
    }
    if (direction == 3)
    {
        return 'l';
    }
    return 'r';
}

void setCompBoard(char **compBoard)
{
    Ship s;
    int direction;
    printf("Starting giga ships!\n");
    for (int i = 0; i < NUMBER_OF_GIGA_SHIPS; i++)
    {
        s.p.x = rand() % BOARD_SIDE_SIZE;
        s.p.y = rand() % BOARD_SIDE_SIZE;
        direction = rand() % 4 + 1;
        s.direction = intDirToCharDir(direction);
        if (setShip(compBoard, s, GIGA_SHIP_LENGTH) < 0)
        {
            i--;
            continue;
        }
    }
    printf("Starting big ships!\n");
    for (int i = 0; i < NUMBER_OF_BIG_SHIPS; i++)
    {
        s.p.x = rand() % BOARD_SIDE_SIZE;
        s.p.y = rand() % BOARD_SIDE_SIZE;
        direction = rand() % 4 + 1;
        s.direction = intDirToCharDir(direction);
        if (setShip(compBoard, s, BIG_SHIP_LENGTH) < 0)
        {
            i--;
            continue;
        }
    }
    printf("Starting mid ships!\n");
    for (int i = 0; i < NUMBER_OF_MID_SHIPS; i++)
    {
        s.p.x = rand() % BOARD_SIDE_SIZE;
        s.p.y = rand() % BOARD_SIDE_SIZE;
        direction = rand() % 4 + 1;
        s.direction = intDirToCharDir(direction);
        if (setShip(compBoard, s, MID_SHIP_LENGTH) < 0)
        {
            i--;
            continue;
        }
    }
    printf("Starting small ships!\n");
    for (int i = 0; i < NUMBER_OF_SMALL_SHIPS; i++)
    {
        s.p.x = rand() % BOARD_SIDE_SIZE;
        s.p.y = rand() % BOARD_SIDE_SIZE;
        direction = rand() % 4 + 1;
        s.direction = intDirToCharDir(direction);
        if (setShip(compBoard, s, SMALL_SHIP_LENGTH) < 0)
        {
            i--;
            continue;
        }
    }
}

void dumbComp(Point *hit)
{
    hit->x = rand() % BOARD_SIDE_SIZE;
    hit->y = rand() % BOARD_SIDE_SIZE;
}

void smartComp(Point *hit, Point alrHit, int hitParts)
{
    int direction;
    if (hitParts == 1)
    {
        while (1)
        {
            direction = rand() % 4 + 1;
            if (direction == 1 && isWithinBoardCoordinate(hit->x - 1))
            {
                hit->x--;
                return;
            }
            else if (direction == 2 && isWithinBoardCoordinate(hit->x + 1))
            {
                hit->x++;
                return;
            }
            else if (direction == 3 && isWithinBoardCoordinate(hit->y - 1))
            {
                hit->y--;
                return;
            }
            else if (isWithinBoardCoordinate(hit->y + 1))
            {
                hit->y++;
                return;
            }
        }
    }
    else
    {
        if (hit->x == alrHit.x)
        {
            if (hit->y < alrHit.y)
            {
                if (isWithinBoardCoordinate(hit->y - 1))
                {
                    hit->y--;
                }
                else
                {
                    hit->y = alrHit.y + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoardCoordinate(hit->y + 1))
                {
                    hit->y++;
                }
                else
                {
                    hit->y = alrHit.y - 1;
                }
                return;
            }
        }
        else
        {
            if (hit->x < alrHit.x)
            {
                if (isWithinBoardCoordinate(hit->x - 1))
                {
                    hit->x--;
                }
                else
                {
                    hit->x = alrHit.x + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoardCoordinate(hit->x + 1))
                {
                    hit->x++;
                }
                else
                {
                    hit->x = alrHit.x - 1;
                }
                return;
            }
        }
    }
}

// IN PROCESS
void gamePvsComp(char **playerBoard, char **compBoard)
{
    char **playerSea = setSea();
    char **compSea = setSea();
    bool end = false;
    int turn = 0;
    int playerHits = 0;
    int computerHits = 0;
    bool compFound = false;
    Point hit;
    Point alrHit;
    alrHit.x = -1;
    alrHit.y = -1;
    int numberOfHitParts = 0;
    while (!end)
    {
        printf("Computer board:\n");
        printBoard(compBoard);
        printf("Player's hits:\n");
        printBoard(playerSea);
        printf("Player's board:\n");
        printBoard(playerBoard);
        if (turn % 2 == 0)
        {
            printf("Player's turn:\n");
            enterCoordinates(&hit);
            playerHits = isShipHit(playerSea, compBoard, hit, playerHits, &turn);
        }
        else
        {
            printf("Computer's turn:\n");
            if (!compFound)
            {
                dumbComp(&hit);
            }
            else
            {
                smartComp(&hit, alrHit, numberOfHitParts);
            }
            int newComputerHits = isShipHit(compSea, playerBoard, hit, computerHits, &turn);
            if (newComputerHits == -1)
            {
                continue;
            }
            computerHits = newComputerHits;
            if (isShipDestroyed(compSea, playerBoard, hit))
            {
                compFound = false;
                alrHit.x = -1;
                alrHit.y = -1;
                numberOfHitParts = 0;
                continue;
            }
            else if (playerBoard[hit.x][hit.y] == HIT_SHIP_SIGN)
            {
                compFound = true;
                numberOfHitParts++;
                if (alrHit.x == -1 && alrHit.y == -1)
                {
                    alrHit.x = hit.x;
                    alrHit.y = hit.y;
                }
            }
            else if (playerBoard[hit.x][hit.y] == MISSED_SHIP_SIGN && compFound)
            {
                if (hit.x != alrHit.x)
                {
                    int LorR = rand() % 2 + 1;
                    if (hit.x < alrHit.x)
                    {
                        if (isWithinBoardCoordinate(alrHit.x + 1))
                        {
                            hit.x = alrHit.x + 1;
                        }
                        else if (LorR == 1)
                        {
                            if (isWithinBoardCoordinate(alrHit.y - 1))
                            {
                                hit.y = alrHit.y - 1;
                                hit.x = alrHit.x;
                            }
                        }
                        else
                        {
                            hit.y = alrHit.y + 1;
                            hit.x = alrHit.x;
                        }
                    }
                    else
                    {
                        if (isWithinBoardCoordinate(alrHit.x - 1))
                        {
                            hit.x = alrHit.x - 1;
                        }
                        else if (LorR == 1)
                        {
                            if (isWithinBoardCoordinate(alrHit.y - 1))
                            {
                                hit.y = alrHit.y - 1;
                                hit.x = alrHit.x;
                            }
                        }
                        else
                        {
                            hit.y = alrHit.y + 1;
                            hit.x = alrHit.x;
                        }
                    }
                }
                else
                {
                    int UorD = rand() % 2 + 1;
                    if (hit.y < alrHit.y)
                    {
                        if (isWithinBoardCoordinate(alrHit.y + 1))
                        {
                            hit.y = alrHit.y + 1;
                        }
                        else if (UorD == 1)
                        {
                            if (isWithinBoardCoordinate(alrHit.x - 1))
                            {
                                hit.x = alrHit.x - 1;
                                hit.y = alrHit.y;
                            }
                        }
                        else
                        {
                            hit.x = alrHit.x + 1;
                            hit.y = alrHit.y;
                        }
                    }
                    else
                    {
                        if (isWithinBoardCoordinate(alrHit.y - 1))
                        {
                            hit.y = alrHit.y - 1;
                        }
                        else if (UorD == 1)
                        {
                            if (isWithinBoardCoordinate(alrHit.x - 1))
                            {
                                hit.x = alrHit.x - 1;
                                hit.y = alrHit.y;
                            }
                        }
                        else
                        {
                            hit.x = alrHit.x + 1;
                            hit.y = alrHit.y;
                        }
                    }
                }
                newComputerHits = isShipHit(compSea, playerBoard, hit, computerHits, &turn);
                if (newComputerHits == -1)
                {
                    continue;
                }
                computerHits = newComputerHits;
                if (playerBoard[hit.x][hit.y] == HIT_SHIP_SIGN)
                {
                    numberOfHitParts++;
                    turn++;
                }
            }
        }
        if (playerHits == countShipSigns())
        {
            printf("\n---------PLAYER WINS---------");
            return;
        }
        if (computerHits == countShipSigns())
        {
            printf("\n---------COMPUTER WINS---------");
            return;
        }
        turn++;
        // SLEEP
        sleep(2);
        // Sleep(2000);
        //  SYSTEM
        system("clear");
        // system("cls");
    }
}

// TO DO
void readBoardFromFile(char **board, char *filename)
{
}

// IN PROCCESS
void replay(ReplayList *rlist)
{
    printReplayList(rlist);
}

int main()
{

    ReplayList rlist = init();
    ReplayList templist = init();
    int choice = 0;
    printf("------------BATTLESHIPS------------\n");
    while (1)
    {
        char **board1 = setSea();
        char **board2 = setSea();
        printf("1. Rules\n");
        printf("2. Start game person VS person\n");
        printf("3. Start game person VS computer\n");
        printf("4. Replay\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            printf("--------------------------------------------------RULES----------------------------------------------------\n");
            printf("Battleship is a game for two people where you have to sink all enemy^s ships.\n");
            printf("Both players have to place their ships on board. Every ship is surrounded by water (even on the corners).\n");
            printf("When all ships are placed the game begin. Every player gives a guess in which enemy^s sell has a ship.\n");
            printf("The first player who finds all ships (whole ships) is the winner!\n");
            printf("------------------------------------------------------------------------------------------------------------\n");
            break;

        case 2:
            printf("Player 1 set ships:\n");
            setAllShips(board1);
            printf("Player 2 set ships:\n");
            setAllShips(board2);
            rlist = gamePvsP(board1, board2);
            break;

        case 3:
            printf("Set your ships:\n");
            setAllShips(board1);
            setCompBoard(board2);
            gamePvsComp(board1, board2);
            break;

        case 4:
            replay(&rlist);
            break;

        default:
            break;
        }
    }

    return 0;
}
