#ifndef _JSCOPE_H_
#define _JSCOPE_H_


#include "nrf.h"
#include "SEGGER_RTT.h"


void JSCOPE_init(void);
void JSCOPE_update(int x, int y, int z);


#endif
