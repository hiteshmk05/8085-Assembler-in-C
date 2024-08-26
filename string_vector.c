#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "string_vector.h"

void insert_string(string_vector** str_vector,size_t* size,const char* str){
    *str_vector=realloc(*str_vector,(*size+1)*sizeof(string_vector));
    (*str_vector)[*size].string=strdup(str);
    (*size)++;
}

void free_string_vector(string_vector* str_vector,size_t size){
    for(size_t i=0;i<size;i++){
        free(str_vector[i].string);
    }

    free(str_vector);
}

void print_string_vector(const string_vector* vector, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (vector[i].string != NULL) { 
            printf("%s\n",vector[i].string);
        }
    }
}