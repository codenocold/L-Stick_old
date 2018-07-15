#include "jscope.h"


#pragma pack(push, 1)
struct {
    signed int Sine1;
    signed int Sine2;
    signed int Sine3;
} acValBuffer;
#pragma pack(pop)


//----------------------------------------------------------------------------------
// Varable Definitions
static char JS_RTT_UpBuffer[4096];    // J-Scope RTT Buffer
static int  JS_RTT_Channel = 1;       // J-Scope RTT Channel


//----------------------------------------------------------------------------------
// Function Declarations


void JSCOPE_init(void)
{
	// Init
    SEGGER_RTT_ConfigUpBuffer(JS_RTT_Channel, "JScope_i4i4i4", &JS_RTT_UpBuffer[0], sizeof(JS_RTT_UpBuffer), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void JSCOPE_update(int x, int y, int z)
{
    // Output
    acValBuffer.Sine1 = x;
    acValBuffer.Sine2 = y;
    acValBuffer.Sine3 = z;
    SEGGER_RTT_Write(JS_RTT_Channel, (const char*)&acValBuffer, sizeof(acValBuffer));
}
