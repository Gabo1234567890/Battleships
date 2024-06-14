#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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
#define HIT_SHIP_SIGN '@'

typedef struct Ship
{
    int x;
    int y;
    char direction;
}Ship;

// WORKS
char** setSea()
{
    char **board = (char **)malloc(BOARD_SIDE_SIZE * sizeof(char *));
    for (char i = 0; i < BOARD_SIDE_SIZE; ++i) 
    {
        board[i] = (char *)malloc(BOARD_SIDE_SIZE * sizeof(char));
    }
    for (int i = 0; i < BOARD_SIDE_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIDE_SIZE; ++j) {
            board[i][j] = SEA_SIGN;
        }
    }
    return board;
}

// WORKS
void printBoard(char **board)
{
    bool flag = false;
    for(int i = -1; i < BOARD_SIDE_SIZE; i++)
    {
        if(i >= 0)
        {
            printf(" %d ", i);
        }
        else
        {
            printf("x/y");
        }
        for(int j = 0; j < BOARD_SIDE_SIZE; j++)
        {
            if(i == -1)
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
void setShip(char** board, Ship s, int shipLength)
{
    if(s.direction == 'u')
    {
        for(int i = 0; i < shipLength; i++)
        {
            board[s.x][s.y] = SHIP_SIGN;
            s.x--;
        }
        return;
    }
    if(s.direction == 'd')
    {
        for(int i = 0; i < shipLength; i++)
        {
            board[s.x][s.y] = SHIP_SIGN;
            s.x++;
        }
        return;
    }
    if(s.direction == 'r')
    {
        for(int i = 0; i < shipLength; i++)
        {
            board[s.x][s.y] = SHIP_SIGN;
            s.y++;
        }
        return;
    }
    if(s.direction == 'l')
    {
        for(int i = 0; i < shipLength; i++)
        {
            board[s.x][s.y] = SHIP_SIGN;
            s.y--;
        }
        return;
    }
}

// WORKS
bool isWithinBoard(int x, int y) {
    return (x >= 0 && x < BOARD_SIDE_SIZE && y >= 0 && y < BOARD_SIDE_SIZE);
}

// WORKS
bool isValidPlacement(char **board , int x, int y, int shipLength, bool horizontal) 
{
    int xOffset[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int yOffset[] = {0, 0, 1, -1, 1, -1, 1, -1};
    for (int i = 0; i < shipLength; i++) 
    {
        int currentX = horizontal ? x : x + i;
        int currentY = horizontal ? y + i : y;

        if (!isWithinBoard(currentX, currentY) || board[currentX][currentY] == SHIP_SIGN) 
        {
            return false;
        }

        for (int j = 0; j < 8; j++) 
        {
            int adjacentX = currentX + xOffset[j];
            int adjacentY = currentY + yOffset[j];
            
            if (isWithinBoard(adjacentX, adjacentY) && board[adjacentX][adjacentY] == SHIP_SIGN) 
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
    if(s.direction == 'u' && s.x - shipLength + 1 >= 0)
    {
        res = isValidPlacement(board, s.x, s.y, shipLength, false);
        if(res == false)
        {
            return -1;
        }
        return 1;
    }
    if(s.direction == 'd' && s.x + shipLength - 1 <= BOARD_SIDE_SIZE - 1)
    {
        res = isValidPlacement(board, s.x, s.y, shipLength, false);
        if(res == false)
        {
            return -2;
        }
        return 1;
    }
    if(s.direction == 'l' && s.y - shipLength + 1 >= 0)
    {
        res = isValidPlacement(board, s.x, s.y, shipLength, true);
        if(res == false)
        {
            return -2;
        }
        return 1;
    }
    if(s.direction == 'r' && s.y + shipLength - 1 <= BOARD_SIDE_SIZE - 1)
    {
        res = isValidPlacement(board, s.x, s.y, shipLength, true);
        if(res == false)
        {
            return -2;
        }
        return 1;
    }

    return -1;
}

// WORKS
void enterCoordinates(Ship *s)
{
    s->x = -1;
    s->y = -1;
    s->direction = '-';
    while(!isWithinBoard(s->x, s->y))
    {
        printf("Enter x: ");
        scanf("%d", &s->x);
        getchar();
        printf("Enter y: ");
        scanf("%d", &s->y);
        getchar();
    }
}

// WORKS
int removeShip(char **board, Ship s)
{ 
    int shipLength = 0;
    int x = s.x;
    int y = s.y;

    x--;
    while(isWithinBoard(x, y))
    {
        if(board[x][y] == SHIP_SIGN)
        {
            board[x][y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        x--;
    }

    x = s.x++;
    y = s.y;
    while(isWithinBoard(x, y))
    {
        if(board[x][y] == SHIP_SIGN)
        {
            board[x][y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        x++;
    }

    x = s.x;
    y = s.y++;
    while(isWithinBoard(x, y))
    {
        if(board[x][y] == SHIP_SIGN)
        {
            board[x][y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        y++;
    }

    x = s.x;
    y = s.y--;
    while(isWithinBoard(x, y))
    {
        if(board[x][y] == SHIP_SIGN)
        {
            board[x][y] = SEA_SIGN;
            shipLength++;
        }
        else
        {
            break;
        }
        y--;
    }

    return shipLength;
}

// WORKS
bool moveShip(char** board, Ship s1, Ship s2)
{
    int shipLength = removeShip(board, s1);
    if(shipLength == -1)
    {
        return false;
    }
    printf("moveShip\n");

    int dRes = checkDirection(board, s2, shipLength);
    if(dRes == -1)
    {
        printf("Can't put ship there! You are out of the board!\n");
        return false;
    }
    else if(dRes == -2)
    {
        printf("\nThere is another ship next to this one! You can't put it here!\n");
        return false;
    }

    setShip(board, s2, shipLength);

    return true;
}

// WORKS
char** setOneKindShips(char ** board, int numberOfShips, int shipLength)
{
    Ship s;
    for(int i = 0; i < numberOfShips; i++)
    {
        printf("\nShip %d:\n", i + 1);
        s.x = -1;
        s.y = -1;
        s.direction = '-';
        printBoard(board);
        while(s.x < 0 || s.x > BOARD_SIDE_SIZE - 1)
        {
            printf("Enter start x: \n");
            printf("x must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
            printf("x: ");
            scanf("%d", &s.x);
            getchar();
        }
        
        while (s.y < 0 || s.y > BOARD_SIDE_SIZE - 1)
        {
            printf("Enter start y: \n");
            printf("y must be between 0 and %d\n", BOARD_SIDE_SIZE - 1);
            printf("y: ");
            scanf("%d", &s.y);
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
        
        if(direction == -1)
        {
            printf("Can't put ship there! You are out of the board!\n");
            i--;
            sleep(2);
        }
        else if(direction == -2)
        {
            printf("\nThere is another ship next to this one! You can't put it here!\n");
            i--;
            sleep(2);
        }
        else
        {
            setShip(board, s, shipLength);       
        }

        char choice = 'y';

        while(direction != -1 && direction != -2)
        {
            system("clear");
            printBoard(board);
            printf("Do you want to move a ship? (yes - y, no - n)\n");
            scanf("%c", &choice);
            getchar();
            if(choice == 'n')
            {
                break;
            }
            Ship s1;
            while(1)
            {
                printf("The ship you want to move:\n");
                enterCoordinates(&s1);
                if(board[s1.x][s1.y] != SHIP_SIGN)
                {
                    printf("\nThere is no ship!\n\n");
                }
                break;
            }
            printf("New place of this ship:\n");
            bool place = false;
            while(place == false)
            {
                Ship s2;
                enterCoordinates(&s2);
                printf("Enter direction: ");
                scanf("%c", &s2.direction);
                getchar();
                if(moveShip(board, s1, s2) == true)
                {
                    place = true;
                }
            }
        }
        system("clear");     
    }
    
    return board;
}

// WORKS
void setAllShips(char **board)
{
    printf("\nSET SMALL SHIPS (%d)\n", NUMBER_OF_SMALL_SHIPS);
    setOneKindShips(board, NUMBER_OF_SMALL_SHIPS, SMALL_SHIP_LENGTH);

    printf("\nSET MID SHIPS (%d)\n", NUMBER_OF_MID_SHIPS);
    setOneKindShips(board, NUMBER_OF_MID_SHIPS, MID_SHIP_LENGTH);

    printf("\nSET BIG SHIPS (%d)\n", NUMBER_OF_BIG_SHIPS);
    setOneKindShips(board, NUMBER_OF_BIG_SHIPS, BIG_SHIP_LENGTH);

    printf("\nSET GIGA SHIPS (%d)\n", NUMBER_OF_GIGA_SHIPS);
    setOneKindShips(board, NUMBER_OF_GIGA_SHIPS, GIGA_SHIP_LENGTH);
}

int hitShip(char **board, int x, int y, int hits, int *i)
{
    if(board[x][y] == SHIP_SIGN)
    {
        printf("\nYou hit a ship!\n");
        board[x][y] = HIT_SHIP_SIGN;
        i--;
        hits++;
    }
    else
    {
        printf("\nYou missed it!\n");
        board[x][y] = MISSED_SHIP_SIGN;
    }
    return hits;
}

int countShipSighs()
{
    return NUMBER_OF_SMALL_SHIPS * SMALL_SHIP_LENGTH + NUMBER_OF_MID_SHIPS * MID_SHIP_LENGTH + NUMBER_OF_BIG_SHIPS * BIG_SHIP_LENGTH + NUMBER_OF_GIGA_SHIPS * GIGA_SHIP_LENGTH;
}

// IN PROCESS
void gamePvsP(char **board1, char **board2)
{
    bool end = true;
    Ship s;
    int i = 1;
    int hits1 = 0;
    int hits2 = 0;
    while(!end)
    {
        enterCoordinates(&s);
        if(i % 2 == 1)
        {
            printf("\nPlayer 1:\n");
            hits1 = hitShip(board1, s.x, s.y, hits1, i);
        }
        else
        {
            printf("\nPlayer 2:\n");
            hits2 = hitShip(board2, s.x, s.y, hits2, i);
        }
        if(hits1 == countShipSighs())
        {
            printf("\n---------PLAYER 1 WINS---------");
            return;
        }
        if(hits2 == countShipSighs())
        {
            printf("\n---------PLAYER 2 WINS---------");
            return;
        }
        i++;
    }
}

// TO DO
void gamePvsComp(char **board1, char **board2)
{
}

int main()
{
    char** board1 = setSea();
    char** board2 = setSea();

    int choice = 0;
    printf("------------BATTLESHIP------------\n");
    while(1)
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
            printf("");
            break;

        case 2:
            printf("Player 1 set ships:\n");
            setAllShips(board1);
            printf("Player 2 set ships:\n");
            setAllShips(board2);


            break;

        case 3:
            printf("");
            break;
        
        default:
            break;
        }
    }

    return 0;
}
