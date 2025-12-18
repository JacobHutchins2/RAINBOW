#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define SYSCALL_READ		3
#define SYSCALL_WRITE		4
#define SYSCALL_TIME		13
#define SYSCALL_BLINK		8192

// system call / software interrupt
static inline uint32_t syscall3(long sysnum, long a, long b, long c) {

	//syscall arguments
	register long _r0 asm("r0")=(long)(a);
	register long _r1 asm("r1")=(long)(b);
	register long _r2 asm("r2")=(long)(c);
	register long _r7 asm("r7")=(long)(sysnum);

	// Trigger software interrupt to enter kernel mode
	asm volatile(
		"swi #0\n"
		: "=r"(_r0)				/* Outputs */
		:  "r"(_r0), "r"(_r1), "r"(_r2),	/* Inputs */
		   "r"(_r7)
		: "memory");				/* Clobbers */

	//error handling
	// return values in range [-4095, -1] indicate an error
	if(_r0 >=(unsigned long) -4095) {
		/* FIXME: Update errno */
		_r0=(unsigned long) -1;
	}

	//return result or error(-1)
	return (long) _r0;

}