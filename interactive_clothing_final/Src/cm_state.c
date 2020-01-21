#include "main.h"
#include "gpio.h"
#include "led_tape.h"
#include "dalekomir.h"

void cm_state() {
    int is_far = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    int distance_cm;
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10) == GPIO_PIN_SET) {
        HAL_Delay(1);
    }
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10) != GPIO_PIN_SET) {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(1);
        return;
    }
    udelay(16);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10) != GPIO_PIN_RESET) {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
        HAL_Delay(1);
        return;
    }
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET) {
        HAL_Delay(1);
        return;
    }

    uint32_t watchdog_begin = get_us();
    int didnt_had_1_at_echo = 0;
    while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET) {
        if (get_us() - watchdog_begin > 50000) {
            didnt_had_1_at_echo = 1;
            break;
        }
    }
    if (didnt_had_1_at_echo) {
        HAL_Delay(1);
        return;
    }

    uint32_t before = get_us();
    int didnt_had_0_at_echo = 0;
    while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET) {
        if (get_us() - watchdog_begin > 50000) {
            didnt_had_0_at_echo = 1;
            break;
        }
    }
    if (didnt_had_0_at_echo) {
        HAL_Delay(1);
        return;
    }

    uint32_t pulse_time = get_us() - before;

    distance_cm = (int) ((float) pulse_time / 58);
    if (distance_cm > 3 && distance_cm < 500) {

        light(distance_cm);


        udelay(10);
    }
}