#include <stdio.h>

#define MAXCHAR 10

int main() {
    FILE *fp;
    char str[MAXCHAR];


    fp = fopen("input.txt", "r");
    
    int sum = 0;
    
    while(fgets(str, MAXCHAR, fp) != NULL) {
        int number;
        sscanf(str, "%d", &number);
        
        sum += number;
    }
    
    fclose(fp);
    
    
    printf("%d\n", sum);

    return 0;
}
