/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 22/03/2026 | Document creation		                         |
 *
 * @author Zechin Alex
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_mcu.h" // Bibliteca para los pines GPIO

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

void gpio_bcd(gpioConf_t *gpio_conf, uint8_t numero_vector)
{
	// La funcion entra un vector con los pines y un numero de 4 bits
	for (uint8_t i = 0; i < 4; i++)
	{
		if ((numero_vector >> i) & 1) // Va desplzando el valor y lo va comparando con 1 para verificar si el bit esta activo o inactivo
		{
			// Activa el GPIO correspondiente al bit activo
			GPIOOn(gpio_conf[i].pin);
		}
		else
		{
			// Apaga el GPIO correspondiente al bit inactivo
			GPIOOff(gpio_conf[i].pin);
		}
	}
}
/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	// Define los pines 
	gpioConf_t gpio_conf[4] = {
		{GPIO_20, GPIO_OUTPUT},
		{GPIO_21, GPIO_OUTPUT},
		{GPIO_22, GPIO_OUTPUT},
		{GPIO_23, GPIO_OUTPUT}
	};

	// Inicializa los pines GPIO 
	for (uint8_t i = 0; i < 4; i++)
	{
		GPIOInit(gpio_conf[i].pin, gpio_conf[i].dir); // Configura los pines GPIO como salida
	}


	// Testeo de funcion 
	uint8_t numero = 9;
	
	while (1)
	{
		gpio_bcd(gpio_conf, numero);
		vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1 segundo
	}
	
}
/*==================[end of file]============================================*/