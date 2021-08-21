assembler:  assembler.o get_input.o  first_pass.o second_pass.o symbol.o build_files.o convert_to_machine_code.o
	gcc -g -ansi -pedantic -Wall assembler.o get_input.o first_pass.o second_pass.o symbol.o build_files.o convert_to_machine_code.o -o assembler
assembler.o:  assembler.c assembler.h
	gcc -c -ansi -pedantic -Wall assembler.c -o assembler.o
get_input.o:  get_input.c assembler.h
	gcc -c -ansi -pedantic -Wall get_input.c -o get_input.o
second_pass.o:  second_pass.c assembler.h
	gcc -c -ansi -pedantic -Wall second_pass.c -o second_pass.o
first_pass.o:  first_pass.c assembler.h
	gcc -c -ansi -pedantic -Wall first_pass.c -o first_pass.o
symbol.o:  symbol.c assembler.h
	gcc -c -ansi -pedantic -Wall symbol.c -o symbol.o
build_files.o:  build_files.c assembler.h
	gcc -c -ansi -pedantic -Wall build_files.c -o build_files.o
convert_to_machine_code.o:  convert_to_machine_code.c assembler.h
	gcc -c -ansi -pedantic -Wall convert_to_machine_code.c -o convert_to_machine_code.o
