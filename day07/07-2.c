#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define WORKERS_COUNT 5
#define NO_STEP '_'

typedef struct {
    uint8_t  id;
    uint32_t finishedAt;
    char     currentStep;
} worker_t;

uint8_t stepToInt(char step) {
    return step - 'A';
}

void addDependency(uint32_t dependencies[], char step, char dependencyStep) {
    uint8_t stepIndex = stepToInt(step);
    uint8_t dependencyIndex = stepToInt(dependencyStep);
    
    dependencies[stepIndex] |= (1 << dependencyIndex);
}

bool doesStepMeetDependencies(uint32_t dependencies[], uint32_t doneSteps, char step) {
    uint8_t stepIndex = stepToInt(step);
    uint32_t stepDependencies = dependencies[stepIndex];
    
    return (doneSteps & stepDependencies) == stepDependencies;
}

void addStep(uint32_t *steps, char step) {
    uint8_t stepIndex = stepToInt(step);
    *steps |= (1 << stepIndex);
}

bool isStep(uint32_t steps, char step) {
    uint8_t stepIndex = stepToInt(step);
    return steps & (1 << stepIndex);
}

uint32_t indexOfMin(uint32_t array[], uint32_t arraySize) {
    uint32_t min = UINT32_MAX;
    uint32_t minIndex = 0;
    
    for(uint32_t i = 0; i < arraySize; i++) {
        if(array[i] < min) {
            min = array[i];
            minIndex = i;
        }
    }
    
    return minIndex;
}

void doStep(uint32_t *doneSteps, char step) {
    uint8_t stepIndex = stepToInt(step);
    *doneSteps |= (1 << stepIndex);
}

bool isStepDone(uint32_t doneSteps, char step) {
    uint8_t stepIndex = stepToInt(step);
    return (doneSteps & (1 << stepIndex)) == (1 << stepIndex);
}

void readInstructions(uint32_t *steps, uint32_t dependencies[26]) {
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    char dependencyStep, step;
    
    while(fscanf(fp, "Step %c must be finished before step %c can begin.\n", &dependencyStep, &step) == 2) {
        addStep(steps, step);
        addStep(steps, dependencyStep);
        addDependency(dependencies, step, dependencyStep);
    }
    
    fclose(fp);
}

char getNextStep(uint32_t steps, uint32_t doneSteps, uint32_t assignedSteps, uint32_t dependencies[26]) {
    char nextStep = NO_STEP;
    
    for(char step = 'A'; step <= 'Z'; step++) {
        if(!isStep(steps, step) || isStepDone(assignedSteps, step)) {
            continue;
        }
        
        if(doesStepMeetDependencies(dependencies, doneSteps, step)) {
            if(step < nextStep) {
                nextStep = step;
            }
        }
    }
    
    return nextStep;
}

worker_t *getFreeWorker(worker_t workers[WORKERS_COUNT]) {
    
    for(int i = 0; i < WORKERS_COUNT; i++) {
        if(workers[i].currentStep == NO_STEP) {
            return &workers[i];
        }
    }
    
    return NULL;
}

worker_t *getNextFreeWorker(worker_t workers[WORKERS_COUNT]) {
    uint32_t minFinishedAt = UINT32_MAX;
    worker_t *minFinishedAtWorker = NULL;
    
    for(int i = 0; i < WORKERS_COUNT; i++) {
        if(workers[i].currentStep != NO_STEP && workers[i].finishedAt < minFinishedAt) {
            minFinishedAt = workers[i].finishedAt;
            minFinishedAtWorker = &workers[i];
        }
    }
    
    return minFinishedAtWorker;
}

int main() {
    
    uint32_t steps = 0;
    uint32_t dependencies[26] = {0};
    
    readInstructions(&steps, dependencies);
    
    uint32_t doneSteps = 0;
    uint32_t assignedSteps = 0;
    worker_t workers[WORKERS_COUNT];
    
    for(int i = 0; i < WORKERS_COUNT; i++) {
        workers[i].id = i;
        workers[i].finishedAt = 0;
        workers[i].currentStep = NO_STEP;
    }
    
    uint32_t time = 0;
    
    do {
        char nextStep = getNextStep(steps, doneSteps, assignedSteps, dependencies);
        
        worker_t *freeWorker;
        freeWorker = getFreeWorker(workers);
        
        if(freeWorker != NULL && nextStep != NO_STEP) {
            printf("[%3d] Worker %d: Doing step %c\n", time, freeWorker->id, nextStep);
            doStep(&assignedSteps, nextStep);
            
            freeWorker->finishedAt = time + 60 + stepToInt(nextStep) + 1;
            freeWorker->currentStep = nextStep;
        } else {
            worker_t *nextFreeWorker;
            nextFreeWorker = getNextFreeWorker(workers);
            doStep(&doneSteps, nextFreeWorker->currentStep);
            nextFreeWorker->currentStep = NO_STEP;
            time = nextFreeWorker->finishedAt;
        }
        
    } while(steps != doneSteps);
    
    printf("All finished at: %d\n", time);
    
    
    
    return 0;
}
