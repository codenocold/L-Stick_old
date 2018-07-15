#ifndef _KEY_H_
#define _KEY_H_


#include "nrf.h"
#include "nrf_gpio.h"


#define KEY_PIN     (13)


typedef uint32_t (*tkey_timestamp_func)(void);


/*****************************************************************************
 * @bref        KEY_init
 * @param[in]   func Get tick func ptr
 * @retval      none
 *****************************************************************************/
void KEY_init(tkey_timestamp_func func);

/*****************************************************************************
 * @bref   KEY_scan
 * @param  none
 * @retval Key index
 *****************************************************************************/
uint8_t KEY_scan(void);

/*****************************************************************************
 * @bref   KEY_isPressing
 * @param  none
 * @retval 0 if the key is not pressed
 *****************************************************************************/
__STATIC_INLINE uint32_t KEY_isPressing(void)
{
	return nrf_gpio_pin_read(KEY_PIN);
}


#endif
