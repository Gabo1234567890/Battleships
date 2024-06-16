#ifndef BATTLESHIP_H
#define BATTLESHIP_H

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

char **setSea();
void printBoard(char **board);
void clearBoard(char **board);
bool isWithinBoard(Point p);
bool isWithinBoardCoordinate(int coord);
bool isShipAdjacent(char **board, Point p, int shipLength, bool horizontal);
int isShipValid(char **board, Ship s, int shipLength);
int setShip(char **board, Ship s, int shipLength);
void enterCoordinates(Point *p);
int removeShip(char **board, Point p);
bool moveShip(char **board, Ship s1, Ship s2);
void **setOneKindShips(char **board, int numberOfShips, int shipLength, bool first, bool last);
bool checkBoardFromFile(char **board);
void readBoardFromFile(char **board, const char *filename);
void setAllShips(char **board);
bool isShipDestroyed(char **sea, char **board, Point p);
int isShipHit(char **sea, char **board, Point p, int hits, int *i);
int countShipSigns();
Point enterAdjacentHit(Point previousHit);
ReplayList gamePvsP(char **board1, char **board2);
char intDirToCharDir(int direction);
void setCompBoard(char **compBoard);
void dumbComp(Point *hit);
void smartComp(Point *hit, Point firstHit, Point closestHit, int hitParts, bool missed);
ReplayList gamePvsComp(char **playerBoard, char **compBoard);
void replay(ReplayList rlist, char **board1, char **board2);

#endif