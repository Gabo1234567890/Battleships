#include <stdio.h>
#include <stdlib.h>
#include "battleship.c"

int main()
{
    ReplayList rlist = init();

    int choice = 0;
    printf("------------BATTLESHIPS------------\n");
    while (1)
    {
        srand(time(0));
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
            rlist = gamePvsComp(board1, board2);
            break;

        case 4:
            replay(rlist, board1, board2);
            break;

        default:
            break;
        }
    }

    return 0;
}