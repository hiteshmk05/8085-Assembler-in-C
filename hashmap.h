#ifndef HASHMAP_H
#define HASHMAP_H
#define MAP_SIZE 103

typedef struct node{
    char instruction[20];
    struct node* next; 
}node;

typedef struct{
    node* bucket[MAP_SIZE];
}map;

void initialize(map* hashmap);
void insert_map(map* hashmap,char* instruction);
int search_map(map* hashmap,char* instruction,char* hit);
int hash_function(char* instruction);
void free_map(map* hashmap);
void print_map(map*hashmap);

#endif
