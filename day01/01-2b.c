#include <stdio.h>

#define MAXCHAR 10

#define MIN -100000
#define MAX  100000
#define SIZE (MAX - MIN)

int main() {
    FILE *fp;
    char str[MAXCHAR];

    int sum = 0;
    int numbers[SIZE];
    
    while(1) {
        
        fp = fopen("input.txt", "r");
        
        while(fgets(str, MAXCHAR, fp) != NULL) {
            int number;
            sscanf(str, "%d", &number);
            
            sum += number;
            
            int index = sum - MIN;
            
            if(numbers[index] == 1) {
                printf("Seen %d before!\n", sum);
                return 0;
            } else {
                numbers[index] = 1;
            }
            
        }
        
        fclose(fp);
        
    }
    

    return 0;
}
