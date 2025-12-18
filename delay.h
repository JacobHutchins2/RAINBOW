// command for delay loop
/* running a 1.4Mhz clock */
static inline void delay(int32_t count) {       // inline assembly to acoid optimization
	asm volatile("__delay_%=: subs %[count], %[count], #1; "    // setting up count
			"bne __delay_%=\n"                       // branch if not equal to zero
		: [count]"+r"(count) /* outputs */              // increment
		: /* inputs */
		: "cc" /* clobbers */);     // 
}

// ms delay
static inline void delay_ms(int ms) {
    for (int i = 0; i < ms; i++) {
		asm("");        // avoid optimization
        delay(5000);   // ≈ 1 ms 
    }
}