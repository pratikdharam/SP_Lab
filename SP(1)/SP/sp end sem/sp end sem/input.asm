START: MOV AX, 20
MOV BX, NUM1 ADD AX, BX MOV CX, 5
SUB AX, CX
LOOP: MOV DX, NUM2 INC DX
MOV NUM2, DX DEC CX
JNZ LOOP MOV SP, 0
HLT
NUM1: DB 15 NUM2: DB 10