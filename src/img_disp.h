#ifndef _IMG_DISP_H_
#define _IMG_DISP_H_


#include "nrf.h"
#include "ws2812b.h"
#include "adxl345.h"
#include "dwt.h"
#include "disp.h"
#include "storage.h"


/*****************************************************************************
 * @bref        IMG_DISP_loop
 * @param[in]   pIMG Img data
 * @retval      none
 *****************************************************************************/
void IMG_DISP_loop(tIMG *pIMG);


#endif
