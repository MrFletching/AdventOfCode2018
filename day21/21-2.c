#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main() {
    int64_t r0 = 0;
    int64_t r1 = 0;
    int64_t r2 = 0;
    int64_t r3 = 0;
    int64_t r4 = 0;
    int64_t r5 = 0;
    bool b1 = false;
    
    do {
        r1 = r3 | 0x10000;
        r3 = 10373714;
        
        do {
            // printf("r1: %lld, r3: %lld\n", r1, r3);
            
            b1 = false;
            
            r3 += r1 & 0xFF;
            r3 = ((r3 & 0xFFFFFF) * 65899) & 0xFFFFFF;
            
            if (256 <= r1) {
                b1 = true;
                r1 = r1 >> 8;
            }
            
        } while(b1);
        
        printf("%lld\n", r3);
        
        //printf("[%lld, %lld, %lld, %lld, %lld, %lld]\n", r0, r1, r2, r3, r4, r5);
        
    } while(r3 != r0);

}
