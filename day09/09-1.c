#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void insertIntoArray(int array[], int *arraySize, const int index, const int val) {
    memmove(&array[index+1], &array[index], sizeof(array[0]) * (*arraySize - index));
    (*arraySize)++;
    array[index] = val;
}

void removeFromArray(int array[], int *arraySize, const int index) {
    memmove(&array[index], &array[index+1], sizeof(array[0]) * (*arraySize - index - 1));
    (*arraySize)--;
}

void printMarbles(const int marbles[], const int marblesCount, const int currentMarble) {
    printf("Marbles: ");
    
    for(int i = 0; i < marblesCount; i++) {
        if(currentMarble == i) {
            printf("(%d) ", marbles[i]);
        } else {
            printf("%d ", marbles[i]);
        }
        
    }
    
    printf("\n");
}

void takeTurn(int marbles[], int *marblesCount, int *currentMarble, int *marblesUsed, int playerScores[], int playerCount, int *currentPlayer) {

    if(*marblesUsed % 23 == 0) {
        
        
        int removeMarbleIndex = *currentMarble - 7;
        
        if(removeMarbleIndex < 0) {
            removeMarbleIndex += *marblesCount;
        }
        
        playerScores[*currentPlayer] += marbles[removeMarbleIndex];
        removeFromArray(marbles, marblesCount, removeMarbleIndex);
        
        playerScores[*currentPlayer] += *marblesUsed;
        
        (*marblesUsed)++;
        
        *currentMarble = removeMarbleIndex % *marblesCount;
        
    } else {
        int newMarbleIndex = (*currentMarble + 2) % (*marblesCount);
        
        if(newMarbleIndex == 0) {
            newMarbleIndex = *marblesCount;
        }
        
        insertIntoArray(marbles, marblesCount, newMarbleIndex, *marblesUsed);
        
        *currentMarble = newMarbleIndex;
        (*marblesUsed)++;
    }
    
    *currentPlayer = (*currentPlayer + 1) % playerCount;
}

int maxInArray(int array[], int arraySize) {
    int max = INT_MIN;
    
    for(int i = 0; i < arraySize; i++) {
        if(array[i] > max) {
            max = array[i];
        }
    }
    
    return max;
}

int main() {
    
    const int playerCount = 403;
    const int marbleMax = 71920;
    const int totalMarbles = marbleMax + 1;
    
    int marbles[totalMarbles];
    
    marbles[0] = 0;
    int marblesCount = 1;
    int marblesUsed = 1;
    int currentMarble = 0;
    int currentPlayer = 0;
    
    int playerScores[playerCount] = {0};
    
    //printMarbles(marbles, marblesCount, currentMarble);
    
    while(marblesUsed != totalMarbles) {
        takeTurn(marbles, &marblesCount, &currentMarble, &marblesUsed, playerScores, playerCount, &currentPlayer);
        
        //printMarbles(marbles, marblesCount, currentMarble);
    }
    
    int highScore = maxInArray(playerScores, playerCount);
    
    printf("High score: %d\n", highScore);
    
    return 0;
}
