#include <iostream>
#include <string.h>

#define CAPACITY 50000

//Poor hash function to create collisions
unsigned long hash_function(char* str) {
    
    unsigned long result = 0;
    for (int i = 0; str[i]; i++) {
        result += str[i];
    }

    return result % CAPACITY;
}

//Structures for Hash Table
typedef struct hash_item {
    char* key;
    char* value;
} hash_item;

typedef struct hash_table {
    hash_item** items; //Double pointer to array of items
    int size;
    int count;
} hash_table;

//Init items and hash table
hash_item* create_item(char* key, char* value) {
    //Allocate memory
    hash_item* item = (hash_item*) malloc(sizeof(hash_item));
    item -> key = (char*) malloc(strlen(key) + 1);
    item -> value = (char*) malloc(strlen(value) + 1);

    //Set values
    strcpy(item -> key, key);
    strcpy(item -> value, value);

    return item;
} 

hash_table* create_table(int size) {
    //Allocate memory
    hash_table* table = (hash_table*) malloc(sizeof(hash_table));
    table -> items = (hash_item**) calloc(size, sizeof(hash_item*));

    //Set values
    table -> size = size;
    table -> count = 0;
    for (int i = 0; i < size; i++) {
        table -> items[i] = NULL;
    }

    return table;
}

int main() {
    printf("Calculated index for value 'Test': %i\n", hash_function((char*)"Test"));
    printf("Calculated index for value 'Tset': %i\n", hash_function((char*)"Tset"));
    return 0;
}