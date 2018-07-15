#include "dynamic_disp.h"


//----------------------------------------------------------------------------------
// Varable Definitions
static uint32_t mTimestamp;
static tColor mColor[4];

static int mTime;
static int mPos;
static int mStartPos;
static int mSpeed;
static float mStartSpeed;
static float mAccel;


//----------------------------------------------------------------------------------
// Function Declarations


void DYNAMIC_DISP_0(uint8_t isFirst)
{
    int t;

    if(isFirst){
        mTime = 0;
        mAccel = +4.8f;
        mStartSpeed = 0;
        mStartPos = 1500;
        DISP_gen_random_color(255, 255, &mColor[0]);
        mTimestamp = SYSTICK_get_tick();
        return;
    }

    if(SYSTICK_get_ms_since(mTimestamp) < 5){
        return;
    }
    mTimestamp = SYSTICK_get_tick();

    // Calc Position from mTime
    mTime ++;
    if (mTime <= 50) {
        t = mTime - 0;
        mPos = mStartPos + (mStartSpeed * t) + mAccel * t * t / 2;
        if(mTime == 50){
            mStartPos = mPos;
            mStartSpeed += mAccel * t;
            mAccel = -4.8f;
        }
    }else if (mTime <= 150) {
        t = mTime - 50;
        mPos = mStartPos + (mStartSpeed * t) + mAccel * t * t / 2;
        if(mTime == 150){
            mStartPos = mPos;
            mStartSpeed += mAccel * t;
            mAccel = +4.8f;
        }
    }else if (mTime < 200) {
        t = mTime - 150;
        mPos = mStartPos + (mStartSpeed * t) + mAccel * t * t / 2;
    }else {
        mTime = 0;
        mAccel = +4.8f;
        mStartSpeed = 0;
        mStartPos = 1500;
        return;
    }

    if(mTime == 1 || mTime == 100){
        DISP_gen_random_color(255, 255, &mColor[0]);
    }

    DISP_add_back(0, 0, 0);
    DISP_add_point(mColor[0].R, mColor[0].G, mColor[0].B, 4000, 500, mPos);
    DISP_update();
}

void DYNAMIC_DISP_1(uint8_t isFirst)
{
    int t;

    if(isFirst){
        mTime = 0;
        mPos = 2500;
        mAccel = +0.5f;
        mSpeed = 0;
        mStartSpeed = 0;
        DISP_gen_random_color(255, 255, &mColor[0]);
        DISP_gen_random_color(255, 255, &mColor[1]);
        DISP_gen_random_color(255, 255, &mColor[2]);
        DISP_gen_random_color(255, 255, &mColor[3]);
        mTimestamp = SYSTICK_get_tick();
        return;
    }

    if(SYSTICK_get_ms_since(mTimestamp) < 5){
        return;
    }
    mTimestamp = SYSTICK_get_tick();

    // Calc Speed from mTime
    mTime ++;
    if (mTime <= 300) {
        t = mTime - 0;
        mSpeed = mStartSpeed + mAccel * t;
        if(mTime == 300){
            mStartSpeed = mSpeed;
            mAccel = -0.5f;
        }
    }else if(mTime <= 600){

    }else if (mTime <= 1200) {
        t = mTime - 600;
        mSpeed = mStartSpeed + mAccel * t;
        if(mTime == 1200){
            mStartSpeed = mSpeed;
            mAccel = +0.5f;
        }
    }else if(mTime <= 1500){

    }else if (mTime < 1800) {
        t = mTime - 1500;
        mSpeed = mStartSpeed + mAccel * t;
    }else {
        mTime = 0;
        mAccel = +0.5f;
        mSpeed = 0;
        mStartSpeed = 0;
        return;
    }

    mPos += mSpeed;
    if(mPos > 7500){
        mPos = 2500;
        mColor[2] = mColor[1];
        mColor[1] = mColor[0];
        mColor[0] = mColor[3];
        DISP_gen_random_color(255, 255, &mColor[3]);
    }
    if(mPos < 2500){
        mPos = 7500;
        mColor[3] = mColor[0];
        mColor[0] = mColor[1];
        mColor[1] = mColor[2];
        DISP_gen_random_color(255, 255, &mColor[2]);
    }

    int pos;
    DISP_add_back(0, 0, 0);
    pos = mPos;
    DISP_add_point(mColor[0].R, mColor[0].G, mColor[0].B, 3000, 500, pos);
    pos = mPos + 5000;
    DISP_add_point(mColor[1].R, mColor[1].G, mColor[1].B, 3000, 500, pos);
    pos = mPos + 10000;
    DISP_add_point(mColor[2].R, mColor[2].G, mColor[2].B, 3000, 500, pos);
    pos = mPos + 15000;
    if(pos > 17500){
        pos -= 20000;
    }
    DISP_add_point(mColor[3].R, mColor[3].G, mColor[3].B, 3000, 500, pos);
    DISP_update();
}

void DYNAMIC_DISP_2(uint8_t isFirst)
{
    int t;

    if(isFirst){
        mTime = 0;
        mAccel = +80.0f;
        mStartSpeed = 0;
        DISP_gen_random_color(255, 255, &mColor[0]);
        mTimestamp = SYSTICK_get_tick();
        return;
    }

    if(SYSTICK_get_ms_since(mTimestamp) < 5){
        return;
    }
    mTimestamp = SYSTICK_get_tick();

    mTime ++;
    if(mTime <= 200){
        t = mTime;
        mSpeed = mStartSpeed + mAccel * t;
        if(mTime == 200){
            mStartSpeed = mSpeed;
            mAccel = -80.0f;
        }
    }else if(mTime < 400){
        t = mTime - 200;
        mSpeed = mStartSpeed + mAccel * t;
    }else{
        mTime = 0;
        mAccel = +80.0f;
        mStartSpeed = 0;
        DISP_gen_random_color(255, 255, &mColor[0]);
    }

    DISP_add_back(0, 0, 0);
    DISP_add_point(mColor[0].R, mColor[0].G, mColor[0].B, mSpeed, 500, 7500);
    DISP_update();
}

void DYNAMIC_DISP_3(uint8_t isFirst)
{
    if(isFirst){
        mTime = 0;
        DISP_gen_random_color(255, 255, &mColor[0]);
        mTimestamp = SYSTICK_get_tick();
        return;
    }

    if(SYSTICK_get_ms_since(mTimestamp) < 15){
        return;
    }
    mTimestamp = SYSTICK_get_tick();

    mTime ++;
    if(mTime >= 360){
        mTime = 0;
    }

    tHSV hsv;
    hsv.H = mTime;
    hsv.S = 255;
    hsv.V = 255;
    DISP_hsv2rgb(&hsv, &mColor[0]);
    DISP_add_back(mColor[0].R, mColor[0].G, mColor[0].B);
    DISP_update();
}

void DYNAMIC_DISP_4(uint8_t isFirst)
{
    if(isFirst){
        DISP_gen_random_color(255, 255, &mColor[0]);
        mTimestamp = SYSTICK_get_tick();
        return;
    }

    if(SYSTICK_get_ms_since(mTimestamp) < 100){
        return;
    }
    mTimestamp = SYSTICK_get_tick();

    DISP_gen_random_color(255, 255, &mColor[0]);
    uint8_t index = DISP_gen_random()%16;
    WS2812B_setColorRGB(&mColor[0], DISP_gen_random()%16, false);
    mColor[0].R = 0;
    mColor[0].G = 0;
    mColor[0].B = 0;
    index --;
    WS2812B_setColorRGB(&mColor[0], index%16, false);
    index += 2;
    if(index > 15){
        index = 0;
    }
    index += 2;
    WS2812B_setColorRGB(&mColor[0], index%16, true);
}
