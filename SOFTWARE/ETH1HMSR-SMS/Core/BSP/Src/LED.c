/*
 * LED.c
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include "types.h"
#include "cmsis_os.h"

#include "main.h"
#include "LED.h"
#include "BSP.h"

// --------------------------------------------------------------------------------------------------------------------

xQueueHandle QueueLEDHandle;
extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

void LED_Control(led_select_t LEDs, bool enable)
{
	if (LEDs & RED) {
		(enable) ? (LL_GPIO_ResetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin)) : (LL_GPIO_SetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin));
	}
	if (LEDs & GREEN) {
		(enable) ? (LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin)) : (LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin));
	}
	if (LEDs & BLUE) {
		(enable) ? (LL_GPIO_ResetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin)) : (LL_GPIO_SetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin));
	}
}


// --------------------------------------------------------------------------------------------------------------------

void LED_Toggle(led_select_t LEDs, uint32_t time_on, uint32_t time_off)
{
	LED_Control(LEDs, true);
	HAL_Delay(time_on);
	LED_Control(LEDs, false);
	HAL_Delay(time_off);
}


// --------------------------------------------------------------------------------------------------------------------

void LED_osQueue(led_select_t LED_state)
{

	static uint8_t led_color_send;

	LL_GPIO_SetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
	LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	LL_GPIO_SetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin);

	switch(LED_state)
	{
		case GREEN:
		{
			led_color_send = GREEN;
			xQueueSend(QueueLEDHandle, &led_color_send, portMAX_DELAY);

		}; break;

		case RED:
		{
			led_color_send = RED;
			xQueueSend(QueueLEDHandle, &led_color_send, portMAX_DELAY);

		}; break;

		case BLUE:
		{
			if(bsp.led)
				LL_GPIO_ResetOutputPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

		}; break;

		default:
		{
			led_color_send = GREEN;
			xQueueSend(QueueLEDHandle, &led_color_send, portMAX_DELAY);

		}; break;

	}

}
