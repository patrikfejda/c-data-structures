/*
SEMINARNY PROJEKT Z PREDMETU
DATOVE STRUKTURY A ALGORITMY

PATRIK FEJDA
FAKULTA INFORMATIKY A INFORMACNYCH TECHNOLOGII
SLOVENSKA TECHNICKA UNIVERZITA V BRATISLAVE

CVICIACI: ING. LUKAS KOHUTKA, PHD.

KOMPILATOR: GCC

//////////////////////////////////////////////////////////
///// PROSIM KOMPILOVAT A TESTOVAT V GCC (NIE VO VS) /////
//////////////////////////////////////////////////////////

KOMPILOVAT PRIKAZOM:
clear ; rm main ; gcc task3.c -std=c99 -pedantic -Wall -Werror -Wextra -o main ; ./main

Schema navrhu:

|    | ITEMS  | OVERFLOW BUCKETS |              |              |              |
|----|--------|------------------|--------------|--------------|--------------|
|  0 | item   | item -> next     | item -> NULL |              |              |
|  1 | item   | item -> next     | item -> NULL |              |              |
|  2 | item   | item -> next     | item -> NULL |              |              |
|  3 | item   | item -> next     | item -> next | item -> next | item -> NULL |
|  4 | item   | item -> next     | item -> next | item -> next | item -> NULL |
|  5 | item   | item -> next     | item -> next | item -> NULL |              |
|  6 | item   | item -> next     | item -> next | item -> NULL |              |
|  7 | item   | item -> next     | item -> NULL |              |              |
|  8 | item   | item -> next     | item -> NULL |              |              |
|  9 | item   | item -> NULL     |              |              |              |
| 10 | item   | item -> NULL     |              |              |              |


*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define CAPACITY 10
#define MAXNAMELENGTH 30
#define MAXFILELINELENGTH 40
#define RANDOMDATASETFILE "./datasets/dataset_rand"
#define DATASETFILE "./datasets/dataset"
#define CLOCKS_PER_MINISEC 1000 // how many clocks in one minisecond

int hash(int key) {
    return key % CAPACITY;
}
typedef struct HashTableItem {
    int key;
    char value[MAXNAMELENGTH];
} HASHTABLEITEM;

typedef struct LinkedList {
    HASHTABLEITEM* item;
    struct LinkedList* next;
} LINKEDLIST;
typedef struct HashTable {
    HASHTABLEITEM** items;
    LINKEDLIST** overflow_buckets;
} HASHTABLE;

LINKEDLIST* insert_item_to_list(LINKEDLIST* list, HASHTABLEITEM* item) {
    LINKEDLIST* temp, *node;

    temp = list;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    node = (LINKEDLIST*) calloc (1, sizeof(LINKEDLIST));
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;

}

HASHTABLEITEM* create_hashtable_item(int key, char* value) {
    HASHTABLEITEM* item = (HASHTABLEITEM*) calloc (1, sizeof(HASHTABLEITEM));
    item->key = key;
    strcpy(item->value, value);
    return item;
}

HASHTABLE* create_hashtable() {
    HASHTABLE* table;

    table = (HASHTABLE*) calloc (1, sizeof(HASHTABLE));
    table->items = (HASHTABLEITEM**) calloc (CAPACITY, sizeof(HASHTABLEITEM*));
    table->overflow_buckets = (LINKEDLIST**) calloc (CAPACITY, sizeof(LINKEDLIST*));
    for (int i=0; i<CAPACITY; i++) {
        table->items[i] = NULL;
        table->overflow_buckets[i] = NULL;
    }
    return table;
}

void handle_collision(HASHTABLE* table, int index, HASHTABLEITEM* item) {
    LINKEDLIST* linked_list_start = table->overflow_buckets[index];

    if (linked_list_start == NULL) {
        linked_list_start = (LINKEDLIST*) calloc (1, sizeof(LINKEDLIST));
        linked_list_start->item = item;
        table->overflow_buckets[index] = linked_list_start;
        return;
    }
    else {
        table->overflow_buckets[index] = insert_item_to_list(linked_list_start, item);
        return;
    }
 }

void insert(HASHTABLE* table, int key, char* value) {
    HASHTABLEITEM* item = create_hashtable_item(key, value);

    int index = hash(key);

    HASHTABLEITEM* currentent_item = table->items[index];
    if (currentent_item == NULL) {
        table->items[index] = item;
    }

    else {
        handle_collision(table, index, item);
        return;
    }
}

char* search(HASHTABLE* table, int key) {
    int index = hash(key);
    HASHTABLEITEM* item;
    LINKEDLIST* linked_list_start;

    item = table->items[index];
    if (item->key == key) {
        return item->value;
    }

    linked_list_start = table->overflow_buckets[index];
    if (linked_list_start == NULL) {
        return NULL;
    }

    do {
        if (item->key == key) {
            return item->value;
        }
        if (linked_list_start == NULL) {
            return NULL;
        }
        item = linked_list_start->item;
        linked_list_start = linked_list_start->next;
    } while (item != NULL);

    return NULL;
}

void delete(HASHTABLE* table, int key) {
    int index = hash(key);
    HASHTABLEITEM* item = table->items[index];
    LINKEDLIST* linked_list_start = table->overflow_buckets[index];
    if (item == NULL) {
        return;
    }
    else {
        // item is in table and OB is empty
        if (linked_list_start == NULL && item->key == key) {
            table->items[index] = NULL;
            free(item);
            return;
        }
        else if (linked_list_start != NULL) {
            // item is in table and OB is NOT empty - move first from OB to table
            if (item->key == key) {
                free(item);
                LINKEDLIST* node = linked_list_start;
                linked_list_start = linked_list_start->next;
                node->next = NULL;
                table->items[index] = create_hashtable_item(node->item->key, node->item->value);
                table->overflow_buckets[index] = linked_list_start;
                return;
            }
            // item is OB
            else {
                LINKEDLIST* current = linked_list_start;
                LINKEDLIST* previous = NULL;

                while (current) {
                    if (current->item->key == key) {
                        if (previous == NULL) {
                            table->overflow_buckets[index] = NULL;
                            return;
                        }
                        else {
                            previous->next = current->next;
                            current->next = NULL;
                            free(current);
                            table->overflow_buckets[index] = linked_list_start;
                            return;
                        }
                    }
                    previous = current;
                    current = current->next;
                }
            }
        }
    }
}

void print_search(HASHTABLE* table, int key) {
    char* value;
    value = search(table, key);
    if (value == NULL) {
        printf("Key [%i] is not in table!\n", key);
    }
    else {
        printf("Key:%i has value:%s\n", key, value);
    }
}

void print_table(HASHTABLE* table) {
// https://www.journaldev.com/35238/hash-table-in-c-plus-plus
    printf("\n-------------------\n");
    for (int i=0; i<CAPACITY; i++) {
        if (table->items[i]) {
            printf("(%d) %i-->%s", i, table->items[i]->key, table->items[i]->value);
            if (table->overflow_buckets[i]) {
                printf(" -> OVERFLOW -> ");
                LINKEDLIST* linked_list_start;
                linked_list_start = table->overflow_buckets[i];
                while (linked_list_start) {
                    printf("%i->%s ", linked_list_start->item->key, linked_list_start->item->value);
                    linked_list_start = linked_list_start->next;
                }
            }
            printf("\n");

        }
        else {
            printf("(%d)\n", i);
        }
    }
    printf("-------------------\n");
}


int main() {
    clock_t time;
    double time_taken;

    HASHTABLE* ht = create_hashtable(CAPACITY);
    time = clock();

    FILE *file;
    file = fopen(DATASETFILE, "r+");
    int i;
    char file_text[MAXFILELINELENGTH], *substring;
    // int counter = 0;
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {
        // printf("%i %s", counter++, file_text);

        substring = strtok(file_text, ";");
        i = atoi(substring);

        substring = strtok(NULL, ";");

        insert(ht, i, substring);
    }
    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> Reading from file to hash table - %f miniseconds\n", time_taken);



    // printf("||||||||||||||||||||||\n");

    time = clock();
    file = fopen(RANDOMDATASETFILE, "r+");

    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {
        substring = strtok(file_text, ";");
        i = atoi(substring);
        search(ht, i);
    }

    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> test_search_all_records - %f miniseconds\n", time_taken);


    // print_table(ht);

    time = clock();
    file = fopen(RANDOMDATASETFILE, "r+");

    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {
        substring = strtok(file_text, ";");
        i = atoi(substring);
        delete(ht, i);
    }

    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> test_delete_all_records - %f miniseconds\n", time_taken);
    // print_table(ht);

    // printf("||||||||||||||||||||||\n");


    // delete(ht, 1);
    // delete(ht, 100);
    return 0;
}
