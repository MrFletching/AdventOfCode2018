#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    uint64_t data;
} Node;

typedef struct CircularList {
    struct Node *entry;
    int size;
} CircularList;

void initList(CircularList *list) {
    list->entry = NULL;
    list->size = 0;
}

void printList(CircularList *list) {
    printf("Size: %d | Data: ", list->size);
    
    Node *n = list->entry;
    
    for(int i = 0; i < list->size; i++) {
        printf("%llu ", n->data);
        n = n->next;
    }
    
    printf("\n");
}

void insertIntoList(CircularList *list, uint64_t data) {
    Node *newNode = NULL;
    
    newNode = malloc(sizeof(Node));
    newNode->data = data;
    
    Node *entryNode = list->entry;
    
    if(entryNode == NULL) {
        newNode->next = newNode;
        newNode->prev = newNode;
        list->entry = newNode;
    } else {
        newNode->next = entryNode;
        newNode->prev = entryNode->prev;
        
        entryNode->prev->next = newNode;
        entryNode->prev = newNode;
        
        list->entry = newNode;
    }
    
    list->size++;
}

uint64_t removeFromList(CircularList *list) {
    Node *entryNode = list->entry;
    
    Node *prevNode = entryNode->prev;
    Node *nextNode = entryNode->next;
    
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
    
    uint64_t data = entryNode->data;
    
    list->entry = nextNode;
    list->size--;
    
    free(entryNode);
    
    return data;
}

void rotateClockwise(CircularList *list, int count) {
    for(int i = 0; i < count; i++) {
        list->entry = list->entry->next;
    }
}

void rotateCounterClockwise(CircularList *list, int count) {
    for(int i = 0; i < count; i++) {
        list->entry = list->entry->prev;
    }
}

uint64_t maxInArray(uint64_t array[], uint64_t arraySize) {
    uint64_t max = 0;
    
    for(int i = 0; i < arraySize; i++) {
        if(array[i] > max) {
            max = array[i];
        }
    }
    
    return max;
}

int main() {
    
    CircularList list;
    initList(&list);
    
    const uint64_t maxMarble = 7192000;
    const int playerCount = 403;
    
    uint64_t playerScores[playerCount] = {0};
    int currentPlayer = 0;
    
    uint64_t nextMarble = 1;
    insertIntoList(&list, 0);
    // printList(&list);
    
    while(nextMarble <= maxMarble) {
        if(nextMarble % 23 == 0) {
            rotateCounterClockwise(&list, 7);
            uint64_t score = removeFromList(&list);
            playerScores[currentPlayer] += score + nextMarble;
        } else {
            rotateClockwise(&list, 2);
            insertIntoList(&list, nextMarble);
        }
        
        // printList(&list);
        
        nextMarble++;
        currentPlayer = (currentPlayer + 1) % playerCount;
    }
    
    
    uint64_t maxScore = maxInArray(playerScores, playerCount);
    
    printf("Max Score: %" PRIu64 "\n", maxScore);
    
    return 0;
}
