#define B_PUSH_RAX ".byte 0x50\n\t" 
#define B_PUSH_RBX ".byte 0x53\n\t" 
#define B_POP_RAX  ".byte 0x58\n\t" 
#define B_POP_RBX  ".byte 0x5b\n\t"

#define B_NOP ".byte 0x48,0x87,0xc0\n\t"

#define H_PUSH     0x50 
#define H_POP      0x58 
#define H_NOP_0    0x48 
#define H_NOP_1    0x87 
#define H_NOP_2    0xC0 

#define JUNK_ASM __asm__ __volatile__ (B_PUSH_RBX B_PUSH_RAX B_NOP B_NOP B_POP_RAX B_POP_RBX)
#define JUNKLEN 10

