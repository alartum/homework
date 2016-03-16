call main
jmp END

print:
out
ret

read:
in
pop eax
ret
factorr:
pop ebp
IF_0:
push ebx
push 1
neq
push 0
je IF_END_0
push ebx

call print
push ebp

ret

IF_END_0:
push 1
push ebx
sub

pop ebx
push ecx
push ebx
mul

pop ecx
push ebp

call factorr
factorr_end:
ret
factor:
pop eax
push 1

pop ecx
push 1

pop ebx
WHILE_0:
push ecx
push eax
grq
push 0
je WHILE_END_0
push ebx
push ecx
mul

pop ebx
push 1
push ecx
add

pop ecx

jmp WHILE_0
WHILE_END_0:
push ebx

ret
factor_end:
ret
main:
push eax

call read
push eax

call factor
push ebx

call print
push ebp

call read
push eax

pop ebx
push 1

pop ecx
push eax
call factor
pop edx
push edx

call print
push 0

ret
main_end:
ret


END:
end
