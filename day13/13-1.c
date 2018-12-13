#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAP_SIZE 200
#define MAX_CARTS 20

typedef enum {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
} Direction;

typedef enum {
    TRACK_NONE = 0,
    TRACK_VERTICAL,
    TRACK_HORIZONTAL,
    TRACK_UP_RIGHT,
    TRACK_UP_LEFT,
    TRACK_DOWN_RIGHT,
    TRACK_DOWN_LEFT,
    TRACK_INTERSECTION
} Track;

typedef struct {
    int x;
    int y;
    Direction direction;
    int intersection_state;
} Cart;

typedef struct {
    Track map[MAP_SIZE][MAP_SIZE];
    int map_width;
    int map_height;
    Cart carts[MAX_CARTS];
    int carts_count;
    bool crashed;
    int crash_x;
    int crash_y;
} World;

const char TRACK_CHARS[] = " |-\\//\\+";
const char DIRECTION_CHARS[] = "^>v<";

void init_map(Track map[MAP_SIZE][MAP_SIZE]) {
    for(int y = 0; y < MAP_SIZE; y++) {
        for(int x = 0; x < MAP_SIZE; x++) {
            map[y][x] = TRACK_NONE;
        }
    }
}

void init_world(World *world) {
    init_map(world->map);
    world->map_width = 0;
    world->map_height = 0;
    world->carts_count = 0;
    world->crashed = false;
    world->crash_x = 0;
    world->crash_y = 0;
}

void print_world(World *world) {
    
    for(int y = 0; y < world->map_height; y++) {
        for(int x = 0; x < world->map_width; x++) {
            
            // Count the number of carts at this position
            int cart_count = 0;
            Direction cart_direction;
            
            for(int c = 0; c < world->carts_count; c++) {
                if(world->carts[c].x == x && world->carts[c].y == y) {
                    cart_direction = world->carts[c].direction;
                    cart_count++;
                }
            }
            
            if(cart_count == 0) {
                Track t = world->map[y][x];
                printf("%c", TRACK_CHARS[t]);
            } else if(cart_count == 1) {
                printf("%c", DIRECTION_CHARS[cart_direction]);
            } else {
                printf("X");
            }
            
            
        }
        printf("\n");
    }
}

Track track_from_char(char track_char, bool going_horizontal) {
    if(track_char == '|' || track_char == '^' || track_char == 'v') {
        return TRACK_VERTICAL;
    } else if(track_char == '-' || track_char == '<' || track_char == '>') {
        return TRACK_HORIZONTAL;
    } else if(track_char == '\\') {
        if(going_horizontal) {
            return TRACK_DOWN_LEFT;
        } else {
            return TRACK_UP_RIGHT;
        }
    } else if(track_char == '/') {
        if(going_horizontal) {
            return TRACK_UP_LEFT;
        } else {
            return TRACK_DOWN_RIGHT;
        }
    } else if(track_char == '+') {
        return TRACK_INTERSECTION;
    }
    
    return TRACK_NONE;
}

void add_cart(World *world, int y, int x, Direction direction) {
    Cart *cart = &world->carts[world->carts_count];
    
    cart->y = y;
    cart->x = x;
    cart->direction = direction;
    cart->intersection_state = 0;
    
    world->carts_count++;
}

void read_world(World *world, char *map_filename) {
    FILE *fp;
    fp = fopen(map_filename, "r");
    
    int y = 0;
    int x = 0;
    bool going_horizontal = false;
    
    char track_char;
    
    while(fscanf(fp, "%c", &track_char) == 1) {
        if(track_char == '\n') {
            if(x > world->map_width) {
                world->map_width = x;
            }
            
            y++;
            x = 0;
        } else {
            world->map[y][x] = track_from_char(track_char, going_horizontal);
            
            // Check if track is currently going horizontally
            // This is because / could be up-and-left or down-and-right
            going_horizontal = (
                world->map[y][x] == TRACK_HORIZONTAL ||
                world->map[y][x] == TRACK_INTERSECTION ||
                world->map[y][x] == TRACK_UP_RIGHT ||
                world->map[y][x] == TRACK_DOWN_RIGHT
            );
            
            // Add cart to world if this is a cart character
            if(track_char == '^') {
                add_cart(world, y, x, DIR_UP);
            } else if(track_char == '>') {
                add_cart(world, y, x, DIR_RIGHT);
            } else if(track_char == 'v') {
                add_cart(world, y, x, DIR_DOWN);
            } else if(track_char == '<') {
                add_cart(world, y, x, DIR_LEFT);
            }
            
            x++;
        }
    }
    
    world->map_height = y;
    
    fclose(fp);
}

int cart_comparison(const void *a, const void *b) {
    const Cart *cart_a = a;
    const Cart *cart_b = b;
    
    if(cart_a->y == cart_b->y) {
        return cart_a->x - cart_b->x;
    }
    
    return cart_a->y - cart_b->y;
}

void tick_world(World *world) {
    // Order all carts: top to bottom, left to right
    qsort(&world->carts, world->carts_count, sizeof(Cart), cart_comparison);
    
    for(int c = 0; c < world->carts_count; c++) {
        Cart *cart = &world->carts[c];
        
        if(cart->direction == DIR_UP) {
            cart->y--;
        } else if(cart->direction == DIR_DOWN) {
            cart->y++;
        } else if(cart->direction == DIR_LEFT) {
            cart->x--;
        } else if(cart->direction == DIR_RIGHT) {
            cart->x++;
        }
        
        Track track_under_cart = world->map[cart->y][cart->x];
        
        if(track_under_cart == TRACK_UP_RIGHT) {
            if(cart->direction == DIR_DOWN) {
                cart->direction = DIR_RIGHT;
            } else {
                cart->direction = DIR_UP;
            }
        } else if(track_under_cart == TRACK_UP_LEFT) {
            if(cart->direction == DIR_DOWN) {
                cart->direction = DIR_LEFT;
            } else {
                cart->direction = DIR_UP;
            }
        } else if(track_under_cart == TRACK_DOWN_RIGHT) {
            if(cart->direction == DIR_UP) {
                cart->direction = DIR_RIGHT;
            } else {
                cart->direction = DIR_DOWN;
            }
        } else if(track_under_cart == TRACK_DOWN_LEFT) {
            if(cart->direction == DIR_UP) {
                cart->direction = DIR_LEFT;
            } else {
                cart->direction = DIR_DOWN;
            }
        } else if(track_under_cart == TRACK_INTERSECTION) {
            if(cart->direction == DIR_UP) {
                if(cart->intersection_state == 0) {
                    cart->direction = DIR_LEFT;
                } else if(cart->intersection_state == 2) {
                    cart->direction = DIR_RIGHT;
                }
            } else if(cart->direction == DIR_DOWN) {
                if(cart->intersection_state == 0) {
                    cart->direction = DIR_RIGHT;
                } else if(cart->intersection_state == 2) {
                    cart->direction = DIR_LEFT;
                }
            } else if(cart->direction == DIR_LEFT) {
                if(cart->intersection_state == 0) {
                    cart->direction = DIR_DOWN;
                } else if(cart->intersection_state == 2) {
                    cart->direction = DIR_UP;
                }
            } else if(cart->direction == DIR_RIGHT) {
                if(cart->intersection_state == 0) {
                    cart->direction = DIR_UP;
                } else if(cart->intersection_state == 2) {
                    cart->direction = DIR_DOWN;
                }
            }
            
            cart->intersection_state = (cart->intersection_state + 1) % 3;
        }
    }
    
    // Check if any carts crashed
    for(int i = 1; i < world->carts_count; i++) {
        Cart *cart_i = &world->carts[i];
        
        for(int j = 0; j < i; j++) {
            Cart *cart_j = &world->carts[j];
            
            if(cart_i->x == cart_j->x && cart_i->y == cart_j->y) {
                world->crashed = true;
                world->crash_x = cart_i->x;
                world->crash_y = cart_i->y;
            }
        }
    }
}

int main() {
    
    World world;
    init_world(&world);
    
    int tick = 0;
    
    printf("Tick: %d\n", tick);
    read_world(&world, "input.txt");
    //print_world(&world);
    
    while(!world.crashed) {
        tick++;
        tick_world(&world);
        printf("Tick: %d\n", tick);
        //print_world(&world);
    }
    
    printf("Crash at: %d,%d\n", world.crash_x, world.crash_y);
    
    
    return 0;
}
