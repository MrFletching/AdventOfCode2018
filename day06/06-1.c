#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define COORDINATES_COUNT 50
#define INVALID_COORDINATE_INDEX -1
#define INFINATE_AREA -1

int manhattenDistance(int point1[2], int point2[2]) {
    return abs(point1[0] - point2[0]) + abs(point1[1] - point2[1]);
}

int indexOfClosestCoordinate(int point[2], int coordinates[COORDINATES_COUNT][2]) {
    int minDistance = INT_MAX;
    int closestCoordinateIndex = INVALID_COORDINATE_INDEX;
    
    for(int i = 0; i < COORDINATES_COUNT; i++) {
        int *coordinate = coordinates[i];
        
        int distance = manhattenDistance(point, coordinate);
        
        if(distance < minDistance) {
            minDistance = distance;
            closestCoordinateIndex = i;
        } else if(distance == minDistance) {
            closestCoordinateIndex = INVALID_COORDINATE_INDEX;
        }
    }
    
    return closestCoordinateIndex;
}

void readCoordinates(int coordinates[COORDINATES_COUNT][2]) {
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
    // Read coordinates into array
    int coordinates[COORDINATES_COUNT][2];
    readCoordinates(coordinates);
    
    
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
    int area[COORDINATES_COUNT] = {0};
    
    for(int y = minY; y <= maxY; y++) {
        for(int x = minX; x <= maxX; x++) {
            int point[2] = {x, y};
            int closestCoordinateIndex = indexOfClosestCoordinate(point, coordinates);
            
            if(closestCoordinateIndex != INVALID_COORDINATE_INDEX) {
                
                if(x == minX || x == maxX || y == minY || y == maxY) {
                    area[closestCoordinateIndex] = INFINATE_AREA;
                } else if(area[closestCoordinateIndex] != INFINATE_AREA) {
                    area[closestCoordinateIndex]++;
                }
                
            }
        }
    }
    
    int maxArea = maxInArray(area, COORDINATES_COUNT);
    
    printf("Max Area: %d\n", maxArea);
    
    return 0;
}
