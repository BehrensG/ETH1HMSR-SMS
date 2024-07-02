/*
 * SCPI_Format.c
 *
 *  Created on: Apr 24, 2024
 *      Author: BehrensG
 */

// --------------------------------------------------------------------------------------------------------------------

#include "SCPI_Format.h"
#include "BSP.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

scpi_choice_def_t format_data_select[] =
{
    {"ASCII", FORMAT_DATA_ASCII},
    {"REAL", FORMAT_DATA_REAL32},
    SCPI_CHOICE_LIST_END
};


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_FormatData(scpi_t * context)
{
	int32_t value;

	if (!SCPI_ParamChoice(context, format_data_select, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	bsp.format.data = (format_data_t)value;

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_FormatDataQ(scpi_t * context)
{
	if (FORMAT_DATA_ASCII == bsp.format.data)
	{
		SCPI_ResultCharacters(context, "ASCII", 5);
	}
	else if (FORMAT_DATA_REAL32 == bsp.format.data)
	{
		SCPI_ResultCharacters(context, "REAL", 5);
	}
	return SCPI_RES_OK;
}
