#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define COORDINATES_COUNT 50
#define INVALID_COORDINATE_INDEX -1
#define INFINATE_AREA -1

int main() {
    // Read coordinates into array
    int coordinates[COORDINATES_COUNT][2];
    
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    int x, y;
    int coordinatesCount = 0;
    
    while(fscanf(fp, "%d, %d", &x, &y) == 2) {
        coordinates[coordinatesCount][0] = x;
        coordinates[coordinatesCount][1] = y;
        coordinatesCount++;
    }
    
    fclose(fp);
    
    
    int minX = INT_MAX;
    int maxX = INT_MIN;
    int minY = INT_MAX;
    int maxY = INT_MIN;
    
    for(int i = 0; i < COORDINATES_COUNT; i++) {
        int *coordinate = coordinates[i];
        
        if(coordinate[0] < minX) {
            minX = coordinate[0];
        }
        
        if(coordinate[0] > maxX) {
            maxX = coordinate[0];
        }
        
        if(coordinate[1] < minY) {
            minY = coordinate[1];
        }
        
        if(coordinate[1] > maxY) {
            maxY = coordinate[1];
        }
    }
    
    // Loop through each point, looking to find the closest coordinate
    int area = 0;
    
    for(int y = minY; y <= maxY; y++) {
        for(int x = minX; x <= maxX; x++) {
            
            int minDistance = INT_MAX;
            int closestCoordinateIndex = INVALID_COORDINATE_INDEX;
            
            int sumDistance = 0;
            
            // Find closest coordinate to this cell
            for(int i = 0; i < COORDINATES_COUNT; i++) {
                int *coordinate = coordinates[i];
                
                sumDistance += abs(coordinate[0] - x) + abs(coordinate[1] - y);
            }
            
            if(sumDistance < 10000) {
                area++;
            }
            
        }
    }
    
    printf("Area: %d\n", area);
    
    return 0;
}
