#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_MAP_SIZE 40
#define MAX_UNITS 50

#define ATTACK_POWER 3

typedef bool Map[MAX_MAP_SIZE][MAX_MAP_SIZE];

typedef enum {
    UNIT_TYPE_ELF,
    UNIT_TYPE_GOBLIN
} UnitType;

typedef struct {
    int x;
    int y;
} Vector;

typedef struct VectorNode {
    struct VectorNode *next;
    Vector v;
} VectorNode;

typedef struct {
    VectorNode *start;
    int size;
} VectorList;

typedef struct {
    bool alive;
    UnitType type;
    Vector pos;
    int hit_points;
} Unit;

typedef struct {
    Map map;
    int map_width;
    int map_height;
    Unit units[MAX_UNITS];
    int unit_count;
} World;

void world_add_unit(World *world, UnitType type, int x, int y) {
    Unit *unit = &world->units[world->unit_count];
    unit->alive = true;
    unit->type = type;
    unit->pos.x = x;
    unit->pos.y = y;
    unit->hit_points = 200;
    
    world->unit_count++;
}

void world_read_file(World *world, char *filename) {
    
    FILE *fp;
    fp = fopen(filename, "r");
    
    int y = 0;
    int x = 0;
    
    world->map_width  = 0;
    world->map_height = 0;
    world->unit_count = 0;
    
    char c;
    
    do {
        c = fgetc(fp);
        
        if(feof(fp)) break;
        
        
        if(c == '\n') {
            if(x > world->map_width) {
                world->map_width = x;
            }
            
            x = 0;
            y++;
        } else {
            if(c == '#') {
                world->map[y][x] = false;
            } else {
                world->map[y][x] = true;
                
                if(c == 'E') {
                    world_add_unit(world, UNIT_TYPE_ELF, x, y);
                } else if(c == 'G') {
                    world_add_unit(world, UNIT_TYPE_GOBLIN, x, y);
                }
                
            }
            
            x++;
        }
    } while(true);
    
    world->map_height = y;
    
    fclose(fp);
}

Unit *world_find_unit_at(World *world, int x, int y) {
    for(int i = 0; i < world->unit_count; i++) {
        Unit *unit = &world->units[i];
        if(unit->alive && unit->pos.x == x && unit->pos.y == y) {
            return unit;
        }
    }
    
    return NULL;
}

void world_print(World *world) {
    
    char c;
    
    for(int y = 0; y < world->map_height; y++) {
        for(int x = 0; x < world->map_width; x++) {
            
            if(!world->map[y][x]) {
                c = '#';
            } else {
                
                Unit *unit;
                unit = world_find_unit_at(world, x, y);
                if(unit == NULL) {
                    c = '.';
                } else if(unit->type == UNIT_TYPE_ELF) {
                    c = 'E';
                } else if(unit->type == UNIT_TYPE_GOBLIN) {
                    c = 'G';
                }
            }
            
            printf("%c", c);
        }
        printf("\n");
    }
    
}

void vector_list_init(VectorList *list) {
    list->start = NULL;
    list->size = 0;
}

void vector_list_insert(VectorList *list, int x, int y) {
    VectorNode *new_node = malloc(sizeof(Vector));
    new_node->next = NULL;
    new_node->v.x = x;
    new_node->v.y = y;
    
    VectorNode *next_node = list->start;
    VectorNode *prev_node = NULL;
    
    if(next_node == NULL) {
        list->start = new_node;
    } else {
        
        while((next_node != NULL) && ((next_node->v.y < y) || (next_node->v.y == y && (next_node->v.x < x)))) {
            prev_node = next_node;
            next_node = next_node->next;
        }
        
        if(prev_node == NULL) {
            new_node->next = list->start;
            list->start = new_node;
        } else {
            new_node->next = next_node;
            prev_node->next = new_node;
        }
        
    }
    
    list->size++;
}

void vector_print(Vector *v) {
    printf("(%d,%d)", v->x, v->y);
}

void vector_list_print(VectorList *list) {
    VectorNode *node = list->start;
    
    while(node != NULL) {
        vector_print(&node->v);
        printf(" ");
        
        node = node->next;
    }
    
    printf("\n");
}

void vector_list_empty(VectorList *list) {
    VectorNode *node = list->start;
    VectorNode *current_node;
    
    while(node != NULL) {
        current_node = node;
        node = node->next;
        
        free(current_node);
    }
}

bool world_is_space_free(World *world, int x, int y) {
    if(!world->map[y][x]) {
        return false;
    }
    
    Unit *unit = world_find_unit_at(world, x, y);
    
    if(unit != NULL) {
        return false;
    }
    
    return true;
}

void world_get_unit_move_options(World *world, Unit *current_unit, VectorList *list) {
    vector_list_init(list);
    
    for(int i = 0; i < world->unit_count; i++) {
        Unit *unit = &world->units[i];
        
        if((!unit->alive) || (current_unit->type == unit->type)) {
            continue;
        }
        
        if(world_is_space_free(world, unit->pos.x, unit->pos.y - 1)) {
            vector_list_insert(list, unit->pos.x, unit->pos.y - 1);
        }
        
        if(world_is_space_free(world, unit->pos.x + 1, unit->pos.y)) {
            vector_list_insert(list, unit->pos.x + 1, unit->pos.y);
        }
        
        if(world_is_space_free(world, unit->pos.x, unit->pos.y + 1)) {
            vector_list_insert(list, unit->pos.x, unit->pos.y + 1);
        }
        
        if(world_is_space_free(world, unit->pos.x - 1, unit->pos.y)) {
            vector_list_insert(list, unit->pos.x - 1, unit->pos.y);
        }
    }
}

bool are_adjacent(int x1, int y1, int x2, int y2) {
    return ((x1 == x2) && (y1 == y2 - 1 || y1 == y2 + 1)) ||
           ((y1 == y2) && (x1 == x2 - 1 || x1 == x2 + 1));
}

int world_distance_between(World *world, Unit *unit, int x2, int y2, Vector *step) {
    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE];
    
    for(int y = 0; y < world->map_height; y++) {
        for(int x = 0; x < world->map_width; x++) {
            visited[y][x] = !world->map[y][x];
        }
    }
    
    for(int i = 0; i < world->unit_count; i++) {
        Unit *unit = &world->units[i];
        
        if(unit->alive) {
            visited[unit->pos.y][unit->pos.x] = true;
        }
    }
    
    bool path_found = false;
    bool can_move = true;
    int dist = 0;
    
    VectorList last_discovered;
    vector_list_init(&last_discovered);
    vector_list_insert(&last_discovered, x2, y2);
    // vector_list_print(&last_discovered);
    
    while(can_move && !path_found) {
        dist++;
        // printf("Current Dist: %d\n", dist);
        VectorList next_discovered;
        vector_list_init(&next_discovered);
        
        int possible_moves = 0;
        
        VectorNode *node = last_discovered.start;
        while(node != NULL) {
            
            // printf("Current Node : ");
            // vector_print(&node->v);
            // printf("\n");
            
            int y = node->v.y;
            int x = node->v.x;
            
            if(are_adjacent(unit->pos.x, unit->pos.y, x, y)) {
                path_found = true;
                step->x = x;
                step->y = y;
                break;
            }
            
            if(!visited[y - 1][x]) {
                vector_list_insert(&next_discovered, x, y - 1);
                visited[y - 1][x] = true;
                possible_moves++;
            }
            
            if(!visited[y][x + 1]) {
                vector_list_insert(&next_discovered, x + 1, y);
                visited[y][x + 1] = true;
                possible_moves++;
            }
            
            if(!visited[y + 1][x]) {
                vector_list_insert(&next_discovered, x, y + 1);
                visited[y + 1][x] = true;
                possible_moves++;
            }
            
            if(!visited[y][x - 1]) {
                vector_list_insert(&next_discovered, x - 1, y);
                visited[y][x - 1] = true;
                possible_moves++;
            }
            
            // vector_list_print(&next_discovered);
            
            node = node->next;
        }
        
        if(possible_moves == 0) {
            can_move = false;
            break;
        }
        
        vector_list_empty(&last_discovered);
        last_discovered.start = next_discovered.start;
    }
    
    // printf("Can Move: %d\n", can_move);
    // printf("Path Found: %d\n", path_found);
    // 
    // printf("StepAAA: (%d,%d)\n", step->x, step->y);
    
    if(!path_found) {
        return 999;
    }
    
    return dist;
}

int unit_comparison(const void *a, const void *b) {
    Unit *unit_a = (Unit*)a;
    Unit *unit_b = (Unit*)b;
    
    if(unit_a->pos.y == unit_b->pos.y) {
        return unit_a->pos.x - unit_b->pos.x;
    } else {
        return unit_a->pos.y - unit_b->pos.y;
    }
}

Unit *find_adjacent_unit(World *world, Unit *current_unit) {
    int y = 999;
    int x = 999;
    int min_hit_points = 999;
    Unit *chosen_unit = NULL;
    
    for(int j = 0; j < world->unit_count; j++) {
        Unit *other_unit = &world->units[j];
        
        if(!other_unit->alive || other_unit->type == current_unit->type) {
            continue;
        }
        
        if(are_adjacent(current_unit->pos.x, current_unit->pos.y, other_unit->pos.x, other_unit->pos.y)) {
            
            if((other_unit->hit_points < min_hit_points) || (other_unit->hit_points == min_hit_points && other_unit->pos.y < y) || (other_unit->hit_points == min_hit_points && other_unit->pos.y == y && other_unit->pos.x < x)) {
                chosen_unit = other_unit;
                min_hit_points = other_unit->hit_points;
                y = other_unit->pos.y;
                x = other_unit->pos.x;
            }
        }
    }
    
    return chosen_unit;
}

bool run(int elf_attack_power) {
    
    World world;
    world_read_file(&world, "input.txt");
    
    
    // world_print(&world);
    
    int round = 0;
    int remaining_hit_points = 0;
    bool opponents_alive = true;
    int elf_count = 0;
    int goblin_count = 0;
    
    do {
        round++;
        // printf("\nRound %d:\n", round);
        
        // Sort units in position order
        qsort(world.units, world.unit_count, sizeof(Unit), unit_comparison);
        
        for(int i = 0; i < world.unit_count; i++) {
            Unit *current_unit = &world.units[i];
        
            if(!current_unit->alive) {
                continue;
            }
            
            // Count elves and goblins
            elf_count = 0;
            goblin_count = 0;
            remaining_hit_points = 0;
            
            for(int i = 0; i < world.unit_count; i++) {
                Unit *unit = &world.units[i];
                
                if(unit->alive) {
                    remaining_hit_points += unit->hit_points;
                    
                    if(unit->type == UNIT_TYPE_ELF) {
                        elf_count++;
                    } else {
                        goblin_count++;
                    }
                }
            }
            
            if(elf_count == 0 || goblin_count == 0) {
                opponents_alive = false;
                break;
            }
            
            // printf("Looking at unit (%d,%d)\n", current_unit->pos.x, current_unit->pos.y);
            
            Unit *adjacent_unit = find_adjacent_unit(&world, current_unit);
            
            if(adjacent_unit == NULL) {
                
                VectorList options;
                world_get_unit_move_options(&world, current_unit, &options);
                
                int dist_min = 999;
                VectorNode *option_min = NULL;
                Vector step_min;
                
                VectorNode *option = options.start;
                while(option != NULL) {
                    Vector step;
                    int dist = world_distance_between(&world, current_unit, option->v.x, option->v.y, &step);
                    // printf("Step: (%d,%d)\n", step.x, step.y);
                    
                    if(dist == 999) {
                        // printf("Cannot get from (%d,%d) to (%d,%d)\n", current_unit->pos.x, current_unit->pos.y, option->v.x, option->v.y);
                    } else {
                        // printf("Distance from (%d,%d) to (%d,%d) = %d\n", current_unit->pos.x, current_unit->pos.y, option->v.x, option->v.y, dist);
                        
                        if(dist < dist_min) {
                            dist_min = dist;
                            option_min = option;
                            step_min = step;
                        }
                    }
                    
                    
                    
                    option = option->next;
                }
                
                if(dist_min != 999) {
                    // printf("Minimum Distance: %d\n", dist_min);
                    // printf("Option Chosen: (%d,%d)\n", option_min->v.x, option_min->v.y);
                    // printf("Next Step: (%d,%d)\n", step_min.x, step_min.y);
                    
                    current_unit->pos = step_min;
                    
                    // Check if there are now any adjacent units
                    adjacent_unit = find_adjacent_unit(&world, current_unit);
                }
                
            }
            
            if(adjacent_unit != NULL) {
                // Attack!
                if(current_unit->type == UNIT_TYPE_GOBLIN) {
                    adjacent_unit->hit_points -= ATTACK_POWER;
                } else {
                    adjacent_unit->hit_points -= elf_attack_power;
                }
                
                
                if(adjacent_unit->hit_points <= 0) {
                    adjacent_unit->alive = false;
                    
                    if(adjacent_unit->type == UNIT_TYPE_ELF) {
                        return false;
                    }
                    
                }
            }
            
            
            
            
        }
        
        // world_print(&world);
        
    } while(opponents_alive);
    
    int outcome = remaining_hit_points * (round - 1);
    
    printf("Outcome: %d\n", outcome);
    
    return true;
}

int main() {
    
    bool elf_wins = false;
    int elf_attack_power = 3;
    
    do {
        elf_attack_power++;
        printf("Elf attack power: %d\n", elf_attack_power);
        elf_wins = run(elf_attack_power);
    } while(!elf_wins);
    
    printf("Elf attack power: %d\n", elf_attack_power);
    
    return 0;
}
