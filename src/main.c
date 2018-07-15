#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"

#include "systick.h"
#include "power.h"
#include "ws2812b.h"
#include "key.h"
#include "adxl345.h"
#include "light_mode.h"
#include "bt.h"
#include "storage.h"
#include "disp.h"
#include "jscope.h"
#include "dwt.h"


/*****************************************************************************
 * @bref    Function for initializing the nrf log module.
 * @param   none
 * @retval  none
 *****************************************************************************/
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/*****************************************************************************
 * @bref    Function for initializing the timer module.
 * @param   none
 * @retval  none
 *****************************************************************************/
static void timer_init(void)
{
    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


int main(void)
{
    log_init();
    DWT_init();
    timer_init();
    SYSTICK_init();
    KEY_init(SYSTICK_get_tick);
    POWER_init();
    ADXL345_init();
    WS2812B_init();

    POWER_indic_battery_percent();

    BT_init();

    JSCOPE_init();

    // Enter main loop.
    for (;;){
        NRF_LOG_PROCESS();

        STATE_CTRL_loop();

        POWER_loop();

        switch(KEY_scan()){
            case 1:     // Click
                if(POWER_is_power_locked()){
                    LIGHT_MODE_index_next();
                }
                break;

            case 2:     // Double click
                if(POWER_is_power_locked()){
                    LIGHT_MODE_index_prev();
                }
                break;

            case 3:     // Triple click

                break;

            case 255:   // Long press
                if(POWER_is_power_locked()){
                    // Power off
                    if(BT_is_connected()){
                        BT_disconnect();
                        nrf_delay_ms(30);
                    }
                    if(BT_is_advertising()){
                        BT_advertising_stop();
                    }
                    STATE_CTRL_set_state(STATE_POWER_DOWN);
                }else{
                    // Power on
                    POWER_lock();
                    BT_advertising_start(false);
                    STATE_CTRL_set_state(STATE_POWER_UP);
                }
                break;

            default:
                break;
        }
    }
}
