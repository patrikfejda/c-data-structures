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
clear ; rm main ; gcc task4.c -std=c99 -pedantic -Wall -Werror -Wextra -o main ; ./main

*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int CAPACITY;
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

typedef struct HashTable {
    HASHTABLEITEM** items;
} HASHTABLE;

HASHTABLEITEM* create_item(int key, char* value) {
    HASHTABLEITEM* item = (HASHTABLEITEM*) calloc (1, sizeof(HASHTABLEITEM));
    strcpy(item->value, value);
    item->key = key;
    return item;
}

HASHTABLE* create_table() {
    HASHTABLE* table = (HASHTABLE*) calloc (1, sizeof(HASHTABLE));
    table->items = (HASHTABLEITEM**) calloc (CAPACITY, sizeof(HASHTABLEITEM*));
    for (int i=0; i<CAPACITY; i++)
        table->items[i] = NULL;
    return table;
}

void free_item(HASHTABLEITEM* item) {
    free(item);
}

void handle_collision(HASHTABLE* table, int index, HASHTABLEITEM* item) {

    while(table->items[index] != NULL) {
        index++;
        if (index > CAPACITY) {
            index = 0;
        }
        // printf("INDEX++ %li\n", index);
    }

   table->items[index] = item;
 }

void insert(HASHTABLE* table, int key, char* value) {

    HASHTABLEITEM* item, *current_item;
    item = create_item(key, value);

    int index = hash(key);

    current_item = table->items[index];
    if (current_item == NULL) {
        table->items[index] = item;
    }

    else {
        // printf(">> %i (%i)\n", key, index);
        handle_collision(table, index, item);
    }
}

char* search(HASHTABLE* table, int key) {
    int index = hash(key);
    while(table->items[index] == NULL || table->items[index]->key != key) {
        index++;
        if (index >= CAPACITY) {
            index = 0;
        }

    }
    HASHTABLEITEM* item = table->items[index];
    if (item->key == key) {
        return item->value;
    }
    return NULL;
}

void delete(HASHTABLE* table, int key) {
    int index = hash(key);
    // printf("DEL %i %i\n", key, index);

    while(table->items[index] == NULL || table->items[index]->key != key) {
        index++;
        if (index >= CAPACITY) {
            index = 0;
        }

    }
    HASHTABLEITEM* item = table->items[index];
    table->items[index] = NULL;
    free_item(item);
    return;

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
    printf("\n-------------------\n");
    for (int i=0; i<CAPACITY; i++) {
        if (table->items[i]) {
            printf("(%d) %i-->%s\n", i, table->items[i]->key, table->items[i]->value);
        }
        else {
            printf("(%d)\n", i);
        }
    }
    printf("-------------------\n");
}

void get_capacity() {
    CAPACITY = 1;
    FILE *file;
    file = fopen(DATASETFILE, "r+");
    char file_text[MAXFILELINELENGTH];
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {

       CAPACITY++;
    }
    fclose(file);
}


int main() {
    clock_t time;
    double time_taken;
    get_capacity();

    HASHTABLE* ht = create_table(CAPACITY);
    time = clock();
    FILE *file;
    file = fopen(DATASETFILE, "r+");
    int i;
    char file_text[MAXFILELINELENGTH], *substring;
    // int count = 0;
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {
        // printf("%i %s", count++, file_text);
        substring = strtok(file_text, ";");
        i = atoi(substring);

        substring = strtok(NULL, ";");

        insert(ht, i, substring);
    }
    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> Reading from file to hash table - %f miniseconds\n", time_taken);


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

    // printf("||||||||||||||||||||||\n");

    // print_table(ht);

    time = clock();
    file = fopen(RANDOMDATASETFILE, "r+");

    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {
        substring = strtok(file_text, ";");
        i = atoi(substring);
        // printf("%s", substring);
        delete(ht, i);
    }

    fclose(file);

    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> test_delete_all_records - %f miniseconds\n", time_taken);



    // printf("||||||||||||||||||||||\n");


    // print_search(ht, 1);
    // print_search(ht, 2);
    // print_search(ht, 3);
    // print_search(ht, 88);
    // print_search(ht, 123);

    // printf("||||||||||||||||||||||\n");

    // print_table(ht);

    // printf("||||||||||||||||||||||\n");


    // delete(ht, 1);
    // delete(ht, 100);
    // print_table(ht);
    return 0;
}
