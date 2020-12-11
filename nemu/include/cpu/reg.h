#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"

void sreg_load(uint8_t sreg_num);

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

enum { R_CS,R_SS,R_DS,R_ES,R_FS,R_GS };


/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct{
	uint16_t selector;// visible
	/*invisible*/
	uint16_t attribute;//read,write,execute
	uint32_t limit;
	uint32_t base;
}Segment_Reg;

// typedef union{
// 	struct{
// 	uint32_t PE:	1;
// 	uint32_t MP:	1;
// 	uint32_t EM:	1;
// 	uint32_t TS:	1;
// 	uint32_t ET:	1;
// 	uint32_t NE:	1;
// 	uint32_t :	9;
// 	uint32_t WP:	1;
// 	uint32_t :	1;
// 	uint32_t AM:	1;
// 	uint32_t NW:	1;
// 	uint32_t CD:	1;
// 	uint32_t PG:	1;
// 	};

// 	uint32_t val;
	
// }CR0;

typedef struct {
	union {
		union{
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		}gpr[8];

		struct 
		{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
			union{
				struct{
					uint32_t CF:	1;
					uint32_t :	1;
					uint32_t PF:	1;
					uint32_t :	1;
					uint32_t AF:	1;
					uint32_t :	1;
					uint32_t ZF:	1;
					uint32_t SF:	1;
					uint32_t TF:	1;
					uint32_t IF:	1;
					uint32_t DF:	1;
					uint32_t OF:	1;
					uint32_t IOPL:	2;
					uint32_t NT:	1;
					uint32_t :	1;
					uint32_t RF:	1;
					uint32_t VM:	1;
					uint32_t :	14;
				};
				uint32_t EFLAGS;
			};
			
		};
		
	};

	

	CR0 CR0;

	union{
		struct{
			Segment_Reg CS,SS,DS,ES,FS,GS;
		};

		Segment_Reg segment_reg[6];
	};

	struct GDTR{
		uint32_t base;
		uint16_t limit;
	}GDTR;
	

	CR3 CR3;
	//store 
	swaddr_t eip;

} CPU_state;

//global cpu
extern CPU_state cpu;

typedef struct{
	union{
		struct{
			uint32_t base1:		16;
			uint32_t limit1:	16;
		};
		uint32_t part1;
	};

	union{
		struct{
		uint32_t base2:		8;
		uint32_t a:			1;
		uint32_t type:		3;
		uint32_t s:			1;
		uint32_t dpl:		2;
		uint32_t p:			1;
		uint32_t limit2:	4;
		uint32_t avl:		1;
		uint32_t :			1;
		uint32_t x:			1;
		uint32_t g:			1;
		uint32_t base3:		8;
		};
		uint32_t part2;
		
	};
}SegmentDescriptor;

typedef struct {
	union {
		struct {
			uint32_t p 	:1;
			uint32_t rw	:1;
			uint32_t us	:1;
			uint32_t 	:2;
			uint32_t a	:1;
			uint32_t d 	:1;
			uint32_t 	:2;
			uint32_t avail	:3;
			uint32_t addr 	:20;
		};
		uint32_t val;
	};
}PageDescriptor;

SegmentDescriptor *seg_desc;
uint8_t current_sreg;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
