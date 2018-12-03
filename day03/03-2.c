#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 50
#define NUM_CLAIMS 2000

typedef struct {
    int id;
    int x;
    int y;
    int w;
    int h;
} claim;

claim parseClaim(const char *line) {
    claim newArea;
    
    sscanf(line, "#%d @ %d,%d: %dx%d", &newArea.id, &newArea.x, &newArea.y, &newArea.w, &newArea.h);
    
    return newArea;
}

void printClaim(const claim c) {
    printf("id: %d\n", c.id);
    printf("x: %d\n", c.x);
    printf("y: %d\n", c.y);
    printf("w: %d\n", c.w);
    printf("h: %d\n", c.h);
}

bool claimsOverlap(const claim claim1, const claim claim2) {
    if(claim1.x + claim1.w <= claim2.x) {
        return false;
    }
    
    if(claim2.x + claim2.w <= claim1.x) {
        return false;
    }
    
    if(claim1.y + claim1.h <= claim2.y) {
        return false;
    }
    
    if(claim2.y + claim2.h <= claim1.y) {
        return false;
    }
    
    return true;
}

bool claimOverlaps(const claim claim1, const claim claims[NUM_CLAIMS], const int claimsCount) {
    for(int j = 0; j < claimsCount; j++) {
        claim claim2 = claims[j];
        
        if(claim1.id == claim2.id) continue;
        
        if(claimsOverlap(claim1, claim2)) {
            return true;
        }
    }
    
    return false;
}


int main() {
    
    
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    char line[MAX_LINE_LENGTH];
    
    claim claims[NUM_CLAIMS];
    int claimsCount = 0;
    
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        claim newClaim = parseClaim(line);
        claims[claimsCount] = newClaim;
        
        claimsCount++;
    }
    
    fclose(fp);
    
    // Loop through each claim, looking for a claim that does not overlap with
    // any other claim
    for(int i = 0; i < claimsCount; i++) {
        claim claim1 = claims[i];
        
        if(!claimOverlaps(claim1, claims, claimsCount)) {
            printf("Claim ID %d does not overlap!\n", claim1.id);
            return 0;
        }
    }
    
    printf("All claims overlap!\n");
    return 1;
}
