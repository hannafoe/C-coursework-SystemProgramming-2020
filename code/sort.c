#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int compare(const void *, const void *);
int compare_int(const void *, const void *);
int main(int argc, const char *argv[]){
    FILE *inputFile = NULL;
    char **list = NULL;
    int ptr = 0;
    int n_flag = 0;
    int o_flag = 0;
    int r_flag = 0;
    int argumentpointer = 1;//Pointer to the commandline inputs
    int read_stdin=0;//Needed for the -o option when only an outputfile is given
    int inputfile_flag=0;//Needed for the -o option to signal how many inputfiles where given
    if (argc > 1){
        int all_input_files_read=0;//Needed for the -o option, to signal all input files have been read
        while (argumentpointer < argc){//For all commandline options and files given
            if(all_input_files_read==1){//For -o option, when all input files have been read
                break;
            }
            char a = argv[argumentpointer][0];//The first character of string in commandline
            char b = argv[argumentpointer][1];//The second character of string in commandline
            switch (a){
                case '-'://If string is an option
                    switch (b){//Options
                        case 'n':
                            n_flag = 1;
                            break;
                        case 'o':
                            o_flag = 1;
                            break;
                        case 'r':
                            r_flag = 1;
                            break;
                        case 'h':
                            printf("SORT:\n");
                            printf("NAME\n\tsort - sort lines of text files\n");
                            printf("SYNOPSIS\n\tI have managed to implement the options -o,-n,-r,-h\n");
                            printf("\tMultiple options can be chosen at once e.g. -n -o\n");
                            printf("\tsort [OPTION]...[FILE]...\n");
                            printf("DESCRIPTION\n");
                            printf("\tWrites sorted concatenation of all FILE(s) to standard output.\n");
                            printf("\tIf no file is given then standard input is read");
                            printf(" until CTRL+D is pressed in a new line to signal EOF.\n");
                            printf("\tIf only one file is given, then this is the output file and standard input is read as input.\n");
                            printf("\tIf no options are given, sort.c will sort the input alphabetically.\n");
                            printf("\tOrdering options:\n");
                            printf("\t-n, compare according to string numerical value\n");
                            printf("\t-r, reverse the result of comparisons\n");
                            printf("\t-o, the output is written to the first file given, instead of being output to stdout\n");
                            printf("\t-h, Usage explanation\n");
                            printf("\nDo you want to proceed with sorting? (y/n)\n");
                            if(getc(stdin)!='y'){//Check if Usage explanation was the only thing wanted or if sorting should also be done
                                return 0;
                            }
                            break;
                        }
                    break;
                default://Read input files
                    if(o_flag==1){
                        if(argv[argumentpointer+1]==NULL&&inputfile_flag==0){//Case when only output file is given
                            all_input_files_read=1;
                            read_stdin++;
                            break;
                        }else{
                            if(argv[argumentpointer]==NULL){//Case when all input files have been read
                                all_input_files_read=1;
                                break;
                            }
                            inputfile_flag++;
                            if(inputfile_flag==1){//Skip first file, because this is the outputfile
                                argumentpointer++;
                            }
                        }
                    }
                    //Read file input
                    inputFile = fopen(argv[argumentpointer], "r");
                    if(inputFile==NULL){
                        fprintf(stderr,"Commandline arguments were not valid arguments.\n");
                        exit(1);
                    }
                    while (1){
                        char *buffer = calloc(256, sizeof(char)); //Create space to store the character array from each line of input
                        //Anything with length bigger than 255 in one line will not fit into this buffer
                        if (buffer == NULL){
                            fprintf(stderr, "out of memory.\n");
                            exit(1);
                        }
                        if (fscanf(inputFile, "%s", buffer) == EOF){//All the input was read, so return
                            free(buffer);//free the last buffer that was initiliased because it was not used
                            buffer = NULL;
                            break;
                        }
                        int bufpointer = 0;//points to how far the previous input has filled the buffer
                        char c = fgetc(inputFile);//Check next character
                        if (c ==' '){//If this character is a space then there might be more input in this line
                            char d = fgetc(inputFile);
                            if(d!='\n'){//Check that the character after the space is not a new line, because if it is then we can just continue to the new line
                                fseek(inputFile,-sizeof(char),SEEK_CUR);//Move the file pointer one character back to get the complete string
                                while (buffer[bufpointer] != '\0'){//search where the first word ends in the buffer
                                    bufpointer++;
                                }
                                char *buffer2 = calloc(255, sizeof(char)); //Initialise a new buffer for the next word in the same line
                                if (buffer2 == NULL){
                                    fprintf(stderr, "out of memory.\n");
                                    exit(1);
                                }
                                fscanf(inputFile, "%s", buffer2);
                                int bufpointer2 = 0;
                                buffer[bufpointer] = ' ';
                                bufpointer++;
                                while (buffer2[bufpointer2] != '\0' && bufpointer < 256){//Now put the second word behind the first in the buffer
                                    buffer[bufpointer] = buffer2[bufpointer2];
                                    bufpointer++;
                                    bufpointer2++;
                                }
                                free(buffer2);//is not needed anymore, string was copied to buffer
                                buffer2 = NULL;
                                //Same process for any further words after space in the same line
                                c = fgetc(inputFile);
                                while (c == ' '){
                                    d = fgetc(inputFile);
                                    if(d!='\n'){//Check whether the character after space is new line
                                        fseek(inputFile,-sizeof(char),SEEK_CUR);//Move one char back to get the full word
                                        char *buffer2 = calloc(255, sizeof(char)); 
                                        if (buffer2 == NULL){
                                            fprintf(stderr, "out of memory.\n");
                                            exit(1);
                                        }
                                        fscanf(inputFile, "%s", buffer2);
                                        int bufpointer2 = 0;
                                        buffer[bufpointer] = ' ';
                                        bufpointer++;
                                        while (buffer2[bufpointer2] != '\0' && bufpointer < 256){
                                            buffer[bufpointer] = buffer2[bufpointer2];
                                            bufpointer++;
                                            bufpointer2++;
                                        }
                                        free(buffer2);
                                        buffer2 = NULL;
                                        c = fgetc(inputFile);
                                    }else{//DONE WITH THIS LINE
                                        fseek(inputFile,-sizeof(char),SEEK_CUR);//Move one char back to go to the right starting point for string on the next line
                                        break;
                                    }
                                }
                            }else{//DONE WITH THIS LINE
                                fseek(inputFile,-sizeof(char),SEEK_CUR);//Move one char back to got to the right starting point for string on the next line
                            }
                        }
                        //Increase space in list with realloc
                        list = realloc(list, sizeof(char[256]) * (ptr + 1));
                        if (list == NULL){
                            fprintf(stderr, "out of memory.\n");
                            exit(1);
                        }
                        //Finally input this string into list
                        list[ptr] = buffer;
                        ptr++;
                    }//DONE WITH THIS FILE
                    fclose(inputFile);
                    break;
            }
            argumentpointer++;
        }
    }
    //CASE WHEN NO FILE WAS GIVEN
    //Similar process as with file input just with stdin, and do not need to care about multiple files
    if (inputFile == NULL||read_stdin==1){
        while (1){//While true
            char *buffer = calloc(256, sizeof(char));//Create space to store string
            if (buffer == NULL){
                fprintf(stderr, "out of memory.\n");
                exit(1);
            }
            if (fscanf(stdin, "%s", buffer) == EOF){//If fscanf returns EOF Ctrl+D was pressed and program exits from asking for user input
                free(buffer);//Last buffer is freed, because it was not used
                buffer = NULL;
                break;
            }
            //Look for explanation above: The same process as with multiple string in one line with file input
            int bufpointer = 0;
            if (fgetc(stdin) != '\n'){
                while (buffer[bufpointer] != '\0'){
                    bufpointer++;
                }
                char *buffer2 = calloc(255, sizeof(char)); //MAYBE LATER CHECK THAT INPUT REALLY FIT IN 256
                if (buffer2 == NULL){
                    fprintf(stderr, "out of memory.\n");
                    exit(1);
                }
                fscanf(stdin, "%s", buffer2);
                int bufpointer2 = 0;
                buffer[bufpointer] = ' ';
                bufpointer++;
                while (buffer2[bufpointer2] != '\0' && bufpointer < 256){
                    buffer[bufpointer] = buffer2[bufpointer2];
                    bufpointer++;
                    bufpointer2++;
                }
                free(buffer2);
                buffer2 = NULL;
                while (fgetc(stdin) != '\n'){
                    char *buffer2 = calloc(255, sizeof(char)); //MAYBE LATER CHECK THAT INPUT REALLY FIT IN 256
                    if (buffer2 == NULL){
                        fprintf(stderr, "out of memory.\n");
                        exit(1);
                    }
                    fscanf(stdin, "%s", buffer2);
                    int bufpointer2 = 0;
                    buffer[bufpointer] = ' ';
                    bufpointer++;
                    while (buffer2[bufpointer2] != '\0' && bufpointer < 256){
                        buffer[bufpointer] = buffer2[bufpointer2];
                        bufpointer++;
                        bufpointer2++;
                    }
                    free(buffer2);
                    buffer2 = NULL;
                }
            }
            //Increase space in list with realloc
            list = realloc(list, sizeof(char[256]) * (ptr + 1));
            if (list == NULL){
                fprintf(stderr, "out of memory.\n");
                exit(1);
            }
            list[ptr] = buffer;//Input string into list
            ptr++;
        }
    }
    if(n_flag==1){//Do qsort with numerical comparison instead
        qsort(list, ptr, sizeof(char *), compare_int);
    }else{
        qsort(list, ptr, sizeof(char *), compare);
    }
    if(o_flag==1){//Output to file instead
        if(read_stdin==0){//Only output file was given
            argumentpointer=argumentpointer-inputfile_flag-1;//Find the file to output to
        }else{//Inputfiles were also given
            argumentpointer=argumentpointer-1;//Find the file to output to
        }
        FILE *outputFile=fopen(argv[argumentpointer], "w");
        if(outputFile==NULL){
            fprintf(stderr,"Commandline arguments were not valid arguments.\n");
            exit(1);
        }
        if(r_flag==1){//Reverse order case
            for (int i = ptr-1; i >=0; i--)
                fprintf(outputFile,"%s\n", list[i]);//Print to file
        }else{//Normal case
            for (int i = 0; i < ptr; i++)
                fprintf(outputFile,"%s\n", list[i]);//Print to file
        }
        fprintf(outputFile,"\n");
        fclose(outputFile);//And close the file
    }else{
        printf("\n");
        if(r_flag==1){//Reverse order case
            for (int i = ptr-1; i >=0; i--)
                printf("%s\n", list[i]);//print to stdout
        }else{
            for (int i = 0; i < ptr; i++)
                printf("%s\n", list[i]);//print to stdout
        }
        printf("\n");
    }
    //cleanup memory
    for (int i = 0; i < ptr; i++){
        free(list[i]);
        list[i] = NULL;
    }
    free(list);
    list = NULL;
    return 0;
}
int compare(const void *arg1, const void *arg2){//Comparison function (alphabetical comparison)
    const char **a1 = (const char **)arg1;
    const char **a2 = (const char **)arg2;
    return strcmp(*a1, *a2);
}
int compare_int(const void *arg1, const void *arg2){//Comparison function 2 (numerical comparison)
    const char **a1 = (const char **)arg1;
    const char **a2 = (const char **)arg2;
    int x=atoi(*a1);
    int y=atoi(*a2);
    return x-y;
}
