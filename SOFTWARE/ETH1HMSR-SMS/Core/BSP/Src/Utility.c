/*
 * utility.c
 *
 *  Created on: Mar 22, 2024
 *      Author: grzegorz
 */

#include <string.h>
#include <ctype.h>

#include "Utility.h"
#include "ADC.h"
#include "FloatToString.h"
#include "HiSLIP.h"
#include "BSP.h"


extern bsp_t bsp;
extern float measurements[];

inline bool UTIL_Timeout(uint32_t start, uint32_t timeout)
{
	if (((HAL_GetTick() - start) >= timeout) || (0U == timeout))
	{
		return false;
	}
	else
	{
		return true;
	}
}




char buffer[sizeof(hislip_msg_t) + ADC_MEASUREMENT_BUFFER * UTIL_ASCII_SIZE +1];

char* UTIL_FloatArrayToASCII(scpi_t * context, enum netconn_type conn_type, float* float_array, uint32_t num_floats, size_t* frame_size)
{

    size_t size;
    size_t sum = 0;

    if((VISA_HISLIP == bsp.resource) && (NETCONN_TCP == conn_type))
    {
        size_t header_size = 0;
        hislip_msg_t header;

    	hislip_instr_t* hislip_instr = (hislip_instr_t*)context->user_context;

    	header_size = sizeof(hislip_msg_t);

        for (uint32_t x = 0; x < num_floats; x++)
        {
        	size = floatToString(buffer + header_size + sum, float_array[x]);
        	sum +=size;

        }

    	buffer[sum - 1] = '\0'; // Delete last ','

    	hislip_DataHeader(hislip_instr, &header, HISLIP_DATAEND, sum);
    	memcpy(buffer, &header, sizeof(hislip_msg_t));
    	memcpy(buffer + header_size + sum - 1, HISLIP_LINE_ENDING, strlen(HISLIP_LINE_ENDING));
    	buffer[sum - 1 + strlen(HISLIP_LINE_ENDING)] = '\0';

    	*frame_size = header_size + sum;

    }
    else
    {
        for (uint32_t x = 0; x < num_floats; x++)
        {
        	size = floatToString(buffer + sum, float_array[x]);
        	sum +=size;
        }

        buffer[sum - 1] = '\0'; // Delete last ','
    }

    return buffer;
}

int32_t UTIL_WhiteSpace(const char* string, uint32_t size)
{
	for (int32_t i = 0; i < size; i++)
	{
	        if (isspace((unsigned char)string[i]))
	        {
	            return i;
	        }
	    }

	return -1;
}

char* UTIL_FloatArrayToREAL(scpi_t * context, enum netconn_type conn_type, float* float_array, uint32_t num_floats, size_t* frame_size)
{

    uint32_t size = num_floats * sizeof(float); // +1 for the null terminator
    size_t header_size = 0;

    if((VISA_HISLIP == bsp.resource) && (NETCONN_TCP == conn_type))
	{
        hislip_msg_t header;
     	hislip_instr_t* hislip_instr = (hislip_instr_t*)context->user_context;
        size_t header_len;
        char block_header[12];

    	// Create the REAL block data header
    	// ----------------------------------------------------------------------------------

        memset(block_header, 0, 12);

    	block_header[0] = '#';
        SCPI_UInt32ToStrBase((uint32_t) size, block_header + 2, 10, 10);

        header_len = strlen(block_header + 2);
        block_header[1] = (char) (header_len + '0');

        // ----------------------------------------------------------------------------------


        // Create the HiSLIP header
        // ----------------------------------------------------------------------------------

    	hislip_DataHeader(hislip_instr, &header, HISLIP_DATAEND, strlen(block_header) + size + strlen(HISLIP_LINE_ENDING));

        // Build the data frame
    	// ----------------------------------------------------------------------------------

    	header_size = sizeof(hislip_msg_t);

    	memcpy(buffer, &header, header_size);
    	memcpy(buffer + header_size, &block_header, strlen(block_header));
    	memcpy(buffer + header_size + strlen(block_header), float_array, size);
     	memcpy(buffer + header_size + strlen(block_header) + size, HISLIP_LINE_ENDING, strlen(HISLIP_LINE_ENDING));

     	*frame_size = header_size + strlen(block_header) + size + strlen(HISLIP_LINE_ENDING);
    }
    else
    {
    	memcpy(buffer, float_array, size);

    	buffer[size] = '\0';
    }



    return buffer;
}
