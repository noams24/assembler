;no operands (with or without space or tab)
.dh
.asciz 
.entry	

;unrecognized
.dd $2
command $2

;insructions
.asciz abc123
.asciz "abc123
.db a
.db 128
.db -129
.dh 32768
.dh -32769
.dw 2147483648
.dw -2147483649
.db ,1
.db 2,
.db 1,,2
.db ++1

;labels
LABEL: 
LABEL$: add 
1LABEL: add
LABELLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL: add
add:
label: add $15,$27,$31
label: add $15,$27,$31

.extern doublelabel
.entry double label

;commands
jmp
jmp $1, $2
move $1
move $2,$3, $4
move label
add $1
add $1, $2,$3, $4
bne $1, $2, $3
jmp $154
jmp $a
