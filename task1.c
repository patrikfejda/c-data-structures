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
clear ; rm main ; gcc task1.c -std=c99 -pedantic -Wall -Werror -Wextra -o main ; ./main

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>


#define COUNT 10
#define DEBUG_LOGS 0
#define ERROROPENINGFILE 99
#define MAXFILELINELENGTH 40
#define NAMELENGTH 30
#define RANDOMDATASETFILE "./datasets/dataset_rand"
#define DATASETFILE "./datasets/dataset"
#define CLOCKS_PER_MINISEC 1000 // how many clocks in one minisecond


typedef struct node
{
    int val;
    char name[NAMELENGTH];
    struct node *left;
    struct node *right;
    int height;
} NODE;

NODE* add_node_to_root(NODE* node, NODE* root);
NODE* new_node(int val);
int print_node(NODE* node);
int first_is_smaller(int first, int second);
int add_node_left(NODE* node, NODE* root);
int add_node_right(NODE* node, NODE* root);
void print2D(NODE *root);
void print2DUtil(NODE*root, int space, char ch);
NODE* find_node(int val, NODE* root);
NODE* delete_node(int val, NODE* root);
int try_open_file(void);
int test_search_all_records(NODE* root);
int test_delete_all_records(NODE* root);
int getBalanceFactor(NODE* node);
int get_height(NODE* node);
NODE *rightRotate(NODE *y);
NODE *leftRotate(NODE *y);
int max(int a, int b);

int main() {

    NODE* node;
    // printf("PROGRAM START!\n");

    NODE* root;
    root = NULL;
    clock_t time;
    double time_taken;

    // read file
    time = clock();
    FILE *file;
    file = fopen(DATASETFILE, "r+");
    char file_text[MAXFILELINELENGTH], *substring;
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {

        substring = strtok(file_text, ";");
        node = new_node(atoi(substring));

        substring = strtok(NULL, ";");
        strcpy(node->name, substring);

        if (root == NULL) {
            root = node;
        }

        else {
            add_node_to_root(node, root);
        }
    }
    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> Reading from file to binary tree - %f miniseconds\n", time_taken);

    if (DEBUG_LOGS) {
        print2D(root);
    }

    time = clock();
    test_search_all_records(root);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> test_search_all_records() - %f miniseconds\n", time_taken);


    time = clock();
    test_delete_all_records(root);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    // printf("%f\n", time_taken);
    printf(">> test_delete_all_records() - %f miniseconds\n", time_taken);



    // printf("THE END!\n");
    return 0;
}

int test_search_all_records(NODE* root) {

    FILE *file;
    file = fopen(RANDOMDATASETFILE, "r+");

    char file_text[MAXFILELINELENGTH], *substring;
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {

        substring = strtok(file_text, ";");
        find_node(atoi(substring), root);
    }

    fclose(file);

    return 0;
}

int test_delete_all_records(NODE* root) {

    FILE *file;
    file = fopen(RANDOMDATASETFILE, "r+");

    char file_text[MAXFILELINELENGTH], *substring;
    while (fgets(file_text, MAXFILELINELENGTH, file) != NULL)
    {

        substring = strtok(file_text, ";");
        delete_node(atoi(substring), root);
    }

    fclose(file);

    return 0;
}


int try_open_file(void) {
    FILE *file;
    if ((file = fopen(DATASETFILE, "r+")) == NULL)
    {
        printf("Error opening file.\n");
        return ERROROPENINGFILE;
    }
    fclose(file);
    return 0;
}


NODE* new_node(int val) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->height = 0;
    node->val = val;
    if (DEBUG_LOGS) {
        printf("NEW DONE val:%i adress:%p\n", val, (void *) node);
    }
    return node;
}

NODE* delete_node(int val, NODE* root) {
    NODE* temp;

    // val != root-> val
    if (val < root->val) {
        root->left = delete_node(val, root->left);
        return root;
    }
    if (val > root->val) {
        root->right = delete_node(val, root->right);
        return root;
    }
    // val = root-> val
    if (root->left == NULL) {
        temp = root->right;
        free(root);
        return temp;
    }
    if (root->right == NULL) {
        temp = root->left;
        free(root);
        return temp;
    }
    // find smallest node in right subroot
    temp = root->right;
    while (temp && temp->left != NULL) {
        temp = temp->left;
    }

    root->val = temp->val;
    strcpy(root->name, temp->name);
    root->right = delete_node(temp->val, root->right);

    return root;
}

NODE* find_node(int val, NODE* root) {
    if (root->val == val) {
        return root;
    }

    if (first_is_smaller(val, root->val)) {
        if (root->left == NULL) {
            printf("Val %i was not found in root!", val);
            perror("Val was not found in root!");
        }
        return find_node(val, root->left);
    }
    if (root->right == NULL) {
        printf("Val %i was not found in root!", val);
        perror("Val not found in root!");
    }
    return find_node(val, root->right);
}

int print_node(NODE* node) {
    printf("NODE val:%i left:%p right:%p\n", node->val, (void*)node->left, (void*)node->right);
    return 0;
}

int first_is_smaller(int first, int second) {
    if (first < second) {
        return 1;
    }
    return 0;
}

int add_node_left(NODE* node, NODE* root) {
    if (root->left == NULL) {
        root->left = node;
        return 0;
    }
    add_node_to_root(node, root->left);
    return 0;
}

int add_node_right(NODE* node, NODE* root) {
    if (root->right == NULL) {
        root->right = node;
        return 0;
    }
    add_node_to_root(node, root->right);
    return 0;
}

int max(int a, int b) {
    if (a>b) {
        return a;
    }
    return b;
}

NODE* add_node_to_root(NODE* node, NODE* root) {
    int balance;
    if (first_is_smaller(node->val, root->val)) {
        add_node_left(node, root);
    }
    else {
        add_node_right(node, root);
    }

    node->height = get_height(node);
    balance = getBalanceFactor(node);

    if (balance > 1 && node->val > node->left->val)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance > 1 && node->val < node->left->val) {
        return rightRotate(node);
    }

    if (balance < -1 && node->val < node->right->val)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    if (balance < -1 && node->val > node->right->val) {
        return leftRotate(node);
    }

    return node;
}

NODE *rightRotate(NODE *root) {
    NODE *left_node = root->left;
    NODE *left_node_right_child = left_node->right;

    left_node->right = root;
    root->left = left_node_right_child;

    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    left_node->height = max(get_height(left_node->left), get_height(left_node->right)) + 1;

    return left_node;
}

NODE *leftRotate(NODE *root) {
    NODE *right_node = root->right;
    NODE *right_node_left_child = right_node->left;

    right_node->left = root;
    root->right = right_node_left_child;

    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    right_node->height = max(get_height(right_node->left), get_height(right_node->right)) + 1;

    return right_node;
}


int getBalanceFactor(NODE* node) {
    if (node == NULL)
        return 0;
    return get_height(node->left) - get_height(node->right);
}

int get_height(NODE* node) {
    if (node == NULL)
        return 0;
    return max(get_height(node->left), get_height(node->right)) + 1;
}







// PRINT NODE root TO CONSOLE:
// src: https://www.geeksforgeeks.org/print-binary-root-2-dimensions/
// my contribution: printing +/- for right/left node
void print2DUtil(NODE*root, int space, char ch) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space, '+');

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%c", ch);
    printf("%d\n", root->val);

    // Process left child
    print2DUtil(root->left, space, '-');
}

// src: https://www.geeksforgeeks.org/print-binary-root-2-dimensions/
void print2D(NODE *root) {
   // Pass initial space count as 0
   print2DUtil(root, 0, ' ');
}
