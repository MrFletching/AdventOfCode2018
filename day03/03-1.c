#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 50
#define FABRIC_SIZE 1000

// const char *line = "#20 @ 295,560: 20x15";

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

void claimFabric(int fabric[FABRIC_SIZE][FABRIC_SIZE], int *overlapping, const claim c) {
    for(int x = c.x; x < c.x + c.w; x++) {
        for(int y = c.y; y < c.y + c.h; y++) {
            fabric[y][x]++;
    
            if(fabric[y][x] == 2) {
                (*overlapping)++;
            }
        }
    }

}

int main() {
    
    // Zero out fabric array
    int fabric[FABRIC_SIZE][FABRIC_SIZE];
    memset(fabric, 0, sizeof(fabric[0][0]) * FABRIC_SIZE * FABRIC_SIZE);
    
    // Record number of overlapping square inches
    int overlapping = 0;
    
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    char line[MAX_LINE_LENGTH];
    
    while(fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        claim newClaim = parseClaim(line);
        //printClaim(newClaim);
        claimFabric(fabric, &overlapping, newClaim);
    }
    
    fclose(fp);
    
    printf("Overlapping: %d\n", overlapping);
}
