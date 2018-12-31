#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int w;
    int x;
    int y;
    int z;
} Vector;

typedef struct VectorNode_s {
    struct VectorNode_s *next;
    Vector v;
} VectorNode;

typedef struct {
    VectorNode *start;
} Constellation;

typedef struct ConstellationNode_s {
    struct ConstellationNode_s *next;
    Constellation c;
} ConstellationNode;

typedef struct {
    ConstellationNode *start;
    int size;
} Collection;

void vector_print(Vector *v) {
    printf("[%d,%d,%d,%d]", v->w, v->x, v->y, v->z);
}

int vector_manhatten_distance(Vector *v1, Vector *v2) {
    int wd = abs(v1->w - v2->w);
    int xd = abs(v1->x - v2->x);
    int yd = abs(v1->y - v2->y);
    int zd = abs(v1->z - v2->z);
    return wd + xd + yd + zd;
}

VectorNode *vector_node_new(int w, int x, int y, int z) {
    VectorNode *vn = malloc(sizeof(VectorNode));
    vn->next = NULL;
    vn->v.w = w;
    vn->v.x = x;
    vn->v.y = y;
    vn->v.z = z;
    
    return vn;
}

void constellation_init(Constellation *c) {
    c->start = NULL;
}

ConstellationNode *constellation_node_new() {
    ConstellationNode *cn = malloc(sizeof(ConstellationNode));
    cn->next = NULL;
    constellation_init(&cn->c);
    
    return cn;
}

void constellation_print(Constellation *vectors) {
    VectorNode *current = vectors->start;
    
    while(current != NULL) {
        vector_print(&current->v);
        current = current->next;
        
        if(current != NULL) {
            printf(", ");
        }
    }
    
    printf("\n");
}

void constellation_append(Constellation *c, VectorNode *vector_node) {
    if(c->start == NULL) {
        c->start = vector_node;
    } else {
        
        VectorNode *current = c->start;
        
        while(current->next != NULL) {
            current = current->next;
        }
        
        current->next = vector_node;
    }
}

void constellation_merge(Constellation *dest, Constellation *src) {
    VectorNode *dest_current = dest->start;
    
    while(dest_current->next != NULL) {
        dest_current = dest_current->next;
    }
    
    dest_current->next = src->start;
}

void constellation_read_file(Constellation *vectors, char *filename) {
    FILE *fp = fopen(filename, "r");
    
    int w, x, y, z;
    
    while(fscanf(fp, "%d,%d,%d,%d", &w, &x, &y, &z) == 4) {
        VectorNode *new_vector_node = vector_node_new(w, x, y, z);
        constellation_append(vectors, new_vector_node);
    }
    
    fclose(fp);
}

void collection_init(Collection *collection) {
    collection->start = NULL;
    collection->size = 0;
}

void collection_append(Collection *collection, ConstellationNode *cn) {
    
    if(collection->start == NULL) {
        collection->start = cn;
    } else {
        ConstellationNode *current = collection->start;
        
        while(current->next != NULL) {
            current = current->next;
        }
        
        current->next = cn;
        
    }
    
    collection->size++;
}

void collection_remove(Collection *collection, ConstellationNode *cn) {
    
    if(collection->start == cn) {
        collection->start = collection->start->next;
    } else {
        
        ConstellationNode *current = collection->start;
        
        while(current->next != cn) {
            current = current->next;
        }
        
        current->next = current->next->next;
        
    }
    
    free(cn);
    collection->size--;
}

void collection_print(Collection *collection) {
    ConstellationNode *current = collection->start;
    
    while(current != NULL) {
        constellation_print(&current->c);
        current = current->next;
    }
}

bool constellation_should_merge(ConstellationNode *cn1, ConstellationNode *cn2) {
    VectorNode *vn1 = cn1->c.start;
    
    while(vn1 != NULL) {
        
        VectorNode *vn2 = cn2->c.start;
        
        while(vn2 != NULL) {
            
            // printf("Comparing: ");
            // vector_print(&vn1->v);
            // printf(" to ");
            // vector_print(&vn2->v);
            // printf("\n");
            
            if(vector_manhatten_distance(&vn1->v, &vn2->v) <= 3) {
                return true;
            }
            
            vn2 = vn2->next;
        }
        
        vn1 = vn1->next;
    }
    
    return false;
}

int count_constellations(char *filename) {
    Constellation unused_vectors;
    constellation_init(&unused_vectors);
    constellation_read_file(&unused_vectors, filename);
    
    Collection collection;
    collection_init(&collection);
    
    // Put each vector into its own constellation
    VectorNode *current = unused_vectors.start;
    
    while(current != NULL) {
        ConstellationNode *new_constellation_node = constellation_node_new();
        collection_append(&collection, new_constellation_node);
        
        VectorNode *next = current->next;
        current->next = NULL;
        constellation_append(&new_constellation_node->c, current);
        current = next;
    }
    
    // Merge constellations until no more merges occur
    bool merge_occured = false;
    do {
        merge_occured = false;
    
        ConstellationNode *cn1 = collection.start;
    
        while(cn1 != NULL) {
    
            ConstellationNode *cn2 = cn1->next;
    
            while(cn2 != NULL) {
                bool should_merge = constellation_should_merge(cn1, cn2);
    
                if(should_merge) {
                    merge_occured = true;
                    // printf("Constellations should be merged\n");
                    
                    constellation_merge(&cn1->c, &cn2->c);
                    collection_remove(&collection, cn2);
                    
                } else {
                    // printf("Constellations should not be merged\n");
                }
    
                cn2 = cn2->next;
            }
    
            cn1 = cn1->next;
        }
    
    } while(merge_occured);
    
    printf("Constellations:\n");
    collection_print(&collection);
    
    return collection.size;
}

int main() {
    
    int count = count_constellations("input.txt");
    
    printf("Constellation count: %d\n", count);
    
    return 0;
}
