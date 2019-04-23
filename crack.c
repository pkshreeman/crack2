#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings

char *contents;
char **words;

int tryguess(char *hash, char *guess)
{
    char *md5chars = md5(guess, strlen(guess));
    if (strcmp(hash,md5chars )==0) {
        free (md5chars);
        return 1;
}
    
    free (md5chars);
    return 0;
}

int file_length(char *filename){
    
        struct stat info;
        if (stat(filename, &info) == -1) {
            printf("Error in reading filename");
            exit(1);
        }
        else return info.st_size;
    }
    
char **read_dictionary(char *filename, int *size)
{
    
    int len = file_length(filename);
    
    contents =  malloc (len);
    
    FILE *f = fopen(filename, "r");
    if (!f){
        printf("Can't open %s\n", filename);
        exit(1);
    }
    
    //Read entire file into memory all at once
    fread(contents, 1, len, f); 
    fclose(f);
    
    //setting up the counting to find out how many "lines"
    int numOfLines = 0;
    
    //Replacing all new lines with null characters and counting lines
    for (int i = 0; i < len; i++){
        if (contents[i] == '\n') contents[i] = '\0';
        //printf("%c", contents[i]);
        numOfLines++;
    }
    
    words = malloc( numOfLines * sizeof(char *));
    
    //Fill in each address
    words[0] = contents;
    int j = 1;
    for (int i = 0; i < len-1; i++){
        if (contents[i] == '\0'){
            words[j] = &contents[i+1];
            j++;
        }
    }
    //printf("Value of j: %d\n",j);
    /*Print out each word
    for (int i = 0; i < j; i++){
        //printf("%d %s\n", i,words[i]);
    }
    */

    // From what I understand, size is how many words/lines
    *size = j;
    return words;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(argv[2], &dlen);
 
    //printf("Value of dlen: %d", dlen);
    
    // Open the hash file for reading.
    
    FILE *h = fopen (argv[1],"r");
    if (!h){
        printf("Can't open hash file");
        exit(1);
    }

    char line[50];
   
    while(fgets(line, 50, h) != NULL )
    {
        line[strlen(line)-1] = '\0';
        for (int i = 0; i < dlen; i++){
            //printf("Loop number: %d\n",i);
            
           if(tryguess(line,dict[i]) ==1){
               printf("%s %s\n", line, dict[i]);
               break;
           }	
            
        }
    }
       
    free(contents);
    free(words);
    fclose(h);
    
}

    