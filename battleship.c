#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

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
#define MISSED_SHIP_SIGN 'x'
#define HIT_SHIP_SIGN 'O'
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

struct ReplayNode {
  int player;
  int x;
  int y;
  char aftermath[10];
  struct ReplayNode * next;
};

typedef struct {
  struct ReplayNode * head;
  int size;
} ReplayList;

//WORKS
static struct ReplayNode * createnode(int player, int x, int y, char *aftermath) {
  struct ReplayNode * newnode = (struct ReplayNode *) malloc(sizeof(struct ReplayNode));
  if (newnode == NULL) {
    printf("Error allocating memory \n");
    exit(1);
  }
  newnode->player = player;
  newnode->x = x;
  newnode->y = y;
  strcpy(newnode->aftermath, aftermath);
  
  return newnode;
}

//WORKS
ReplayList init() {
  ReplayList list = {size: 0, head:NULL};
  return list;
}

struct ReplayNode * get(ReplayList * list, int index) {
  if (index < 0 || index >= list->size) {
    return NULL;
  }

  struct ReplayNode * currentnode = list->head;
  for (int i = 0; i < index; i++) {
    currentnode = currentnode->next;
  }

  return currentnode;
}

//WORKS
void pushfront(ReplayList * list, int player, int x, int y, char *aftermath) {
  struct ReplayNode * newnode = createnode(player, x, y, aftermath);
  newnode->next = list->head;
  list->head = newnode;
  list->size++;
}

//WORKS
void push(ReplayList * list, int index, int player, int x, int y, char *aftermath) {
  if (index == 0) {
    pushfront(list, player, x, y, aftermath);
    return;
  }

  struct ReplayNode * prev = get(list, index-1);
  struct ReplayNode * newnode = createnode(player, x, y, aftermath);
  newnode->next = prev->next;
  prev->next = newnode;
  list->size++;
}

//WORKS
void pushback(ReplayList * list, int player, int x, int y, char *aftermath) {
  push(list, list->size, player, x, y, aftermath);
}

//WORKS
void printReplayList(ReplayList * list) {
  struct ReplayNode * currentnode = list->head;
  while (currentnode != NULL) {
    printf("Player %d shoots at X:%d and Y:%d and %s \n", currentnode->player, currentnode->x, currentnode->y, currentnode->aftermath);
    currentnode = currentnode->next;
  }
  printf("\n");
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
void setShip(char **board, Ship s, int shipLength)
{
    if (s.direction == 'u')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.x--;
        }
        return;
    }
    if (s.direction == 'd')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.x++;
        }
        return;
    }
    if (s.direction == 'r')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.y++;
        }
        return;
    }
    if (s.direction == 'l')
    {
        for (int i = 0; i < shipLength; i++)
        {
            board[s.p.x][s.p.y] = SHIP_SIGN;
            s.p.y--;
        }
        return;
    }
}

// WORKS
bool isWithinBoard(Point p)
{
    return (p.x >= 0 && p.x < BOARD_SIDE_SIZE && p.y >= 0 && p.y < BOARD_SIDE_SIZE);
}

// WORKS
bool isValidPlacement(char **board, Point p, int shipLength, bool horizontal)
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
int checkDirection(char **board, Ship s, int shipLength)
{
    bool res = true;
    if (s.direction == 'u' && s.p.x - shipLength + 1 >= 0)
    {
        res = isValidPlacement(board, s.p, shipLength, false);
        if (res == false)
        {
            return -1;
        }
        return 1;
    }
    if (s.direction == 'd' && s.p.x + shipLength - 1 <= BOARD_SIDE_SIZE - 1)
    {
        res = isValidPlacement(board, s.p, shipLength, false);
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    if (s.direction == 'l' && s.p.y - shipLength + 1 >= 0)
    {
        res = isValidPlacement(board, s.p, shipLength, true);
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    if (s.direction == 'r' && s.p.y + shipLength - 1 <= BOARD_SIDE_SIZE - 1)
    {
        res = isValidPlacement(board, s.p, shipLength, true);
        if (res == false)
        {
            return -2;
        }
        return 1;
    }

    return -1;
}

// WORKS
void enterCoordinates(Point *p)
{
    p->x = -1;
    p->y = -1;
    Point newP;
    newP.x = p->x;
    newP.y = p->y;
    while (!isWithinBoard(newP))
    {
        printf("Enter x: ");
        scanf("%d", &p->x);
        getchar();
        printf("Enter y: ");
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

    int dRes = checkDirection(board, s2, shipLength);
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
char **setOneKindShips(char **board, int numberOfShips, int shipLength)
{
    Ship s;
    for (int i = 0; i < numberOfShips; i++)
    {
        printf("\nShip %d:\n", i + 1);
        s.p.x = -1;
        s.p.y = -1;
        s.direction = '-';
        printBoard(board);
        while (s.p.x < 0 || s.p.x > BOARD_SIDE_SIZE - 1)
        {
            printf("Enter start x: \n");
            printf("x must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
            printf("x: ");
            scanf("%d", &s.p.x);
            getchar();
        }

        while (s.p.y < 0 || s.p.y > BOARD_SIDE_SIZE - 1)
        {
            printf("Enter start y: \n");
            printf("y must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
            printf("y: ");
            scanf("%d", &s.p.y);
            getchar();
        }

        while (s.direction != 'u' && s.direction != 'd' && s.direction != 'l' && s.direction != 'r')
        {
            printf("The direction must be up - u, down - d, left - l, right - r!\n");
            printf("Enter direction: ");
            scanf("%c", &s.direction);
            getchar();
        }

        int direction = checkDirection(board, s, shipLength);

        if (direction == -1)
        {
            printf("Can't put ship there! You are out of the board!\n");
            i--;
            Sleep(2000);
        }
        else if (direction == -2)
        {
            printf("\nThere is another ship next to this one! You can't put it here!\n");
            i--;
            Sleep(2000);
        }
        else
        {
            setShip(board, s, shipLength);
        }

        char choice = '-';

        while (direction != -1 && direction != -2)
        {
            system("cls");
            printBoard(board);
            while (choice != 'y' && choice != 'n')
            {
                printf("Do you want to move a ship? (yes - y, no - n)\n");
                scanf("%c", &choice);
                getchar();
            }
            if (choice == 'n')
            {
                break;
            }
            choice = '-';
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
            bool place = false;
            while (place == false)
            {
                printf("New place of this ship:\n");
                Ship s2;
                enterCoordinates(&s2.p);
                printf("Enter direction: ");
                scanf("%c", &s2.direction);
                getchar();
                if (moveShip(board, s1, s2) == true)
                {
                    place = true;
                }
            }
        }
        system("cls");
    }

    return board;
}

// WORKS
void setAllShips(char **board)
{
    printf("\nSET SMALL SHIPS (%d)\n", NUMBER_OF_SMALL_SHIPS);
    setOneKindShips(board, NUMBER_OF_SMALL_SHIPS, SMALL_SHIP_LENGTH);

    // printf("\nSET MID SHIPS (%d)\n", NUMBER_OF_MID_SHIPS);
    // setOneKindShips(board, NUMBER_OF_MID_SHIPS, MID_SHIP_LENGTH);

    // printf("\nSET BIG SHIPS (%d)\n", NUMBER_OF_BIG_SHIPS);
    // setOneKindShips(board, NUMBER_OF_BIG_SHIPS, BIG_SHIP_LENGTH);

    // printf("\nSET GIGA SHIPS (%d)\n", NUMBER_OF_GIGA_SHIPS);
    // setOneKindShips(board, NUMBER_OF_GIGA_SHIPS, GIGA_SHIP_LENGTH);
}

// WORK
int isHitShip(char **sea, char **board, Point p, int hits, int *i)
{
    if (board[p.x][p.y] == SHIP_SIGN)
    {
        printf("\nYou hit a ship!\n");
        sea[p.x][p.y] = HIT_SHIP_SIGN;
        board[p.x][p.y] = HIT_SHIP_SIGN;
        i--;
        hits++;
    }
    else
    {
        printf("\nYou missed it!\n");
        sea[p.x][p.y] = MISSED_SHIP_SIGN;
        board[p.x][p.y] = MISSED_SHIP_SIGN;
    }
    return hits;
}

int countShipSigns()
{
    return NUMBER_OF_SMALL_SHIPS * SMALL_SHIP_LENGTH + NUMBER_OF_MID_SHIPS * MID_SHIP_LENGTH + NUMBER_OF_BIG_SHIPS * BIG_SHIP_LENGTH + NUMBER_OF_GIGA_SHIPS * GIGA_SHIP_LENGTH;
}

// IN PROCESS
void isShipDestroyed(char **board, Point p)
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
            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));
            destroyed = true;
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
            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));
            destroyed = true;
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
            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));
            destroyed = true;
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
            wholeShipCoordinates[size - 1].x = newP.x;
            wholeShipCoordinates[size - 1].y = newP.y;
            size++;
            wholeShipCoordinates = (Point *)realloc(wholeShipCoordinates, size * sizeof(Point));
            destroyed = true;
        }
        newP.y--;
    }

    if (destroyed == true)
    {
        for (int i = 0; i < size - 1; i++)
        {
            board[wholeShipCoordinates[i].x][wholeShipCoordinates[i].y] = DESTROYED_SHIP_SIGN;
        }
        printf("\nYOU DESTROYED A SHIP!\n");
    }
}

// IN PROCESS
void gamePvsP(char **board1, char **board2)
{
    char **sea1 = setSea();
    char **sea2 = setSea();
    bool end = true;
    Ship s;
    int i = 1;
    int hits1 = 0;
    int hits2 = 0;
    while (!end)
    {
        enterCoordinates(&s.p);
        if (i % 2 == 1)
        {
            printf("\n------------PLAYER 1-----------\n");
            printf("Enemy's board:\n");
            printBoard(sea1);
            printf("---------------------------------\n");
            printf("Your board:\n");
            printBoard(board1);
            hits1 = isHitShip(sea1, board2, s.p, hits1, &i);
            isShipDestroyed(board2, s.p);
        }
        else
        {
            printf("\n-------------PLAYER 2----------\n");
            printf("Enemy's board:\n");
            printBoard(sea2);
            printf("---------------------------------\n");
            printf("Your board:\n");
            printBoard(board2);
            hits2 = isHitShip(sea2, board1, s.p, hits2, &i);
            isShipDestroyed(board1, s.p);
        }
        if (hits1 == countShipSigns())
        {
            printf("\n---------PLAYER 1 WINS---------\n");
            return;
        }
        if (hits2 == countShipSigns())
        {
            printf("\n---------PLAYER 2 WINS---------\n");
            return;
        }
        i++;
    }
}

bool setCompShip(char **board, int shipLength, int startX, int startY, int direction)
{
    if (direction == 1 || direction == 3)
    {
        if (!isValidPlacement(board, startX, startY, 2, false))
        {
            return false;
        }
        else if (direction == 1)
        {
            for (int i = 0; i < shipLength; i++)
            {
                board[startX - i][startY] = SHIP_SIGN;
            }
            return true;
        }
        else
        {
            for (int i = 0; i < shipLength; i++)
            {
                board[startX + i][startY] = SHIP_SIGN;
            }
            return true;
        }
    }
    else
    {
        if (!isValidPlacement(board, startX, startY, 2, true))
        {
            return false;
        }
        else if (direction == 2)
        {
            for (int i = 0; i < shipLength; i++)
            {
                board[startX][startY - i] = SHIP_SIGN;
            }
            return true;
        }
        else
        {
            for (int i = 0; i < shipLength; i++)
            {
                board[startX][startY + i] = SHIP_SIGN;
            }
            return true;
        }
    }
}

void setCompBoard(char **compBoard)
{
    int direction, x, y, i;
    for (i = 0; i < 4; i++)
    {
        x = rand() % 10;
        y = rand() % 10;
        direction = rand() % 4 + 1;
        if (!setCompShip(compBoard, 2, x, y, direction))
        {
            i--;
            continue;
        }
    }
    for (i = 0; i < 3; i++)
    {
        x = rand() % 10;
        y = rand() % 10;
        direction = rand() % 4 + 1;
        if (!setCompShip(compBoard, 3, x, y, direction))
        {
            i--;
            continue;
        }
    }
    for (i = 0; i < 2; i++)
    {
        x = rand() % 10;
        y = rand() % 10;
        direction = rand() % 4 + 1;
        if (!setCompShip(compBoard, 4, x, y, direction))
        {
            i--;
            continue;
        }
    }
    i = 0;
    while (i < 1)
    {
        x = rand() % 10;
        y = rand() % 10;
        direction = rand() % 4 + 1;
        if (!setCompShip(compBoard, 6, x, y, direction))
        {
            continue;
        }
        i++;
    }
}

void dumbComp(int *x, int *y)
{
    x = rand() % 10;
    y = rand() % 10;
}

void smartComp(int *x, int *y, int correctX, int correctY, int hitParts)
{
    int direction;
    if (hitParts == 1)
    {
        while (1)
        {
            direction = rand() % 4 + 1;
            if (direction == 1 && isWithinBoard(x - 1, y))
            {
                x--;
                return;
            }
            else if (direction == 2 && isWithinBoard(x, y - 1))
            {
                y--;
                return;
            }
            else if (direction == 3 && isWithinBoard(x + 1, y))
            {
                x++;
                return;
            }
            else if (isWithinBoard(x, y + 1))
            {
                y++;
                return;
            }
        }
    }
    else
    {
        if (x == correctX)
        {
            if (y < correctY)
            {
                if (isWithinBoard(x, y - 1))
                {
                    y--;
                }
                else
                {
                    y = correctY + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoard(x, y + 1))
                {
                    y++;
                }
                else
                {
                    y = correctY - 1;
                }
                return;
            }
        }
        else
        {
            if (x < correctX)
            {
                if (isWithinBoard(x - 1, y))
                {
                    x--;
                }
                else
                {
                    x = correctX + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoard(x + 1, y))
                {
                    x++;
                }
                else
                {
                    x = correctX - 1;
                }
                return;
            }
        }
    }
}

// TO DO
void gamePvsComp(char **playerBoard, char **compBoard)
{
    setCompBoard(compBoard);
    setAllShips(playerBoard);
    bool end = true;
    int turn = 0;
    Ship s;
    int playerHits = 0;
    int computerHits = 0;
    bool compFound = false;
    char **playerSea = setSea();
    int hitX, hitY;
    int alrHitX = -1;
    int alrHitY = -1;
    int numberOfHitParts = 0;
    while (!end)
    {
        printf("Player's board:\n");
        printBoard(playerBoard);
        printf("Player's hits:\n");
        printBoard(playerSea);
        if (turn % 2 == 0)
        {
            printf("Player's turn:\n");
            enterCoordinates(&s);
            playerHits = hitShip(compBoard, s.x, s.y, playerHits, &turn);
        }
        else
        {
            printf("Computer's turn:\n");
            if (!compFound)
            {
                dumbComp(&hitX, &hitY);
            }
            else
            {
                smartComp(&hitX, &hitY, alrHitX, alrHitY, numberOfHitParts);
            }
            computerHits = hitShip(playerBoard, hitX, hitY, computerHits, &turn);
            if (playerBoard[hitX][hitY] == HIT_SHIP_SIGN)
            {
                compFound = true;
                numberOfHitParts++;
                if (alrHitX == -1 && alrHitY == -1)
                {
                    alrHitX = hitX;
                    alrHitY = hitY;
                }
            }
            if (playerBoard[hitX][hitY] == MISSED_SHIP_SIGN && compFound)
            {
                if (hitX < alrHitX)
                {
                    hitX = alrHitX + 1;
                }
                else
                {
                    hitX = alrHitX - 1;
                }
                if (hitY < alrHitY)
                {
                    hitY = alrHitY + 1;
                }
                else
                {
                    hitY = alrHitY - 1;
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
        system("cls");
    }
}

//IN PROCCESS
void replay(){
    ReplayList rlist = init();
    pushback(&rlist, 1, 6, 3, "Misses");
    pushback(&rlist, 2, 5, 7, "Hits");
    pushback(&rlist, 2, 5, 8, "Misses");
    pushback(&rlist, 1, 3, 0, "Hits");
    pushback(&rlist, 1, 3, 1, "Misses");
    printReplayList(&rlist);
}

int main()
{
    char **board1 = setSea();
    char **board2 = setSea();

    int choice = 0;
    printf("------------BATTLESHIP------------\n");
    while (1)
    {
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
            gamePvsP(board1, board2);
            break;

        case 3:
            printf("");
            break;
        
        case 4:
            replay();
            break;

        default:
            break;
        }
    }

    return 0;
}
