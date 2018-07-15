#include "interact_disp.h"


//----------------------------------------------------------------------------------
// Varable Definitions
static uint32_t mTimestamp;
static tColor mColor;


//----------------------------------------------------------------------------------
// Function Declarations


void INTERACT_DISP_reset(void)
{
    mTimestamp = 0;
    DISP_gen_random_color(255, 255, &mColor);
}

/*****************************************************************************
 * @bref    INTERACT_DISP_0
 * @param   none
 * @retval  none
 *****************************************************************************/
void INTERACT_DISP_0(void)
{
	static float accel_filtered = 0;

	if(SYSTICK_get_ms_since(mTimestamp) > 5){
        mTimestamp = SYSTICK_get_tick();
    }else{
        return;
    }

    ADXL345_trig_read(false);

    if(gGSensor.y > 600 || gGSensor.y < -600){
    	DISP_gen_random_color(255, 255, &mColor);
    }

    accel_filtered = ( accel_filtered * 9.0f + (-3 * gGSensor.y) ) / 10.0f;
    if(accel_filtered > 800){
    	accel_filtered = 800;
    }else if(accel_filtered < -800){
    	accel_filtered = -800;
    }

    DISP_add_back(0, 0, 0);
    //DISP_add_point(mColor.R, mColor.G, mColor.B, 20, accel_filtered);
    DISP_update();
}

void INTERACT_DISP_1(void)
{
	uint8_t rand;

	if(SYSTICK_get_ms_since(mTimestamp) > 5){
        mTimestamp = SYSTICK_get_tick();
    }else{
        return;
    }

    ADXL345_trig_read(false);
    
    if(gGSensor.x > 600 || gGSensor.x < -600 || gGSensor.y > 600 || gGSensor.y < -600 || gGSensor.z > 600 || gGSensor.z < -600){
    	rand = DISP_gen_random();

    	DISP_add_back(0, 0, 0);
    	int i = 0;
    	do{
            DISP_gen_random_color(255, 255, &mColor);
	    	WS2812B_setColorRGB(&mColor, i, false);
	    	i++;
    	}while(i<rand%17);
	    DISP_update();
    }
}
