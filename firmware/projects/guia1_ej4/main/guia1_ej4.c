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
 * | 18/03/2026 | Document creation		                         |
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
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
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

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	uint32_t number = 1234; 	// Numero a convertir a BCD
	uint8_t bcd_array[4];		// Arreglo para almacenar el resultado 
	convertToBcdArray(number, 4, bcd_array);	// Llama a la funcion
	printf("BCD Array: [%d, %d, %d, %d]\n", bcd_array[0], bcd_array[1], bcd_array[2], bcd_array[3]);	// Imprime el resultado
}
/*==================[end of file]============================================*/