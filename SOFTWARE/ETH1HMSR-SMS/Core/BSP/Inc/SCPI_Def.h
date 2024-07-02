#ifndef __SCPI_DEF_H_
#define __SCPI_DEF_H_

#include <DEVICE_INFO.h>
#include "scpi/scpi.h"

#define SCPI_INPUT_BUFFER_LENGTH 512
#define SCPI_ERROR_QUEUE_SIZE 17
#define SCPI_IDN1 MANUFACTURER
#define SCPI_IDN2 MODEL
#define SCPI_IDN3 "3806244C52"
#define SCPI_IDN4 SOFTWARE_VERSION

#define MAX_PART_SAMPLES 1000

extern const scpi_command_t scpi_commands[];
extern scpi_interface_t scpi_interface;
extern char scpi_input_buffer[];
extern scpi_error_t scpi_error_queue_data[];
extern scpi_t scpi_context;
extern scpi_interface_t scpi_hislip_interface;
extern scpi_t scpi_hislip_context;

size_t SCPI_Write(scpi_t * context, const char * data, size_t len);
size_t SCPI_WriteHiSLIP(scpi_t * context, const char * data, size_t len);
int SCPI_Error(scpi_t * context, int_fast16_t err);
scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val);
scpi_result_t SCPI_Reset(scpi_t * context);
scpi_result_t SCPI_Flush(scpi_t * context);

void scpi_server_init(void);


#endif /* __SCPI_DEF_H_ */
