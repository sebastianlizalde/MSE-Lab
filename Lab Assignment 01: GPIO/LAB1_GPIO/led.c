#include "led.h"
#include "GPIO_stm32f4.h"

#define LED_PORT  GPIO_PORT_A
#define LED_PIN   5U

void led_init(void)
{
    // encendemos el reloj del puerto A
    gpio_initPort(LED_PORT);

    // pin 5 como salida (va a mandar señal, no recibirla)
    gpio_setPinMode(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
}

void led_on(void)
{
    gpio_setPin(LED_PORT, LED_PIN); // manda 1 → LED prende
}

void led_off(void)
{
    gpio_clearPin(LED_PORT, LED_PIN); // manda 0 → LED apaga
}

void led_toggle(void)
{
    gpio_togglePin(LED_PORT, LED_PIN); // cambia el estado actual
}