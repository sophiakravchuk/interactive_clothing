#include <stdlib.h>
#include <stdint.h>

//--------------------------------------------------
#define SystemCoreClockInMHz (SystemCoreClock/1000000);

//--------------------------------------------------
int DWT_Init(void);
void udelay_DWT (uint32_t useconds);
uint32_t get_DWT_cycles();
uint32_t get_DWT_us();
void reset_DWT_cycles();
uint32_t get_us();
void udelay (uint32_t useconds);
