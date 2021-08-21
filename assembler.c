#include "assembler.h"

/*
*    the program assembler is program that receive the names of assembler files and complie them
*    if the compilation was successfully with out errors then the program will crate 3 output file 
*    if the compilation was not successfully with out errors,
*    then the program will print the specific error to the user and no output will be created
*    the three output files are object, enteries and external
*    the object's file will have the number of instuction words and data words and their coded words
*    the enteries's file will have the name of the entry symbols and their address 
*    the external's file will have the name of the extern symbols and their address of where they was used
*    the maximum size of the given file name is SIZE_OF_FILE_NAME  
*/


int main(int argc, char *argv[])
{
    
    FILE *input = NULL;  /* open the given file and name it "input" */
    symbol_pointer head = NULL;  /* pointer to the symbol list head */
    symbol_pointer ext = NULL;  /* pointer to the external list head */
     int *instruction_array = (int*)calloc(SIZE_OF_WORDS,sizeof(int));   /*int array that saves the data of all the instructions commands */
    char *data_array = (char*)calloc(SIZE_OF_WORDS,sizeof(char));    /* char array that saves the data of all the data commands */
    char file_name[SIZE_OF_FILE_NAME];  /* the name of the input file */
    char line[SIZE_OF_LINE];    /* copy each line from the input to line array */
    int IC;  /*  instruction counter  */
    int DC;   /*  data counter */
    int error_flag; /* indicate if there was an error in the input file */
    int i = 0;  /* counter for reading all the input files */
    int current_line; /* int to show the line in case of error */
    while(++i < argc)  /* reading the input files that located in the argv array */
    {
        IC = FIRST_ADDRESS; /* 100 */
        DC = 0;         
        error_flag = 0;
        current_line=1;
        head = NULL;    
        strcpy(file_name,argv[i]);  
        if (!(input= fopen(file_name,"r")))  /*open the file */
            fprintf(stderr,"Error, can't open file\n"); /* FAIL */

        else    /*opened the file successfully! */
        {   
            fprintf(stderr,"Reading file %s\n\n",file_name);
            /* FIRST PASS! */
            while (fgets(line, SIZE_OF_LINE, input))  /* get each line from the file */
                first_pass(line,&head,&IC,&DC,&error_flag,&current_line);

            update_symbols(&head,IC);  /*update the data symbols (add for each IC) */
            fprintf(stderr,"End of first pass!\n");
                /* SECOND PASS! */
            if(!error_flag)
            {
                rewind(input); /* return to the first line of the input */
                file_name[strlen(file_name)-3]='\0';  /* ignore ".as" */
                secondPass(input,&head,&ext,&IC,&DC,file_name,instruction_array,data_array,&error_flag);  /* second pass */
                fprintf(stderr,"End of second pass!\n");
            }
            /* BUILDING FILES! */
            if(!error_flag){
                build_files(file_name,&head,&ext,instruction_array,data_array,IC,DC);  
                fprintf(stderr,"files builded successfully!\n");
                fprintf(stderr,"file '%s'successfully created and filled\n",argv[i]);
            }
            else
                fprintf(stderr,"no output files generated\n");
        }
        /* free the symbol lists */
        if (head!=NULL)
            free_symbols(&head);
        if (ext!=NULL)
            free_symbols(&ext);
        fprintf(stderr,"*********************************************\n\n");

    }
    /* free and clear all the dynamic memory and arrays */
    free(instruction_array); 
    free(data_array);
    
    if (input!=NULL)
       fclose(input); 
    fprintf(stderr,"End of program, I hope you enjoyed! :)\n");
    exit(0);
    return 0;
}
