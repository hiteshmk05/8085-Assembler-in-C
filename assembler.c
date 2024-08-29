#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include "hashmap.h"
#include "string_vector.h"
#include "preprocessing.h"
#define MAX_LABELS 100

typedef struct{
    char label[50];
    char address[5];
}symbol;

typedef struct{
    char instruction[20];
    char opcode[3];
    int traversed;
}data_vector;

symbol symbol_table[MAX_LABELS];
data_vector* parsed_asm;


unsigned int symbol_index=0;
unsigned int parsed_index=0;
unsigned int symbol_index_curr=0;

void parse_assembly(map* hashmap,string_vector* file_vector,size_t file_lines);
int first_pass(const char* instruction,map* hashmap);
void second_pass(FILE* assembled_file,const char* filename);
int hex_to_decimal(const char* hex_string);
void decimal_to_hex(int decimal_numer,char* hex_number);
int is_string_empty(const char* str);


int main(int argc,char* argv[]){

    FILE* asm_pointer=fopen(argv[1],"r");

    FILE* inst_pointer=fopen("instruction_set.txt","r");;
    
    map hashmap;

    string_vector* file_vector=NULL;

    size_t file_lines=0;

    initialize(&hashmap);

    char buffer[100];
    while(fgets(buffer,sizeof(buffer),inst_pointer)){
        preprocess_instruction(buffer);
        insert_map(&hashmap,buffer);
    }

    fclose(inst_pointer);

    memset(buffer,0,sizeof(buffer));

    while(fgets(buffer,sizeof(buffer),asm_pointer)){
        preprocess_instruction(buffer);
        if(buffer[0]!='\0'){
            insert_string(&file_vector,&file_lines,buffer);
        }
    }

    fclose(asm_pointer);

    parsed_asm=(data_vector*)malloc(file_lines*sizeof(data_vector));
    parse_assembly(&hashmap,file_vector,file_lines);

    FILE* assembled_file=fopen(argv[2],"w");
    
    second_pass(assembled_file,argv[2]);
    
    free_map(&hashmap);
    free_string_vector(file_vector,file_lines);

    return 0;
}


void parse_assembly(map* hashmap,string_vector* file_vector,size_t file_lines){
    int location_counter=0;
    for(size_t i=0;i<file_lines;i++){
        const char* line=file_vector[i].string;
        if(strstr(line,"ORG")){
            char* starting_address=strstr(line,"ORG")+3;
            
            size_t len=strlen(starting_address);

            starting_address[len-1]='\0';
            location_counter=hex_to_decimal(starting_address);

            goto last;
        }

        char label[20];
        char mnumeniic[20];

        if(strchr(line,':')){
            char* colon_position=strchr(line,':');
            size_t length=colon_position-line;

            strncpy(label,line,length);

            label[length]='\0';

            strcpy(symbol_table[symbol_index].label,label);

            char hex_string[20];
            decimal_to_hex(location_counter,hex_string);

            strcpy(symbol_table[symbol_index].address,hex_string);

            symbol_index++;

            strncpy(mnumeniic, colon_position+1, sizeof(mnumeniic) - 1);

            mnumeniic[sizeof(mnumeniic) - 1] = '\0';        

        }else{
            strcpy(mnumeniic,line);
        }
        if(is_string_empty(mnumeniic)==0){
            int increment_by=first_pass(mnumeniic,hashmap);
            location_counter+=increment_by;
        }
        last:
    }

}

int first_pass(const char* instruction,map* hashmap){
    size_t length=strlen(instruction);
    
    char buffer[21];

    int buffer_cntr=0;
    int instruction_cntr=0;

    char hit[20];

    int increment_by=0;

    while(instruction_cntr<length){
        buffer[buffer_cntr++]=instruction[instruction_cntr];
        buffer[buffer_cntr]='\0';

        int is_hit=search_map(hashmap,buffer,hit);

        size_t hit_len=strlen(hit);

        if(is_hit){
            strcpy(parsed_asm[parsed_index].instruction,instruction);
            strncpy(parsed_asm[parsed_index].opcode,hit+hit_len-3,2);
            parsed_asm[parsed_index].opcode[2]='\0';
            parsed_asm[parsed_index].traversed=instruction_cntr+1;
            increment_by=hit[hit_len-1]-'0';
        }

        instruction_cntr++;
    }
    
    parsed_index++;

    return increment_by;

}

void second_pass(FILE* assembled_file,const char* filename){
    for (int i=0 ;i<parsed_index;i++) {
        char buffer[20];

        strcpy(buffer,parsed_asm[i].instruction+parsed_asm[i].traversed);
        fprintf(assembled_file,parsed_asm[i].opcode);
        fprintf(assembled_file,"\n");

        if(is_string_empty(buffer)){
            goto last;
        }

        size_t len=strlen(buffer);
        if(strcmp(buffer,symbol_table[symbol_index_curr].label)==0){
            char last_two[3]; 
            char first_two[3];

            char * address=symbol_table[symbol_index_curr].address;
            strncpy(last_two, address+strlen(address)-2, 2);
            last_two[2]='\0';

            strncpy(first_two,address,2);
            first_two[2]='\0';
            
            fprintf(assembled_file,last_two);
            fprintf(assembled_file,"\n");

            fprintf(assembled_file,first_two);
            fprintf(assembled_file,"\n");

            symbol_index_curr++;
        }else{
            buffer[len-1]='\0';

            int buffer_length=strlen(buffer);
            if(buffer_length==2){
                fprintf(assembled_file,buffer);
                fprintf(assembled_file,"\n");
            }else{
                char last_two[3];
                char first_two[3]; 

                strncpy(last_two, buffer+strlen(buffer)-2, 2);
                last_two[2] = '\0';

                strncpy(first_two,buffer,2);
                first_two[2] = '\0';

                fprintf(assembled_file,last_two);
                fprintf(assembled_file,"\n");

                fprintf(assembled_file,first_two);
                fprintf(assembled_file,"\n");
            }

        }

        last:

    }
}

int hex_to_decimal(const char* hex_string){
    return (int)strtol(hex_string,NULL,16);
}

void decimal_to_hex(int decimal_number,char* hex_string){
    sprintf(hex_string,"%X",decimal_number);
}   

int is_string_empty(const char* str){
    if (str == NULL || *str == '\0') {
        return 1;
    }

    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0; 
        }
        str++;
    }

    return 1;
}