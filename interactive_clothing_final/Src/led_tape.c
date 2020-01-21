#include "led_tape.h"
#include "gpio.h"

//------------------------------------------------------------------------------------
void delay_L(void) {
    int useconds = 10;
    asm volatile("   mov r0, %[useconds]    \n"
                 "1: subs r0, #1            \n"
                 "   bhi 1b                 \n": :[useconds] "r" (useconds): "r0");
}

//------------------------------------------------------------------------------------
void delay_H(void) {
    int useconds = 20;
    asm volatile("   mov r0, %[useconds]    \n"
                 "1: subs r0, #1            \n"
                 "   bhi 1b                 \n":: [useconds] "r" (useconds): "r0");
}

//------------------------------------------------------------------------------------
inline void send_byte(int a) {
    for (int i = 7; i >= 0; i--) {
        int x = (a >> i) & 1;
        if (x == 1) {
        	GPIOB->BSRR = GPIO_PIN_8;
			delay_H();
			GPIOB->BSRR = (uint32_t) GPIO_PIN_8 << 16U;
			delay_L();
        } else {
			GPIOB->BSRR = GPIO_PIN_8;
			delay_L();
			GPIOB->BSRR = (uint32_t) GPIO_PIN_8 << 16U;
			delay_H();
        }
    }
}

//------------------------------------------------------------------------------------
int distance_to_state(int distance) {
    if (distance > GREEN) {
        return 0;
    } else if (distance > YELLOW) {
        return STATE_STEP;
    } else if (distance > ORANGE) {
        return 2 * STATE_STEP;
    } else if (distance > RED) {
        return 3 * STATE_STEP;
    }
    return 0;
}

//------------------------------------------------------------------------------------
void light_grad(int dist) {
    int state = distance_to_state(dist);
    for (int j = 0; j<61; j++) {
        char hex = 0x00;
        hex +=state;

        send_byte(0xFF-hex);
        send_byte(hex);
        send_byte(0x00);
        state++;
    }
    HAL_Delay(500);
}

//------------------------------------------------------------------------------------

void light(int distance) {
    for (int i = 0; i < distance; i++) {
        int num = i % 7;
        if (num == 0) { //red
            send_byte(0x00);
            send_byte(0xFF);
            send_byte(0x00);
        } else if (num == 1) { //orange
             send_byte(0x45);
             send_byte(0xFF);
             send_byte(0x00);
        } else if (num == 2) { //yellow
            send_byte(0xFF);
            send_byte(0xFF);
            send_byte(0x00);
        } else if (num == 3) { //green
            send_byte(0xFF);
            send_byte(0x00);
            send_byte(0x00);
        } else if (num == 4) { //lightblue
            send_byte(0xBF);
            send_byte(0x00);
            send_byte(0xFF);
        } else if (num == 5) { //blue
            send_byte(0x00);
            send_byte(0x00);
            send_byte(0xFF);
        } else { //purple
            send_byte(0x2B);
            send_byte(0x8A);
            send_byte(0xE2);
        }
    }
    for (int i = distance; i < 61; i++) {
        send_byte(0x00);
        send_byte(0x00);
        send_byte(0x00);
    }
}

