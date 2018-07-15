#include "key.h"


//----------------------------------------------------------------------------------
// Varable Definitions
static tkey_timestamp_func get_ts;


//----------------------------------------------------------------------------------
// Function Declarations


/*****************************************************************************
 * @bref        KEY_init
 * @param[in]   func Get tick func ptr
 * @retval      none
 *****************************************************************************/
void KEY_init(tkey_timestamp_func func)
{
    get_ts = func;

    nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_NOPULL);

    // // Wake up IO init
    // nrf_gpio_cfg_sense_set(KEY_PIN, NRF_GPIO_PIN_SENSE_HIGH);
}

/*****************************************************************************
 * @bref   KEY_scan
 * @param  none
 * @retval Key index
 *****************************************************************************/
uint8_t KEY_scan(void)
{
    uint8_t KeyIndex = 0;
    static bool isPressed = false;
    static bool isWaitRelease = false;
    static uint32_t PressTick;
    static uint32_t ReleseTick;
    static uint8_t PressCnt = 0;

    if(KEY_isPressing()){     // KEY Down
        if(isPressed == false){
            PressTick = get_ts();
            isPressed = true;
        }else{
            if(isWaitRelease == false){
                if(get_ts()-PressTick > 1000){
                    // Long Press
                    KeyIndex = 0xFF;
                    isWaitRelease = true;
                }
            }
        }
    }else{                              // KEY Up
        if(isPressed == true){
            if(get_ts()-PressTick > 1000){
                // Long Press release

            }else{
                if(get_ts()-PressTick > 20){
                    PressCnt ++;
                    ReleseTick = get_ts();
                }
            }

            isPressed = false;
            isWaitRelease = false;
        }
    }

    if(PressCnt != 0){
        if(get_ts()-ReleseTick > 300){
            if(PressCnt == 1){
                KeyIndex = 1;
            }else if(PressCnt == 2){
                KeyIndex = 2;
            }else if(PressCnt == 3){
                KeyIndex = 3;
            }

            PressCnt = 0;
        }
    }

    return KeyIndex;
}
