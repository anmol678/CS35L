#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

int option_f = 0;

//compares two frobnicated string arrays character by character
int frobcmp (char const *a, char const *b) {
    while (*a != ' ' || *b != ' ') {
        char* atemp = (*a ^ 42);
        char* btemp = (*b ^ 42);
        
        if (option_f == 1) {
            atemp = toupper(atemp);
            btemp = toupper(btemp);
        }
        
        if (atemp == btemp) {
            a++;
            b++;
        }
        else if (atemp > btemp)
            return 1;
        else if (atemp < btemp)
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
    fprintf(stderr, "Error (re)allocating memory\n");
    exit(1);
}

//for all errors while reading from file
void readErr(ssize_t temp) {
    if (temp < 0) {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }
}

//for all errors while writing to file
void printErr() {
    if (ferror(stdout)) {
        fprintf(stderr, "Error writing file\n");
        exit(1);
    }
}

int main(int argc, char** argv) {
    if (argc > 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        exit(1);
    }
    else if (argc == 2){
        if (strncmp(argv[1], "-f", 2) == 0)
            option_f = 1;
        else {
            fprintf(stderr, "Invalid option\n");
            exit(1);
        }
    }
    
    struct stat sb;
    if (fstat(0, &sb) < 0) {
        fprintf(stderr, "fstat error\n");
        exit(1);
    }
    
    //store all pointers to words
    char** lines;
    //counter for 2d char array lines
    int lcount = 0;
    //stores all contents of the file
    char* file;
    //stores the size of the file
    size_t size = sb.st_size;
    
    //check if it is a regular file
    if (S_ISREG(sb.st_mode)) {
        //allocate memory to accomodate all contents of the file
        file = (char*)malloc(size * sizeof(char));
        
        //read everything using read system call and store into file array
        ssize_t bytes_read = read(0, file, size);
        //check if the file is empty
        if (bytes_read == 0) {
            fprintf(stderr, "Empty file, 0 comparisons\n");
            return 0;
        }

        //keeps count of the number of words in the file
        int wordcount = 0;
        //traverse through the file to fnd the number of words in total
        for (size_t i = 0; i < size; i++) {
            if (file[i] == ' ')
                wordcount++;
            else if (file[i] == '\n') {
                wordcount++;
                file[i] = ' ';
            }
        }
        if (file[size - 1] != ' ')
            wordcount++;

        //allocate memory based on the number of words in the file
        lines = (char**)malloc(wordcount * sizeof(char*));

        int stop = 0;
        for (size_t i = 0; i < size; i++) {
            if (stop == 0 && file[i] != ' ') {
                lines[lcount] = &file[i];
                lcount++;
                stop = 1;
            }
            else if (stop == 1 && file[i] == ' ')
                stop = 0;
        }
    }
    else {
        //allocate memory to lines storing one word at a time
        lines = (char**)malloc(sizeof(char*));
    }
    //store a single word separated by a space
    char* word;
    word = (char*)malloc(sizeof(char));
    //counter for char array word
    int wcount = 0;
    //char array to store one byte at a time
    char ch[1];
    //read byte by byte from standard input
    ssize_t cstate = read(0, ch, 1);
    readErr(cstate);
    while (cstate > 0) {
        //keep adding characters to word till we reach end of word or (space) as mentioned above
        word[wcount] = ch[0];
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
        if (ch[0] == ' '){
            lines[lcount] = word;
            lcount++;
            
            char** temp = realloc(lines, (lcount+1) * sizeof(char*));
            
            //expand lines by reallocating memory and reset word to store new incoming characters
            if (temp != NULL) {
                lines = temp;
                //make space to store a new word
                word = NULL;
                word = (char*)malloc(sizeof(char));
                wcount = 0;
            }
            else {
                free(lines);
                reallocErr();
            }
        }
        
        cstate = read(0, ch, 1);
        readErr(cstate);
        if (ch[0] == EOF) {
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
        size_t wsize = 0;
        for (size_t j = 0;; j++) {
            wsize++;
            if (lines[i][j] == ' ')
                break;
        }
        write(1, lines[i], wsize);
    }
    
    //free memory occupied
    free(lines);
    
    return 0;
}