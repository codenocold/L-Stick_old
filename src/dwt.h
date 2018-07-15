#ifndef _DWT_H_
#define _DWT_H_


#include "nrf.h"
#include "nrf_systick.h"


/*****************************************************************************
 * @bref    DWT_init
 * @param   none
 * @retval  none
 *****************************************************************************/
void DWT_init(void);

static inline uint32_t DWT_get_tick(void){return DWT->CYCCNT;}


#endif
