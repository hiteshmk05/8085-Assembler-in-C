#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "hashmap.h"

void initialize(map* hashmap){
    for(int i=0;i<MAP_SIZE;i++){
        hashmap->bucket[i]=NULL;
    }
}

void insert_map(map* hashmap,char* instruction){
    int hash_value=hash_function(instruction);
    
    node* new_node=(node*)malloc(sizeof(node));
    strcpy(new_node->instruction,instruction);
    
    new_node->next=NULL;
    new_node->instruction[sizeof(new_node->instruction) - 1]='\0';

    if(hashmap->bucket[hash_value]==NULL){

        hashmap->bucket[hash_value]=new_node;
        
    }else{
        node* iterator = hashmap->bucket[hash_value];

        while(iterator->next){
            iterator=iterator->next;
        }

        iterator->next=new_node;
    }

}

int search_map(map* hashmap,char* instruction,char *hit){
    int hash_value=hash_function(instruction);

    if(hashmap->bucket[hash_value]==NULL){
        return 0;
    }

    node* current=hashmap->bucket[hash_value];

    while(current){
        const char* colon_position=strchr(current->instruction,':');
        size_t length=colon_position-current->instruction;
        char buffer[20];
        strncpy(buffer,current->instruction,length);
        buffer[length]='\0';

        if(strcmp(buffer,instruction)==0){
            strcpy(hit,current->instruction);
            return 1;
        }
        current=current->next;
    }

    return 0;
}

int hash_function(char* instruction){
    
    size_t colon_position=strcspn(instruction,":");
    char* buffer=(char*)malloc((colon_position+1)*sizeof(char));

    strncpy(buffer,instruction,colon_position);
    buffer[colon_position]='\0';

    const int prime = 31; 
    const int mod = 1e9 + 9; 
    unsigned long long hash_value = 0;
    unsigned long long p_pow = 1;
    int len = strlen(buffer);

    for (int i = 0; i < len; i++) {
        hash_value = (hash_value + (buffer[i] - 'a' + 1) * p_pow) % mod;
        p_pow = (p_pow * prime) % mod;
    }
    return (hash_value%MAP_SIZE);
}

void free_map(map* hashmap){
    for (int i = 0; i < MAP_SIZE; i++) {
        node *current = hashmap->bucket[i];
        while (current != NULL) {
            node *to_free = current;
            current = current->next;
            free(to_free);
        }
    }
}

void print_map(map*hashmap){
    for (int i = 0; i < MAP_SIZE; i++) {
        node *current = hashmap->bucket[i];
        if (current != NULL) {
            printf("Index %d: ", i);
            while (current != NULL) {
                printf("%s -> ", current->instruction);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}