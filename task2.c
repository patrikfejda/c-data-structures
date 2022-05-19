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

fixup(), right_rotate() and left_rotate() original source, with changes
https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/

KOMPILOVAT PRIKAZOM:
clear ; rm main ; gcc task2.c -std=c99 -pedantic -Wall -Werror -Wextra -o main ; ./main

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
    struct node *parent;
    char red; // 1 -> red ; 0 -> black
} NODE;

int add_node_to_root(NODE* node, NODE* root);
int first_is_smaller(int first, int second);
int add_node_left(NODE* node, NODE* root);
int add_node_right(NODE* node, NODE* root);

// global root for the entire tree
NODE* root = NULL;


NODE* new_node(int val) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->val = val;
    node->red = 1;
    if (DEBUG_LOGS) {
        printf("NEW DONE val:%i adress:%p\n", val, (void *) node);
    }
    return node;
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
        node->parent = root;
        return 0;
    }
    add_node_to_root(node, root->left);
    return 0;
}

int add_node_right(NODE* node, NODE* root) {
    if (root->right == NULL) {
        root->right = node;
        node->parent = root;
        return 0;
    }
    add_node_to_root(node, root->right);
    return 0;
}

int add_node_to_root(NODE* node, NODE* root) {
    if (first_is_smaller(node->val, root->val)) {
        add_node_left(node, root);
        return 0;
    }
    add_node_right(node, root);
        return 0;
}

void rightrotate(NODE* parent)
{
	NODE* left;
	left = parent->left;
	parent->left = left->right;
	if (parent->left)
		parent->left->parent = parent;
	left->parent = parent->parent;
	if (!parent->parent)
		root = left;
	else if (parent == parent->parent->left)
		parent->parent->left = left;
	else
		parent->parent->right = left;
	left->right = parent;
	parent->parent = left;
}

void leftrotate(NODE* temp)
{
	NODE* right = temp->right;
	temp->right = right->left;
	if (temp->right)
		temp->right->parent = temp;
	right->parent = temp->parent;
	if (!temp->parent)
		root = right;
	else if (temp == temp->parent->left)
		temp->parent->left = right;
	else
		temp->parent->right = right;
	right->left = temp;
	temp->parent = right;
}

void fixup(NODE* root, NODE* pt)
{
	NODE* parent_pt = NULL;
	NODE* grand_parent_pt = NULL;

	while ((pt != root) && (pt->red != 0) && (pt->parent->red == 1)) {
		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		if (parent_pt == grand_parent_pt->left) {

			NODE* uncle_pt = grand_parent_pt->right;

			if (uncle_pt != NULL && uncle_pt->red == 1)
			{
				grand_parent_pt->red = 1;
				parent_pt->red = 0;
				uncle_pt->red = 0;
				pt = grand_parent_pt;
			}

			else {

				if (pt == parent_pt->right) {
					leftrotate(parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				rightrotate(grand_parent_pt);
				int t = parent_pt->red;
				parent_pt->red = grand_parent_pt->red;
				grand_parent_pt->red = t;
				pt = parent_pt;
			}
		}

		else {
			NODE* uncle_pt = grand_parent_pt->left;

			if ((uncle_pt != NULL) && (uncle_pt->red == 1))
			{
				grand_parent_pt->red = 1;
				parent_pt->red = 0;
				uncle_pt->red = 0;
				pt = grand_parent_pt;
			}
			else {

				if (pt == parent_pt->left) {
					rightrotate(parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				leftrotate(grand_parent_pt);
				int t = parent_pt->red;
				parent_pt->red = grand_parent_pt->red;
				grand_parent_pt->red = t;
				pt = parent_pt;
			}
		}
	}

	root->red = 0;
}

NODE* delete_node(int val, NODE* root)
{
    NODE* temp;

    if (val < root->val) {
        root->left = delete_node(val, root->left);
        return root;
    }
    else if (val > root->val) {
        root->right = delete_node(val, root->right);
        return root;
    }

    else {
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }


        // find smallest node in right sub-root
        temp = root->right;
        while (temp && temp->left != NULL) {
            temp = temp->left;
        }

        root->val = temp->val;
        root->right = delete_node(temp->val, root->right);
        return root;
    }
}

NODE* find_node(int val, NODE* root) {
    if (root->val == val) {
        return root;
    }

    if (first_is_smaller(val, root->val)) {
        if (root->left == NULL) {
            perror("Val not found in root!");
        }
        return find_node(val, root->left);
    }
    if (root->right == NULL) {
        perror("Val not found in root!");
    }
    return find_node(val, root->right);
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


// driver code
int main()
{
    NODE* node;
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
        fixup(root, node);
    }
    fclose(file);
    time = clock() - time;
    time_taken = ((double)time)/CLOCKS_PER_MINISEC;
    printf(">> Reading from file to binary tree - %f miniseconds\n", time_taken);


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
