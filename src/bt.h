#ifndef _BT_H_
#define _BT_H_


#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_dis.h"
#include "ble_nus.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "peer_manager.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "app_timer.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "key.h"
#include "disp.h"
#include "light_mode.h"


void BT_init(void);
void BT_advertising_start(bool erase_bonds);
void BT_advertising_stop(void);
bool BT_is_advertising(void);
void BT_disconnect(void);
bool BT_is_connected(void);
void BT_nus_write(uint8_t *pData, uint16_t Len);


#endif
