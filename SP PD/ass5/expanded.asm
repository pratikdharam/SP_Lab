DATA SEGMENT
LC EQU 100
STUDENTS DW 101, 102, 103, 104, 105
GRADES DW 85, 90, 78, 92, 88
SORTED_STUDENTS DW 10 DUP(?)
SORTED_GRADES DW 10 DUP(?)
DATA ENDS
CODE SEGMENT
LC EQU 300
START 350
ENTRY STUDENTS
EXTRN PRINT_RESULT:PROC
MOV AX, 100
MOV DS, AX
MOV SI, OFFSET STUDENTS
MOV DI, OFFSET SORTED_STUDENTS
MOV CX, 10
DEC CX
OUTER_LOOP:
MOV BX, 0
INNER_LOOP:
CMP GRADES[BX], GRADES[BX+2]
JGE SKIP_SWAP
PUSH AX
MOV AX, GRADES[BX]
XCHG AX, GRADES[BX+2]
MOV GRADES[BX], AX
POP AX
SKIP_SWAP:
ADD BX, 2
CMP BX, 20
JL INNER_LOOP
LOOP OUTER_LOOP
MOV CX, 10
MOV SI, OFFSET GRADES
MOV DI, OFFSET SORTED_GRADES
REP MOVSW
CALL PRINT_RESULT
MOV AX, 4C00H
INT 21H
CODE ENDS
END
