    push 0
    pop eax
BEGIN:
    push 10
    push eax
    les
    out
    push 10
    push eax
    ja END
    push eax
    push eax
    mul
    push eax
    mul
    out
    push 1
    push eax
    add
    pop eax
    jmp BEGIN
END:
    end
