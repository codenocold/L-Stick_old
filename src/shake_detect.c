#include "shake_detect.h"


#define SHAKE_THRESHOLD		800


//----------------------------------------------------------------------------------
// Varable Definitions


//----------------------------------------------------------------------------------
// Function Declarations


bool SHAKE_DETECT_is_recognition(void)
{
	static uint32_t timestamp = 0;
	static int last_dis;
	int dis;

	// 10Hz
    if(SYSTICK_get_ms_since(timestamp) > 100){
        timestamp = SYSTICK_get_tick();
    }else{
        return false;
    }

    ADXL345_trig_read(true);

    dis = gGSensor.x + gGSensor.y + gGSensor.z;
    if(abs(dis-last_dis) > SHAKE_THRESHOLD){
        last_dis = dis;
        return true;
    }
    last_dis = dis;

    return false;
}
