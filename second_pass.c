#include "assembler.h"

/*    This file has one function:
*    1) function is 'second_pass' get parameters for checking, updating machine code, and create list with extern parameters
* main function to checking and updating machine code and create list with extern parameters */
void secondPass(FILE *input,symbol_pointer *head,symbol_pointer *ext,int *IC,int *DC,char *fileName, int *instruction_array, char *data_array,int *error_flag)
{
    /*FILE *write_external = NULL; */
    char *p;                    /*  pointer to the line  */
    int labelFlag;              /*  flag that reprecent if the line have or have not a symbol  */
    char line[SIZE_OF_LINE];    /*  string that will save each line from input file  */
    int par1 ,par2 ,par3 ;      /*  ints that hold the parameters from line  */
    int type_value;             /*  int that hold the label type  */
    int opcode;                 /*  int that hold the opcode  */
    int funct;                  /*  int that hold the funct  */
    int machine_code;           /*  int that hold the machine code that we will print in the ob file  */
    int current_line_error=1;         /*  int that hold the current line number for errors  */
    int current_line= FIRST_ADDRESS; /*  int that hold the current line number, jumps each time in length of 4  */
    int dataCounter =0;         /*  counts how many data numbers are in the data array  */
    int i;                      /*  int that saves the command name  */
    int j=0;                    /*  counter to save the instruction lines  */
    int state;
    
    /*loop to read all the line from input*/
    while (fgets(line, SIZE_OF_LINE, input))
    {
        i=0;
        type_value = 0;
        p = line;
        p = skipSpacesAndTabs(p);

        if (p[0]!=';' && p[0]!='\n')
        {
            labelFlag= symbolCheck(p);/*checks if the first word is a label */
            if(labelFlag)/* skip the label name */
            {  
                while(p[0]!=':')
                    p++;
                p++;
            }
            p=skipSpacesAndTabs(p);
            i = firstWord(p);  /* classify the command name */
            opcode = iToOpcode(i);      /* convert the command to opcode */
            funct = iToFunct(i);        /* convert the command to funct */
            p += strlen(commandNames[i]); /* We no longer need the command name */
            if(i!=30) /* in case asciz we need spaces */
            removeSpaces(p); /* In this part of the line we do not need spaces */
        }

        if(p[0]==';' || p[0]=='\n') /* skip this line */
            state= 10;
        else if (i<=4)
            state=1;
        else if (i<=7)
            state=2;
        else if (i<=12)
            state=3;
        else if(i<=16)
            state=4;
        else if(i<=22)
            state=5;
        else if(i<=25)
            state=6;
        else if (i==26)
            state=7;
        else if(i>=27 && i<=29)
            state=8;
        else if(i==30)
            state=9;
        else
            state =10;

        switch(state)
        {
            case 1:        /* case add, sub, and, or ,nor*/
                get_three_parameters(p, &par1, &par2, &par3);
                machine_code = printR(opcode,par1,par2,par3,funct);
                break;
        
            case 2:      /* case move, mvhi, mvlo*/
                get_three_parameters(p, &par1, &par2, &par3);
                machine_code = printR(opcode,par1,0,par2,funct);
                break;
        
            case 3:     /* case addi,subi,andi,ori,nori */
               get_three_parameters(p, &par1, &par2, &par3); 
               machine_code = printI(opcode,par1,par3,par2);
               break;

            case 4:     /* case beq, bne, blt, bgt*/
                get_two_parameters_and_label(p, &par1, &par2, &par3,head);
                if(par3==-1)
                    {
                        fprintf(stderr,"Line %d: the label doesn't exist\n",current_line_error);
                        *error_flag=1;
                    }
                machine_code = printI(opcode,par1,par2,par3-current_line);  
                break;
        
            case 5:     /* case lb,sb,lw,sw,lh,sh*/ 
                get_three_parameters(p, &par1, &par2, &par3);
                machine_code = printI(opcode,par1,par3,par2);
                break;
        
            case 6:     /* case jmp ,la ,call */
                if(p[0]== '$') /* it's a register */
                {
                    par1 = atoi(p+1); /* convert it to number, skip '$' */
                    machine_code = printJ(opcode,1,par1);
                } 
                else   /* it's a label */
                {
                    type_value = get_type_value(p,head);
                    if (type_value== EXTERNAL)  
                    {
                        machine_code = printJ(opcode,0,0); /* prints opcode and 00.... */
                        add_extern_node(ext,p,current_line); /* add to the external symbol list */
                    }
                    else
                    {
                        par1 = getLabelNumber(p,head); 
                        if(par1==-1)
                        {
                            fprintf(stderr,"Line %d: the label '%s' doesn't exist \n",current_line_error,p);
                            *error_flag=1;
                        }
                        else
                            machine_code = printJ(opcode,0,par1);
                    }
                }
                break;
        
            case 7:     /* case stop */
                machine_code = printJ(opcode,0,0);
                break;

            case 8:     /* case .dh, .db, .dw */
                while(1)
                {
                    if((p[0] =='-')||(p[0] =='+')){
                        addData(data_array,&dataCounter,atoi(p),i);
                        p++; 
                    }
                    else
                        addData(data_array,&dataCounter,atoi(p),i);
                    while(isdigit(p[0]))  /* move to the next number */
                        p++;
                    if (p[0]=='\0')  /*end of line */
                        break;
                    p++;
                }
                break;

            case 9:     /* case .asciz */
                p = skipSpacesAndTabs(p);
                p++;
                while(p[0]!='"'){
                    addData(data_array,&dataCounter,p[0],i);
                    p++;
                }
                addData(data_array,&dataCounter,0,i);
                break;
            
            case 10: /* case of note */
                break;

        } /* end of cases */

        /* add the instruction command data to the instruction array */
        if (i<27&& state !=10)
        {
            current_line+=4;
            instruction_array[j]=machine_code;
            j++;
        }
        current_line_error++;
    } /* end of while loop */
    return;
}
