#include <stdio.h>
#include <stdbool.h>

#define MAX_X 1000
#define MAX_Y 2000

#define SPRING_X 500
#define SPRING_Y 0

typedef enum {
    CELL_TYPE_SAND,
    CELL_TYPE_CLAY,
    CELL_TYPE_SPRING,
    CELL_TYPE_WATER,
    CELL_TYPE_FLOW
} CellType;

typedef CellType Map[MAX_Y][MAX_X];

void read_map(Map map, char *filename) {
    // Set all cells to sand
    for(int y = 0; y < MAX_Y; y++) {
        for(int x = 0; x < MAX_X; x++) {
            map[y][x] = CELL_TYPE_SAND;
        }
    }
    
    // Read clay cells from file
    FILE *fp = fopen(filename, "r");
    
    char axis_1, axis_2;
    int axis_1_val, axis_2_start, axis_2_end;
    
    while(fscanf(fp, "%c=%d, %c=%d..%d\n", &axis_1, &axis_1_val, &axis_2, &axis_2_start, &axis_2_end) == 5) {
        if(axis_1 == 'x') {
            int x = axis_1_val;
            for(int y = axis_2_start; y <= axis_2_end; y++) {
                map[y][x] = CELL_TYPE_CLAY;
            }
        } else {
            int y = axis_1_val;
            for(int x = axis_2_start; x <= axis_2_end; x++) {
                map[y][x] = CELL_TYPE_CLAY;
            }
        }
    }
    
    // Set spring cell
    map[SPRING_Y][SPRING_X] = CELL_TYPE_SPRING;
    
    fclose(fp);
}

char get_cell_type_char(CellType cell_type) {
    switch(cell_type) {
        case CELL_TYPE_SAND:   return '.';
        case CELL_TYPE_CLAY:   return '#';
        case CELL_TYPE_SPRING: return '+';
        case CELL_TYPE_WATER:  return '~';
        case CELL_TYPE_FLOW:   return '|';
    }
}

void print_map(Map map) {
    int min_x = SPRING_X;
    int max_x = SPRING_X;
    int min_y = SPRING_Y;
    int max_y = SPRING_Y;
    
    // Find map bounds
    for(int y = 0; y < MAX_Y; y++) {
        for(int x = 0; x < MAX_X; x++) {
            if(map[y][x] != CELL_TYPE_SAND && map[y][x] != CELL_TYPE_FLOW) {
                if(x < min_x) {
                    min_x = x;
                }
                
                if(x > max_x) {
                    max_x = x;
                }
                
                if(y > max_y) {
                    max_y = y;
                }
            }
        }
    }
    
    // Output map
    for(int y = min_y; y <= max_y; y++) {
        for(int x = min_x - 1; x <= max_x + 1; x++) {
            printf("%c", get_cell_type_char(map[y][x]));
        }
        printf("\n");
    }
    
}

bool update_map(Map map) {
    bool changes_made = false;
    
    for(int y = 0; y < MAX_Y; y++) {
        
        bool is_lake = false;
        int  lake_start_x;
        
        for(int x = 0; x < MAX_X; x++) {
            if(map[y][x] == CELL_TYPE_SPRING && map[y+1][x] == CELL_TYPE_SAND) {
                map[y+1][x] = CELL_TYPE_FLOW;
                changes_made = true;
            }
            
            if(map[y][x] == CELL_TYPE_FLOW && y < MAX_Y - 1) {
                
                if(map[y+1][x] == CELL_TYPE_SAND) {
                    map[y+1][x] = CELL_TYPE_FLOW;
                    is_lake = false;
                    changes_made = true;
                } else if(map[y+1][x] == CELL_TYPE_FLOW) {
                    is_lake = false;
                } else if(map[y+1][x] != CELL_TYPE_FLOW) {
                    
                    if(map[y][x-1] == CELL_TYPE_SAND) {
                        map[y][x-1] = CELL_TYPE_FLOW;
                        changes_made = true;
                    } else if(map[y][x-1] == CELL_TYPE_CLAY) {
                        is_lake = true;
                        lake_start_x = x;
                    }
                    
                    if(map[y][x+1] == CELL_TYPE_SAND) {
                        map[y][x+1] = CELL_TYPE_FLOW;
                        changes_made = true;
                    } else if(map[y][x+1] == CELL_TYPE_CLAY && is_lake) {
                        for(int lake_x = lake_start_x; lake_x <= x; lake_x++) {
                            map[y][lake_x] = CELL_TYPE_WATER;
                            changes_made = true;
                        }
                        
                    }
                    
                }
                
            }
            
            if(map[y][x] == CELL_TYPE_CLAY) {
                is_lake = false;
            }
            
        }
    }
    
    return changes_made;
}

int count_water(Map map) {
    
    int min_x = SPRING_X;
    int max_x = SPRING_X;
    int min_y = 9999;
    int max_y = SPRING_Y;
    
    // Find map bounds
    for(int y = 1; y < MAX_Y; y++) {
        for(int x = 0; x < MAX_X; x++) {
            if(map[y][x] != CELL_TYPE_SAND && map[y][x] != CELL_TYPE_FLOW) {
                if(x < min_x) {
                    min_x = x;
                }
                
                if(x > max_x) {
                    max_x = x;
                }
                
                if(y > max_y) {
                    max_y = y;
                }
                
                if(y < min_y) {
                    min_y = y;
                }
            }
        }
    }
    
    // Count water
    int water_count = 0;
    for(int y = min_y; y <= max_y; y++) {
        for(int x = min_x - 1; x <= max_x + 1; x++) {
            if(map[y][x] == CELL_TYPE_WATER) {
                water_count++;
            }
        }
    }
    
    return water_count;
}

int main() {
    
    Map map;
    read_map(map, "input.txt");
    
    bool changes_made = true;
    
    while(changes_made) {
        changes_made = update_map(map);
    }
    
    print_map(map);
    
    printf("\nWater Count: %d\n", count_water(map));
    
    
    
    return 0;
}
