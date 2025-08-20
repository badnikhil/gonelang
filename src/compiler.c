#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
enum TokenType{
    _EXIT = 1
};
struct TOKEN{
    enum TokenType type;
    int value;
};

void generate_for_exit(int value) {
    // This function would generate assembly code for a return statement
    FILE *asm_file = fopen("output.s", "w");

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
    int tokidx = 0;
    for(int i = 0 ; code[i] != '\0'; i++) {
        int idx = 0;
        char token [7];
        while (isalpha(code[i]) || isdigit(code[i])) {
            token[idx++] = code[i];
            i++;
        }
        token[idx] = '\0';  
        
        if (strcmp(token, "return") == 0) {
            struct TOKEN token; 
            token.type = _EXIT;

            while(code[i] == ' ')i++;
            int value;
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
        else
        {
            printf("Error: Unknown token '%s'\n", token);
        }
        
    }
}
char* readfile() {
    FILE *file = fopen("main.gl", "r");
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

void execute() {
    system("as -64 output.s -o output.o");

    system("ld output.o -o output");

    
    int exit_code = system("./output");
    
    // Extract actual exit code (system() returns exit_code << 8)
    int actual_exit_code = WEXITSTATUS(exit_code);
    printf("Program exited with code: %d\n", actual_exit_code);
   
}
void main(){
   char* code = readfile();

    tokenise(code);
    execute();
    exit(0);
}
