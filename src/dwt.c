#include "dwt.h"


//----------------------------------------------------------------------------------
// Varable Definitions


//----------------------------------------------------------------------------------
// Function Declarations


/*****************************************************************************
 * @bref    DWT_init
 * @param   none
 * @retval  none
 *****************************************************************************/
void DWT_init(void)
{
    // Save the current state of the DEMCR register to be able to restore it before exiting
    // this function. Enable the trace and debug blocks (DWT is one of them).
    uint32_t core_debug = CoreDebug->DEMCR;
    CoreDebug->DEMCR = core_debug | CoreDebug_DEMCR_TRCENA_Msk;

    // Save the current state of the CTRL register in DWT block. Make sure
    // that cycle counter is enabled.
    uint32_t dwt_ctrl = DWT->CTRL;
    DWT->CTRL = dwt_ctrl | DWT_CTRL_CYCCNTENA_Msk;
}
