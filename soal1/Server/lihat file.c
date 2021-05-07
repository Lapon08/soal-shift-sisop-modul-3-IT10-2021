#include <stdio.h>

int main(int argc, char* argv[])
{
//read any text file from currect directory
char const* const fileName = "cppbuzz1.txt";

FILE* file = fopen(fileName, "r"); 

if(!file){
printf("\n Unable to open : %s ", fileName);
return -1;
}
    // Returns first token
    char* token = strtok(file, "\n");
  
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, "\n");
    }
fclose(file);
return 0;
}