extern uint32_t io_base;

static inline void mmio_write(uint32_t reg, uint32_t data) {        // mmio writes to register
    uint32_t *ptr = (uint32_t *)reg;
    asm volatile("str %[data], [%[reg]]" :
        : [reg] "r"(ptr), [data] "r"(data));
}

static inline uint32_t mmio_read(uint32_t reg) {                    // mmio reads from register
    uint32_t *ptr = (uint32_t *)reg;
    uint32_t data;
    asm volatile("ldr %[data], [%[reg]]" :
        [data] "=r"(data)
        : [reg] "r"(ptr));
    return data;
}

static inline void bcm2835_write(uint32_t reg, uint32_t data) {   // bcm2835 write to register with base offset
    mmio_write(io_base + reg, data);
}

static inline uint32_t bcm2835_read(uint32_t reg) {             // bcm2835 read from register with base offset
    return mmio_read(io_base + reg);
}