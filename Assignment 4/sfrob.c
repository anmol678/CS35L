#include<stdio.h>
#include<stdlib.h>

//compares two frobnicated string arrays character by character
int frobcmp (char const *a, char const *b) {
    while (*a != ' ' || *b != ' ') {
        
        if ((*a ^ 42) == (*b ^ 42)) {
            a++;
            b++;
        }
        else if ((*a ^ 42) > (*b ^ 42))
            return 1;
        else if ((*a ^ 42) < (*b ^ 42))
            return -1;
        else
            return 0;
    }
    return 0;
}

//casting void pointers to int pointers
int cmp(const void* pa, const void* pb) {
    const char* a = *(const char**)pa;
    const char* b = *(const char**)pb;
    return frobcmp(a, b);
}

//for all errors while reallocating memory
void reallocErr() {
    fprintf(stderr, "%s", "Error (re)allocating memory");
    exit(1);
}

//for all errors while reading from file
void readErr() {
    if (ferror(stdin)) {
        fprintf(stderr, "%s", "Error reading file");
        exit(1);
    }
}

//for all errors while writing to file
void printErr() {
    if (ferror(stdout)) {
        fprintf(stderr, "%s", "Error writing file");
        exit(1);
    }
}

int main() {
    //store a single word read from stdin
    char* word;
    word = (char*)malloc(sizeof(char));
    //store all pointers to words
    char** lines;
    lines = (char**)malloc(sizeof(char*));
    //counter for char array word
    int wcount = 0;
    //counter for 2d char array lines
    int lcount = 0;
    
    //read each char from standard input
    //int to check for EOF
    char ch = getchar();
    readErr();
    
    //check if we reached end of file
    while (ch != EOF && !ferror(stdin)) {
        //keep adding characters to word till we reach end of word or (space) as mentioned above
        word[wcount] = ch;
        wcount++;
        
        char* temp = realloc(word, (wcount+1) * sizeof(char));
        
        //expand word to accomodate new incoming characters
        if (temp != NULL)
            word = temp;
        else {
            free(word);
            reallocErr();
        }
        
        //the delimiter for a line here is a (space) - add contents of word to a new slot in lines
        if (ch == ' '){
            lines[lcount] = word;
            lcount++;
            char** temp = realloc(lines, (lcount+1) * sizeof(char*));
            
            //expand lines by reallocating memory and reset word to store new incoming characters
            if (temp != NULL) {
                lines = temp;
                
                word = NULL;
                word = (char*)malloc(sizeof(char));
                wcount = 0;
            }
            else {
                free(lines);
                reallocErr();
            }
        }

        ch = getchar();
        readErr();
        if (ch == EOF) {
            //if we have reached EOF and the previous word ended with a (space) then we are good
            if (word[wcount] == ' ') {
                free(word);
                wcount = 0;
                break;
            }
            //otherwise, the word is completed and stored in lines
            else {
                word[wcount] = ' ';
                wcount = 0;
                lines[lcount] = word;
                lcount++;
                break;
            }
        }
    }

    //sorting using qsort
    qsort(lines, lcount, sizeof(char*), cmp);
    
    //output sorted contents of lines to standard output
    for (size_t i = 0; i < lcount; i++) {
        size_t j = 0;
        do {
            putchar(lines[i][j]);
            j++;
        } while (lines[i][j] != ' ');
        putchar(' ');
    }

    //free memory occupied by each word in lines then free memory used by lines
    for (size_t i = 0; i < lcount; i++)
        free(lines[i]);
    free(lines);
    
    exit(0);
}