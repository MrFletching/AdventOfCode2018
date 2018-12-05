#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define POLYMER_MAX_LENGTH 50100
#define CHARACTER_DIFF ('a' - 'A')

void readPolymer(char *polymer, int polymerMaxLength) {
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    fgets(polymer, polymerMaxLength, fp);
    
    // Remove the newline characters from string
    polymer[strcspn(polymer, "\r\n")] = '\0';
    
    fclose(fp);
}

bool doUnitsReact(char c1, char c2) {
    return (c1 - c2 == CHARACTER_DIFF || c2 - c1 == CHARACTER_DIFF);
}

void removePairAt(char *polymer, int index) {
    memmove(&polymer[index], &polymer[index+2], strlen(polymer) - index - 1);
}

bool react(char *polymer) {
    int polymerLength = strlen(polymer);
    
    int c1 = 0;
    
    bool didReact = false;
    
    // Go through each character pair, removing reacting units
    while(c1 < polymerLength - 1) {
        int c2 = c1 + 1;
        
        if(doUnitsReact(polymer[c1], polymer[c2])) {
            removePairAt(polymer, c1);
            didReact = true;
        } else {
            c1++;
        }
    }
    
    return didReact;
}

void removeUnit(char *polymer, char unit) {
    char *src, *dst;
    
    for(src = dst = polymer; *src != '\0'; src++) {
        *dst = *src;
        if((*dst != unit) && (*dst != (unit + CHARACTER_DIFF))) dst++;
    }
    
    *dst = '\0';
}


int main() {
    char polymer[POLYMER_MAX_LENGTH];
    readPolymer(polymer, POLYMER_MAX_LENGTH);
    
    int polymerLength = strlen(polymer);
    
    int minLength = polymerLength;
    
    for(char l = 'A'; l <= 'Z'; l++) {
        char newPolymer[POLYMER_MAX_LENGTH];
        memcpy(newPolymer, polymer, polymerLength+1);
        
        removeUnit(newPolymer, l);
        
        // Keep reacting until no reactions occur
        while(react(newPolymer)) {}
        
        int newPolymerLength = strlen(newPolymer);
        printf("Removed unit %c, length: %d\n", l, newPolymerLength);
        
        if(newPolymerLength < minLength) {
            minLength = newPolymerLength;
        }
    }
    
    printf("Shortest length: %d\n", minLength);
    
    return 0;
}
