extern uint32_t tick_counter;
extern uint32_t hour;
extern uint32_t minute;
int timer_init(void);
void clock_set(void);
void get_time(void);
void run_preset(void);