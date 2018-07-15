#include "img_disp.h"


//----------------------------------------------------------------------------------
// Varable Definitions


//----------------------------------------------------------------------------------
// Function Declarations
static bool find_peak(uint8_t *pDirec, uint32_t *pPeriod, bool *pIsUp);


/*****************************************************************************
 * @bref        IMG_DISP_loop
 * @param[in]   pIMG Img data
 * @retval      none
 *****************************************************************************/
void IMG_DISP_loop(tIMG *pIMG)
{
    static uint32_t tick_disp = 0;
    static bool isUp;
    static uint8_t Direc;
    static uint32_t Period;
    static uint32_t Column;
    static bool isDispEnd = true;

    if(find_peak(&Direc, &Period, &isUp)){
        if(Direc){           //left->right
            Column = 48-1;
        }else{               //right->left
            Column = 0;
        }
        //Period = (Period - 100) / 48-1;
        Period /= (48-1);
        isDispEnd = false;
    }

    if(!isDispEnd){
        if((DWT_get_tick() - tick_disp) > Period){
            tick_disp = DWT_get_tick();

            if(Column > 48-1){
                DISP_add_back(0, 0, 0);
                DISP_update();
                isDispEnd = true;
            }else{
                // Set LEDs
                if(isUp){
                    for(uint32_t i=0; i<WS2812B_NUMLEDS; i++){
                        WS2812B_setColorRGB(&pIMG->color[Column][i], i, false);
                    }
                }else{
                    for(uint32_t i=0; i<WS2812B_NUMLEDS; i++){
                        WS2812B_setColorRGB(&pIMG->color[Column][i], 15-i, false);
                    }
                }

                // Update LEDs
                DISP_update();

                if(Direc){
                    Column --;
                }else{
                    Column ++;
                }
            }
        }
    }
}

/*****************************************************************************
 * @bref        find_peak
 * @param[out]  pDirec swing direction(1: left->right 0: right->left)
 * @param[out]  pPeriod period in ms
 * @param[out]  pIsUp Hand-held direction
 * @retval      true: Peak find
                false: No Peak
 *****************************************************************************/
static bool find_peak(uint8_t *pDirec, uint32_t *pPeriod, bool *pIsUp)
{
    bool isFind = false;
    uint8_t direc;
    uint32_t period;
    int16_t accel;
    static uint32_t last_tick;
    static uint8_t last_direc;
    static int16_t last_accel;
    static uint8_t cnt = 0;
    static uint32_t LoopTick = 0;

    if((DWT_get_tick() - LoopTick) > 5*64000){
        LoopTick = DWT_get_tick();
    }else{
        return isFind;
    }

    ADXL345_trig_read(false);

    if(!gGSensor.isNew){
        return isFind;
    }
    gGSensor.isNew = false;

    accel = gGSensor.x;
    if(accel - last_accel > 0){
        direc = 0;
    }else{
        direc = 1;
    }
    last_accel = accel;
    if(direc != last_direc){
        if(cnt++ > 5){
            last_direc = direc;
            period = DWT_get_tick() - last_tick - 80*64000;
            last_tick = DWT_get_tick();
            *pPeriod = period;
            if(gGSensor.y < 0){
                *pIsUp = true;
                *pDirec = direc;
            }else{
                *pIsUp = false;
                *pDirec = !direc;
            }
            isFind = true;
        }
    }else{
        cnt = 0;
    }

    return isFind;
}
