#include "state_ctrl.h"


//----------------------------------------------------------------------------------
// Varable Definitions
static tState CurrState = STATE_NULL;
static uint32_t StateCtrlStep;
static uint32_t StateCtrlTick;


//----------------------------------------------------------------------------------
// Function Declarations
static bool power_up_animat(void);
static bool power_down_animat(void);
static bool power_on_charg_animat(void);
static bool power_on_discharg_animat(void);
static void power_off_charg_animat(void);
static void power_off_charg_complete(void);
static bool light_mode_indicate_animat(void);


void STATE_CTRL_loop(void)
{
	switch(CurrState){
        case STATE_NULL:

            break;

		case STATE_POWER_UP:
            if(false == power_up_animat()){
                STORAGE_init();
                STATE_CTRL_set_state(STATE_LIGHT_MODE_INDICATE);
            }
			break;

    	case STATE_POWER_DOWN:
            if(false == power_down_animat()){
                if(POWER_is_usb_detect()){
                    STATE_CTRL_set_state(STATE_POWER_OFF_CHARGING);
                }else{
                    STATE_CTRL_set_state(STATE_NULL);
                }
                STORAGE_deinit();
                POWER_release();
            }
    		break;

        case STATE_POWER_OFF_CHARGING: 
            power_off_charg_animat();
            break;

        case STATE_POWER_OFF_CHARG_COMPLETE:
            power_off_charg_complete();
            break;

        case STATE_POWER_ON_CHARG:
            if(false == power_on_charg_animat()){
                STATE_CTRL_set_state(STATE_LIGHT_MODE);
            }
            break;

        case STATE_POWER_ON_DISCHARG:
            if(false == power_on_discharg_animat()){
                STATE_CTRL_set_state(STATE_LIGHT_MODE);
            }
            break;

    	case STATE_LIGHT_MODE_INDICATE:
            if(false == light_mode_indicate_animat()){
                STATE_CTRL_set_state(STATE_LIGHT_MODE);
            }
    		break;

    	case STATE_LIGHT_MODE:
            LIGHT_MODE_loop(0);
    		break;

    	default:
    		break;
	}
}

tState STATE_CTRL_get_state(void)
{
    return CurrState;
}

void STATE_CTRL_set_state(tState newState)
{
    CurrState = newState;
    StateCtrlStep = 0;
    StateCtrlTick = SYSTICK_get_tick();

    if(CurrState == STATE_LIGHT_MODE){
        LIGHT_MODE_loop(1);
    }
}

static bool power_up_animat(void)
{
    if(StateCtrlStep == 0){
        // Clear LEDs
        DISP_add_back(0, 0, 0);
        DISP_update();

        StateCtrlStep ++;
    }else if(StateCtrlStep <= 255*3){
        if(SYSTICK_get_ms_since(StateCtrlTick) > 5){
            StateCtrlTick = SYSTICK_get_tick();

            tColor color;
            color.R = (StateCtrlStep <= 255) ? StateCtrlStep : 0;
            color.G = (StateCtrlStep > 255 && StateCtrlStep <= 255*2) ? StateCtrlStep%255 : 0;
            color.B = (StateCtrlStep > 255*2 && StateCtrlStep <= 255*3) ? StateCtrlStep%255 : 0;
            for(int i=0; i<WS2812B_NUMLEDS; i++){
                WS2812B_setColorRGB(&color, i, false);
            }
            DISP_update();

            StateCtrlStep += 4;
        }
    }else{
        return false;
    }

    return true;
}

static bool power_down_animat(void)
{
    if(StateCtrlStep == 0){
        // Clear LEDs
        DISP_add_back(200, 200, 200);
        DISP_update();

        StateCtrlStep ++;
    }else if(StateCtrlStep <= 200){
        if(SYSTICK_get_ms_since(StateCtrlTick) > 5){
            StateCtrlTick = SYSTICK_get_tick();

            tColor color;
            color.R = 200 - StateCtrlStep;
            color.G = 200 - StateCtrlStep;
            color.B = 200 - StateCtrlStep;
            for(int i=0; i<WS2812B_NUMLEDS; i++){
                WS2812B_setColorRGB(&color, i, false);
            }
            DISP_update();

            StateCtrlStep ++;
        }
    }else{
        return false;
    }

    return true;
}

static bool power_on_charg_animat(void)
{
    static uint8_t led;
    
    if(StateCtrlStep == 0){
        led = 0;

        // Clear LEDs
        DISP_add_back(0, 0, 0);
        DISP_update();

        StateCtrlStep ++;
    }else{
        if(SYSTICK_get_ms_since(StateCtrlTick) > 5){
            StateCtrlTick = SYSTICK_get_tick();

            tColor color;
            color = WS2812B_getColorRGB(led);
            color.G += 20;
            WS2812B_setColorRGB(&color, led, true);
            if(color.G == 200){
                if(led == WS2812B_NUMLEDS-1){
                    return false;
                }
                led ++;
            }

            StateCtrlStep += 3;
        }
    }

    return true;
}

static bool power_on_discharg_animat(void)
{
    static uint8_t led;

    if(StateCtrlStep == 0){
        led = WS2812B_NUMLEDS-1;

        // Clear LEDs
        DISP_add_back(0, 0, 0);
        DISP_update();

        StateCtrlStep ++;
    }else{
        if(SYSTICK_get_ms_since(StateCtrlTick) > 5){
            StateCtrlTick = SYSTICK_get_tick();

            tColor color;
            color = WS2812B_getColorRGB(led);
            color.G -= 20;
            WS2812B_setColorRGB(&color, led, true);
            if(color.G == 0){
                if(led == 0){
                    return false;
                }
                led --;
            }

            StateCtrlStep += 3;
        }
    }

    return true;
}

static void power_off_charg_animat(void)
{
    static int8_t inc = +1;

    if(StateCtrlStep == 0){
        // Clear LEDs
        DISP_add_back(0, 0, 0);
        DISP_update();

        StateCtrlStep ++;
    }else{
        if(SYSTICK_get_ms_since(StateCtrlTick) > 5){
            StateCtrlTick = SYSTICK_get_tick();

            tColor color;
            color = WS2812B_getColorRGB(0);
            color.R += inc;
            WS2812B_setColorRGB(&color, 0, true);
            if(color.R >= 200){
                inc = -1;
            }else if(color.R <= 20){
                inc = +1;
            }
        }
    }
}

static void power_off_charg_complete(void)
{
    if(StateCtrlStep == 0){
        // Clear LED buff
        DISP_add_back(0, 0, 0);

        // Set LEDs
        tColor color = {.R = 0, .G = 50, .B = 0};
        WS2812B_setColorRGB(&color, 0, false);

        // Update LEDs
        DISP_update();

        StateCtrlStep ++;
    }
}

static bool light_mode_indicate_animat(void)
{
    if(StateCtrlStep == 0){
        // Clear LED buff
        DISP_add_back(0, 0, 0);

        // Set LEDs
        tColor color = {.R = 0, .G = 0, .B = 50};
        for(uint32_t i=0; i<LIGHT_MODE_get_index()+1; i++){
            WS2812B_setColorRGB(&color, i, false);
        }

        // Update LEDs
        DISP_update();

        StateCtrlStep ++;
    }else{
        if(SYSTICK_get_ms_since(StateCtrlTick) > 500){
            return false;
        }
    }

    return true;
}
