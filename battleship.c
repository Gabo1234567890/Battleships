#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "battleship.h"
#include "linked_list_to_string.c"
#include "encrypt_and_decrypt.c"
// SYSTEM
// SLEEP

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
void clearBoard(char **board)
{
    for (int i = 0; i < BOARD_SIDE_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIDE_SIZE; j++)
        {
            board[i][j] = SEA_SIGN;
        }
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
    int lastCoord = s.p.x - shipLength + 1;
    Point last;
    last.x = lastCoord;
    last.y = s.p.y;
    if (s.direction == 'u' && isWithinBoardCoordinate(lastCoord))
    {
        while (s.p.x >= lastCoord)
        {
            res = isShipAdjacent(board, last, shipLength, false) && res;
            s.p.x--;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    lastCoord = s.p.x + shipLength - 1;
    if (s.direction == 'd' && isWithinBoardCoordinate(lastCoord))
    {
        while (s.p.x < lastCoord)
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
    lastCoord = s.p.y - shipLength + 1;
    last.x = s.p.x;
    last.y = lastCoord;
    if (s.direction == 'l' && isWithinBoardCoordinate(lastCoord))
    {
        while (s.p.y >= lastCoord)
        {
            res = isShipAdjacent(board, last, shipLength, true) && res;
            s.p.y--;
        }
        if (res == false)
        {
            return -2;
        }
        return 1;
    }
    lastCoord = s.p.y + shipLength - 1;
    if (s.direction == 'r' && isWithinBoardCoordinate(lastCoord))
    {
        while (s.p.y < lastCoord)
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
            if (first && i == 0)
            {
                choice = 1;
                first = false;
            }
            else if (move == 'y')
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
                    first = true;
                    i--;
                    // SLEEP
                    // sleep(2);
                    Sleep(2000);
                }
                else if (validShip == -2)
                {
                    printf("\nThere is another ship next to this one! You can't put it here!\n");
                    i--;
                    // SLEEP
                    // sleep(2);
                    Sleep(2000);
                }
                else
                {
                    setShip(board, s, shipLength);
                }
                if (last && i == numberOfShips - 1)
                {
                    // SYSTEM
                    // system("clear");
                    system("cls");
                    printBoard(board);
                    move = '-';
                    while (move != 'y' && move != 'n')
                    {
                        printf("Do you want to move a ship? (yes - y, no - n)\n");
                        scanf("%c", &move);
                        getchar();
                    }
                    if (move == 'y')
                    {
                        i--;
                    }
                }
                break;

            case 2:
                // SYSTEM
                // system("clear");
                system("cls");
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
                        if (move != 'y')
                        {
                            i--;
                        }
                        break;
                    }
                }
                // SYSTEM
                // system("clear");
                system("cls");
                break;

            default:
                break;
            }
        }
    }
}

// WORKS
bool checkBoardFromFile(char **board)
{
    int shipHorizontalLength = 0;
    int shipVerticalLength = 0;
    int currentNumberOfSmallShips = 0;
    int currentNumberOfMidShips = 0;
    int currentNumberOfBigShips = 0;
    int currentNumberOfGigaShips = 0;
    Ship *wholeShipCoordinates = (Ship *)malloc(sizeof(Ship));
    int size = 1;
    Ship s;
    Ship newS;
    for (int i = 0; i < BOARD_SIDE_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIDE_SIZE; j++)
        {
            if (board[i][j] != SEA_SIGN && board[i][j] != SHIP_SIGN)
            {
                printf("There are invalid symbols!\n");
                return false;
            }
            else if (board[i][j] == SHIP_SIGN)
            {
                s.p.x = i;
                s.p.y = j;

                newS.p.x = s.p.x;
                newS.p.y = s.p.y;
                if (isWithinBoard(newS.p) && board[newS.p.x][newS.p.y] == SHIP_SIGN)
                {
                    wholeShipCoordinates[0].p.x = newS.p.x;
                    wholeShipCoordinates[0].p.y = newS.p.y;
                    shipHorizontalLength++;
                    shipVerticalLength++;
                }

                newS.p.x = s.p.x - 1;
                while (isWithinBoard(newS.p))
                {
                    if (board[newS.p.x][newS.p.y] == SHIP_SIGN)
                    {
                        size++;
                        wholeShipCoordinates = (Ship *)realloc(wholeShipCoordinates, size * sizeof(Ship));
                        wholeShipCoordinates[size - 1].p.x = newS.p.x;
                        wholeShipCoordinates[size - 1].p.y = newS.p.y;
                        shipVerticalLength++;
                    }
                    else
                    {
                        break;
                    }
                    newS.p.x--;
                }

                newS.p.x = s.p.x + 1;
                newS.p.y = s.p.y;
                while (isWithinBoard(newS.p))
                {
                    if (board[newS.p.x][newS.p.y] == SHIP_SIGN)
                    {
                        size++;
                        wholeShipCoordinates = (Ship *)realloc(wholeShipCoordinates, size * sizeof(Ship));
                        wholeShipCoordinates[size - 1].p.x = newS.p.x;
                        wholeShipCoordinates[size - 1].p.y = newS.p.y;
                        shipVerticalLength++;
                    }
                    else
                    {
                        break;
                    }
                    newS.p.x++;
                }

                newS.p.x = s.p.x;
                newS.p.y = s.p.y + 1;
                while (isWithinBoard(newS.p))
                {
                    if (board[newS.p.x][newS.p.y] == SHIP_SIGN)
                    {
                        size++;
                        wholeShipCoordinates = (Ship *)realloc(wholeShipCoordinates, size * sizeof(Ship));
                        wholeShipCoordinates[size - 1].p.x = newS.p.x;
                        wholeShipCoordinates[size - 1].p.y = newS.p.y;
                        shipHorizontalLength++;
                    }
                    else
                    {
                        break;
                    }
                    newS.p.y++;
                }

                newS.p.x = s.p.x;
                newS.p.y = s.p.y - 1;
                while (isWithinBoard(newS.p))
                {
                    if (board[newS.p.x][newS.p.y] == HIT_SHIP_SIGN)
                    {
                        size++;
                        wholeShipCoordinates = (Ship *)realloc(wholeShipCoordinates, size * sizeof(Ship));
                        wholeShipCoordinates[size - 1].p.x = newS.p.x;
                        wholeShipCoordinates[size - 1].p.y = newS.p.y;
                        shipHorizontalLength++;
                    }
                    else
                    {
                        break;
                    }
                    newS.p.y--;
                }

                int shipLength = 0;
                if ((shipHorizontalLength > 1 && shipVerticalLength > 1) || shipHorizontalLength > GIGA_SHIP_LENGTH || shipVerticalLength > GIGA_SHIP_LENGTH)
                {
                    printf("Invalid board! Your ships are too close to one another!\n");
                    return false;
                }
                else if (shipHorizontalLength == 1 && shipVerticalLength == 1)
                {
                    printf("You can't have one cell ship!\n");
                    return false;
                }
                else if (shipHorizontalLength == SMALL_SHIP_LENGTH || shipVerticalLength == SMALL_SHIP_LENGTH)
                {
                    currentNumberOfSmallShips++;
                    if (currentNumberOfSmallShips > NUMBER_OF_SMALL_SHIPS)
                    {
                        printf("You put too much small ships!\n");
                        return false;
                    }
                }
                else if (shipHorizontalLength == MID_SHIP_LENGTH || shipVerticalLength == MID_SHIP_LENGTH)
                {
                    currentNumberOfMidShips++;
                    if (currentNumberOfMidShips > NUMBER_OF_MID_SHIPS)
                    {
                        printf("You put too much mid ships!\n");
                        return false;
                    }
                }
                else if (shipHorizontalLength == BIG_SHIP_LENGTH || shipVerticalLength == BIG_SHIP_LENGTH)
                {
                    currentNumberOfBigShips++;
                    if (currentNumberOfBigShips > NUMBER_OF_BIG_SHIPS)
                    {
                        printf("You put too much big ships!\n");
                        return false;
                    }
                }
                else if (shipHorizontalLength == GIGA_SHIP_LENGTH || shipVerticalLength == GIGA_SHIP_LENGTH)
                {
                    currentNumberOfGigaShips++;
                    if (currentNumberOfGigaShips > NUMBER_OF_GIGA_SHIPS)
                    {
                        printf("You put too much giga ships!\n");
                        return false;
                    }
                }

                if (shipHorizontalLength > 1)
                {
                    shipLength = shipHorizontalLength;
                }
                else
                {
                    shipLength = shipVerticalLength;
                }

                for (int i = 0; i < size; i++)
                {
                    isShipValid(board, wholeShipCoordinates[i], shipLength);
                }
            }
        }
    }
}

// WORKS
char **readBoardFromFile(char *filename)
{
    char **board = setSea();
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Can't open file!\n");
        return NULL;
    }
    char line[BOARD_SIDE_SIZE + 2];
    int i = 0;
    while (fgets(line, BOARD_SIDE_SIZE + 2, f) != NULL)
    {
        if (strlen(line) > BOARD_SIDE_SIZE + 1 || i >= BOARD_SIDE_SIZE)
        {
            printf("Ivalid file!\n");
            return NULL;
        }
        strcpy(board[i], line);
        i++;
    }

    return board;
}

// WORKS
void setAllShips(char **board)
{
    int choice = 0;
    while (choice != 1 && choice != 2)
    {
        printf("1. Read from file\n");
        printf("2. Set ships manually\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
    }

    if (choice == 1)
    {
        while (1)
        {
            char filename[20];
            printf("Enter filename: ");
            scanf("%19[^\n]", filename);
            board = readBoardFromFile(filename);
            if (board != NULL)
            {
                break;
            }
            bool valid = checkBoardFromFile(board);
            if (valid == false)
            {
                clearBoard(board);
                setAllShips(board);
                break;
            }
        }
    }

    if (choice == 2)
    {
        printf("\nSET GIGA SHIPS (%d)\n", NUMBER_OF_GIGA_SHIPS);
        setOneKindShips(board, NUMBER_OF_GIGA_SHIPS, GIGA_SHIP_LENGTH, true, true);

        printf("\nSET BIG SHIPS (%d)\n", NUMBER_OF_BIG_SHIPS);
        setOneKindShips(board, NUMBER_OF_BIG_SHIPS, BIG_SHIP_LENGTH, false, false);

        printf("\nSET MID SHIPS (%d)\n", NUMBER_OF_MID_SHIPS);
        setOneKindShips(board, NUMBER_OF_MID_SHIPS, MID_SHIP_LENGTH, false, false);

        printf("\nSET SMALL SHIPS (%d)\n", NUMBER_OF_SMALL_SHIPS);
        setOneKindShips(board, NUMBER_OF_SMALL_SHIPS, SMALL_SHIP_LENGTH, false, true);
    }
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
    if (isWithinBoard(newP) && (board[newP.x][newP.y] == HIT_SHIP_SIGN || board[newP.x][newP.y] == DESTROYED_SHIP_SIGN))
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
        else if (board[newP.x][newP.y] == MISSED_SHIP_SIGN || board[newP.x][newP.y] == SEA_SIGN)
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
        else if (board[newP.x][newP.y] == MISSED_SHIP_SIGN || board[newP.x][newP.y] == SEA_SIGN)
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
        else if (board[newP.x][newP.y] == MISSED_SHIP_SIGN || board[newP.x][newP.y] == SEA_SIGN)
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
        else if (board[newP.x][newP.y] == MISSED_SHIP_SIGN || board[newP.x][newP.y] == SEA_SIGN)
        {
            break;
        }
        newP.y--;
    }

    if (destroyed == true && size > 1)
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
    else if (board[p.x][p.y] == HIT_SHIP_SIGN || board[p.x][p.y] == MISSED_SHIP_SIGN || board[p.x][p.y] == DESTROYED_SHIP_SIGN)
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

// WORKS
Point enterAdjacentHit(Point previousHit)
{
    Point newHit;
    newHit.x = previousHit.x;
    newHit.y = previousHit.y;
    char direction = '-';
    while (direction != 'u' && direction != 'd' && direction != 'l' && direction != 'r')
    {
        printf("Chooce direction (up - u, down - d, left - l, right - r): ");
        scanf("%c", &direction);
        getchar();
    }
    if (direction == 'u')
    {
        newHit.x = previousHit.x - 1;
    }
    if (direction == 'd')
    {
        newHit.x = previousHit.x + 1;
    }
    if (direction == 'l')
    {
        newHit.y = previousHit.y - 1;
    }
    if (direction == 'r')
    {
        newHit.y = previousHit.y + 1;
    }

    return newHit;
}

// WORKS
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
            if (!hit)
            {
                enterCoordinates(&p);
            }
            else
            {
                int choice = 0;
                while (choice != 1 && choice != 2)
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
            if (newHits1 == -1)
            {
                printf("You already hit there!\n");
                hit = false;
            }
            else if (hits1 < newHits1)
            {
                pushback(&rlist, 1, true, p);
                printf("You hit a ship!\n");
                hits1 = newHits1;
                hit = true;
            }
            else
            {
                pushback(&rlist, 1, false, p);
                printf("You missed!\n");
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
            if (!hit)
            {
                enterCoordinates(&p);
            }
            else
            {
                int choice = 0;
                while (choice != 1 && choice != 2)
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
            if (newHits2 == -1)
            {
                printf("You already hit there!\n");
                hit = false;
            }
            else if (hits2 < newHits2)
            {
                pushback(&rlist, 2, true, p);
                printf("You hit a ship!\n");
                hits2 = newHits2;
                hit = true;
            }
            else
            {
                pushback(&rlist, 2, false, p);
                printf("You missed!\n");
                hit = false;
            }
        }
        if (hits1 == countShipSigns())
        {
            pushback(&rlist, true, hits2, p);
            printf("\n---------PLAYER 1 WINS----------\n");
            return rlist;
        }
        if (hits2 == countShipSigns())
        {
            pushback(&rlist, 2, true, p);
            printf("\n---------PLAYER 2 WINS---------\n");
            return rlist;
        }
        i++;
        // SLEEP
        // sleep(1);
        Sleep(1000);
    }
}

// WORKS
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

// WORKS
void setCompBoard(char **compBoard)
{
    Ship s;
    int direction;
    time_t t = time(0);
    for (int i = 0; i < NUMBER_OF_GIGA_SHIPS; i++)
    {
        if (time(0) - t >= 5)
        {
            srand(time(0));
            clearBoard(compBoard);
            setCompBoard(compBoard);
            return;
        }
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
    for (int i = 0; i < NUMBER_OF_BIG_SHIPS; i++)
    {
        if (time(0) - t >= 5)
        {
            srand(time(0));
            clearBoard(compBoard);
            setCompBoard(compBoard);
            return;
        }
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
    for (int i = 0; i < NUMBER_OF_MID_SHIPS; i++)
    {
        if (time(0) - t >= 5)
        {
            srand(time(0));
            clearBoard(compBoard);
            setCompBoard(compBoard);
            return;
        }
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
    for (int i = 0; i < NUMBER_OF_SMALL_SHIPS; i++)
    {
        if (time(0) - t >= 5)
        {
            srand(time(0));
            clearBoard(compBoard);
            setCompBoard(compBoard);
            return;
        }
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

// WORKS
void dumbComp(Point *hit)
{
    hit->x = rand() % BOARD_SIDE_SIZE;
    hit->y = rand() % BOARD_SIDE_SIZE;
}

// WORKS
void smartComp(Point *hit, Point firstHit, Point closestHit, int hitParts, bool missed)
{
    int direction;
    if (hitParts == 1)
    {
        while (1)
        {
            direction = rand() % 4 + 1;
            hit->x = closestHit.x;
            hit->y = closestHit.y;
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
        if (hit->x == firstHit.x)
        {
            if (hit->y < firstHit.y)
            {
                if (isWithinBoardCoordinate(hit->y - 1) && !missed)
                {
                    hit->y--;
                }
                else
                {
                    hit->y = firstHit.y + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoardCoordinate(hit->y + 1) && !missed)
                {
                    hit->y++;
                }
                else
                {
                    hit->y = firstHit.y - 1;
                }
                return;
            }
        }
        else
        {
            if (hit->x < firstHit.x)
            {
                if (isWithinBoardCoordinate(hit->x - 1) && !missed)
                {
                    hit->x--;
                }
                else
                {
                    hit->x = firstHit.x + 1;
                }
                return;
            }
            else
            {
                if (isWithinBoardCoordinate(hit->x + 1) && !missed)
                {
                    hit->x++;
                }
                else
                {
                    hit->x = firstHit.x - 1;
                }
                return;
            }
        }
    }
}

// WORKS
ReplayList gamePvsComp(char **playerBoard, char **compBoard)
{
    ReplayList rlist = init();
    char **playerSea = setSea();
    char **compSea = setSea();
    int turn = 0;
    int playerHits = 0;
    int computerHits = 0;
    bool compFound = false;
    bool missed = false;
    bool hit = false;
    Point playerHit;
    Point computerHit;
    Point closestHit;
    Point firstHit;
    firstHit.x = -1;
    firstHit.y = -1;
    int numberOfHitParts = 0;
    while (1)
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
            if (!hit)
            {
                enterCoordinates(&playerHit);
            }
            else
            {
                int choice = 0;
                while (choice != 1 && choice != 2)
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
                    enterCoordinates(&playerHit);
                    break;

                case 2:
                    playerHit = enterAdjacentHit(playerHit);
                    break;

                default:
                    break;
                }
            }
            int newPlayerHits = isShipHit(playerSea, compBoard, playerHit, playerHits, &turn);
            if (newPlayerHits == -1)
            {
                printf("You already hit there!\n");
            }
            else if (playerHits < newPlayerHits)
            {
                printf("You hit a ship!\n");
                playerHits = newPlayerHits;
                pushback(&rlist, 1, true, playerHit);
                hit = true;
            }
            else
            {
                printf("You missed!\n");
                pushback(&rlist, 1, false, playerHit);
                hit = false;
            }
        }
        else
        {
            printf("Computer's turn:\n");
            if (!compFound)
            {
                dumbComp(&computerHit);
            }
            else
            {
                smartComp(&computerHit, firstHit, closestHit, numberOfHitParts, missed);
            }
            printf("Computer hits at:%d %d\n", computerHit.x, computerHit.y);
            int newComputerHits = isShipHit(compSea, playerBoard, computerHit, computerHits, &turn);
            if (newComputerHits == -1)
            {
                turn++;
                // SYSTEM
                // system("clear");
                system("cls");
                continue;
            }
            if (computerHits == newComputerHits)
            {
                pushback(&rlist, 2, false, computerHit);
            }
            else
            {
                computerHits = newComputerHits;
                pushback(&rlist, 2, true, computerHit);
            }
            if (isShipDestroyed(compSea, playerBoard, computerHit))
            {
                compFound = false;
                firstHit.x = -1;
                firstHit.y = -1;
                numberOfHitParts = 0;
            }
            else if (playerBoard[computerHit.x][computerHit.y] == HIT_SHIP_SIGN)
            {
                compFound = true;
                missed = false;
                numberOfHitParts++;
                closestHit.x = computerHit.x;
                closestHit.y = computerHit.y;
                if (firstHit.x == -1 && firstHit.y == -1)
                {
                    firstHit.x = computerHit.x;
                    firstHit.y = computerHit.y;
                }
            }
            else if (compFound)
            {
                missed = true;
            }
        }
        if (playerHits == countShipSigns())
        {
            // SLEEP
            // sleep(2);
            Sleep(2000);
            // SYSTEM
            // system(clear);
            system("cls");
            pushback(rlist, 1, true, playerHit);
            printf("Computer board:\n");
            printBoard(compBoard);
            printf("Player's hits:\n");
            printBoard(playerSea);
            printf("Player's board:\n");
            printf("\n---------PLAYER WINS---------\n");
            return rlist;
        }
        if (computerHits == countShipSigns())
        {
            // SLEEP
            // sleep(2);
            Sleep(2000);
            // SYSTEM
            // system(clear);
            system("cls");
            pushback(rlist, 2, true, computerHit);
            printf("Computer board:\n");
            printBoard(compBoard);
            printf("Player's hits:\n");
            printBoard(playerSea);
            printf("Player's board:\n");
            printf("\n---------COMPUTER WINS---------\n");
            return rlist;
        }
        turn++;
        // SLEEP
        // sleep(2);
        Sleep(1000);
        //   SYSTEM
        // system("clear");
        system("cls");
    }
}

// WORKS
void replay(ReplayList rlist, char **board1, char **board2)
{
    char **sea1 = setSea();
    char **sea2 = setSea();
    struct ReplayNode *currentnode = rlist.head;
    while (currentnode->next != NULL)
    {
        if (currentnode->player == 1)
        {
            if (currentnode->isHit == true)
            {
                sea1[currentnode->point.x][currentnode->point.y] = HIT_SHIP_SIGN;
                board2[currentnode->point.x][currentnode->point.y] = HIT_SHIP_SIGN;
            }
            else
            {
                sea1[currentnode->point.x][currentnode->point.y] = MISSED_SHIP_SIGN;
                board2[currentnode->point.x][currentnode->point.y] = MISSED_SHIP_SIGN;
            }
        }
        else if (currentnode->player == 2)
        {
            if (currentnode->isHit == true)
            {
                sea2[currentnode->point.x][currentnode->point.y] = HIT_SHIP_SIGN;
                board1[currentnode->point.x][currentnode->point.y] = HIT_SHIP_SIGN;
            }
            else
            {
                sea2[currentnode->point.x][currentnode->point.y] = MISSED_SHIP_SIGN;
                board1[currentnode->point.x][currentnode->point.y] = MISSED_SHIP_SIGN;
            }
        }
        printf("PLAYER 1: \n");
        printBoard(sea2);
        printf("PLAYER 2 \n");
        printBoard(sea1);
        printf("\n");
        currentnode = currentnode->next;
        sleep(1);
    }
}