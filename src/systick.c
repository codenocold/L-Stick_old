#include "systick.h"


//----------------------------------------------------------------------------------
// Varable Definitions
volatile uint32_t G_Tick = 0;


//----------------------------------------------------------------------------------
// Function Declarations


/*****************************************************************************
 * @bref    SYSTICK_init
 * @param   none
 * @retval  none
 *****************************************************************************/
void SYSTICK_init(void)
{
    NVIC_SetPriority(SysTick_IRQn, 0);

    nrf_systick_load_set(64000);

    nrf_systick_csr_set(
        NRF_SYSTICK_CSR_CLKSOURCE_CPU |
        NRF_SYSTICK_CSR_TICKINT_ENABLE |
        NRF_SYSTICK_CSR_ENABLE);
}

/**************************************************************************************
 * @bref        Waits for a specific number of mS
 * @param[in]   mS: ms delay
 * @retval      none
**************************************************************************************/
void SYSTICK_wait_ms(uint32_t mS)
{
    uint32_t elapsed    = 0;
    uint32_t last_count = SYSTICK_get_tick();

    for (;;) {
        uint32_t current_count = SYSTICK_get_tick();

        /* measure the time elapsed since the last time we checked */
        elapsed += current_count - last_count;
        last_count = current_count;
        if (elapsed >= mS) {
            break;
        }
    }
}

/*****************************************************************************
 * @bref   SysTick_Handler Callback
 * @param  none
 * @retval none
 *****************************************************************************/
void SysTick_Handler(void)
{
    G_Tick ++;
}
