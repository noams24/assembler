#include "assembler.h"

/*
*    printR:  this function receive 5 parameters and return number
*    the first parameter is the opcode
*    the second parameter is the first par
*    the third parameter is the second par
*    the fourth parameter is the third par
*    the fifth parameter is the number of funct
*    this function convert given number to machinecode 
*/
int printR(int opcode, int par1 ,int par2 ,int par3,int funct){

    unsigned int flag;
    unsigned int machine_code = 0;

    /* flag = machine_code  =32 bits */
    flag = funct;
    flag = flag << 6; 
    machine_code = machine_code |flag; /* funct */

    flag= par3;
    flag = flag << 11;
    machine_code = machine_code |flag; /* rd */

    flag= par2;
    flag = flag << 16;
    machine_code = machine_code |flag; /* rt */

    flag= par1;
    flag = flag << 21;
    machine_code = machine_code |flag;  /*rs */

    flag= opcode;
    flag = flag << 26;
    machine_code = machine_code |flag;  /*rs */

    return machine_code ;
}

/*
*    printI:  this function receive 4 parameters and return number
*    the first parameter is the opcode
*    the second parameter is the first par
*    the third parameter is the second par
*    the fourth parameter is the number of immed
*    this function convert given number to machine code 
*/

int printI(int opcode, int par1 ,int par2 ,int immed){

    unsigned int machine_code  = 0;
    unsigned int flag = immed;

    /* immed */
    machine_code =machine_code |flag;  
    machine_code = machine_code &0xFFFF;

     /* rt */
    flag= par2;
    flag = flag << 16;
    machine_code =machine_code |flag; 
    
    /*rs*/
    flag= par1;
    flag = flag << 21;
    machine_code =machine_code |flag;  

    /*opcode*/
    flag= opcode;
    flag = flag << 26;
    machine_code =machine_code |flag;   
          
    return machine_code ;
}

/*
*    printJ:  this function receive 3 parameters and return number
*    the first parameter is the opcode
*    the second parameter is the first par
*    the third parameter is the second par
*    this function convert given number to machine code 
*/
int printJ(int opcode, int parOrAddress, int par1){

    unsigned int machine_code  = 0;
    unsigned int flag = par1;       /*address */
    machine_code =machine_code |flag;  
    machine_code = machine_code &0x7FFF;

    flag= parOrAddress;     /* reg */
    flag = flag << 25;
    machine_code =machine_code |flag; 
    
    flag= opcode;  /*opcode*/
    flag = flag << 26;
    machine_code =machine_code |flag;  

    return machine_code ;
}

/*
*    iToOpcode:  this function receive 1 parameters and return int
*    the first parameter is the command number
*    this function convert the command named i to opcode
*/
int iToOpcode(int i){

    if(i>=0 && i<=4)
        return 0;
    if(i<=7)
        return 1;
    if(i==8)
        return 10;
    if(i==9)
        return 11;
    if(i==10)
        return 12;
    if(i==11)
        return 13;
    if(i==12)
        return 14;
    if(i==13)
        return 15;
    if(i==14)
        return 16;
    if(i==15)
        return 17;
    if(i==16)
        return 18;
    if(i==17)
        return 19;
    if(i==18)
        return 20;
    if(i==19)
        return 21;
    if(i==20)
        return 22;
    if(i==21)
        return 23;
    if(i==22)
        return 24;
    if(i==23)
        return 30;
    if(i==24)
        return 31;
    if(i==25)
        return 32;
    else
        return 63; 
}

/*
*    iToFunct:  this function receive 1 parameters and return int
*    the first parameter is the command number
*    this function convert the command named i to funct 
*/
int iToFunct(int i){

    if(i==0||i==5)
        return 1;
    if(i==1||i==6)
        return 2;
    if(i==2||i==7)
        return 3;
    if(i==3)
        return 4; 
    if(i==4)
        return 5;

    return -1;
}
