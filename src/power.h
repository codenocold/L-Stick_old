#ifndef _POWER_H_
#define _POWER_H_


#include "nrf.h"
#include "app_timer.h"
#include "nrf_drv_saadc.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "systick.h"
#include "state_ctrl.h"
#include "nrf_log.h"
#include "bt.h"


#define USB_DETECT_PIN			3	// input
#define POWER_LOCK_PIN			14	// output
#define CHARG_ENABLE_PIN		9	// output
#define CHARGING_PIN			10	// input
#define CHARGE_COMPLETE_PIN		11	// input


/*****************************************************************************
 * @bref   Function for configuring power module.
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_init(void);

/*****************************************************************************
 * @bref   Function for show battery percentage.
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_indic_battery_percent(void);

/*****************************************************************************
 * @bref   Power ctrl loop
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_loop(void);

/*****************************************************************************
 * @bref   Function for get battery percentage
 * @param  none
 * @retval Battery percent
 *****************************************************************************/
uint8_t POWER_get_battery_percent(void);

__STATIC_INLINE void POWER_lock(void)
{
    nrf_gpio_pin_set(POWER_LOCK_PIN);
}

__STATIC_INLINE void POWER_release(void)
{
    nrf_gpio_pin_clear(POWER_LOCK_PIN);
}

__STATIC_INLINE void POWER_charge_enable(void)
{
    nrf_gpio_pin_set(CHARG_ENABLE_PIN);
}

__STATIC_INLINE void POWER_charge_disable(void)
{
    nrf_gpio_pin_clear(CHARG_ENABLE_PIN);
}

__STATIC_INLINE uint32_t POWER_is_charge_enable(void)
{
    return nrf_gpio_pin_out_read(CHARG_ENABLE_PIN);
}

__STATIC_INLINE uint32_t POWER_is_usb_detect(void)
{
    return nrf_gpio_pin_read(USB_DETECT_PIN);
}

__STATIC_INLINE uint32_t POWER_is_power_locked(void)
{
    return nrf_gpio_pin_out_read(POWER_LOCK_PIN);
}

__STATIC_INLINE uint32_t POWER_is_charging(void)
{
    return nrf_gpio_pin_read(CHARGING_PIN);
}

__STATIC_INLINE uint32_t POWER_is_charg_complete(void)
{
    return nrf_gpio_pin_read(CHARGE_COMPLETE_PIN);
}


#endif
