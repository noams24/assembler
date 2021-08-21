#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define SIZE_OF_WORDS 1000  /*  size of words array  */
#define SIZE_OF_LINE 80  /*  size of line  */
#define FIRST_ADDRESS 100  /*  index of the first address  */
#define SIZE_OF_PAR 50  /*  size of parameter  */
#define SIZE_OF_FILE_NAME 33  /*  size of the given file name  */
#define NUM_FUNC 33
#define MAX_SIZE_32_BITS 2147483647  /*  max number in 32 bits (0xFFFFFFFF)  */
#define MIN_SIZE_32_BITS -2147483648  /*  min number in 32 bits (-0xFFFFFFFF) */
#define MAX_SIZE_16_BITS 32767  /*  max number in 16 digits  */
#define MIN_SIZE_16_BITS -32768  /*  min number in 16 digits  */
#define MAX_SIZE_8_BITS 127  /*  max number in 8 bits  */
#define MIN_SIZE_8_BITS -128  /*  min number in 8 bits  */
#define MAX_SIZE_LABEL 31  /*  max size of label name  */

/*  setting enums for the symbol's field 'type'  */
typedef enum {DATA = 0, CODE = 1, DATA_AND_ENTRY = 2, CODE_AND_ENTRY = 3,ENTRY = 4, EXTERNAL = 5, EXTERNAL_AND_ENTRY = 6,EXTERNAL_BONUS = 7}types;   

/*  pointer to linked list of symbols  */
typedef struct node *symbol_pointer;

/*  linked list of symbols  */
typedef struct node 
    {
        char label[32];
        int value;
        types type;
        symbol_pointer next;
    } symbol;

/*  function prototype  */

/* first_pass.c */
void first_pass(char line[],symbol_pointer *head, int *IC, int *DC,int *error_flag,int *current_line);

/*get_input.c*/
char *commandNames[NUM_FUNC];
void get_three_parameters(char *p, int *reg1 ,int *reg2 ,int *reg3); 
void get_two_parameters_and_label(char *p, int *reg1 ,int *reg2 ,int *reg3,symbol_pointer *head); 
void addData(char *dataArray,int *dataCounter, int num,int i);
int firstWord(char *line);
char *skipSpacesAndTabs(char *s);
void removeSpaces(char *str);

/*symbols.c*/
void add_symbol(symbol_pointer *head,char labelName[],int *IC,int *DC,types test,int *error_flag,int *current_line);
symbol_pointer already_exists(symbol_pointer *head, char labelName[]);
int symbolCheck(char *p);
int getLabelNumber(char *p,symbol_pointer *head);
int get_type_value(char *p,symbol_pointer *head);
void update_symbols(symbol_pointer *head,int IC);
void add_extern_node(symbol_pointer *ext,char name[], int line);
int unvalid_name(char *name);
void free_symbols(symbol_pointer *head);

/* second_pass.c */
void secondPass(FILE *input,symbol_pointer *head, symbol_pointer *ext,int *IC,int *DC,char *fileName, int *instruction_array, char *data_array,int *error_flag);

/*convert_to_machine_code.c*/
int printR(int opcode, int reg1 ,int reg2 ,int reg3,int funct);
int printI(int opcode, int reg1 ,int reg2 ,int immed);
int printJ(int opcode, int regOrAddress, int reg1);
int iToOpcode(int i);
int iToFunct(int i);

/*build_files.c*/
void build_files(char *file_name, symbol_pointer *head, symbol_pointer *ext, int *instruction_array, char *dataArray, int IC, int DC);
void print_instructions(FILE *writeFile,int num);
void printData(FILE *writeFile,char *dataArray);
void print_entry(char *write_entry,symbol_pointer *head);
void print_extern(char *write_extern,symbol_pointer *head);
