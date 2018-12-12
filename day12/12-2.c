#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_RULES 32
#define STATE_WIDTH 100000

void print_pot(bool contains_plant) {
    printf(contains_plant ? "#" : ".");
}

void print_rule(int current_gen_bitmap, bool next_gen) {
    
    for(int i = 0; i < 5; i++) {
        int pot_contains_plant = current_gen_bitmap & (1 << (4 - i));
        print_pot(pot_contains_plant);
    }
    
    printf(" => ");
    print_pot(next_gen);
    printf("\n");
}

void print_state(bool state[STATE_WIDTH]) {
    int min = STATE_WIDTH - 1;
    int max = 0;
    
    for(int i = 0; i < STATE_WIDTH; i++) {
        if(state[i]) {
            if(i < min) {
                min = i;
            }
            
            if(i > max) {
                max =i ;
            }
        }
    }
    
    for(int i = min; i <= max; i++) {
        print_pot(state[i]);
    }
    printf("\n");
}

void parse_rule(bool rules[NUM_RULES], char rule_str[5], char rule_result) {
    int bitmap = 0;
    for(int i = 0; i < 5; i++) {
        if(rule_str[i] == '#') {
            bitmap += (1 << (4 - i));
        }
    }
    
    rules[bitmap] = (rule_result == '#');
}

void read_input(bool rules[NUM_RULES], bool initial_state[STATE_WIDTH], int *start_pos) {
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    char initial_state_str[STATE_WIDTH];
    
    fscanf(fp, "initial state: %s\n", initial_state_str);
    
    int state_width = strlen(initial_state_str);
    
    *start_pos = (STATE_WIDTH - state_width) / 2;
    
    for(int i = 0; i < state_width; i++) {
        char pot = initial_state_str[i];
        if(pot == '#') {
            initial_state[*start_pos+i] = true;
        }
    }
    
    char rule_str[5];
    char rule_result;
    
    while(fscanf(fp, "%s => %c", rule_str, &rule_result) == 2) {
        parse_rule(rules, rule_str, rule_result);
    }
    
    fclose(fp);
}

void generate_next_state(bool state[STATE_WIDTH], bool rules[NUM_RULES]) {
    bool current_state[STATE_WIDTH];
    memcpy(current_state, state, STATE_WIDTH * sizeof(bool));
    
    for(int i = 0; i < STATE_WIDTH - 4; i++) {
        
        int bitmap = 0;
        for(int j = 0; j < 5; j++) {
            if(current_state[i+j]) {
                bitmap += (1 << (4 - j));
            }
        }
        
        state[i+2] = rules[bitmap];
    }
}

int main() {
    
    bool rules[NUM_RULES] = {0};
    bool state[STATE_WIDTH] = {0};
    
    int start_pos = 0;
    read_input(rules, state, &start_pos);
    
    printf("Generation %2d: ", 0);
    print_state(state);
    
    // for(int i = 0; i < NUM_RULES; i++) {
    //     print_rule(i, rules[i]);
    // }
    
    int prev_sum = 0;
    
    for(int gen = 1; gen <= 1000; gen++) {
        generate_next_state(state, rules);
        //print_state(state);
        
        int sum = 0;
        for(int i = 0; i < STATE_WIDTH; i++) {
            if(state[i]) {
                sum += i - start_pos;
            }
        }
        
        printf("Generation %2d | Sum: %d | Diff: %d\n", gen, sum, sum - prev_sum);
        
        prev_sum = sum;
    }
    
    return 0;
}
