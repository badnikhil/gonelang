#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <stdio.h>

void execute();


void printsentence(char* sentence, int newline) {
    FILE *asm_file = fopen("build/output.s", "w");

    fprintf(asm_file,
        ".section .data\n"
        "msg:\n"
        ".ascii \"%s%s\"\n"   
        "len = . - msg\n\n"

        ".section .text\n"
        ".global _start\n"
        "_start:\n"
        "    mov $1, %%rax\n"         
        "    mov $1, %%rdi\n"        
        "    lea msg(%%rip), %%rsi\n"
        "    mov $len, %%rdx\n"      
        "    syscall\n\n"

        "    mov $60, %%rax\n"        
        "    xor %%rdi, %%rdi\n"
        "    syscall\n",
        sentence,
        newline ? "\\n" : ""   
    );
    fclose(asm_file);
    execute();
}


enum TokenType{
    _EXIT = 1
};
struct TOKEN{
    enum TokenType type;
    int value;
};

void generate_for_exit(int value) {
    FILE *asm_file = fopen("build/output.s", "w");

  fprintf(asm_file, 
    ".global _start\n"
    "_start:\n"
    "    mov $%d, %%rdi\n"
    "    mov $60, %%rax\n"
    "    syscall\n",
    value
);

fclose(asm_file);

}

void codegen(struct TOKEN* token) {
    struct TOKEN currtoken = *token;
    switch (currtoken.type){
    case 1:
        generate_for_exit(currtoken.value);
        break;
    default:
        break;
    }
}


void tokenise(char* code) {
    for(int i = 0 ; code[i] != '\0'; i++) {
        while (code[i] == ' ' || code[i] == '\n' || code[i] == '\t')
        i++;
        if (code[i] == '\0') break;
        
        int idx = 0;
        char token [7];
        while (isalpha(code[i]) || isdigit(code[i])) {
            token[idx++] = code[i];
            i++;
        }
        
        token[idx] = '\0'; 
        
        if (strcmp(token, "exit") == 0) {
            struct TOKEN token; 
            token.type = _EXIT;

            while(code[i] == ' ')i++;
            int value = 0;
            while(isdigit(code[i])){
                value = value * 10 + (code[i] - '0');
                i++;
            }
            while(code[i] == ' ')i++;
            if(code[i] == ';') {
                token.value = value;
             printf("Token: EXIT, Value: %d\n", token.value);
             codegen(&token);
            } else {
                printf("Error: Expected ';' after  statement.\n");
                exit(1);
            }
        }

        else if (strcmp(token, "print") == 0 || strcmp(token, "printn") == 0){
            while(code[i] == ' ')i++;
            if(code[i++] == '('){
               if(code[i] == ')' ){printf("No arguements");exit(0);}
               else if (code[i] == '"'){
                        int sentidx = 0;
                        i++;
                        int j = i;
                        int len = 0;
                        while((code[j]) != '"'){
                            
                            j++;
                            len++;
                        }
                        char sentence[len + 2];    
                        
                         while((code[i]) != '"'){
                            sentence[sentidx++] = code[i];
                            i++;
                        }
                        sentence[sentidx++]='\0';
                        i++;
                        while(code[i] == ' ')i++;

                        if(code[i++] != ')'){
                            printf("Expected )");
                            exit(1);
                        }
                    
                        while(code[i] == ' ')i++;
                        if(code[i] != ';'){
                            printf("Expected ;");
                        }
                        else {
                           printsentence(sentence , strcmp(token, "printn") == 0);
                        }


               }



             }
             else
             {
                printf("Invalid\n");
             }
             
        }   
        else
        {
            printf("Error: Unknown token '%s'\n", token);
        }
        
    }
}
char* readfile() {
    FILE *file = fopen("example/main.gl", "r");
    long file_size;
    char* buffer;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    
    fclose(file);
    return buffer;
}

void execute(int exit) {
    system("as -64 build/output.s -o build/output.o");

    system("ld build/output.o -o build/output");

    
    system("./build/output");
// update to pass relevant exit code
   if(exit) exit(0);
}
void main(){
   char* code = readfile();     
    tokenise(code);
    exit(0);
}
