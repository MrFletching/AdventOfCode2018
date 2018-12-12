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
    
    
    int max_sum = 0;
    int max_x = 0;
    int max_y = 0;
    int max_square_size = 1;
    
    for(int square_size = 1; square_size <= 300; square_size++) {
        
        // Loop through each square
        for(int y = 1; y <= GRID_SIZE + 1 - square_size; y++) {
            for(int x = 1; x <= GRID_SIZE + 1 - square_size; x++) {
                
                int sum = 0;
                
                // Loop through all fuel cells in this square
                for(int y2 = y; y2 < y + square_size; y2++) {
                    for(int x2 = x; x2 < x + square_size; x2++) {
                        sum += fuel_cells[y2-1][x2-1];
                    }
                }
                
                if(sum > max_sum) {
                    max_sum = sum;
                    max_x = x;
                    max_y = y;
                    max_square_size = square_size;
                }
                
            }
        }
        
    }
    
    
    
    printf("Max square: (%d,%d,%d)\n", max_x, max_y, max_square_size);
    
    return 0;
}
