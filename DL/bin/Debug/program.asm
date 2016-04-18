call main
jmp END

print:
out
ret

read:
in
pop eax
ret


fib:
pop eax
push 1
pop ecx
push 0
pop edx
push 0
pop ebp
WHILE_0:
push ebx
push eax
grq
push 0
je WHILE_END_0
push ecx
pop ebx
push ecx
push ebx
add
pop ecx
push ebx
pop edx
push ebp
push 1
add
pop ebp

jmp WHILE_0
WHILE_END_0:
push ecx

ret
fib_end:
ret
main:
push eax

call read
push eax

call factor
push eax

call print
push 0

ret
main_end:
ret


END:
end
