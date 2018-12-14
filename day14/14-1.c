#include <stdio.h>

#define MAX_RECIPES 1000000

int main() {
    
    int scores[MAX_RECIPES];
    scores[0] = 3;
    scores[1] = 7;
    int recipe_count = 2;
    
    int index_a = 0;
    int index_b = 1;
    
    const int RECIPE_TARGET = 765071;
    const int EXTRA_RECIPES = 10;
    
    while(recipe_count < RECIPE_TARGET + EXTRA_RECIPES) {
        int score_a = scores[index_a];
        int score_b = scores[index_b];
        int sum = score_a + score_b;
        
        if(sum >= 10) {
            scores[recipe_count] = sum / 10;
            recipe_count++;
            
            scores[recipe_count] = sum % 10;
            recipe_count++;
        } else {
            scores[recipe_count] = sum;
            recipe_count++;
        }
        
        index_a = (index_a + scores[index_a] + 1) % recipe_count;
        index_b = (index_b + scores[index_b] + 1) % recipe_count;
        
        // for(int i = 0; i < recipe_count; i++) {
        //     printf(" %d ", scores[i]);
        // }
        // printf("\n");
    }
    
    for(int i = RECIPE_TARGET; i < RECIPE_TARGET + EXTRA_RECIPES; i++) {
        printf("%d", scores[i]);
    }
    
    printf("\n");
    
    return 0;
}
