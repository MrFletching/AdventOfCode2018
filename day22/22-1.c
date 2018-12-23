#include <stdio.h>

#define DEPTH 3558
#define TARGET_X 15
#define TARGET_Y 740

int main() {
    
    int erosion_levels[TARGET_Y+1][TARGET_X+1];
    
    int risk = 0;
    
    for(int y = 0; y <= TARGET_Y; y++) {
        for(int x = 0; x <= TARGET_X; x++) {
            
            int geologic_index;
            
            if(y == 0 && x == 0) {
                geologic_index = 0;
            } else if(y == TARGET_Y && x == TARGET_X) {
                geologic_index = 0;
            } else if(y == 0) {
                geologic_index = x * 16807;
            } else if(x == 0) {
                geologic_index = y * 48271;
            } else {
                geologic_index = erosion_levels[y][x-1] * erosion_levels[y-1][x];
            }
            
            erosion_levels[y][x] = (geologic_index + DEPTH) % 20183;
            
            risk += erosion_levels[y][x] % 3;
        }
    }
    
    printf("Risk: %d\n", risk);
    
}
