#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define SYSCALL_READ		3
#define SYSCALL_WRITE		4

static inline uint32_t syscall3(long sysnum, long a, long b, long c) {

	register long _r0 asm("r0")=(long)(a);
	register long _r1 asm("r1")=(long)(b);
	register long _r2 asm("r2")=(long)(c);
	register long _r7 asm("r7")=(long)(sysnum);

	asm volatile(
		"swi #0\n"
		: "=r"(_r0)				/* Outputs */
		:  "r"(_r0), "r"(_r1), "r"(_r2),	/* Inputs */
		   "r"(_r7)
		: "memory");				/* Clobbers */

	if(_r0 >=(unsigned long) -4095) {
		/* FIXME: Update errno */
		_r0=(unsigned long) -1;
	}
	return (long) _r0;

}