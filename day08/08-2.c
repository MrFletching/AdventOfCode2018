#include <stdio.h>

#define DATA_MAX 20000
#define CHILD_MAX 10

void readData(int data[]) {
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    int val;
    int i = 0;
    
    while(fscanf(fp, "%d", &val) == 1) {
        data[i] = val;
        i++;
    }
    
    fclose(fp);
}

int sumMeta(int **data) {
    
    // Read the header
    int nodeCount = **data;
    (*data)++;
    int metaCount = **data;
    (*data)++;
    
    int sum = 0;
    
    int childSums[CHILD_MAX];
    
    // Read other nodes
    for(int i = 0; i < nodeCount; i++) {
        childSums[i] = sumMeta(data);
    }
    
    // Read metadata
    for(int i = 0; i < metaCount; i++) {
        if(nodeCount == 0) {
            sum += **data;
        } else {
            int index = **data - 1;
            
            if(index >= 0 && index < nodeCount) {
                sum += childSums[index];
            }
            
        }
        (*data)++;
    }
    
    return sum;
}

int main() {
    int data[DATA_MAX];
    readData(data);
    
    int *data_p = data;
    int sum = sumMeta(&data_p);
    
    printf("Sum of Metadata: %d\n", sum);
    
    return 0;
}
