#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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

int manhattan_dist(int x1, int y1, int z1, int x2, int y2, int z2) {
    return abs(x1 - x2) + abs(y1 - y2) + abs(z1 - z2);
}

bool is_nanobot_in_range(Nanobot *nanobot_a, int x, int y, int z) {
    int dist = manhattan_dist(nanobot_a->x, nanobot_a->y, nanobot_a->z, x, y, z);    
    return dist <= nanobot_a->r;
}

int count_nanobots_in_range(Nanobot nanobots[], int nanobot_count, int x, int y, int z) {
    int count = 0;
    
    for(int i = 0; i < nanobot_count; i++) {
        if(is_nanobot_in_range(&nanobots[i], x, y, z)) {
            count++;
        }
    }
    
    return count;
}

typedef struct {
    int x;
    int y;
    int z;
    int in_range;
} Vector;

typedef struct {
    Vector p;
    Vector v;
    Vector best;
} Particle;

#define PARTICLE_COUNT 1000

bool is_closer_than(Vector v1, Vector v2) {
    if(v1.in_range > v2.in_range) {
        return true;
    } else if(v1.in_range == v2.in_range) {
        if(manhattan_dist(v1.x, v1.y, v1.z, 0, 0, 0) < manhattan_dist(v2.x, v2.y, v2.z, 0, 0, 0)) {
            return true;
        }
    }
    
    return false;
}

int main() {
    Nanobot nanobots[NANOBOT_COUNT_MAX];
    int nanobot_count;
    read_nanobots(nanobots, &nanobot_count, "input.txt");
    
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int min_z = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    int max_z = INT_MIN;
            
    for(int i = 0; i < nanobot_count; i++) {
        if(nanobots[i].x < min_x) { min_x = nanobots[i].x; }
        if(nanobots[i].x > max_x) { max_x = nanobots[i].x; }
        if(nanobots[i].y < min_y) { min_y = nanobots[i].y; }
        if(nanobots[i].y > max_y) { max_y = nanobots[i].y; }
        if(nanobots[i].z < min_z) { min_z = nanobots[i].z; }
        if(nanobots[i].z > max_z) { max_z = nanobots[i].z; }
    }
    
    int range_x = max_x - min_x;
    int range_y = max_y - min_y;
    int range_z = max_z - min_z;
    
    Particle particles[PARTICLE_COUNT];
    time_t t;
    srand((unsigned) time(&t));
    
    Vector global_best;
    global_best.in_range = 0;
    
    for(int i = 0; i < PARTICLE_COUNT; i++) {
        particles[i].p.x = (rand() % (range_x)) + min_x;
        particles[i].p.y = (rand() % (range_y)) + min_y;
        particles[i].p.z = (rand() % (range_z)) + min_z;
        
        particles[i].v.x = (rand() % (range_x*2)) - range_x;
        particles[i].v.y = (rand() % (range_y*2)) - range_y;
        particles[i].v.z = (rand() % (range_z*2)) - range_z;
        
        particles[i].p.in_range = count_nanobots_in_range(nanobots, nanobot_count, particles[i].p.x, particles[i].p.y, particles[i].p.z);
        printf("Nanobots in range of particle %d: %d\n", i, particles[i].p.in_range);
        
        particles[i].best.x = particles[i].p.x;
        particles[i].best.y = particles[i].p.y;
        particles[i].best.z = particles[i].p.z;
        particles[i].best.in_range = particles[i].p.in_range;
        
        if(is_closer_than(particles[i].best, global_best)) {
            global_best = particles[i].best;
        }
    }
    
    while(true) {
        
        for(int i = 0; i < PARTICLE_COUNT; i++) {
            // Update the particle's velocity
            particles[i].v.x = 0.2*particles[i].v.x + 0.2*(particles[i].best.x - particles[i].p.x) + 0.5*(global_best.x - particles[i].p.x);
            particles[i].v.y = 0.2*particles[i].v.y + 0.2*(particles[i].best.y - particles[i].p.y) + 0.5*(global_best.y - particles[i].p.y);
            particles[i].v.z = 0.2*particles[i].v.z + 0.2*(particles[i].best.z - particles[i].p.z) + 0.5*(global_best.z - particles[i].p.z);
            
            // Update the particle's position
            particles[i].p.x += particles[i].v.x;
            particles[i].p.y += particles[i].v.y;
            particles[i].p.z += particles[i].v.z;
            
            particles[i].p.in_range = count_nanobots_in_range(nanobots, nanobot_count, particles[i].p.x, particles[i].p.y, particles[i].p.z);
            
            if(is_closer_than(particles[i].p, particles[i].best)) {
                particles[i].best = particles[i].p;
                
                if(is_closer_than(particles[i].best, global_best)) {
                    global_best = particles[i].best;
                }
            }
        }
        
        printf("Closest point with max nanobots in range: (%d, %d, %d)\n", global_best.x, global_best.y, global_best.z);
        printf("Manhattan Distance: %d\n", manhattan_dist(global_best.x, global_best.y, global_best.z, 0, 0, 0));
        printf("Nanobots in range: %d\n", global_best.in_range);
    }
    
    
    
    
    
    return 0;
}
