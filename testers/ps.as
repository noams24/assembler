.entry Next
.extern wNumber
STR: .asciz "aBcd"
MAIN: add $3,$5,$9 
LOOP: ori $9,-5,$2 
 la vall
 jmp Next 
Next: move $20,$4 
 bgt $4,$2,END 
 la K 
 sw $0,4,$10 
 bne $31,$9,LOOP 
 call vall
 jmp $4 
 la wNumber
 .extern vall
END: stop 
.entry K 
LIST: .db 6,-9 
.dh 27056 
K: .dw 31,-12
