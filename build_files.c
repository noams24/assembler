#include "assembler.h"

/*
*    build_files:  this function receive 7 parameters and return nothing
*    the first parameter is name of the file
*    the second parameter (named head) is pointer to the symbols list
*    the third parameter is pointer to the extern symbol list
*    the fourth parameter is pointer to the instruction array
*    the fifth parameter is pointer to the data array
*    the sisxth parameter is pointer to IC
*    the seventh parameter is pointer to DC
*    this function builds three files: "file_name".ob, "file_name".ent, "file_name".ext
*/

void build_files(char *file_name, symbol_pointer *head, symbol_pointer *ext, int *instruction_array, char *dataArray, int IC, int DC)
{
    int i;
    int current_line=100;  /* int that saves the current line number */
    int counter = 0;
    char file_name_entry[SIZE_OF_FILE_NAME]; /* char array that contains the name of the entry file */
    char file_name_extern[SIZE_OF_FILE_NAME]; /* char array that saves the name of the extern file */
    FILE *file_name_ob = NULL;  /* the name of the ob file */

    /* make entry file */
    strcpy(file_name_entry,file_name);
    print_entry(file_name_entry,head);  
    
    /* make extern file */
    strcpy(file_name_extern,file_name);
    print_extern(file_name_extern,ext); 
   
    /* Build ob file : */
    /*print the instructionsof the ob file: */
    if (!(file_name_ob= fopen(strcat(file_name,".ob"),"w")))
        printf("fail\n"); 
    fprintf(file_name_ob,"\t %d %d",IC-FIRST_ADDRESS,DC);
    for(i=0;i<(IC-100)/4;i++){ 
        fprintf(file_name_ob,"\n0%d ",current_line);
        current_line+=4;
        print_instructions(file_name_ob,instruction_array[i]);
        }
        
    /* print the data of the ob file */
    if (DC!=0)
    {
        fprintf(file_name_ob,"\n0%d ",current_line);
        for(i=0;i<DC*2;i++)
        { 
            fprintf(file_name_ob,"%X",*(dataArray+i));
            counter++;
            if(counter%8==0){
                current_line+=4;
                fprintf(file_name_ob,"\n0%d ",current_line);
            }
            else if(counter%2==0)
                fprintf(file_name_ob," "); 
        }
    }
    return;
}

/*
*    print_instructions:  this function receive 2 parameters and return nothing
*    the first parameter is the file
*    the second parameter is the number to add to the instruction array
*    this function adds the machine code number to the int instruction array
*/
void print_instructions(FILE *file_name_ob,int num)
{
    unsigned int temp;
    temp = num&0xF0;
    temp = temp>>4;
    fprintf(file_name_ob,"%X",temp);
    temp= num&0xF;
    fprintf(file_name_ob,"%X ",temp);

    temp = num&0xF000;
    temp = temp>>12;
    fprintf(file_name_ob,"%X",temp);
    temp = num&0xF00;
    temp = temp>>8;
    fprintf(file_name_ob,"%X ",temp);

    temp = num&0xF00000;
    temp = temp>>20;
    fprintf(file_name_ob,"%X",temp);
    temp = num&0xF0000;
    temp = temp>>16;
    fprintf(file_name_ob,"%X ",temp);

    temp = num&0xF0000000;
    temp = temp>>28;
    fprintf(file_name_ob,"%X",temp);
    temp = num&0xF000000;
    temp = temp>>24;
    fprintf(file_name_ob,"%X",temp);
    return;
}

/*
*    print_Data:  this function receive 2 parameters and return nothing
*    the first parameter is the file
*    the second parameter is the number to add to the data array
*    this function adds the machine code number to the char data array
*/

void printData(FILE *file_name_ob,char *dataArray){

    int i=0;    /* counter */
    int counter =0;
    int currentLine=0;
    fprintf(file_name_ob,"%X",*(dataArray+i));
        counter++;
        if(counter%8==0){
            currentLine+=4; 
            fprintf(file_name_ob,"\n0%d ",currentLine);
        }
        else if(counter%2==0){
            printf(" ");
            fprintf(file_name_ob," ");
        }

    return;
}


/*
*    build_files:  this function receive 2 parameters and return nothing
*    the first parameter is name of the file
*    the second parameter (named head) is pointer to the symbols list
*    this function build the file "file_name".ent
*/
void print_entry(char *write_entry,symbol_pointer *head)
{ 
    FILE *writeEntry = NULL;
    int firstPrint = 0;
    symbol_pointer pntr = *head;
    
    while(pntr!=NULL)
    {
        if((*pntr).type==DATA_AND_ENTRY || (*pntr).type==CODE_AND_ENTRY || (*pntr).type==ENTRY)
        {
            if(!firstPrint){
                if (!(writeEntry= fopen(strcat(write_entry,".ent"),"w")))
                {
                    fprintf(stderr,"Error creating file\n"); 
                    break;
                }
                fprintf(writeEntry,"%s 0%d",(*pntr).label,(*pntr).value);
                firstPrint=1;
            }
            else
                fprintf(writeEntry,"\n%s 0%d",(*pntr).label,(*pntr).value);
        }
        pntr = (*pntr).next;
    }  
    return ;
}

/*
*    print_extern:  this function receive 2 parameters and return nothing
*    the first parameter is name of the file
*    the second parameter (named head) is pointer to the extern symbols list
*    this function build the file "file_name".ext
*/
void print_extern(char *write_extern, symbol_pointer *ext){
    
    FILE *write_extern_file = NULL;  /* the name of the extern file */
    int first_print = 0;    /* flag for the first print */
    symbol_pointer pntr = *ext;     /* pointer for the symbol names */
    
    while(pntr!=NULL)
    {
        if(!first_print)
        {
            if (!(write_extern_file= fopen(strcat(write_extern,".ext"),"w"))){
                fprintf(stderr,"Error creating file\n"); 
                break;
            }
            fprintf(write_extern_file,"%s 0%d",(*pntr).label,(*pntr).value);
            first_print=1;
        }
        else
            fprintf(write_extern_file,"\n%s 0%d",(*pntr).label,(*pntr).value);
        pntr = (*pntr).next;
    }
    return;
}
