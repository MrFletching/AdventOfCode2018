#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define DEPTH 3558
#define TARGET_X  15
#define TARGET_Y 740

// #define DEPTH 510
// #define TARGET_X 10
// #define TARGET_Y 10

#define AREA_WIDTH  (TARGET_X + 30)
#define AREA_HEIGHT (TARGET_Y + 30)

#define MOVE_TIME        1
#define SWITCH_TOOL_TIME 7

typedef enum {
    TOOL_NEITHER,
    TOOL_TORCH,
    TOOL_CLIMBING
} Tool;

#define TOOL_OPTIONS 3

typedef enum {
    TYPE_ROCKY,
    TYPE_WET,
    TYPE_NARROW
} Type;

bool can_use_tool(Type type, Tool tool) {
    switch(type) {
        case TYPE_ROCKY:
            return tool == TOOL_CLIMBING || tool == TOOL_TORCH;
        case TYPE_WET:
            return tool == TOOL_NEITHER || tool == TOOL_CLIMBING;
        case TYPE_NARROW:
            return tool == TOOL_TORCH || tool == TOOL_NEITHER;
    }
    
    return false;
}

int main() {
    
    int erosion_levels[AREA_HEIGHT][AREA_WIDTH];
    
    for(int y = 0; y < AREA_HEIGHT; y++) {
        for(int x = 0; x < AREA_WIDTH; x++) {
            
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
        }
    }
    
    // Dijkstra
    bool visited[AREA_HEIGHT][AREA_WIDTH][TOOL_OPTIONS];
    int dist[AREA_HEIGHT][AREA_WIDTH][TOOL_OPTIONS];
    
    for(int y = 0; y < AREA_HEIGHT; y++) {
        for(int x = 0; x < AREA_WIDTH; x++) {
            for(int tool = 0; tool < TOOL_OPTIONS; tool++) {
                dist[y][x][tool] = INT_MAX;
                visited[y][x][tool] = false;
            }
        }
    }
    
    dist[0][0][TOOL_TORCH] = 0;
    
    int min_dist;
    int selected_tool, selected_y, selected_x;
    
    int path[AREA_HEIGHT][AREA_WIDTH][TOOL_OPTIONS][3];
    
    do {
        // Select the coordinates of the already discovered shortest distance
        min_dist = INT_MAX;
        
        for(int y = 0; y < AREA_HEIGHT; y++) {
            for(int x = 0; x < AREA_WIDTH; x++) {
                for(int tool = 0; tool < TOOL_OPTIONS; tool++) {
                    if(!visited[y][x][tool] && dist[y][x][tool] < min_dist) {
                        min_dist = dist[y][x][tool];
                        selected_y = y;
                        selected_x = x;
                        selected_tool = tool;
                    }
                }
            }
        }
        
        // printf("Selected (%d, %d) tool: %d, dist: %d\n", selected_x, selected_y, selected_tool, min_dist);
        
        if(selected_y == TARGET_Y && selected_x == TARGET_X && selected_tool == TOOL_TORCH) {
            break;
        }
        
        if(min_dist != INT_MAX) {
            visited[selected_y][selected_x][selected_tool] = true;
            
            
            int current_type = erosion_levels[selected_y][selected_x] % 3;
            
            // Above
            if(selected_y > 0) {
                if(!visited[selected_y-1][selected_x][selected_tool]) {
                    int type = erosion_levels[selected_y-1][selected_x] % 3;
                    
                    if(can_use_tool(type, selected_tool)) {
                        if(min_dist + MOVE_TIME < dist[selected_y-1][selected_x][selected_tool]) {
                            dist[selected_y-1][selected_x][selected_tool] = min_dist + MOVE_TIME;
                            // printf("Could move to (%d, %d) for %d\n", selected_x, selected_y-1, min_dist + MOVE_TIME);
                        }
                    }
                }
            }
            
            // Below
            if(selected_y < AREA_HEIGHT - 1) {
                if(!visited[selected_y+1][selected_x][selected_tool]) {
                    int type = erosion_levels[selected_y+1][selected_x] % 3;
                    
                    if(can_use_tool(type, selected_tool)) {
                        if(min_dist + MOVE_TIME < dist[selected_y+1][selected_x][selected_tool]) {
                            dist[selected_y+1][selected_x][selected_tool] = min_dist + MOVE_TIME;
                            // printf("Could move to (%d, %d) for %d\n", selected_x, selected_y+1, min_dist + MOVE_TIME);
                        }
                    }
                }
            }
            
            // Left
            if(selected_x > 0) {
                if(!visited[selected_y][selected_x-1][selected_tool]) {
                    int type = erosion_levels[selected_y][selected_x-1] % 3;
                    
                    if(can_use_tool(type, selected_tool)) {
                        if(min_dist + MOVE_TIME < dist[selected_y][selected_x-1][selected_tool]) {
                            dist[selected_y][selected_x-1][selected_tool] = min_dist + MOVE_TIME;
                            // printf("Could move to (%d, %d) for %d\n", selected_x-1, selected_y, min_dist + MOVE_TIME);
                        }
                    }
                }
            }
            
            // Right
            if(selected_x < AREA_WIDTH - 1) {
                if(!visited[selected_y][selected_x+1][selected_tool]) {
                    int type = erosion_levels[selected_y][selected_x+1] % 3;
                    
                    if(can_use_tool(type, selected_tool)) {
                        if(min_dist + MOVE_TIME < dist[selected_y][selected_x+1][selected_tool]) {
                            dist[selected_y][selected_x+1][selected_tool] = min_dist + MOVE_TIME;
                            // printf("Could move to (%d, %d) for %d\n", selected_x+1, selected_y, min_dist + MOVE_TIME);
                        }
                    }
                }
            }
            
            // Changing tool
            for(int tool = 0; tool < 3; tool++) {
                if(selected_tool != tool && can_use_tool(current_type, tool)) {
                    if(min_dist + SWITCH_TOOL_TIME < dist[selected_y][selected_x][tool]) {
                        dist[selected_y][selected_x][tool] = min_dist + SWITCH_TOOL_TIME;
                        // printf("Could switch tools to %d for %d\n", tool, min_dist + SWITCH_TOOL_TIME);
                    }
                }
            }
            
        }
        
    } while(min_dist != INT_MAX);
    
    printf("Min dist: %d\n", min_dist);
    
    
}
