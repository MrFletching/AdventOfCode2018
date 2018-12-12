#include <assert.h>
#include <stdio.h>

#define GRID_SIZE 300

int calc_power_level(int x, int y, int serial) {
    int rack_id = x + 10;
    int power_level = ((rack_id * y) + serial) * rack_id;
    
    power_level = (power_level / 100) % 10;
    power_level -= 5;
    return power_level;
}

int main() {
    
    assert(calc_power_level(3, 5, 8) == 4);
    assert(calc_power_level(122, 79, 57) == -5);
    assert(calc_power_level(217, 196, 39) == 0);
    assert(calc_power_level(101, 153, 71) == 4);
    
    int fuel_cells[GRID_SIZE][GRID_SIZE];
    const int serial = 5791;
    
    for(int y = 1; y <= GRID_SIZE; y++) {
        for(int x = 1; x <= GRID_SIZE; x++) {
            fuel_cells[y-1][x-1] = calc_power_level(x, y, serial);
        }
    }
    
    
    int maxSum = 0;
    int maxX = 0;
    int maxY = 0;
    
    // Loop through each 3x3 square
    for(int y = 1; y <= GRID_SIZE - 2; y++) {
        for(int x = 1; x <= GRID_SIZE - 2; x++) {
            
            int sum = 0;
            
            // Loop through all 9 fuel cells in this square
            for(int y2 = y; y2 < y + 3; y2++) {
                for(int x2 = x; x2 < x + 3; x2++) {
                    sum += fuel_cells[y2-1][x2-1];
                }
            }
            
            if(sum > maxSum) {
                maxSum = sum;
                maxX = x;
                maxY = y;
            }
            
        }
    }
    
    printf("Max square: (%d,%d)\n", maxX, maxY);
    
    return 0;
}
