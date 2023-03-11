#include <iostream>
#include <string.h>

#define CAPACITY 50000

unsigned long hash_function(char* str) {
    
    unsigned long result = 0;
    for (int i = 0; str[i]; i++) {
        result += str[i];
    }

    return result % CAPACITY;
}

int main() {
    printf("Calculated index for value 'Test': %i\n", hash_function((char*)"Test"));
    printf("Calculated index for value 'Tset': %i\n", hash_function((char*)"Tset"));
    return 0;
}