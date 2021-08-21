#include "assembler.h"

/* Macros for common errors: */

#define MISSING_COMMA(ch) \
    if (ch != ',') {\
        fprintf(stderr,"Line %d: missing comma\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define MULTIPLE_CONSECUTIVE_COMMAS(ch) \
    if (ch == ',') {\
        fprintf(stderr,"Line %d: Multiple consecutive commas\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define MISSING_DOLLAR(ch) \
    if (ch != '$') {\
        fprintf(stderr,"Line %d: missing '$'\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define MISSING_PARAMETER(ch) \
    if (!ch) {\
        fprintf(stderr,"Line %d: missing parameter\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define EXTRANEOUS_TEXT(ch) \
    if (ch) {\
        fprintf(stderr,"Line %d: extraneous text after end of command\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define IS_DIGIT(ch) \
    if (!isdigit(ch)) {\
        fprintf(stderr,"Line %d: not a digit\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define IS_CHAR(ch) \
    if (!isalpha(ch)) {\
        fprintf(stderr,"Line %d: not a char\n",*current_line);\
        *error_flag=1;\
        break;\
    }

#define PROPER_REGISTER(ch) \
    if (ch<0 || ch>32) {\
        fprintf(stderr,"Line %d: register is not in the range 0-32\n",*current_line);\
        *error_flag=1;\
        break;\
    }
    
/*
*    first_pass:  this function receive 5 parameters and return nothing
*    the first parameter is an array of the line given by the input file
*    the second parameter (named head) is pointer to the symbols list
*    the third parameter is int pointer to Instruction Counter, its increas each time a word is added to words_array
*    the fourth parameter is int pointer to Data Counter, its increas each time a word is added to DC_array
*    the fifth parameter is int pointer error_flag, its change to 1 if the program found an error
*    this function represent the first pass to initialize all the encode words in words_array
*    if this function found an error then there will not be a call to the second pass
*/

void first_pass(char line[],symbol_pointer *head,int *IC, int *DC,int *error_flag, int *current_line)
{
    int i; /* counter, and symbolize the command */
    int j=0; /* counter */
    char *p; /* pointer for the line */
    char label[SIZE_OF_LINE];   /* char array that saves the label name */
    int state;  /* int that saves what case to jump*/
    int labelFlag = 0;  /*flag that saves if we have label or not */
    long val;   /* for atol */
    
    p = line;
    p = skipSpacesAndTabs(p); /* if there are spaces or tabs at the beginning we ignore them */
    if (p[0] == '\n') { /* check if the line is only with spaces */
        (*current_line)++;
        return;
    }
    if (p[0] == ';') { /* check if the line note */
        (*current_line)++;
        return;
    }

        /* ********************************** */
        /* stage 1: check if there is a label */
    while(p[j]!='\n')
    {
        if(p[j]==' ')
            break;
        if (p[j]==':')
        {
            labelFlag = 1;
            break;
        }
        j++;
    }

    if (labelFlag)
    { /*check if the label is terminated correcly. */
        if (j>MAX_SIZE_LABEL)
        {
            fprintf(stderr,"Line %d: label is too long\n",*current_line);
            (*current_line)++;
            *error_flag=1;
            return;
        }
        for(i=0;i<j;i++)
        {
            if(!isalpha(p[i]))
            {
                fprintf(stderr,"Line %d: char '%c' is not valid\n",*current_line,p[i]);
                (*current_line)++;
                *error_flag=1;
                return;
            }
        }
        memset(label, '\0', sizeof(label));
        strncpy(label,p,j);  /*copy the label name to to char "label" */
        p+=j+1; /* skip label name */
    }

    for(i=0;i<NUM_FUNC;i++) /* check if the label is named after command label names */
    {
        if(!strcmp(label,commandNames[i]))
        {
            fprintf(stderr,"Line %d: the label '%s' conflicts with an existing label name\n",*current_line,label);
            (*current_line)++;
            *error_flag=1;
            return;
        }

    }

        /* ********************************** */
        /* stage 2: check if the command is extern or data or asciz */
    p = skipSpacesAndTabs(p); /* if there are spaces or tabs at the beginning we ignore them */

    if(p[0]=='\n')/* check if it is a blank line */
    {
        fprintf(stderr,"Line %d: missing command\n",*current_line);
        (*current_line)++;
        *error_flag=1;
        return;
    }
    i = firstWord(p);
    if (i==-1) { /* check if the command exist */
        fprintf(stderr,"Line %d: undefined command name\n",*current_line);
        (*current_line)++;
        *error_flag=1;
        return;
    }
    if(labelFlag)
    {
        if(i>=27 && i<=30)  
            add_symbol(head,label,IC,DC,DATA,error_flag,current_line); /* adds the symbol to the symbol list as data type */
        else
            add_symbol(head,label,IC,DC,CODE,error_flag,current_line);  /* adds the symbol to the symbol list as instruction type */
    }
    p += strlen(commandNames[i]); /* We no longer need the command name */
   
    if(i!=30) /* in case of asciz we need backspaces */
        removeSpaces(p); /* In other cases we do not need spaces */

      /* ********************************** */
        /* stage 3: classify the command name ( not data or asciz) */

    if (i<=4)
        state=1;
    else if (i<=7)
        state=2;
    else if (i<=12)
        state=3;
    else if(i<=16)
        state=4;
    else if(i<=22)
        state=5;
    else if(i<26)
        state=6;
    else if (i==26)
        state=7;
    else if(i>=27 && i<=29)
        state=8;
    else if(i==30)
        state=9;
    else if(i==31)
        state=10;
    else if(i==32)
        state=11;

    switch(state)
    {
        case 1: /* case add, sub, and, or ,nor*/
            MISSING_PARAMETER(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3])
            p+=3;
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            EXTRANEOUS_TEXT(p[2])
            break;

        case 2: /* case move, mvhi, mvlo*/
            MISSING_PARAMETER(p[0])
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2])){
                p++;
            }
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            EXTRANEOUS_TEXT(p[2])
            break;

        case 3: /* case addi,subi,andi,ori,nori*/
            MISSING_PARAMETER(p[0])
            MISSING_DOLLAR(p[0])
            if((p[0] =='-')||(p[0] =='+'))
                p++;
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if(isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            if((p[0] =='-')||(p[0] =='+'))
                p++;
            IS_DIGIT(p[0])
            while(isdigit(p[0]))
                p++;
            MISSING_COMMA(p[0]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[1]);
            MISSING_DOLLAR(p[1])
            IS_DIGIT(p[2])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[3]))
                p++;
            EXTRANEOUS_TEXT(p[3])
            break;

        case 4:  /* case beq, bne, blt, bgt*/
            MISSING_PARAMETER(p[0])
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            IS_CHAR(p[0]);
            while (isalpha(p[0]))
                p++;
            EXTRANEOUS_TEXT(p[0]) 
            break;
        
        case 5: /* case lb,sb,lw,sw,lh,sh*/
            MISSING_PARAMETER(p[0])
            MISSING_DOLLAR(p[0])
            IS_DIGIT(p[1])
            val = atoi(p+1);
            PROPER_REGISTER(val);
            if (isdigit(p[2]))
                p++;
            MISSING_COMMA(p[2]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[3]);
            p+=3;
            if(p[0] =='-'||p[0] =='+'){
                IS_DIGIT(p[1])
                val=atol(p); 
                p++;
            }
            else
            {
                IS_DIGIT(p[0])
                val=atol(p);
            }
            if (val<MIN_SIZE_16_BITS || val>MAX_SIZE_16_BITS){
                fprintf(stderr,"Error in line %d, number is not in the range \n",*current_line);
                *error_flag=1;
                break;
            }
            while(isdigit(p[0]))
                p++;
            MISSING_COMMA(p[0]);
            MULTIPLE_CONSECUTIVE_COMMAS(p[1]);
            MISSING_DOLLAR(p[1])
            IS_DIGIT(p[2])
            val = atoi(p+2);
            PROPER_REGISTER(val);
            if (isdigit(p[3]))
                p++;
            EXTRANEOUS_TEXT(p[3])
            break;

        case 6: /* case jmp ,la ,call */
            MISSING_PARAMETER(p[0])
            if (p[0]=='$')  /* it's a register */
            { 
                IS_DIGIT(p[1])
                val = atoi(p+1);
                PROPER_REGISTER(val);
                if(isdigit(p[2]))
                    p++;
                EXTRANEOUS_TEXT(p[2])
            }
            else
            {
                while(p[0]!='\0') /* checks if the label terminated correctly */
                {
                    if(!isdigit(p[0])&&!isalpha(p[0]))
                    {
                        fprintf(stderr,"Error in line %d, label not terminated correctly \n",*current_line);
                        *error_flag=1;
                        break;
                    }
                    p++;
                }
            }
            break;
    
        case 7: /* case stop */
                EXTRANEOUS_TEXT(p[0])
            break;
        
        case 8: /* case data */
            if(p[0]=='\0') /* checks if missing data */
            {
                fprintf(stderr,"Line %d: no data was given\n",*current_line);
                *error_flag=1;
                break;
            }
            while(1)
            {
                if((p[0] =='-')||(p[0] =='+'))
                {
                    IS_DIGIT(p[1]) 
                    val = atol(p);
                    p++;
                }
                else
                {
                    IS_DIGIT(p[0])
                    val = atol(p);
                }
                
                if(i==27) /* .db */
                {   
                    if(val<MIN_SIZE_8_BITS || val > MAX_SIZE_8_BITS)
                    {
                        fprintf(stderr,"Line %d: given number is out of range\n",*current_line);
                        *error_flag=1;
                        break;
                    }
                    (*DC)++;
                }
                if(i==29)   /* .dh */
                {
                    if(val<MIN_SIZE_16_BITS || val > MAX_SIZE_16_BITS)
                    {
                        fprintf(stderr,"Line %d: given number is out of range\n",*current_line);
                        *error_flag=1;
                        break;
                    }
                    (*DC)+=2;
                }
                if(i==28)   /* dw */
                {
                    if(val<MIN_SIZE_32_BITS || val > MAX_SIZE_32_BITS)
                    {
                        fprintf(stderr,"Line %d: given number is out of range\n",*current_line);
                        *error_flag=1;
                        break;
                    }
                    (*DC)+=4;
                }
                while(isdigit(p[0]))
                    p++;
                if(p[0]=='\0')
                    break;
                MISSING_COMMA(p[0]);
                MULTIPLE_CONSECUTIVE_COMMAS(p[1]);
                p++;
            }
            break;

        case 9: /* case .asciz */
            p = skipSpacesAndTabs(p);
            if(p[0]!='"'){
                fprintf(stderr,"Line %d: command .asciz not teminated correctly\n",*current_line);
                *error_flag=1;
                break;
            }
            p++;
            while(p[0]!='"'){
                if (!isalpha(p[0])&&p[0]!=' ') /*|| p[0]!=' ')*/
                {
                    fprintf(stderr,"Line %d: command .asciz not teminated correctly\n",*current_line);
                    *error_flag=1;
                    (*current_line)++;
                    return;
                }
                (*DC)++;
                p++;
            }
            (*DC)++;
            p++;
            removeSpaces(p);
            EXTRANEOUS_TEXT(p[0])
            break;

        case 10: /* case .entry */
           fprintf(stderr,".entry warrning\n"); 
           MISSING_PARAMETER(p[0])
           memset(label, '\0', sizeof(label));
           strncpy(label,p,strlen(p));
           add_symbol(head,label,IC,DC,ENTRY,error_flag,current_line);
           p+=sizeof(label);
           EXTRANEOUS_TEXT(p[0])
           break;
    
        case 11: /* case .external */
           fprintf(stderr,".external warrning\n");
           MISSING_PARAMETER(p[0])
           memset(label, '\0', sizeof(label));
           strncpy(label,p,strlen(p)); 
           add_symbol(head,label,IC,DC,EXTERNAL,error_flag,current_line);
           p+=sizeof(label);
           EXTRANEOUS_TEXT(p[0])
            break;

    } /* end of cases */
    (*current_line)++; 
    if(i<=26) 
        (*IC)+=4;
    return;
}
