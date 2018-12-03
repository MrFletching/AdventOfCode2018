#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define STR_LENGTH 100
#define ID_LENGTH 26

bool arrayContains(int *arr, int size, int val) {
    for(int i = 0; i < size; i++) {
        int count = arr[i];
        
        if(count == val) {
            return true;
        }
    }
    
    return false;
}

int main() {
    FILE *fp;
    char str[ID_LENGTH+1];


    fp = fopen("input.txt", "r");
    
    int sum = 0;
    
    int dups  = 0;
    int trips = 0;
    
    while(fgets(str, STR_LENGTH, fp) != NULL) {
        
        // Count the number of each letter in the ID
        int alphabetCount[26];
        memset(&alphabetCount, 0, sizeof(alphabetCount));
        
        for(int i = 0; i < ID_LENGTH; i++) {
            char c = str[i];
            int i = c - 'a';
            
            alphabetCount[i] += 1;
        }
        
        if(arrayContains(alphabetCount, 26, 2)) {
            dups++;
        }
        
        if(arrayContains(alphabetCount, 26, 3)) {
            trips++;
        }
        
    }
    
    printf("Dups:  %d\n", dups);
    printf("Trips: %d\n", trips);
    
    int checksum = dups * trips;
    printf("Checksum: %d\n", checksum);

}
