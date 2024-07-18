/*
 * HTTP_Support.c
 *
 *  Created on: Jun 20, 2024
 *      Author: BehrensG
 */

// --------------------------------------------------------------------------------------------------------------------

#include <string.h>
#include <stdint.h>

#include "cmsis_os.h"

#include "HTTP_Task.h"
#include "BSP.h"
#include "DEVICE_INFO.h"
#include "LED.h"
#include "ADC.h"
#include "GPIO.h"
#include "FloatToString.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;
extern ADC_HandleTypeDef hadc3;
extern char buffer[];
extern float measurements[];
extern SemaphoreHandle_t MeasMutex;
extern TIM_HandleTypeDef htim3;

// --------------------------------------------------------------------------------------------------------------------

#define HTTP_POST_CONTROL_BLINK_LED			0
#define HTTP_GET_RESOLUTION					1
#define HTTP_GET_SAMPLING_TIME				2
#define HTTP_GET_OVERSAMPLING_RATIO			3
#define HTTP_GET_GAIN						4
#define HTTP_GET_SAMPLE_COUNT				5

#define HTTP_POST_RESOLUTION				6
#define HTTP_POST_SAMPLING_TIME				7
#define HTTP_POST_OVERSAMPLING_RATIO		8
#define HTTP_POST_GAIN						9
#define HTTP_POST_SAMPLE_COUNT				10
#define HTTP_GET_MEASUREMENTS				11
#define HTTP_GET_READ_AVERAGE				12
#define HTTP_GET_LED_STATUS					13
#define HTTP_POST_LED_STATUS				14

#define DATA_SIZE	128

// --------------------------------------------------------------------------------------------------------------------

static const http_cmd_t http_control[] = {

		{.cmd="GET /ctr_resolution", .value = HTTP_GET_RESOLUTION},
		{.cmd="GET /ctr_sampling_time", .value = HTTP_GET_SAMPLING_TIME},
		{.cmd="GET /ctr_oversampling_ratio", .value = HTTP_GET_OVERSAMPLING_RATIO},
		{.cmd="GET /ctr_gain", .value = HTTP_GET_GAIN},
		{.cmd="GET /measurements", .value = HTTP_GET_MEASUREMENTS},
		{.cmd="GET /ctr_sample_count", .value = HTTP_GET_SAMPLE_COUNT},
		{.cmd="GET /ctr_read_average", .value = HTTP_GET_READ_AVERAGE},
		{.cmd="GET /ctr_led_status", .value = HTTP_GET_LED_STATUS},

		{.cmd="POST /ctr_blink_led", .value = HTTP_POST_CONTROL_BLINK_LED},
		{.cmd="POST /ctr_resolution", .value = HTTP_POST_RESOLUTION},
		{.cmd="POST /ctr_sampling_time", .value = HTTP_POST_SAMPLING_TIME},
		{.cmd="POST /ctr_oversampling_ratio", .value = HTTP_POST_OVERSAMPLING_RATIO},
		{.cmd="POST /ctr_gain", .value = HTTP_POST_GAIN},
		{.cmd="POST /ctr_sample_count", .value = HTTP_POST_SAMPLE_COUNT},
		{.cmd="POST /ctr_led_status", .value = HTTP_POST_LED_STATUS}
};


// --------------------------------------------------------------------------------------------------------------------

static void http_LED(led_select_t LED)
{
	if(bsp.led)
	{
		LED_osQueue(LED);
	}
}


// --------------------------------------------------------------------------------------------------------------------

static u32_t float_to_sampling_time(float value)
{
	u32_t select = (u32_t)(value * 10);
	switch (select)
	{
		case 15 : return ADC_SAMPLETIME_1CYCLE_5; break;
		case 25 : return ADC_SAMPLETIME_2CYCLES_5; break;
		case 85 : return ADC_SAMPLETIME_8CYCLES_5; break;
		case 165 : return ADC_SAMPLETIME_16CYCLES_5; break;
		case 325 : return ADC_SAMPLETIME_32CYCLES_5; break;
		case 645 : return ADC_SAMPLETIME_64CYCLES_5; break;
		case 3875 : return ADC_SAMPLETIME_387CYCLES_5; break;
		case 8105 : return ADC_SAMPLETIME_810CYCLES_5; break;
		default : return UINT32_MAX; break;
	}

	return UINT32_MAX;
}


// --------------------------------------------------------------------------------------------------------------------

static void http_measurements(struct netconn *conn)
{
    size_t size ;
    size_t sum = 0;

	if(pdTRUE == xSemaphoreTake(MeasMutex,  pdMS_TO_TICKS(20000)))
	{
		if(ADC_Measurement(bsp.adc.sample_count))
		{

			for (uint32_t i = 0; i < bsp.adc.sample_count; i++)
			{
				size = floatToString(buffer + sum, measurements[i]);
				sum +=size;
			}

			buffer[sum - 1] = 0;

			netconn_write(conn, buffer,strlen(buffer), NETCONN_NOCOPY);

		}

		xSemaphoreGive(MeasMutex);
	}
}


// --------------------------------------------------------------------------------------------------------------------

static void http_average(struct netconn *conn)
{
	float average = 0.0f;
	float tmp = 0.0f;

	char txt[12];
	memset(txt, 0, 12);

	if(pdTRUE == xSemaphoreTake(MeasMutex,  pdMS_TO_TICKS(20000)))
	{
		if(ADC_Measurement(bsp.adc.sample_count))
		{

			for (uint32_t i = 0; i < bsp.adc.sample_count ; i++)
			{
				tmp += measurements[i];
			}

			average = (float)(tmp/bsp.adc.sample_count);

			sprintf(txt, "%.6f", average);

			netconn_write(conn, txt,strlen(txt), NETCONN_NOCOPY);

		}

		xSemaphoreGive(MeasMutex);
	}
}


// --------------------------------------------------------------------------------------------------------------------

bool http_control_page(struct netconn *conn, char* buf, u16_t buflen)
{
	bool status = false;

	char pagedata[DATA_SIZE];
	memset(pagedata,0,DATA_SIZE);

	char control[DATA_SIZE];
	memset(control,0,DATA_SIZE);

	u16_t post_len = 0;
	s32_t header = -1;
	bool response = true;
	int int_val = 0;

	size_t size = sizeof(http_control)/sizeof(http_control[0]);
	header = http_header(buf, http_control, size);

	if(header >= 0)
		http_LED(BLUE);

	switch(header)
	{
		case HTTP_POST_CONTROL_BLINK_LED : {

			LED_Toggle((GREEN | RED | BLUE), 100, 900);
			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_GET_RESOLUTION : sprintf(pagedata, "%d", bsp.adc.bits); break;
		case HTTP_GET_SAMPLING_TIME : sprintf(pagedata, "%.1f", bsp.adc.cycles); break;

		case HTTP_GET_OVERSAMPLING_RATIO: {

			if(bsp.adc.oversampling.enable)
			{

				sprintf(pagedata, "%d", bsp.adc.oversampling.ratio);
			}
			else
			{
				sprintf(pagedata, "0");
			}
		}; break;

		case HTTP_GET_GAIN : sprintf(pagedata, "%d", bsp.adc.gain.value); break;
		case HTTP_GET_SAMPLE_COUNT : sprintf(pagedata, "%u", (unsigned int)bsp.adc.sample_count); break;


		case HTTP_POST_RESOLUTION : {

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);


			if(sscanf(control, "%d", &int_val) > 0)
			{
				bsp.adc.bits = (u8_t)int_val;
				bsp.adc.resolution = ADC_VREF/pow(2,(double)bsp.adc.bits);
				hadc3.Init.Resolution = ADC_SelectResolution((u8_t)int_val);

				ADC_Reset(bsp.adc.sampling_time);
				ADC_AutoCalibration();
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SAMPLING_TIME : {

			u32_t sampling_time = 0;
			float cycles = 0.0f;

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);

			cycles = (float)atof(control);
			sampling_time = float_to_sampling_time(cycles);

			if (UINT32_MAX != sampling_time)
			{
				bsp.adc.sampling_time = sampling_time;
				bsp.adc.cycles = cycles;
				ADC_Reset(sampling_time);
				ADC_AutoCalibration();
			}


			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_OVERSAMPLING_RATIO : {

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);

			if(sscanf(control, "%d", &int_val) > 0)
			{
				if (int_val > 0)
				{
					bsp.adc.oversampling.enable = true;
					bsp.adc.oversampling.ratio = (u16_t)int_val;
				}
				else
				{
					bsp.adc.oversampling.enable = false;

				}

				ADC_ConfigureOverSampling(bsp.adc.oversampling.enable, bsp.adc.oversampling.ratio);
				ADC_Reset(bsp.adc.sampling_time);
				ADC_AutoCalibration();

				vTaskDelay(pdMS_TO_TICKS(100));

			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_GAIN : {

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);

			if(sscanf(control, "%d", &int_val) > 0)
			{
				bsp.adc.gain.value = (uint8_t)int_val;
				bsp.adc.gain.index = ADC_GainIndex((uint8_t)int_val);
				GPIO_SelectGain((uint8_t)int_val);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SAMPLE_COUNT : {

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);

			if(sscanf(control, "%d", &int_val) > 0)
			{
				bsp.adc.sample_count = (u32_t)int_val;
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_GET_MEASUREMENTS : {



		    http_measurements(conn);

		    response = false; // Disable automatic response

		}; break;

		case HTTP_GET_READ_AVERAGE : {


			http_average(conn);

			response = false; // Disable automatic response

		}; break;

		case HTTP_GET_LED_STATUS : (bsp.led) ? sprintf(pagedata, "ON") : sprintf(pagedata, "OFF") ; break;

		case HTTP_POST_LED_STATUS : {

			char* post = http_post_data(buf, buflen, &post_len);
			memcpy(control, post, post_len);

			if(!strncmp(control, "ON", 2))
			{
				bsp.led = true;
				HAL_TIM_Base_Start_IT(&htim3);
			}
			else if(!strncmp(control, "OFF", 3))
			{
				LED_Control(GREEN | BLUE | RED, false);
				bsp.led = false;
				HAL_TIM_Base_Stop_IT(&htim3);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		default: /* DO NOTHING */; break;
	}

	if(header >= 0)
	{
		if(response)
		{
			http_send(conn, pagedata);
		}

		status = true;
	}
	else
	{
		status = false;
	}

	http_LED(GREEN);

	return status;
}
