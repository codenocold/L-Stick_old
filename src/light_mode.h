#ifndef _LIGHT_MODE_H_
#define _LIGHT_MODE_H_


#include "nrf.h"
#include "disp.h"
#include "adxl345.h"
#include "state_ctrl.h"
#include "dynamic_disp.h"
#include "img_disp.h"
#include "bike_disp.h"
#include "interact_disp.h"
#include "storage.h"
#include "shake_detect.h"


typedef enum eLightMode{
    LM_STATIC_LIGHT_0 = 0,
    LM_STATIC_LIGHT_1,
    LM_STATIC_LIGHT_2,
    LM_STATIC_LIGHT_3,
    LM_STATIC_LIGHT_4,
    LM_STATIC_LIGHT_5,
    LM_IMG_DISP_0,
    LM_IMG_DISP_1,
    LM_IMG_DISP_2,
    LM_IMG_DISP_3,
    LM_IMG_DISP_4,
    LM_IMG_DISP_5,

    LM_INTERACT_0,
    LM_INTERACT_1,
    LM_INTERACT_2,
    LM_INTERACT_3,
    LM_INTERACT_4,

    LM_BIKE_0,
    LM_BIKE_1,
    LM_BIKE_2,
    LM_BIKE_3,
    LM_BIKE_4,

    LM_DYNAMIC_0,
    LM_DYNAMIC_1,
    LM_DYNAMIC_2,
    LM_DYNAMIC_3,
    LM_DYNAMIC_4,
    LM_DYNAMIC_5,
    LM_DYNAMIC_6,
    LM_DYNAMIC_7,
    LM_DYNAMIC_8,
    LM_DYNAMIC_9,
    LM_DYNAMIC_10,
    LM_DYNAMIC_11,
    LM_DYNAMIC_12,
    LM_DYNAMIC_13,
    LM_DYNAMIC_14,
    LM_DYNAMIC_15,
    LM_DYNAMIC_16,
    LM_DYNAMIC_17,
    LM_DYNAMIC_18,
    LM_DYNAMIC_19,
    LM_DYNAMIC_20,

    LM_NULL = 0xFF,
}tLightMode;


void LIGHT_MODE_set_index(int32_t index);
int32_t LIGHT_MODE_get_index(void);
void LIGHT_MODE_index_prev(void);
void LIGHT_MODE_index_next(void);
void LIGHT_MODE_loop(uint8_t isNeedInit);


#endif
