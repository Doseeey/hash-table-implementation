#include <iostream>
#include <string.h>

#define CAPACITY 10

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

//Linked List for handling collisions
typedef struct bucket_list {
    hash_item* item;
    struct bucket_list* next;
} bucket_list;;

typedef struct hash_table {
    hash_item** items; //Double pointer to array of items
    bucket_list** bucket; //Overflow bucket
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

bucket_list* create_list() {
    bucket_list* list = (bucket_list*) malloc(sizeof(bucket_list));
    return list;
}

bucket_list** create_bucket(hash_table* table) {
    //Allocate memory
    bucket_list** bucket = (bucket_list**) calloc(table -> size, sizeof(bucket_list*));
    //Set values
    for (int i = 0; i < table -> size; i++) {
        bucket[i] = NULL;
    }

    return bucket;
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
    table -> bucket = create_bucket(table);

    return table;
}

//Methods to free memory
void free_item(hash_item* item) {
    free(item -> key);
    free(item -> value);
    free(item);
}

void free_list(bucket_list* list) {
    delete[] list;
}

void free_bucket(hash_table* table) {
    bucket_list** buckets = table -> bucket;

    for (int i = 0; i < table -> size; i++) {
        free_list(buckets[i]);
    }

    free(buckets);
}

void free_table(hash_table* table) {
    for (int i = 0; i < table -> size; i++) {
        hash_item* item = table -> items[i];
        //Segmentation fault if trying to free on NULL item
        if (item) {
            free_item(item);
        }
    }

    free_bucket(table);
    free(table -> items);
    free(table);
}
//Bucket list insert:
//Allocate mem if list doesn't exist, or add another item to existing list
bucket_list* bucket_list_insert(bucket_list* list, hash_item* item) {
    if (!list) {
        bucket_list* first = create_list();
        first -> item = item;
        first -> next = NULL;
        //Update list with first value
        list = first; 
        return list;
    } else if (list -> next == NULL) {
        bucket_list* node = create_list();
        node -> item = item;
        node -> next = NULL;
        //Update pointer to next value in list
        list -> next = node;
        return list;
    }
    //Find last item in bucket
    bucket_list* temp_bucket = list;

    while (temp_bucket -> next -> next) {
        temp_bucket = temp_bucket -> next;
    }

    bucket_list* node = create_list();
    node -> item = item;
    node -> next = NULL;

    temp_bucket -> next = node;
    return list;
}

//Separate Chaining approach for handling collisions
void handle_collision(hash_table* table, hash_item* item, unsigned long index) {
    bucket_list* first = table -> bucket[index];

    if (!first) {
        //Case: bucket is empty
        first = create_list();
        first -> item = item;
        table -> bucket[index] = first;
        return;
    } else {
        //Case: bucket has already item(s)
        table -> bucket[index] = bucket_list_insert(first, item);
    }
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
        handle_collision(table, item, index);
    }
}

void delete_value(hash_table* table, char* key)
{
    int index = hash_function(key);
    hash_item* item = table->items[index];
    bucket_list* head = table->bucket[index];

    if (item == NULL)
    {
        return;
    }
    else {
        if (head == NULL && strcmp(item->key, key) == 0)
        {
            // Collision chain does not exist.
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            // Collision chain exists.
            if (strcmp(item->key, key) == 0)
            {
                // Set the head of the list as the new item.
                free_item(item);
                bucket_list* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_list(node);
                table->bucket[index] = head;
                return;
            }

            bucket_list* curr = head;
            bucket_list* prev = NULL;
            //Go through bucket
            while (curr)
            {
                if (strcmp(curr->item->key, key) == 0)
                {
                    if (prev == NULL)
                    {
                        free_list(head);
                        table->bucket[index] = NULL;
                        return;
                    }
                    else
                    {
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_list(curr);
                        table->bucket[index] = head;
                        return;
                    }
                }

                curr = curr->next;
                prev = curr;
            }
        }
    }
}


void print_table(hash_table* table)
{
    printf("---------------\n");
    printf("HASH TABLE\n");
    for(int i = 0; i < table -> size; i++){
        if(table -> items[i]){
            // print item on items[index]
            printf("Index %d Key %s Value %s \n", i, table -> items[i] -> key, table -> items[i] -> value);
            // hash_item* Ptr = table -> bucket[i] -> item;
            bucket_list* Bucket = table -> bucket[i];
            // print other items in bucket if exist
            while(Bucket){
                printf("Index %d Key %s Value %s \n", i, Bucket -> item -> key, Bucket -> item -> value);
                Bucket = Bucket -> next;
            }
        }
    }
    printf("---------------\n");
}

int main() {
    hash_table* small_table = create_table(CAPACITY);

    //Fill the table with initial values for all indexes
    insert_value(small_table, (char*)"a", (char*)"Initial Address");
    insert_value(small_table, (char*)"b", (char*)"Initial Address");
    insert_value(small_table, (char*)"c", (char*)"Initial Address");
    insert_value(small_table, (char*)"d", (char*)"Initial Address");
    insert_value(small_table, (char*)"e", (char*)"Initial Address");
    insert_value(small_table, (char*)"f", (char*)"Initial Address");
    insert_value(small_table, (char*)"g", (char*)"Initial Address");
    insert_value(small_table, (char*)"h", (char*)"Initial Address");
    insert_value(small_table, (char*)"i", (char*)"Initial Address");
    insert_value(small_table, (char*)"j", (char*)"Initial Address");

    print_table(small_table);

    //Add collisions to some addresses
    insert_value(small_table, (char*)"d", (char*)"Collision Address");
    insert_value(small_table, (char*)"e", (char*)"Collision Address");
    insert_value(small_table, (char*)"f", (char*)"Collision Address");
    insert_value(small_table, (char*)"g", (char*)"Collision Address");


    // std::cout << "Hash table contents:\n";
    // std::cout << "Index        Key        Value\n";
    // for (int i = 0; i < small_table -> size; i++) {
    //     if (small_table -> items[i]) {
    //         if (small_table -> bucket[i]) {
    //             printf("%d         %s        %s   ->     %s\n", i, small_table -> items[i] -> key, small_table -> items[i] -> value, small_table -> bucket[i] -> item -> value);
    //         } else {
    //             printf("%d         %s        %s        %s\n", i, small_table -> items[i] -> key, small_table -> items[i] -> value, small_table -> bucket[i]);
    //         }
    //     }
    // }

    print_table(small_table);

    //Remove some initial addresses
    delete_value(small_table, (char*)"d");
    delete_value(small_table, (char*)"e");
    delete_value(small_table, (char*)"f");
    delete_value(small_table, (char*)"g");

    print_table(small_table);

    //Remove some collision addresses
    delete_value(small_table, (char*)"d");
    delete_value(small_table, (char*)"e");

    print_table(small_table);

    //Clean memory
    free_table(small_table);

    //Minimal steps: O(1)
    //Maximal steps: O(n) - measured by linked list (n-elements of the same address)

    return 0;
}