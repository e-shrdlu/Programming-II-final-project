#ifndef champion_h
#define champion_h

/*===========
 definitions
===========*/

// define ChampionRole enumerator
typedef enum ChampionRole {MAGE, FIGHTER, SUPPORT, TANK} ChampionRole;

// define Champion struct
typedef struct Champion {
    ChampionRole role;
    int level;
    struct Champion* next;
} Champion;

/*==========
 prototypes
==========*/

// from assignment
Champion* createChampion();
Champion* addChampion( Champion *head, Champion *c);
Champion* buildChampionList( int n);
void printChampionList( Champion *head);
Champion* removeChampion( Champion *head);
Champion* destroyChampionList( Champion *head);

// my functions
int getLoser(int winner);
int getRandom(int min, int max);
int getWinnerFromLevel(Champion* a, Champion* b);
char getChampChar(Champion* c);
void getChampPairString(char* pairing, Champion* a, Champion* b);

#endif