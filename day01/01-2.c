#include <stdio.h>

#define MAXCHAR 10
#define MAXNUMBERS 1000000

int main() {
    FILE *fp;
    char str[MAXCHAR];

    int sum = 0;
    int numbersSeen = 0;
    int numbers[MAXNUMBERS];
    
    while(1) {
        
        fp = fopen("input.txt", "r");
        
        while(fgets(str, MAXCHAR, fp) != NULL) {
            int number;
            sscanf(str, "%d", &number);
            
            sum += number;
            
            for(int i = 0; i < numbersSeen; i++) {
                if(numbers[i] == sum) {
                    printf("Seen %d before!\n", sum);
                    return 0;
                }
            }
            
            numbers[numbersSeen] = sum;
            
            numbersSeen += 1;
            
            if(numbersSeen % 1000 == 0) {
                printf("Numbers Seen: %d\n", numbersSeen);
            }
            
        }
        
        fclose(fp);
        
    }
    

    return 0;
}
