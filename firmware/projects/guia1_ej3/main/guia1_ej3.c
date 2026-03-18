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
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 500
/*==================[internal data definition]===============================*/
struct leds
{
	uint8_t mode; 		// Variable OFF, ON, TOGGLE
	uint8_t n_led; 		//Numero de led a controlar
	uint8_t n_ciclos; 	//Numero de ciclos de encendido/apagado
	uint16_t periodo; 	//Indica el tiempo en cada ciclo
}my_leds; //creado una instancia

enum {ON=1, OFF=0, TOGGLE=2};	//Le asigna valores de 1 2 3 

	
/*==================[internal functions declaration]=========================*/

void manejar_leds(struct leds *my_leds)
{
		
	switch(my_leds->mode)
	{
		// MODO = 1 -> ON
		case ON:
			LedOn(my_leds->n_led);
			break;

		// MODO = 0 -> OFF
		case OFF:
			LedOff(my_leds->n_led);
			break;

		case TOGGLE:
			uint8_t i = 0;

			while (i < my_leds->n_ciclos*2)
			{
				LedToggle(my_leds->n_led);
				//Aumenta el valor de la variable i en 1
				i++;

				//Retardo
				uint8_t j = 0;
				if (j < my_leds->periodo)
				{
					j++;
					vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
				}
			}
				
	}
}

	

/*==================[external functions definition]==========================*/
void app_main(void){
	LedsInit();

	my_leds.mode = TOGGLE;
	my_leds.n_ciclos = 10;
	my_leds.n_led = LED_3;
	my_leds.periodo = 1000;
	manejar_leds(&my_leds);

}
/*==================[end of file]============================================*/