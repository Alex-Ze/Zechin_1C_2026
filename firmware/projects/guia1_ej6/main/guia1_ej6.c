/*! @mainpage Guia 1 - Ejercicio 6
 *
 * @section genDesc General Description 
 *
 * El progrma convierte un nuemero de 3 digitos a un formato BCD y lo muestra en un panel LCD de 7 segmentos utilizando un conversor de BCD a 7 segmentos y un selector de panel LCD.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral   |   ESP32       |
 * |:---------------:|:--------------|
 * |    D1     |   GPIO_20     |
 * |    D2     |   GPIO_21     |
 * |    D3     |   GPIO_22     |
 * |    D4     |   GPIO_23     |
 * |    SEL_1  |   GPIO_19     |
 * |    SEL_2  |   GPIO_18     |
 * |    SEL_3  |   GPIO_9      |
 * |    +5V   |   +5V      |
 * |    GND   |   GND      |
 * 
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 25/03/2026 | Document creation		                         |
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

// --------------------------------------------------------------------------
/**
 * @brief Convierte un numero entero sin signo de 32 bits a un formato BCD y lo almacena en un arreglo de enteros sin signo de 8 bits
 * @param data Numero entero sin signo de 32 bits a convertir
 * @param digits Cantidad de digitos a convertir
 * @param bcd_number Puntero a un arreglo de enteros sin signo de 8 bits donde se almacenara el resultado
 * @return 0 si la conversion es exitosa, -1 en caso de error
 */

int8_t  convertToBcdArray (uint32_t data, uint8_t digits, uint8_t * bcd_number)
// Recibe un numero entero sin signo de 32 bits, la cantidad de digitos a convertir y un puntero a un arreglo de enteros sin signo de 8 bits donde se almacenara el resultado
{
	int8_t i;	//Permite los valores negativos y no pasa a 255
	for(i = digits - 1; i >= 0; i--)		// i-- para ir decrementando el indice, va a ir llenando el arreglo de atras para adelante
							// mientras i sea mayor o igual a 0, va a seguir llenando el arreglo
	{
		bcd_number[i] = data % 10;	//va agregando el resto de la division del numero por 10, va a ir agregando el ultimo digito del numero al arreglo [3, 2, 1, 0] -->
		data /= 10;			//data = data / 10;	// para ir eliminando el ultimo digito del numero, va a ir dividiendo el numero por 10
	}
	return 0;
}

/**
 * @brief la estructura gpioConf_t hace magia 
 */

typedef struct
{
	gpio_t pin;			/*!< GPIO pin number */
	io_t dir;			/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

// --------------------------------------------------------------------------
// La funcion recibe un vector con los pines y un numero de 4 bits, y activa o desactiva los GPIO correspondientes a cada bit del numero
/**
 * @brief La funcion recibe un vector con los pines y un numero de 4 bits, y activa o desactiva los GPIO correspondientes a cada bit del numero
 * @param gpio_conf Vector con los pines GPIO
 * @param numero_vector Numero de 4 bits
 * @return Nada (es un void)
 */

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

// --------------------------------------------------------------------------
/** 
 * La funcion recibe un vector con los pines y un numero con el tiempo entre pin y pin
 * El objetivo de esta funcion es de actvar cada uno de los paneles de forma secuencial
 * @param gpio_conf Vector con los pines GPIO
 * @param gpio_selector Vector con los pines de seleccion
 * @param numero Numero a mostrar en el panel LCD
 * @return NONE (es un void)
 */
void mostrar_dato_en_display(gpioConf_t *gpio_conf, gpioConf_t *gpio_selector, uint32_t numero){
	// -----------------1---------------------------
	// Convierte el numero para manejar en el BCD
	uint8_t bcd_array[3];		// Arreglo para almacenar el resultado 
	convertToBcdArray(numero, 3, bcd_array);	// Llama a la funcion para convertir el numero a un arreglo de BCD
	
	// -----------------2---------------------------
	for (uint8_t i = 0; i < 3; i++)
	{
		gpio_bcd(gpio_conf, bcd_array[i]);	// Llama a la funcion para mostrar el numero en el panel LCD
		GPIOOn(gpio_selector[i].pin);	// Enciende el GPIO correspondiente al bit activo
		GPIOOff(gpio_selector[i].pin);	// Apaga el GPIO correspondiente al bit activo
	}
}

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	// ------------------------------------------------------------------
	// 						Conversor de BCD a 7 segmentos

	// Define los pines de escritura en el panel LCD
	gpioConf_t gpio_conf[4] = {
		{GPIO_20, GPIO_OUTPUT},
		{GPIO_21, GPIO_OUTPUT},
		{GPIO_22, GPIO_OUTPUT},
		{GPIO_23, GPIO_OUTPUT}
	};

	// Inicializa los pines GPIO correspondientes a cada entrada del conversor de BCD a 7 segmentos
	for (uint8_t i = 0; i < 4; i++)
	{
		GPIOInit(gpio_conf[i].pin, gpio_conf[i].dir); // Configura los pines GPIO como salida
	}
	
	// ------------------------------------------------------------------
	// 						Selector de panel LCD
	gpioConf_t gpio_selector[3] = {
		{GPIO_19, GPIO_OUTPUT},
		{GPIO_18, GPIO_OUTPUT},
		{GPIO_9, GPIO_OUTPUT},
	};

	// Inicializa los pines GPIO correspondientes a cada panle LCD
	for (uint8_t i = 0; i < 3; i++)
	{
		GPIOInit(gpio_selector[i].pin, gpio_selector[i].dir); // Configura los pines GPIO como salida
	}
	// ------------------------------------------------------------------
	// 						Numero a mostrar en el panel LCD

	uint32_t number = 123; 	// Numero a convertir a BCD
	mostrar_dato_en_display(gpio_conf, gpio_selector, number);	// Llama a la funcion para mostrar el numero en el panel LCD
}
/*==================[end of file]============================================*/