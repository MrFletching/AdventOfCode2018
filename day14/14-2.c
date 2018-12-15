#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_RECIPES 1000000

typedef struct Node {
    struct Node *next;
    int data;
} Node;

typedef struct {
    Node *start;
    Node *end;
    int node_count;
} LinkedList;

void linked_list_init(LinkedList *ll) {
    ll->start = NULL;
    ll->end = NULL;
    ll->node_count = 0;
}

void linked_list_add(LinkedList *ll, int data) {
    Node *n = NULL;
    n = malloc(sizeof(Node));
    
    n->data = data;
    
    if(ll->node_count == 0) {
        ll->start = n;
    } else {
        ll->end->next = n;
    }
    
    n->next = ll->start;
    ll->end = n;
    ll->node_count++;
}

void print_scores(LinkedList *scores, Node *elf_1, Node *elf_2) {
    Node *node = scores->start;
    
    printf("Size: %d | Data: ",scores->node_count);
    
    for(int i = 0; i < scores->node_count; i++) {
        if(node == elf_1) {
            printf("(%d)", node->data);
        } else if(node == elf_2) {
            printf("[%d]", node->data);
        } else {
            printf(" %d ", node->data);
        }
        
        
        node = node->next;
    }
    
    printf("\n");
}

bool linked_list_contains(Node *node_a, Node *node_b, int length) {
    
    for(int i = 0; i < length; i++) {
        if(node_a->data != node_b->data) {
            return false;
        }
        
        node_a = node_a->next;
        node_b = node_b->next;
    }
    
    return true;
}

int main() {
    LinkedList target;
    linked_list_init(&target);
    linked_list_add(&target, 7);
    linked_list_add(&target, 6);
    linked_list_add(&target, 5);
    linked_list_add(&target, 0);
    linked_list_add(&target, 7);
    linked_list_add(&target, 1);
    
    
    LinkedList scores;
    linked_list_init(&scores);
    
    linked_list_add(&scores, 3);
    Node *elf_1 = scores.end;
    linked_list_add(&scores, 7);
    Node *elf_2 = scores.end;
    
    // print_scores(&scores, elf_1, elf_2);
    
    
    Node *target_start = NULL;
    
    while(true) {
        int elf_1_score = elf_1->data;
        int elf_2_score = elf_2->data;
        int sum = elf_1_score + elf_2_score;
        int sum_1 = sum / 10;
        int sum_2 = sum % 10;
        
        if(sum_1 != 0) {
            linked_list_add(&scores, sum_1);
            
            if(scores.node_count == target.node_count) {
                target_start = scores.start;
            } else if(target_start != NULL) {
                target_start = target_start->next;
            }
            
            if(target_start != NULL && linked_list_contains(target_start, target.start, target.node_count)) {
                printf("Number of recipes: %d\n", scores.node_count - target.node_count);
                break;
            }
        }
        
        linked_list_add(&scores, sum_2);
        
        if(scores.node_count == target.node_count) {
            target_start = scores.start;
        } else if(target_start != NULL) {
            target_start = target_start->next;
        }
        
        if(target_start != NULL && linked_list_contains(target_start, target.start, target.node_count)) {
            printf("Number of recipes: %d\n", scores.node_count - target.node_count);
            break;
        }
        
        
        for(int i = 0; i < elf_1_score + 1; i++) {
            elf_1 = elf_1->next;
        }
        
        for(int i = 0; i < elf_2_score + 1; i++) {
            elf_2 = elf_2->next;
        }
        
        // print_scores(&scores, elf_1, elf_2);
    }
    
    
    
    return 0;
}
