; FIBANACCI NUMBERS CALCULATOR
;
; INPUT:   number of Fibanacci series
; OUTPUT:  member of Fibanacci series with given number
; COMMENT: let's define F_0 = 0, F_1 = 1   
    
BEGIN:
    ; Storing fib in eax
    in ; N
    ; Else call Fibanacci
    dup
    push 30
    ca FIB
    push eax
    out
    jmp END
; Counts Fibanacci number with number that is in stack
FIB: 
    dup ; N|N
    dup ; N|N|N
    push 1 ; 1|N|N|N
    je ONE
    push 0 ; 0|N|N
    je NULL
    push -1 ; -1|N
    add ; N-1
    dup ; N-1|N-1
    push -1 ; -1|N-1|N-1
    add ; N-2|N-1
    call FIB
    call FIB
; End of Fibanacci
FIB_END:
    ret
; F_0 = 0 
NULL:
    pop ebx ; <empty>
    jmp FIB_END
; F_1 = 1
ONE:
    pop ebx ; Clearing rubish ; N|N -> N, N = 1
    ; There is already 1 in stack
    push eax ; 1|F
    add ; 1+F
    pop eax ; Store F
    jmp FIB_END
; End of the program
END:
    end

