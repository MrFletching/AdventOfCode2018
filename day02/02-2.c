#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUM_IDS 500

#define STR_LEN 100
#define ID_LENGTH 26

typedef char id_t[ID_LENGTH+1];

bool matchIds(id_t* id1, id_t* id2) {
    
    int diffs = 0;
    
    char matchingChars[ID_LENGTH];
    memset(&matchingChars, '\0', ID_LENGTH);
    
    // printf("Comparing %s and %s\n", *id1, *id2);
    
    for(int i = 0; i < ID_LENGTH; i++) {
        if((*id1)[i] != (*id2)[i]) {
            diffs++;
            
            if(diffs == 2) {
                return false;
            }
        } else {
           matchingChars[i-diffs] = (*id1)[i];
       }
    }
    
    printf("Found them:\n");
    printf("ID 1: %s\n", *id1);
    printf("ID 2: %s\n", *id2);
    printf("Matching Characters: %s\n", matchingChars);
    
    return true;
}

int main() {
    FILE *fp;
    id_t currentId;
    
    int idsCount = 0;
    id_t ids[NUM_IDS];
    
    
    fp = fopen("input.txt", "r");
    
    int sum = 0;
    
    int dups  = 0;
    int trips = 0;
    
    while(fgets(currentId, STR_LEN, fp) != NULL) {
        strncpy(ids[idsCount], currentId, ID_LENGTH);
        ids[idsCount][ID_LENGTH] = '\0';
        idsCount++;
    }
    
    fclose(fp);
    
    for(int i = 0; i < idsCount; i++) {
        id_t *id1 = &ids[i];
        
        for(int j = i+1; j < idsCount; j++) {
            id_t *id2 = &ids[j];
            
            if(matchIds(id1, id2)) {
                return 0;
            }
        }
    }
    
    return 1;
}
