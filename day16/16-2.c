#include <stdio.h>
#include <stdbool.h>

#define REGISTER_COUNT 4
#define NOT_A_REGISTER -1
#define OP_COUNT 16

typedef int Registers[REGISTER_COUNT];

typedef struct {
    int opcode;
    int input_a;
    int input_b;
    int output_reg;
} Instruction;

typedef enum {
    OP_ADDR,
    OP_ADDI,
    OP_MULR,
    OP_MULI,
    OP_BANR,
    OP_BANI,
    OP_BORR,
    OP_BORI,
    OP_SETR,
    OP_SETI,
    OP_GTIR,
    OP_GTRI,
    OP_GTRR,
    OP_EQIR,
    OP_EQRI,
    OP_EQRR
} Operation;

int is_valid_register(int index) {
    return (index >= 0 && index <= REGISTER_COUNT);
}

int find_possible_ops(Instruction instruction, Registers before, Registers after) {
    int possible_ops = 0;
    
    int value_a = instruction.input_a;
    int value_b = instruction.input_b;
    
    bool register_a_valid = is_valid_register(value_a);
    bool register_b_valid = is_valid_register(value_b);
    int register_a = before[value_a % REGISTER_COUNT];
    int register_b = before[value_b % REGISTER_COUNT];
    
    int output = after[instruction.output_reg];
    
    if(output == value_a) {
        possible_ops |= (1 << OP_SETI); 
    }
    
    if(register_a_valid) {
        if(output == (register_a + value_b)) {
            possible_ops |= (1 << OP_ADDI);
        }
        
        if(output == (register_a * value_b)) {
            possible_ops |= (1 << OP_MULI); 
        }
        
        if(output == (register_a & value_b)) {
            possible_ops |= (1 << OP_BANI);
        }
        
        if(output == (register_a | value_b)) {
            possible_ops |= (1 << OP_BORI);
        }
        
        if(output == register_a) {
            possible_ops |= (1 << OP_SETR);
        }
        
        if(output == (register_a > value_b)) {
            possible_ops |= (1 << OP_GTRI);
        }
        
        if(output == (register_a == value_b)) {
            possible_ops |= (1 << OP_EQRI);
        }
    }
    
    if(register_b_valid) {
        if(output == (value_a > register_b)) {
            possible_ops |= (1 << OP_GTIR);
        }
        
        if(output == (value_a == register_b)) {
            possible_ops |= (1 << OP_EQIR);
        }
    }
    
    if(register_a_valid && register_b_valid) {
        if(output == (register_a + register_b)) {
            possible_ops |= (1 << OP_ADDR);
        }
        
        if(output == (register_a * register_b)) {
            possible_ops |= (1 << OP_MULR);
        }
        
        if(output == (register_a & register_b)) {
            possible_ops |= (1 << OP_BANR);
        }
        
        if(output == (register_a | register_b)) {
            possible_ops |= (1 << OP_BORR);
        }
        
        if(output == (register_a > register_b)) {
            possible_ops |= (1 << OP_GTRR);
        }
        
        if(output == (register_a == register_b)) {
            possible_ops |= (1 << OP_EQRR);
        }
    }
    
    
    return possible_ops;
}

int count_flags(int flags) {
    int count = 0;
    
    while(flags != 0) {
        if(flags & 1) {
            count++;
        }
        
        flags = flags >> 1;
    }
    
    return count;
}

void perform_op(Instruction i, Registers reg, int operations[OP_COUNT]) {
    
    int operation = operations[i.opcode];
    
    if(operation == (1 << OP_ADDR)) {
        reg[i.output_reg] = reg[i.input_a] + reg[i.input_b];
    }
    
    if(operation == (1 << OP_ADDI)) {
        reg[i.output_reg] = reg[i.input_a] + i.input_b;
    }
    
    if(operation == (1 << OP_MULR)) {
        reg[i.output_reg] = reg[i.input_a] * reg[i.input_b];
    }
    
    if(operation == (1 << OP_MULI)) {
        reg[i.output_reg] = reg[i.input_a] * i.input_b;
    }
    
    if(operation == (1 << OP_BANR)) {
        reg[i.output_reg] = reg[i.input_a] & reg[i.input_b];
    }
    
    if(operation == (1 << OP_BANI)) {
        reg[i.output_reg] = reg[i.input_a] & i.input_b;
    }
    
    if(operation == (1 << OP_BORR)) {
        reg[i.output_reg] = reg[i.input_a] | reg[i.input_b];
    }
    
    if(operation == (1 << OP_BORI)) {
        reg[i.output_reg] = reg[i.input_a] | i.input_b;
    }
    
    if(operation == (1 << OP_SETR)) {
        reg[i.output_reg] = reg[i.input_a];
    }
    
    if(operation == (1 << OP_SETI)) {
        reg[i.output_reg] = i.input_a;
    }
    
    if(operation == (1 << OP_GTIR)) {
        reg[i.output_reg] = i.input_a > reg[i.input_b];
    }
    
    if(operation == (1 << OP_GTRI)) {
        reg[i.output_reg] = reg[i.input_a] > i.input_b;
    }
    
    if(operation == (1 << OP_GTRR)) {
        reg[i.output_reg] = reg[i.input_a] > reg[i.input_b];
    }
    
    if(operation == (1 << OP_EQIR)) {
        reg[i.output_reg] = i.input_a == reg[i.input_b];
    }
    
    if(operation == (1 << OP_EQRI)) {
        reg[i.output_reg] = reg[i.input_a] == i.input_b;
    }
    
    if(operation == (1 << OP_EQRR)) {
        reg[i.output_reg] = reg[i.input_a] == reg[i.input_b];
    }
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    
    int b0, b1, b2, b3, i0, i1, i2, i3, a0, a1, a2, a3;
    
    int possible_ops[OP_COUNT];
    
    for(int i = 0; i < OP_COUNT; i++) {
        possible_ops[i] = (1 << (OP_COUNT)) - 1;
    }
    
    
    while(fscanf(fp, "Before: [%d, %d, %d, %d]\n%d %d %d %d\nAfter:  [%d, %d, %d, %d]\n", &b0, &b1, &b2, &b3, &i0, &i1, &i2, &i3, &a0, &a1, &a2, &a3) == 12) {
        
        Registers registers_before = {b0, b1, b2, b3};
        Registers registers_after  = {a0, a1, a2, a3};
        Instruction instruction    = {i0, i1, i2, i3};
        
        int instruction_possible_ops = find_possible_ops(instruction, registers_before, registers_after);
        
        // printf("Possible ops for %d: %d\n", instruction.opcode, count_flags(instruction_possible_ops));
        
        possible_ops[instruction.opcode] &= instruction_possible_ops;
    }
    
    int taken_ops = 0;
    
    while(taken_ops != ((1 << (OP_COUNT)) - 1)) {
        
        for(int i = 0; i < OP_COUNT; i++) {
            if(count_flags(possible_ops[i]) == 1) {
                taken_ops |= possible_ops[i];
            } else {
                possible_ops[i] &= ~taken_ops;
            }
        }
        
    }
    
    
    Registers reg = {0};
    
    Instruction i;
    while(fscanf(fp, "%d %d %d %d", &i.opcode, &i.input_a, &i.input_b, &i.output_reg) == 4) {
        perform_op(i, reg, possible_ops);
    }
    
    printf("Register 0: %d\n", reg[0]);
    
    fclose(fp);
    
    return 0;
}
