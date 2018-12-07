#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

int main() {
    
    uint32_t steps = 0;
    uint32_t dependencies[26] = {0};
    
    readInstructions(&steps, dependencies);
    
    // addStep(&steps, 'A');
    // addStep(&steps, 'B');
    // addStep(&steps, 'C');
    // addStep(&steps, 'D');
    // addStep(&steps, 'E');
    // addStep(&steps, 'F');
    // 
    // addDependency(dependencies, 'A', 'C');
    // addDependency(dependencies, 'F', 'C');
    // addDependency(dependencies, 'B', 'A');
    // addDependency(dependencies, 'D', 'A');
    // addDependency(dependencies, 'E', 'B');
    // addDependency(dependencies, 'E', 'D');
    // addDependency(dependencies, 'E', 'F');
    
    uint32_t doneSteps = 0;
    
    printf("Step Sequence: ");
    
    do {
        char nextStep = 'Z';
        
        for(char step = 'A'; step <= 'Z'; step++) {
            if(!isStep(steps, step) || isStepDone(doneSteps, step)) {
                continue;
            }
            
            if(doesStepMeetDependencies(dependencies, doneSteps, step)) {
                if(step < nextStep) {
                    nextStep = step;
                }
            }
        }
        
        doStep(&doneSteps, nextStep);
        printf("%c", nextStep);
        
    } while(steps != doneSteps);
    
    printf("\n");
    
    return 0;
}
