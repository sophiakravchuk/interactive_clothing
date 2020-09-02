#include "gpio.h"
#include "dalekomir.h"
#define SystemCoreClockInMHz (SystemCoreClock/1000000);


//------------------------------------------------------------------------------------
int DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= SysTick_CTRL_ENABLE_Msk;
    DWT->CYCCNT = 0;
    uint32_t before = DWT->CYCCNT;
    __NOP();
    __NOP();
    return (DWT->CYCCNT - before);
}

//------------------------------------------------------------------------------------
inline void udelay_DWT (uint32_t useconds) {
    useconds *= SystemCoreClockInMHz;
    while( DWT->CYCCNT < useconds){}
}

//------------------------------------------------------------------------------------
inline uint32_t get_DWT_cycles()
{
    return DWT->CYCCNT;
}

//------------------------------------------------------------------------------------
inline uint32_t get_DWT_us()
{
    return get_DWT_cycles()/SystemCoreClockInMHz;
}

//------------------------------------------------------------------------------------
inline void reset_DWT_cycles()
{
    DWT->CYCCNT = 0;
}

//------------------------------------------------------------------------------------
inline uint32_t get_us(){
    return get_DWT_us();
}

//------------------------------------------------------------------------------------
inline void udelay (uint32_t useconds){
    udelay_DWT(useconds);
}

//------------------------------------------------------------------------------------
