#include<stdio.h>
#include<string.h>
#include "preprocessing.h"

void preprocess_instruction(char* instruction){
    int i=0;
    int j=0;

    while(instruction[i]){
        if(instruction[i]!=' ' && instruction[i]!='\t' && instruction[i]!='\n'){
            instruction[j++]=instruction[i];
        }
        i++;
    }

    instruction[j]='\0';

    return;
}