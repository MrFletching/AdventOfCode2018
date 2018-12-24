#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NANOBOT_COUNT_MAX 1000

typedef struct {
    int x;
    int y;
    int z;
    int r;
} Nanobot;

void read_nanobots(Nanobot nanobots[], int *nanobot_count, char *filename) {
    FILE *fp = fopen(filename, "r");
    
    int x, y, z, r;
    
    *nanobot_count = 0;
    
    while(fscanf(fp, "pos=<%d,%d,%d>, r=%d\n", &x, &y, &z, &r) == 4) {
        
        nanobots[*nanobot_count].x = x;
        nanobots[*nanobot_count].y = y;
        nanobots[*nanobot_count].z = z;
        nanobots[*nanobot_count].r = r;
        
        (*nanobot_count)++;
    }
    
    fclose(fp);
}

int find_largest_radius(Nanobot nanobots[], int nanobot_count) {
    int max_r = 0;
    int nanobot_index;
    
    for(int i = 0; i < nanobot_count; i++) {
        if(nanobots[i].r > max_r) {
            max_r = nanobots[i].r;
            nanobot_index = i;
        }
    }
    
    return nanobot_index;
}

bool is_nanobot_in_range(Nanobot *nanobot_a, Nanobot *nanobot_b) {
    int dist_x = abs(nanobot_a->x - nanobot_b->x);
    int dist_y = abs(nanobot_a->y - nanobot_b->y);
    int dist_z = abs(nanobot_a->z - nanobot_b->z);
    int dist = dist_x + dist_y + dist_z;
    
    return dist <= nanobot_a->r;
}

int count_nanobots_in_range(Nanobot nanobots[], int nanobot_count, int nanobot_index) {
    int count = 0;
    
    for(int i = 0; i < nanobot_count; i++) {
        if(is_nanobot_in_range(&nanobots[nanobot_index], &nanobots[i])) {
            count++;
        }
    }
    
    return count;
}

int main() {
    Nanobot nanobots[NANOBOT_COUNT_MAX];
    int nanobot_count;
    read_nanobots(nanobots, &nanobot_count, "input.txt");
    
    int nanobot_index = find_largest_radius(nanobots, nanobot_count);
    
    int nanobots_in_range = count_nanobots_in_range(nanobots, nanobot_count, nanobot_index);
    
    printf("Nanobots in range: %d\n", nanobots_in_range);
    
    return 0;
}
