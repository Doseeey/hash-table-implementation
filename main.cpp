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

//Methods to free memory
void free_item(hash_item* item) {
    free(item -> key);
    free(item -> value);
    free(item);
}

void free_table(hash_table* table) {
    for (int i = 0; i < table -> size; i++) {
        hash_item* item = table -> items[i];
        //Segmentation fault if trying to free on NULL item
        if (item) {
            free_item(item);
        }
    }

    free(table -> items);
    free(table);
}

//Insert function
void insert_value(hash_table* table, char* key, char* value) {
    //Init item
    hash_item* item = create_item(key, value);

    //Get the index
    int index = hash_function(key);  

    hash_item* table_item = table -> items[index];
    if (!table_item) {
        //Table item is empty, setting a value
        table -> items[index] = item;
        table -> count += 1;
    } else {
        //Naive approach - oversetting value
        table -> items[index] = item;
        table -> count += 1;
    }
}


int main() {
    printf("Calculated index for value 'Test': %i\n", hash_function((char*)"Test"));
    printf("Calculated index for value 'Tset': %i\n", hash_function((char*)"Tset"));

    hash_table* table = create_table(CAPACITY);
    insert_value(table, (char*)"Test", (char*)"Test");
    insert_value(table, (char*)"Tset", (char*)"Different value with the same key value");
    insert_value(table, (char*)"10", (char*)"Another value");
    std::cout << "Hash table contents:\n";
    std::cout << "Index        Key        Value\n";
    for (int i = 0; i < table -> size; i++) {
        if (table -> items[i]) {
            printf("%d         %s        %s\n", i, table -> items[i] -> key, table -> items[i] -> value);
        }
    }
    
    free_table(table);
    return 0;
}