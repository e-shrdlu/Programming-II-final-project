#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "champion.h"

// in this code, there is player 0 and player 1
// when printing results, there is player 1 and player 2

int main(int argc, char* argv[]) {
    int numChamps;
    int curRound = 0;
    int winner = 0;
    int loser = 0;
    char pairing[3];
    int results[2];
    char playerRoles[2][10];
    char roleNames[][10] = {"MAGE", "FIGHTER", "SUPPORT", "TANK"};
    Champion* playerChamps[2] = {NULL, NULL}; // hold player0 & player1's character lists

    /*===============================
     evaluate command line arguments
    ===============================*/

    // give error if no arguments given
    // else set numChamps accordingly
    if (argc < 2) {
        printf("error: missing number of champions\n");
        return -1;
    }
    else {
        sscanf(argv[1], "%d", &numChamps);
    }

    
    /*==========================
     build champion linked list
    ==========================*/
    
    for (int i=0; i < 2; i++) {
        playerChamps[i] = buildChampionList(numChamps);
    }


    /*=========
     main loop
    =========*/

    // loop as long as both players have champions
    // each iteration:
    //  - print round number and each players champions
    //  - decide outcome for champion pairing
    //  - give out rewards/losses
    //  - print outcome

    printf("============= PLAYER 1 V PLAYER 2 SHOWDOWN ============\n\n");

    curRound = 1;
    while (playerChamps[0] != NULL && playerChamps[1] != NULL) { // while both players have champions
        results[0] = 0;
        results[1] = 0;

        /*========================================
         print round number and players champions
        ========================================*/

        printf("----- ROUND %d -----\n", curRound);
        for (int i=0; i<2; i++) {
            printf("Player %d: ", i+1);
            printChampionList(playerChamps[i]);
            printf("\n");
        }

        /*=============
         decide winner
        =============*/
        
        getChampPairString(pairing, playerChamps[0], playerChamps[1]);

        // this is kind of a mess, but I'm not sure how to better structure it
        if (strcmp(pairing, "FF") == 0) { // always tie
            winner = -1;
        }
        else if (strcmp(pairing, "FM") == 0) { // highest lvl wins
            winner = getWinnerFromLevel(playerChamps[0], playerChamps[1]);
        }
        else if (strcmp(pairing, "FS") == 0) { // highest lvl wins
            winner = getWinnerFromLevel(playerChamps[0], playerChamps[1]);
        }
        else if (strcmp(pairing, "FT") == 0) { // Fighter wins
            winner = (playerChamps[0]->role == FIGHTER) ? 0 : 1;
        }
        else if (strcmp(pairing, "MM") == 0) { // highest lvl wins
            winner = getWinnerFromLevel(playerChamps[0], playerChamps[1]);
        }
        else if (strcmp(pairing, "MS") == 0) { // highest lvl wins
            winner = getWinnerFromLevel(playerChamps[0], playerChamps[1]);
        }
        else if (strcmp(pairing, "MT") == 0) { // mage wins
            winner = (playerChamps[0]->role == MAGE) ? 0 : 1;
        }
        else if (strcmp(pairing, "SS") == 0) { // always tie
            winner = -1;
        }
        else if (strcmp(pairing, "ST") == 0) { // tank wins
            winner = (playerChamps[0]->role == TANK) ? 0 : 1;
        }
        else if (strcmp(pairing, "TT") == 0) { // always tie
            winner = -1;
        }
        else {
            printf("error didnt expect pairing: %s\n", pairing);
        }

        loser = getLoser(winner);

        /* ===========================
         give out rewards/punishments
        ============================*/

        // determine rewards/punishments

        if (strcmp(pairing, "FF") == 0) {
            results[0]++;
            results[1]++;
        }
        else if (strcmp(pairing, "FM") == 0) {
            if (winner == -1) {
                // tie -> nothing happens
            } else if (playerChamps[winner]->role == MAGE) {
                results[winner]++;
            } else if (playerChamps[winner]->role == FIGHTER) {
                results[loser]--;
            }
        }
        else if (strcmp(pairing, "FS") == 0) {
            if (winner == -1) {
                // tie -> nothing happens
            } else if (playerChamps[winner]->role == SUPPORT) {
                results[winner]++;
            } else if (playerChamps[winner]->role == FIGHTER) {
                results[loser]--;
            }        
        }
        else if (strcmp(pairing, "FT") == 0) {
            results[winner]++;
        }
        else if (strcmp(pairing, "MM") == 0) {
            if (winner == -1) {
                // tie -> nothing happens
            } else {
                results[winner]++;
                results[loser]--;
            }
        }
        else if (strcmp(pairing, "MS") == 0) {
            if (winner == -1) {
                // tie -> nothing
            } else if (playerChamps[winner]->role == MAGE) {
                // winner +1, loser -2
                results[winner]++;
                results[loser] -= 2;
            } else { // winner is support
                // winner +2, loser -1
                results[winner] += 2;
                results[loser]--;
            }
        }
        else if (strcmp(pairing, "MT") == 0) {
            // winner +1, loser -1
            results[winner]++;
            results[loser]--;
        }
        else if (strcmp(pairing, "SS") == 0) {
            // both lose 1 champion
            results[0]--;
            results[1]--;
        }
        else if (strcmp(pairing, "ST") == 0) {
            // winner +1
            results[winner]++;
        }
        else if (strcmp(pairing, "TT") == 0) {
            // nothing happens
        }
        else {
            printf("error didnt expect pairing: %s\n", pairing);
        }

        // before rew/punish remove first champion for each player
        strcpy(playerRoles[0], roleNames[playerChamps[0]->role]);
        strcpy(playerRoles[1], roleNames[playerChamps[1]->role]);
        playerChamps[0] = removeChampion(playerChamps[0]);
        playerChamps[1] = removeChampion(playerChamps[1]);

        // implement rewards/punishments
        for (int i=0; i<2; i++) {
            if (results[i] == 0) { // do nothing
                continue;
            }
            else if (results[i] > 0) { // give champions
                for (int j=0; j<results[i]; j++) {
                    playerChamps[i] = addChampion(playerChamps[i], createChampion());
                }
            } else { // remove champions
                for (int j=0; j<(-results[i]); j++) {
                    playerChamps[i] = removeChampion(playerChamps[i]);
                }
            }
        }

        /*=============
         print outcome
        =============*/

        printf("Player 1 is a %s and Player 2 is a %s\n", playerRoles[0], playerRoles[1]);
        
        // if nobody has any gain / loss, print special message
        if (results[0]==0 && results[1]==0) {
            printf("Nothing happens - no penalty or reward.\n");
        }
        else {
            for (int i=0; i<2; i++) {
                printf("Player %d (%s) ", i+1, playerRoles[i]);
                if (winner == -1) { // tie
                    printf("ties ");
                }
                else if (winner == i) { // win
                    printf("wins ");
                }
                else { // loss
                    printf("loses " );
                }

                switch(results[i]) {
                    case 0:
                        printf("with no gains or losses.\n");
                        break;
                    case 1:
                    case 2:
                        printf("and gains %d new champion(s).\n", results[i]);
                        break;
                    case -1:
                    case -2:
                        printf("and loses %d champion(s).\n", -results[i]);
                        break;
                }
            }
        }

        printf("\n");
    }

    /*====================
     print final outcomes
    ====================*/
    
    // determine winner
    if (playerChamps[0] == NULL && playerChamps[1] == NULL) // tie
        winner = -1;
    else if (playerChamps[1] == NULL)
        winner = 0;
    else
        winner = 1;

    printf("============ GAME OVER =============\n");
    printf("\n");
    for (int i=0; i<2; i++) {
        printf("Player %d ending champion list: ", i+1);
        printChampionList(playerChamps[i]);
        printf("\n");
    }
    printf("\n");
    if (winner == -1)
        printf("TIE -- both players ran out of champions\n");
    else
        printf("Player %d ran out of champions. Player %d wins.\n", getLoser(winner)+1, winner+1);
    

    /*===========
     free memory
    ===========*/

    destroyChampionList(playerChamps[0]);
    destroyChampionList(playerChamps[1]);

    return 1;
}           