// command for delay loop
static inline void delay(int32_t count) {       // inline assembly to acoid optimization
	asm volatile("__delay_%=: subs %[count], %[count], #1; "    // setting up count
			"bne __delay_%=\n"                       // branch if not equal to zero
		: [count]"+r"(count) /* outputs */              // increment
		: /* inputs */
		: "cc" /* clobbers */);     // 
}
