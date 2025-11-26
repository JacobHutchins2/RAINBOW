#include <stdint.h>

static inline uint32_t get_CPSR(void) {

	uint32_t temp;

	asm volatile ("mrs %0,CPSR":"=r" (temp):) ;

	return temp;
}

static inline void set_CPSR(uint32_t new_cpsr) {
	asm volatile ("msr CPSR_cxsf,%0"::"r"(new_cpsr) );
}

static inline uint32_t enable_interrupts(void){
    asm volatile ("cpsie i"::);
}

static inline uint32_t disable_interrupts(void){
    asm volatile ("cpsid i"::);
    return 0;
}
