#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define MAX_POINTS 500

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
} Point;

typedef struct {
    int xMin;
    int xMax;
    int yMin;
    int yMax;
} Bounds;

void read_points(Point points[], int *points_count) {
    *points_count = 0;
    
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    int x, y, dx, dy;
    
    while(fscanf(fp, "position=<%d,%d> velocity=<%d,%d>\n", &x, &y, &dx, &dy) == 4) {
        points[*points_count].x = x;
        points[*points_count].y = y;
        points[*points_count].dx = dx;
        points[*points_count].dy = dy;
        (*points_count)++;
    }
    
    fclose(fp);
}

Bounds get_points_bounds(Point points[], int points_count) {
    Bounds b = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};
    
    for(int i = 0; i < points_count; i++) {
        if(points[i].x < b.xMin) {
            b.xMin = points[i].x;
        }
        
        if(points[i].x > b.xMax) {
            b.xMax = points[i].x;
        }
        
        if(points[i].y < b.yMin) {
            b.yMin = points[i].y;
        }
        
        if(points[i].y > b.yMax) {
            b.yMax = points[i].y;
        }
    }
    
    // printf("xMin: %d, xMax: %d, yMin: %d, yMax: %d\n", b.xMin, b.xMax, b.yMin, b.yMax);
    return b;
}

void print_points(Point points[], int points_count, Bounds bounds) {
    int map_height = bounds.yMax - bounds.yMin + 1;
    int map_width  = bounds.xMax - bounds.xMin + 1;
    
    bool map[map_height][map_width];
    memset(map, false, sizeof(map[0][0]) * map_height * map_width);
    
    
    // for(int y = bounds.yMin; y <= bounds.yMax; y++) {
    //     for(int x = bounds.xMin; x <= bounds.xMax; x++) {
    //         if(is_point_at(points, points_count, x, y) {
    //             printf("#");
    //         } else {
    //             printf(".");
    //         }
    //     }
    // 
    //     printf("\n");
    // }
    
    for(int i = 0; i < points_count; i++) {
        int map_x = points[i].x - bounds.xMin;
        int map_y = points[i].y - bounds.yMin;
        
        map[map_y][map_x] = true;
        
        // printf("(%d, %d) [%d, %d]\n", points[i].x, points[i].y, points[i].dx, points[i].dy);
    }
    
    for(int y = 0; y < map_height; y++) {
        for(int x = 0; x < map_width; x++) {
            printf(map[y][x] ? "#" : ".");
        }
        
        printf("\n");
    }
}

void move_points(Point points[], int points_count) {
    for(int i = 0; i < points_count; i++) {
        points[i].x += points[i].dx;
        points[i].y += points[i].dy;
    }
}

bool areBoundsContracting(Bounds b1, Bounds b2) {
    int b1_height = b1.yMax - b1.yMin + 1;
    int b1_width  = b1.xMax - b1.xMin + 1;
    
    int b2_height = b2.yMax - b2.yMin + 1;
    int b2_width  = b2.xMax - b2.xMin + 1;
    
    return (b2_height <= b1_height && b2_width <= b1_width);
}

int main() {
    
    Point points[MAX_POINTS];
    int points_count;
    Bounds bounds, bounds_prev;
    read_points(points, &points_count);
    
    int time = 0;
    
    
    bounds = get_points_bounds(points, points_count);
    // printf("Time: %d\n", time);
    // print_points(points, points_count, bounds);
    
    bool contracting = true;
    
    while(contracting) {
        time++;
        move_points(points, points_count);
        
        bounds_prev = bounds;
        bounds = get_points_bounds(points, points_count);
        
        contracting = areBoundsContracting(bounds_prev, bounds);
        
        if(contracting && (bounds.xMax - bounds.xMin + 1) < 100) {
            printf("Time: %d\n", time); 
            print_points(points, points_count, bounds);
        }
    }
    
    return 0;
}
