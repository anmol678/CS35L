#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

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

    char character[1];
    ssize_t bytes_read = read(0, character, 1);

    while (bytes_read > 0) {
    	for (size_t i = 0; i < strlen(from); i++)
    		if (character[0] == from[i]) {
                character[0] = to[i];
                break;
    		}
        write(1, character, 1);
        bytes_read = read(0, character, 1);
    }

	return 0;
}