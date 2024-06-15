void gamePvsComp(char **playerBoard, char **compBoard)
// {
//     setCompBoard(compBoard);
//     setAllShips(playerBoard);
//     bool end = true;
//     int turn = 0;
//     Ship s;
//     int playerHits = 0;
//     int computerHits = 0;
//     bool compFound = false;
//     char **playerSea = setSea();
//     int hitX, hitY;
//     int alrHitX = -1;
//     int alrHitY = -1;
//     int numberOfHitParts = 0;
//     while (!end)
//     {
//         printf("Player's board:\n");
//         printBoard(playerBoard);
//         printf("Player's hits:\n");
//         printBoard(playerSea);
//         if (turn % 2 == 0)
//         {
//             printf("Player's turn:\n");
//             enterCoordinates(&s);
//             playerHits = hitShip(compBoard, s.x, s.y, playerHits, &turn);
//         }
//         else
//         {
//             printf("Computer's turn:\n");
//             if (!compFound)
//             {
//                 dumbComp(&hitX, &hitY);
//             }
//             else
//             {
//                 smartComp(&hitX, &hitY, alrHitX, alrHitY, numberOfHitParts);
//             }
//             computerHits = hitShip(playerBoard, hitX, hitY, computerHits, &turn);
//             if (playerBoard[hitX][hitY] == HIT_SHIP_SIGN)
//             {
//                 compFound = true;
//                 numberOfHitParts++;
//                 if (alrHitX == -1 && alrHitY == -1)
//                 {
//                     alrHitX = hitX;
//                     alrHitY = hitY;
//                 }
//             }
//             if (playerBoard[hitX][hitY] == MISSED_SHIP_SIGN && compFound)
//             {
//                 if (hitX < alrHitX)
//                 {
//                     hitX = alrHitX + 1;
//                 }
//                 else
//                 {
//                     hitX = alrHitX - 1;
//                 }
//                 if (hitY < alrHitY)
//                 {
//                     hitY = alrHitY + 1;
//                 }
//                 else
//                 {
//                     hitY = alrHitY - 1;
//                 }
//             }
//         }
//         if (playerHits == countShipSigns())
//         {
//             printf("\n---------PLAYER WINS---------");
//             return;
//         }
//         if (computerHits == countShipSigns())
//         {
//             printf("\n---------COMPUTER WINS---------");
//             return;
//         }
//         turn++;
//         system("cls");
//     }
// }