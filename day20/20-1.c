#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define DIRECTIONS_MAX_SIZE 15000
#define GRID_SIZE 200

#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define STACK_SIZE 500

typedef struct {
    bool doors_horizontal[GRID_SIZE-1][GRID_SIZE];
    bool doors_vertical[GRID_SIZE][GRID_SIZE-1];
    int start_x;
    int start_y;
} Grid;

typedef struct {
    int coords[STACK_SIZE][3];
    int size;
} VectorStack;

void read_directions(char *directions, char *filename) {
    FILE *fp = fopen(filename, "r");
    
    fgets(directions, DIRECTIONS_MAX_SIZE, fp);
    
    //printf("Directions: %s", directions);
    
    fclose(fp);
}

void init_grid(Grid *grid) {
    for(int y = 0; y < GRID_SIZE; y++) {
        for(int x = 0; x < GRID_SIZE; x++) {
            
            if(y != GRID_SIZE - 1) {
                grid->doors_horizontal[y][x] = false;
            }
            
            if(x != GRID_SIZE - 1) {
                grid->doors_vertical[y][x] = false;
            }
            
        }
    }
    
    grid->start_x = GRID_SIZE / 2;
    grid->start_y = GRID_SIZE / 2;
}

void print_grid(Grid *grid) {
    
    // Find bounds
    int min_x = grid->start_x;
    int max_x = grid->start_x;
    int min_y = grid->start_y;
    int max_y = grid->start_y;
    
    for(int y = 0; y < GRID_SIZE; y++) {
        for(int x = 0; x < GRID_SIZE; x++) {
            
            if((y != GRID_SIZE - 1) && grid->doors_horizontal[y][x]) {
                
                if(y < min_y) {
                    min_y = y;
                }
                
                if(y + 1 > max_y) {
                    max_y = y + 1;
                }
                
                if(x < min_x) {
                    min_x = x;
                }
                
                if(x > max_x) {
                    max_x = x;
                }
                
            }
            
            if((x != GRID_SIZE - 1) && grid->doors_vertical[y][x]) {
                
                if(x < min_x) {
                    min_x = x;
                }
                
                if(x + 1 > max_x) {
                    max_x = x + 1;
                }
                
                if(y < min_y) {
                    min_y = y;
                }
                
                if(y > max_y) {
                    max_y = y;
                }
                
            }
            
        }
    }
    
    // Print top
    for(int x = min_x; x <= max_x; x++) {
        printf("##");
    }
    
    printf("#\n");
    
    
    for(int y = min_y; y <= max_y; y++) {
        
        // Print rooms and vertical doors
        printf("#");
        
        for(int x = min_x; x <= max_x; x++) {
            
            if(y == grid->start_y && x == grid->start_x) {
                printf("X");
            } else {
                printf(ANSI_COLOR_BLUE "." ANSI_COLOR_RESET);
            }
            
            
            if((x < GRID_SIZE - 1) && grid->doors_vertical[y][x]) {
                printf(ANSI_COLOR_GRAY "|" ANSI_COLOR_RESET);
            } else {
                printf("#");
            }
        }
        
        printf("\n");
        
        // Print corners and horizontal doors
        if(y <= max_y - 1) {
            for(int x = min_x; x <= max_x; x++) {
        
                printf("#");
        
                if(grid->doors_horizontal[y][x]) {
                    printf(ANSI_COLOR_GRAY "-" ANSI_COLOR_RESET);
                } else {
                    printf("#");
                }
            }
        
            printf("#\n");
        }
        
        
    }
    
    // Print bottom
    for(int x = min_x; x <= max_x; x++) {
        printf("##");
    }
    
    printf("#\n");
}

void vector_stack_init(VectorStack *stack) {
    stack->size = 0;
}

void vector_stack_push2(VectorStack *stack, int x, int y) {
    stack->coords[stack->size][0] = x;
    stack->coords[stack->size][1] = y;
    
    stack->size++;
}

void vector_stack_pop2(VectorStack *stack, int *x, int *y) {
    *x = stack->coords[stack->size - 1][0];
    *y = stack->coords[stack->size - 1][1];
    
    stack->size--;
}

void vector_stack_push3(VectorStack *stack, int x, int y, int z) {
    stack->coords[stack->size][0] = x;
    stack->coords[stack->size][1] = y;
    stack->coords[stack->size][2] = z;
    
    stack->size++;
}

void vector_stack_pop3(VectorStack *stack, int *x, int *y, int *z) {
    *x = stack->coords[stack->size - 1][0];
    *y = stack->coords[stack->size - 1][1];
    *z = stack->coords[stack->size - 1][2];
    
    stack->size--;
}

void vector_stack_top2(VectorStack *stack, int *x, int *y) {
    *x = stack->coords[stack->size - 1][0];
    *y = stack->coords[stack->size - 1][1];
}

void vector_stack_top3(VectorStack *stack, int *x, int *y, int *z) {
    *x = stack->coords[stack->size - 1][0];
    *y = stack->coords[stack->size - 1][1];
    *z = stack->coords[stack->size - 1][2];
}

void create_grid_from_directions(Grid *grid, char directions[DIRECTIONS_MAX_SIZE]) {
    init_grid(grid);
    
    int directions_len = strlen(directions);
    
    int current_x = grid->start_x;
    int current_y = grid->start_y;
    
    VectorStack stack;
    vector_stack_init(&stack);
    
    for(int i = 0; i < directions_len; i++) {
        char instruction = directions[i];
        
        if(instruction == 'N') {
            grid->doors_horizontal[current_y - 1][current_x] = true;
            current_y--;
        } else if(instruction == 'S') {
            grid->doors_horizontal[current_y][current_x] = true;
            current_y++;
        } else if(instruction == 'W') {
            grid->doors_vertical[current_y][current_x - 1] = true;
            current_x--;
        } else if(instruction == 'E') {
            grid->doors_vertical[current_y][current_x] = true;
            current_x++;
        } else if(instruction == '(') {
            vector_stack_push2(&stack, current_x, current_y);
        } else if(instruction == ')') {
            vector_stack_pop2(&stack, &current_x, &current_y);
        } else if(instruction == '|') {
            vector_stack_top2(&stack, &current_x, &current_y);
        }
    }
}

int get_furthest_dist(Grid *grid) {
    
    VectorStack stack;
    vector_stack_init(&stack);
    vector_stack_push3(&stack, grid->start_x, grid->start_y, 0);
    
    bool visited_rooms[GRID_SIZE][GRID_SIZE] = {false};
    int furthest_room = 0;
    
    int x;
    int y;
    int current_dist;
    
    while(stack.size != 0) {
        
        vector_stack_pop3(&stack, &x, &y, &current_dist);
        
        //printf("Visiting room (%d, %d) - distance %d\n", x, y, current_dist);
        
        visited_rooms[y][x] = true;
        
        // West
        if(x > 0 && grid->doors_vertical[y][x - 1] && !visited_rooms[y][x - 1]) {
            vector_stack_push3(&stack, x - 1, y, current_dist+1);
        }
        
        // East
        if(x < GRID_SIZE - 1 && grid->doors_vertical[y][x] && !visited_rooms[y][x + 1]) {
            vector_stack_push3(&stack, x + 1, y, current_dist+1);
        }
        
        // North
        if(y > 0 && grid->doors_horizontal[y - 1][x] && !visited_rooms[y - 1][x]) {
            vector_stack_push3(&stack, x, y - 1, current_dist+1);
        }
        
        // South
        if(y < GRID_SIZE - 1 && grid->doors_horizontal[y][x] && !visited_rooms[y + 1][x]) {
            vector_stack_push3(&stack, x, y + 1, current_dist+1);
        }
        
        if(current_dist > furthest_room) {
            furthest_room = current_dist;
        }
        
    }
    
    return furthest_room;
}

int main() {
    char directions[DIRECTIONS_MAX_SIZE];
    read_directions(directions, "input.txt");
    
    Grid grid;
    create_grid_from_directions(&grid, directions);
    print_grid(&grid);
    
    int dist = get_furthest_dist(&grid);
    
    printf("Furthest Distance: %d\n", dist);
        
    return 0;
}
