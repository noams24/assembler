#include "assembler.h"

/*
*    add_symbol:  this function receive 7 parameters and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the second parameter is string named name, name have the label name of the symbol
*    the third parameter is int named IC, IC is Instruction Counter that count how many instuction words has been initialize
*    the fourth parameter is int named DC, DC is Data Counter that count how many data/string words has been initialize 
*    the fifth parameter is types variable named type_value, type_value have the type of the symbol
*    the sixth parameter is pointer to the error flag. in case of error we change it to 1
*    the seventh parameter is the current line in case we want to print error of the current line.
*    this function add new symbol to the symbol's linked list and initialize his name, type, and value by the given parameters
*    if the symbol name is already in the linked list, update is type or send error to stderr
*    if the new symbol is valid its will be added to the last symbol in the linked list
*/

void add_symbol(symbol_pointer *head,char labelName[],int *IC,int *DC,types type_value,int *error_flag,int *current_line)
{
    symbol_pointer curr = *head;
    symbol_pointer p1;
     
    /* check if the label already exists in the list */
    p1 = already_exists(head,labelName); 
    
    if(p1!=NULL)
    { 
        if ((type_value == CODE && (*p1).type == CODE) || (type_value == DATA && (*p1).type == DATA))  /*  error  */
        {
            fprintf(stderr, "Line %d: the label '%s' is already exists\n",*current_line, labelName); 
            *error_flag = 1;
            return;
        }
        else if ((*p1).type == DATA && type_value == ENTRY)  /*  update  */
        {
            (*p1).type = DATA_AND_ENTRY;
            return;
        }
        else if ((*p1).type == ENTRY && type_value == DATA)     
        {
            (*p1).value= *DC;
            (*p1).type = DATA_AND_ENTRY;
            return;
        }
         else if ((*p1).type == ENTRY && type_value == CODE)     
        {
            (*p1).value= *IC;
            (*p1).type = CODE_AND_ENTRY;
            return;
        }
        else if((*p1).type == CODE && type_value == ENTRY)
        {
        (*p1).type = CODE_AND_ENTRY;
            return;
        }

        else if(((*p1).type == CODE && type_value == EXTERNAL) || ((*p1).type == DATA && type_value == EXTERNAL) ||
                ((*p1).type == CODE_AND_ENTRY && type_value == EXTERNAL) || ((*p1).type == DATA_AND_ENTRY && type_value == EXTERNAL))
        {
            fprintf(stderr,"Line %d: the label '%s' is already exists as local and local label cannot be declared as external\n",*current_line, labelName);
            *error_flag = 1; 
            return;
        }
       else if (((*p1).type == EXTERNAL && type_value == ENTRY) || ((*p1).type == ENTRY && type_value == EXTERNAL))
       {
            fprintf(stderr,"Line %d: the label '%s' can not be external and entry\n",*current_line, labelName);
            *error_flag = 1; 
            return;
       }
    
        else  /*  p type is DATA_AND_ENTRY/CODE_AND_ENTRY/ENTRY/EXTERNAL and doesn't need to be change  */
        {
            return;
        }

    }

    /*  valid label need to be added to symbol's  */
    p1 = (symbol_pointer)calloc(1, sizeof(symbol));   /* adding the node */
    (*p1).next = NULL;
    strcpy((*p1).label,labelName);

    if(type_value==EXTERNAL||type_value==ENTRY)   /* in case of .external */
        (*p1).value = 0;
    else if(type_value==DATA)
        (*p1).value = *DC;
    else
        (*p1).value = *IC;
    (*p1).type= type_value;

    /*CHECK IF THE LIST IS EMPTY */
    if (*head==NULL) 
        *head = p1;
    else{
        while((*curr).next != NULL)
            curr = (*curr).next;
        (*curr).next = p1;
    } 
    return;
}

/*
* symbol_check: this function receive 1 paramters and return 1 if its a valid label, if the label is not valid return 0;
* the first paramater is pointer to the line
*/
int symbolCheck(char *p){
    int labelFlag,i;
    i = labelFlag = 0;
    while(p[i]!='\n'){
        if (p[i]==':'){
            labelFlag = 1;
            break;
            }
        i++;
        }
    return labelFlag;
}

/*
*    get_label_number:  this function receive 2 parameters and return the symbol value
*    the first paramater is pointer to the line
*    the second parameter is pointer to the linked symbol's list (if its empty, its point to NULL)
*/
int getLabelNumber(char *p,symbol_pointer *head){
    
    char label[SIZE_OF_LINE];
    symbol_pointer pntr = *head;
    memset(label, '\0', sizeof(label));
    strncpy(label,p,strlen(p)); 
    while(pntr!=NULL){
        if(!strcmp(label,(*pntr).label))
            return (*pntr).value;
        pntr = (*pntr).next;
    } 
    return -1;
}

/*
*    get_type_value:  this function receive 2 parameters and return the symbol type value
*    the first paramater is pointer to the line
*    the second parameter is pointer to the linked symbol's list (if its empty, its point to NULL)
*/

int get_type_value(char *p,symbol_pointer *head){
    
    char label[SIZE_OF_LINE];
    symbol_pointer pntr = *head;
    memset(label, '\0', sizeof(label));
    strncpy(label,p,strlen(p)); 
    while(pntr!=NULL){
        if(!strcmp(label,(*pntr).label)){
            return (*pntr).type;
        }
        pntr = (*pntr).next;
    } 
    return -1;
}

/*
*    update_symbols:  this function receive 2 parameters and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the second parameter is int named IC, IC is value of the instruction counter index
*    the function update the symbols by adding to the DATA type symbols the value of IC
*/
void update_symbols(symbol_pointer *head,int IC)
{
    symbol_pointer pntr = *head;
    if(pntr==NULL)
        return;
    while((*pntr).next!= NULL){
        if((*pntr).type==DATA||(*pntr).type==DATA_AND_ENTRY)
            (*pntr).value+= IC;
        pntr = (*pntr).next;
    } 
    if((*pntr).type==DATA||(*pntr).type==DATA_AND_ENTRY)
        (*pntr).value+= IC;
    
    return;
}

/*
*    update_symbols:  this function receive 2 parameters and returns symbol pointer
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the second parameter is the label name 
*    the function checks if the name symbol already exists in the symbol list
*    if the its exist returns a pointer to the suitable node, if not exist returns NULL.
*/
symbol_pointer already_exists(symbol_pointer *head, char labelName[])
{   
    symbol_pointer curr;  /*  set temporary symbols pointers  */
    if (*head == NULL)  /*  empty list  */
    {
        return NULL;
    }
    curr = *head;  /*  set the curr pointer to the first symbol  */
    while ((*curr).next != NULL)  /*  find symbol with same label name  */
    {
        if (strcmp((*curr).label,labelName) == 0)
        { 
            return curr;  /*  found symbol with same label name, return it  */
        }
        curr = (*curr).next;
    }
    if (strcmp((*curr).label,labelName) == 0)
        {  
            return curr;  /*  found symbol with same label name, return it  */
        }
    return NULL;  /*  didn't found symbol with same label name  */
}

/*
*    add_extern_node:  this function receive 3 parameters and return nothing
*    the first parameter is pointer to the linked extern symbol's list (if its empty, its point to NULL)
*    the second paramater is the name of the of symbol to add
*    the third parameter is the value of the the line number
*/

void add_extern_node(symbol_pointer *ext,char name[], int line)
{
    symbol_pointer p1 = (symbol_pointer)calloc(1, sizeof(symbol));
    symbol_pointer curr = *ext;
    
    strcpy((*p1).label,name); 
    (*p1).value = line;
    (*p1).next = NULL;

    if (*ext==NULL) 
        *ext = p1;
    else{
        while((*curr).next != NULL)
            curr = (*curr).next;
        (*curr).next = p1;
    } 
    return;
}


/*
*    free_symbol:  this function receive 1 parameter and return nothing
*    the first parameter (named head) is pointer to pointer to symbol variable,
*    this pointer point to the first linked symbol's list (if its empty, its point to NULL)
*    the function free all the symbols in the linked list from the first to the last
*/
void free_symbols(symbol_pointer *head)
{
    symbol *pntr;  /*  temporary pointer to hold the symbol to be free  */
    while(*head != NULL)  /*  loop to free all the symbols  */
    {
        pntr = *head;
        *head = (*pntr).next;
        free(pntr);
    }
}
