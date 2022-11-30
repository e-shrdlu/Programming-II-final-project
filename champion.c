#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "champion.h"


/*============
 my functions
============*/

// given the winning player, return the losingn player
int getLoser(int winner) {
    if (winner == 0)
        return 1;
    else
        return 0;
}

// returns random int in range [min..max] inclusive
int getRandom(int min, int max) {
    return rand() % (max-min + 1) + min;
}

int getWinnerFromLevel(Champion* a, Champion* b) {
    if (a->level > b->level) {
        return 0;
    } else if (b->level > a->level) {
        return 1;
    } else {
        return -1; // tie
    }
}

// returns single character representing champion's role
char getChampChar(Champion* c) {
    switch(c->role) {
        case MAGE:
            return 'M';
            break;
        case FIGHTER:
            return 'F';
            break;
        case SUPPORT:
            return 'S';
            break;
        case TANK:
            return 'T';
            break;
        default:
            return 'X'; // error
            break;
    }
}

// set pairing to alphabetized string representing matchup
void getChampPairString(char* pairing, Champion* a, Champion* b) {
    char achar;
    char bchar;

    achar = getChampChar(a);
    bchar = getChampChar(b);

    if (achar < bchar) {
        pairing[0] = achar;
        pairing[1] = bchar;
    } else {
        pairing[0] = bchar;
        pairing[1] = achar;
    }

    pairing[2] = '\0';
}

/*==================
 required functions
==================*/

Champion* createChampion() {
    // declare new champ
    Champion* newChamp = NULL;
    newChamp = (Champion*)malloc(sizeof(Champion));

    // decide role
    switch (getRandom(1, 4)) {
        case 1:
            newChamp->role = MAGE;
            break;
        case 2:
            newChamp->role = FIGHTER;
            break;
        case 3:
            newChamp->role = SUPPORT;
            break;
        case 4:
            newChamp->role = TANK;
            break;
    }

    // decide level
    switch (newChamp->role) {
        case MAGE:
            newChamp->level = getRandom(5, 8);
            break;
        case FIGHTER:
            newChamp->level = getRandom(1, 4);
            break;
        case SUPPORT:
            newChamp->level = getRandom(3, 6);
            break;
        case TANK:
            newChamp->level = getRandom(1, 8);
            break;
    }
        
    return newChamp;
}


// insert newChamp into list, maintaining descending sort order (highest level first)
Champion* addChampion( Champion* head, Champion* newChamp ) {
    Champion* curChamp;

    // each case will return during its execution
    // with the exception of the main case, which possibly won't return in the edge case of newChamp belonging after tail

    // edge case: list is empty
    if (head == NULL) {
        head = newChamp;
        head->next = NULL;
        return head;
    }

    // edge case: new item belongs before current head
    if (newChamp->level >= head->level) { // if new item is greater or equal to head
        newChamp->next = head;
        head = newChamp;
        return head;
    }

    // edge case: new item belongs after tail
    // dealt with after main case

    // main case: new item belongs somewhere in the middle of list
    for (curChamp = head; curChamp->next != NULL; curChamp = curChamp->next) { // for each element except the last one
        // if new element belongs before next element, insert it
        if (newChamp->level > curChamp->next->level) {
            newChamp->next = curChamp->next;
            curChamp->next = newChamp;
            return head;
        }
    }

    // edge case: new item belongs after final element
    newChamp->next = NULL;
    curChamp->next = newChamp;
    return head;
}


Champion* buildChampionList( int n ) {
    Champion* head = NULL;
    Champion* c = NULL;

    // create new head node
    // head = (Champion*)malloc(sizeof(Champion));
    // head->next = NULL;
    
    // create & sort n new champions into list
    for (int i=0; i<n; i++) {
            c = createChampion(); // get new random Champion
            head = addChampion(head, c); // insert new champ into sorted position
    }

    return head;
}


void printChampionList( Champion *head ) {
    for (Champion* c = head; c != NULL; c = c->next) {
        printf("%c%d ", getChampChar(c), c->level);
    }
}


Champion* removeChampion( Champion *head ) {
    Champion* newHead;

    if (head == NULL) { // cant remove any more
        return head;
    }

    newHead = head->next;
    free(head);
    return newHead;
}


Champion* destroyChampionList( Champion *head ) {
    Champion* prevNode;

    /*=========
     edge cases
    ==========*/

    // edge case: list is empty
    if (head == NULL)
        return NULL;

    // edge case: list has one item
    if (head->next == NULL)
        return NULL;

    prevNode = head;
    for (Champion* curNode = head->next; curNode != NULL; curNode = curNode->next) {
        free(prevNode);
        prevNode = curNode;
    }

    free(prevNode);

    return NULL;
}
