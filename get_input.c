#include "assembler.h"

/* an array to easily compare the first word to the command names */
char *commandNames[NUM_FUNC] = {"add", "sub", "and", "or", "nor",
                                "move", "mvhi","mvlo","addi","subi",
                                "andi","ori","nori","bne","beq",
                                "blt","bgt","lb","sb","lw",
                                "sw","lh","sh","jmp","la",
                                "call","stop",".db",".dw",".dh",
                                ".asciz",".entry",".extern"};


/*
*    get_three_parameters:  this function receive 4 parameters and return nothing
*    the first parameter is pointer to char array named line, line have the current line from the source file
*    the second parameter is string named par1, in par1 will be set the first parameter from line
*    the third parameter is string named par2, in par2 will be set the second parameter from line
*    the forth parameter is string named par3, in par3 will be set the third parameter from line
*    this function put the three first parameters in par1-par3
*/
void get_three_parameters(char *p, int *par1 ,int *par2 ,int *par3){

    p++;
    *par1 = atoi(p);
    if (isdigit(p[1]))
         p++;
    p+=2;
    if(p[0]=='$')
        p++;
    *par2 = atoi(p);
    if(p[0]=='+'||p[0]=='-')
        p++;
    while(isdigit(p[0]))
        p++;
    if (p[0]=='\n')
        return;
    p+=2;
    *par3 = atoi(p);
    return;
}


/*
*    get_two_parameters_and_label:  this function receive 5 parameters and return nothing
*    the first parameter is pointer to char array named line, line have the current line from the source file
*    the second parameter is string named par1, in par1 will be set the first parameter from line
*    the third parameter is string named par2, in par2 will be set the second parameter from line
*    the forth parameter is string named par3, in par3 will be set the third parameter from line
*    the fifth parameter (named head) is pointer to the symbols list
*    this function put the three first parameters in par1-par3 and the value of the label name
*/
void get_two_parameters_and_label(char *p, int *par1 ,int *par2 ,int *par3,symbol_pointer *head){

    p++;
    *par1 = atoi(p);
    if (isdigit(p[1]))
         p++;
    p+=3;
    *par2 = atoi(p);
    if (isdigit(p[1]))
         p++;
    p+=2;
   
    *par3 = getLabelNumber(p,head); 
    return;
}


/*
*    addData:  this function receive 4 parameters and return nothing
*    the first parameter is pointer to char daata array named dataArray, the array contain all the data values
*    the second parameter is a counter to the data array, so we know we to add the next data value
*    the third parameter is the number we add to the data array, named num
*    the forth parameter is function number: .db, .dh or .dw named i
*    this function convert the a number to machine code and add it to the data array
*/
void addData(char *dataArray,int *dataCounter,int num,int i)
{
    char data = 0;

    data = (num>>4)&0x000F;
    *(dataArray+*dataCounter)= data;
    (*dataCounter)++;
    data = num&0x000F;
    *(dataArray+*dataCounter)= data;
    (*dataCounter)++;
    /* case .dh or .dw */
    if(i==28||i==29)
    {
        data = (num>>12)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;

        data = (num>>8)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;
    }
        /* case .dw */
    if(i==28)
    {
        data = (num>>20)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;

        data = (num>>16)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;

        data = (num>>28)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;
    
        data = (num>>24)&0x000F;
        *(dataArray+*dataCounter)= data;
        (*dataCounter)++;  
    }
    return;  
}

/* A function that gets a pointer to the beginning of the first word in a command and returns a
 * number representing the command (or -1 if the first word is not a command) */
int firstWord(char *p) {
    int i;
    int count = 0;
    
    for (i = 0; p[i] && p[i] != ' ' && p[i] != '\t' && p[i] != ',' && p[i] != '\n'; i++) {
        count++; /* we count how many characters the first word have */
    }
    
    for (i = 0; i < NUM_FUNC; i++) { /* we compare the first word with each of the command names */
        if (strlen(commandNames[i]) == count && !strncmp(commandNames[i], p, count)) /* equals */
            return i;
    }
    return -1; /* the command name is not good */
}

/* Function to skip spaces from a given string */
char *skipSpacesAndTabs(char *s) {
    int n;
    for (n = 0; s[n] != 0 && (s[n] == ' ' || s[n] == '\t'); n++) {} /* Count how many spaces there are at the beginning */
    return s + n;
}

/* Function to remove all spaces from a given string */
void removeSpaces(char *str) {
    int i;
    /* To keep track of non-space character count */
    int count = 0;
    
    /* Traverse the given string. If current character 
     is not space, then place it at index 'count++' */
    for (i = 0; str[i]; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            str[count++] = str[i]; /* here count is incremented */
    }
    
    str[count] = '\0'; /* finish the string */
}
