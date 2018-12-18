#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAP_WIDTH  50
#define MAP_HEIGHT 50
#define TIME_SAMPES 2000

typedef enum {
    ACRE_TYPE_OPEN,
    ACRE_TYPE_TREES,
    ACRE_TYPE_LUMBERYARD
} AcreType;

const char *ACRE_TYPE_CHARS = ".|#";

typedef AcreType Map[MAP_HEIGHT][MAP_WIDTH];

AcreType char_to_acre_type(char c) {
    char *e = strchr(ACRE_TYPE_CHARS, c);
    return (AcreType)(e - ACRE_TYPE_CHARS);
}

char acre_type_to_char(AcreType type) {
    return ACRE_TYPE_CHARS[type];
}

const int TARGET_MINUTES = 1000000000;


void read_map(Map map, char *filename) {
    
    FILE *fp = fopen(filename, "r");
    
    int y = 0, x = 0;
    
    char c;
    char *e;
    
    while((c = fgetc(fp)) != EOF) {
        if(c == '\n') {
            y++;
            x = 0;
        } else {
            map[y][x] = char_to_acre_type(c);
            x++;
        }
    }
    
    fclose(fp);
}

void print_map(Map map) {
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            printf("%c", acre_type_to_char(map[y][x]));
        }
        printf("\n");
    }
}

void update_map(Map map) {
    Map new_map;
    
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            
            int count_open       = 0;
            int count_trees      = 0;
            int count_lumberyard = 0;
            
            for(int y1 = y - 1; y1 <= y + 1; y1++) {
                for(int x1 = x - 1; x1 <= x + 1; x1++) {
                    
                    // Don't include the current acre
                    if(y1 == y && x1 == x) {
                        continue;
                    }
                    
                    if(y1 >= 0 && y1 < MAP_HEIGHT && x1 >= 0 && x1 < MAP_WIDTH) {
                        
                        if(map[y1][x1] == ACRE_TYPE_OPEN) {
                            count_open++;
                        } else if(map[y1][x1] == ACRE_TYPE_TREES) {
                            count_trees++;
                        } else if(map[y1][x1] == ACRE_TYPE_LUMBERYARD) {
                            count_lumberyard++;
                        }
                        
                    }
                    
                }
            }
            
            new_map[y][x] = map[y][x];
            
            if(map[y][x] == ACRE_TYPE_OPEN) {
                if(count_trees >= 3) {
                    new_map[y][x] = ACRE_TYPE_TREES;
                }
            } else if(map[y][x] == ACRE_TYPE_TREES) {
                if(count_lumberyard >= 3) {
                    new_map[y][x] = ACRE_TYPE_LUMBERYARD;
                }
            } else if(map[y][x] == ACRE_TYPE_LUMBERYARD) {
                if(count_lumberyard == 0 || count_trees == 0) {
                    new_map[y][x] = ACRE_TYPE_OPEN;
                }
            }
            
            
        }
    }
    
    memcpy(map, new_map, sizeof(Map));
}

int calculate_resource_value(Map map) {
    int count_trees = 0;
    int count_lumberyard = 0;
    
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            if(map[y][x] == ACRE_TYPE_TREES) {
                count_trees++;
            } else if(map[y][x] == ACRE_TYPE_LUMBERYARD) {
                count_lumberyard++;
            }
        }
    }
    
    return count_trees * count_lumberyard;
}

int main() {
    Map map;
    read_map(map, "input.txt");
    printf("Initial state:\n");
    //print_map(map);
    
    int resource_values[TIME_SAMPES] = {0};
    
    int loop_from = -1;
    int loop_to   = -1;
    
    for(int time = 1; time <= TIME_SAMPES; time++) {
        printf("\nMinutes passed: %d\n", time);
        
        update_map(map);
        //print_map(map);
        
        resource_values[time] = calculate_resource_value(map);
        printf("Resource Value: %d\n", resource_values[time]);
        
        
        
        // See if resource_value was already seen
        if(time > 1000) {
            for(int i = 0; i < time; i++) {
                if(resource_values[i] == resource_values[time]) {
                    loop_from = i;
                    loop_to = time;
                    break;
                }
                
            }
        }
        
        
        if(loop_from != -1) {
            break;
        }
    }
    
    
    printf("Loop from %d to %d\n", loop_from, loop_to);
    
    int loop_size = loop_to - loop_from;
    
    
    
    int target_index = ((TARGET_MINUTES - loop_from) % loop_size) + loop_from;
    
    printf("Resource at %d minutes: %d\n", TARGET_MINUTES, resource_values[target_index]);
    
    
    return 0;
}
