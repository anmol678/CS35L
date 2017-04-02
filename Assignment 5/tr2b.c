#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main (int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments");
        exit(1);
    }
    
    char *from = argv[1];
    char *to = argv [2];
    
    if (strlen(from) != strlen(to)) {
        fprintf(stderr, "input arguments must have same length");
        exit(1);
    }
    
    for (size_t i = 0; i < strlen(from) - 1; i++)
        for (size_t j = i + 1; j < strlen(from); j++)
            if (from[i] == from[j]){
                fprintf(stderr, "every character in from must be unique");
                exit(1);
            }
    
    int character = getchar();
    
    while (character != EOF) {
        int flag = 0;
        for (size_t i = 0; i < strlen(from); i++)
            if (character == from[i]){
                putchar(to[i]);
                flag = 1;
            }
        if (flag != 1)
            putchar(character);
        character = getchar();
    }
    
    return 0;
}
