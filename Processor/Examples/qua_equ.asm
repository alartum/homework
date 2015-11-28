; QUADRATIC EQUATIONS SOLVER
;
; INPUT:   float a, b, c - the coefficients of the equation
; OUTPUT:  solution of the equation
; COMMENT: prints 0 if no solutions, -1 in case of infinite number of roots. Output is like M N K, where M = amount of roots, N, K - roots

BEGIN:
    in ; Read a
    pop eax ; Store in eax
    in ; Read b
    pop ebx ; Store in ebx
    in ; Read c
    pop ecx ; Store in ecx
    call SOLVE_SQUARE
    jmp END

; Calculates sqrt of the top stack element
SQRT:
    pop ebp
    push 1 ; y = 1
    
    LOOP:
        pop esp ; Get y
        push esp ; y_0 = y
        dup ; y_0|y_0
        push ebp ; x|y_0|y_0
        div ; x/y_0|y_0
        add ; x/y_0 + y_0 
        push 0.5 ; 0.5 | x/y_0 + y_0
        mul ; (x/y_0 + y_0)/2
        dup ; (x/y_0 + y_0)/2 | (x/y_0 + y_0)/2
        push esp ; y | (x/y_0 + y_0)/2 | (x/y_0 + y_0)/2
        sub ; y - (x/y_0 + y_0)/2 | (x/y_0 + y_0)/2
        abs ; abs(y - (x/y_0 + y_0)/2)| (x/y_0 + y_0)/2
        push 0.000001
    jb LOOP
SQRT_END:
    ret

; Solves square equation which coefficients a, b, c are in aex, abx, acx
SOLVE_SQUARE:
    push eax
    abs
    push 0.000001
    ja A_=_0
    call calcD
    dup
    push 0
    ja NO_ROOTS
    call SQRT
    pop edx
    push edx
    push 0.000001
    ja ONE_ROOT
    push 2 ; We have 2 roots now
    out
    call calcFirst
    call calcSecond
    out
    out
SOLVE_SQUARE_END:
    ret
    
; In case A = 0
A_=_0:
    push ebx
    abs
    push 0.000001
    ja A_&_B_=_0 ; If B = 0
    push ebx ; Else x = -c/b
    push ecx
    push -1
    mul
    div
    push 1
    out ; Print number of roots
    out ; Print root
    jmp SOLVE_SQUARE_END ; Returning
A_&_B_=_0:
    push ecx
    abs
    push 0.000001
    ja INFINITE_ROOTS
    jmp NO_ROOTS
INFINITE_ROOTS:
    push -1
    out
    jmp SOLVE_SQUARE_END
NO_ROOTS:
    push 0
    out
    jmp SOLVE_SQUARE_END

ONE_ROOT:
    push 1
    out
    call calcFirst
    out
    jmp SOLVE_SQUARE_END 

calcD:
    push eax
    push ecx
    mul
    push 4
    mul
    push ebx
    push ebx
    mul
    sub
    ret

calcFirst:
    push 2
    push eax
    mul
    push ebx
    push -1
    mul
    push edx
    add
    div
    ret
calcSecond:
    push 2
    push eax
    mul
    push ebx
    push -1
    mul
    push edx
    push -1
    mul
    add
    div
    ret
END:
    end
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
