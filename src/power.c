#include "power.h"


//----------------------------------------------------------------------------------
// Varable Definitions
static volatile uint8_t percentage;

APP_TIMER_DEF(m_adc_timer_id);
nrf_saadc_value_t adc_buf;


//----------------------------------------------------------------------------------
// Function Declarations
static void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event);
static void adc_meas_timeout_handler(void * p_context);
static uint16_t adc_2_millivolt(nrf_saadc_value_t adc);
static uint16_t smooth_filter(uint16_t new);
static uint8_t millivolt_2_percent(const uint16_t mvolts);


/*****************************************************************************
 * @bref   Function for configuring power module.
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_init(void)
{
    ret_code_t err_code;

    nrf_gpio_cfg_input(USB_DETECT_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(CHARGING_PIN, NRF_GPIO_PIN_NOPULL);
    nrf_gpio_cfg_input(CHARGE_COMPLETE_PIN, NRF_GPIO_PIN_NOPULL);

    POWER_release();
    POWER_charge_disable();
    nrf_gpio_cfg_output(POWER_LOCK_PIN);
    nrf_gpio_cfg_output(CHARG_ENABLE_PIN);

    nrf_drv_saadc_config_t saadc_config = NRF_DRV_SAADC_DEFAULT_CONFIG;
    err_code = nrf_drv_saadc_init(&saadc_config, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);
    err_code = nrf_drv_saadc_channel_init(0, &config);
    APP_ERROR_CHECK(err_code);

    // Wait to battery voltage stable
    nrf_delay_ms(5);

    uint16_t mvolts;
    nrf_saadc_value_t adc_val;
    for(int i=0; i<30; i++){
        nrf_drv_saadc_sample_convert(0, &adc_val);
        mvolts = adc_2_millivolt(adc_val);
        mvolts = smooth_filter(mvolts);
        nrf_delay_ms(2);
    }
    percentage = millivolt_2_percent(mvolts);

    // Initialize timer module, making it use the scheduler
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Create timers.
    err_code = app_timer_create(&m_adc_timer_id, APP_TIMER_MODE_REPEATED, adc_meas_timeout_handler);
    APP_ERROR_CHECK(err_code);

    // Start application timers.
    err_code = app_timer_start(m_adc_timer_id, 32768, NULL);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * @bref   Function for show battery percentage.
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_indic_battery_percent(void)
{
    tColor color;
    uint8_t DispNum;

    DISP_add_back(0, 0, 0);
    
    uint8_t percent = POWER_get_battery_percent();
    if(percent < 50){
        color.R = 250 - 5 * percent;
        color.G = 5 * percent;
        color.B = 0;
    }else{
        color.R = 0;
        color.G = 250 - 5 * (percent-50);
        color.B = 5 * (percent-50);
    }

    // Calc disp num
    DispNum = WS2812B_NUMLEDS * percent / 100;
    if(DispNum < 1){
        DispNum = 1;
    }

    for(int i=0; i<DispNum; i++){
        WS2812B_setColorRGB(&color, i, false);
    }

    DISP_update();
}

/*****************************************************************************
 * @bref   Power ctrl loop
 * @param  none
 * @retval none
 *****************************************************************************/
void POWER_loop(void)
{
    static uint32_t tick_usb = 0;
    static uint32_t tick_battery = 0;
    static uint8_t usb_state = 0xFF;
    static uint8_t usb_state_last = 0xFF;

    // USB Detect
    if(SYSTICK_get_ms_since(tick_usb) > 10){
        tick_usb = SYSTICK_get_tick();
        if(POWER_is_usb_detect()){
            usb_state = 1;
            if(usb_state != usb_state_last){
                usb_state_last = usb_state;
                if(POWER_is_power_locked()){
                    STATE_CTRL_set_state(STATE_POWER_ON_CHARG);
                }
            }
        }else{
            usb_state = 0;
            if(usb_state_last == 1){
                usb_state_last = usb_state;
                STATE_CTRL_set_state(STATE_POWER_ON_DISCHARG);
            }
        }
    }

    // Battery voltage check
    if(SYSTICK_get_ms_since(tick_battery) > 1000){
        tick_battery = SYSTICK_get_tick();
        if(usb_state == 1){
            if(percentage == 100){
                if(POWER_is_charge_enable()){
                    POWER_charge_disable();
                    if(!POWER_is_power_locked()){
                        STATE_CTRL_set_state(STATE_POWER_OFF_CHARG_COMPLETE);
                    }
                }
            }else if(percentage < 90){
                if(!POWER_is_charge_enable()){
                    POWER_charge_enable();
                    if(!POWER_is_power_locked()){
                        STATE_CTRL_set_state(STATE_POWER_OFF_CHARGING);
                    }
                }
            }
        }else if(usb_state == 0){
            if(percentage == 0){
                if(POWER_is_power_locked()){
                    STATE_CTRL_set_state(STATE_POWER_DOWN);
                }
            }
        }
    }
}

/*****************************************************************************
 * @bref   Function for get battery percentage
 * @param  none
 * @retval Battery percent
 *****************************************************************************/
uint8_t POWER_get_battery_percent(void)
{
    return percentage;
}

/*****************************************************************************
 * @bref   Function for handling the ADC interrupt.
 * @details  This function will fetch the conversion result from the ADC, convert the value into
 *           percentage and send it to peer.
 * @param  none
 * @retval none
 *****************************************************************************/
static void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
    static uint8_t cnt = 0;
    uint16_t milli_volts;

    if(p_event->type == NRF_DRV_SAADC_EVT_DONE){
        cnt ++;
        if(cnt > 50){
            cnt = 0;
            nrf_drv_saadc_calibrate_offset();
            return;
        }else{
            milli_volts = adc_2_millivolt(p_event->data.done.p_buffer[0]);
            milli_volts = smooth_filter(milli_volts);
            percentage = millivolt_2_percent(milli_volts);
        }
    }else if(p_event->type == NRF_DRV_SAADC_EVT_CALIBRATEDONE){
        NRF_LOG_INFO("ADC Calibrate Done.");
    }
}

/*****************************************************************************
 * @bref        Function for handling the Battery measurement timer timeout.
 * @details     This function will be called each time the battery level measurement timer expires.
 * @param[in]   p_context   Pointer used for passing some arbitrary information (context) from the
 *                          app_start_timer() call to the timeout handler.
 * @retval      none
 *****************************************************************************/
static void adc_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);

    nrf_drv_saadc_buffer_convert(&adc_buf, 1);
    nrf_drv_saadc_sample();
}

static uint16_t adc_2_millivolt(nrf_saadc_value_t adc)
{
    uint16_t milli_volts;

    if(adc < 0){
        adc = 0;
    }

    // Convert to ADC milli voltage
    milli_volts = 3600 * adc / 0xFFF;
    // Convert to Battery milli voltage
    milli_volts = milli_volts * 2;

    return milli_volts;
}

static uint16_t smooth_filter(uint16_t new)
{
    uint16_t milli_volts;
    static uint16_t filtered_millivolts = 0xFFFF;

    if(filtered_millivolts == 0xFFFF){
        filtered_millivolts = new;
    }else{
        milli_volts = (filtered_millivolts * 9 + new) / 10;
        filtered_millivolts = milli_volts;
    }

    return filtered_millivolts;
}

/****************************************************************************************
 * @brief Function for converting the input voltage (in milli volts) into percentage of 3.0 Volts.
 *
 * @details The calculation is based on a linearized version of the battery's discharge
 *           curve. 3.0V returns 100% battery level. The limit for power failure is 2.1V and
 *           is considered to be the lower boundary.
 *
 *           The discharge curve for CR2032 is non-linear. In this model it is split into
 *           4 linear sections:
 *           - Section 1: 4.2V - 3.7V = 100% - 50% (50% drop on 500 mV)
 *           - Section 2: 3.7V - 3.5V = 50% - 15% (35% drop on 200 mV)
 *           - Section 3: 3.5V - 3.3V = 15% - 5% (10% drop on 200 mV)
 *           - Section 4: 3.3V - 2.8V = 5% - 0% (5% drop on 500 mV)
 *
 *           These numbers are by no means accurate. Temperature and
 *           load in the actual application is not accounted for!
 *
 * @param[in] mvolts The voltage in mV
 *
 * @return    Battery level in percent.
***************************************************************************************/
static uint8_t millivolt_2_percent(const uint16_t mvolts)
{
    uint16_t volt;
    uint8_t battery_level;

    int sum = 0;
    for(int i=0; i<16; i++){
        sum += (WS2812B_getColorRGB(i).R + WS2812B_getColorRGB(i).G + WS2812B_getColorRGB(i).B);
    }
    volt = mvolts - (200 * sum / (255*3*16));

    if (volt >= 4200){
        battery_level = 100;
    }else if (volt > 3700){
        battery_level = 100 - ((4200 - volt) * 50) / 500;
    }else if (volt > 3500){
        battery_level = 50 - ((3700 - volt) * 35) / 200;
    }else if (volt > 3300){
        battery_level = 15 - ((3500 - volt) * 10) / 200;
    }else if (volt > 2800){
        battery_level = 5 - ((3300 - volt) * 5) / 500;
    }else{
        battery_level = 0;
    }

    return battery_level;
}
