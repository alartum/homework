#ifndef REG_ADDRESS_INCLUDED
#define REG_ADDRESS_INCLUDED
// Right way to define: ADD_INFO(name, address)
#define REG_SIZE 4
#define REG_NUMBER 8

#endif

#ifndef DEFINES_ONLY
///Accumulator register
ADDRESS(eax, 0 * REG_SIZE)
///Base register
ADDRESS(ebx, 1 * REG_SIZE)
///Counter register
ADDRESS(ecx, 2 * REG_SIZE)
///Data register
ADDRESS(edx, 3 * REG_SIZE)
///Source index register
ADDRESS(esi, 4 * REG_SIZE)
///Destination index register
ADDRESS(edi, 5 * REG_SIZE)
///Stack pointer register
ADDRESS(esp, 6 * REG_SIZE)
///Base pointer register
ADDRESS(ebp, 7 * REG_SIZE)
#endif
