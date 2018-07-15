#ifndef _ADXL345_H_
#define _ADXL345_H_


#include "nrf.h"
#include "nrf_twi_mngr.h"

#include "nrf_drv_gpiote.h"
#include "nrf_log.h"


#define ADXL345_SCL_PIN             27    // SCL signal pin
#define ADXL345_SDA_PIN             28    // SDA signal pin
#define ADXL345_INT1_PIN            30    // SDA signal pin

#define ADXL345_SLAVE_ADDR          0x53


typedef struct sGSensor{
	int16_t x;
	int16_t y;
	int16_t z;
	bool isNew;
}tGSensor;


extern volatile tGSensor gGSensor;


/*****************************************************************************
 * @bref    Function for initializing the adxl345 G sensor.
 * @param   none
 * @retval   0: Success
             -: Error
*****************************************************************************/
int32_t ADXL345_init(void);

/*****************************************************************************
 * @bref      Function for triger access read
 * @param[in] isBlock Is block mode
 * @retval   0: Success
            -1: Error
*****************************************************************************/
int32_t ADXL345_trig_read(bool isBlock);

/*****************************************************************************
 * @bref    ADXL345 loop func.
 * @param   none
 * @retval  none
*****************************************************************************/
void ADXL345_loop(void);


#endif
