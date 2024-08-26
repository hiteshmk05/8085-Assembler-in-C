#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H
#include<stddef.h>

typedef struct{
    char* string;
}string_vector;

void insert_string(string_vector** str_vector,size_t* size,const char* str);
void free_string_vector(string_vector* str_vector,size_t size);
void print_string_vector(const string_vector* str_vector,size_t size);

#endif