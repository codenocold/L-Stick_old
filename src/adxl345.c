#include "adxl345.h"


// ADXL345 REGs-------------------------
#define ADXL345_REG_DEVID           0x00
#define ADXL345_REG_THRESH_TAP      0x1D
#define ADXL345_REG_OFSX            0x1E
#define ADXL345_REG_OFSY            0x1F
#define ADXL345_REG_OFSZ            0x20
#define ADXL345_REG_DUR             0x21
#define ADXL345_REG_LATENT          0x22
#define ADXL345_REG_WINDOW          0x23
#define ADXL345_REG_THRESH_ACT      0x24
#define ADXL345_REG_THRESH_INACT    0x25
#define ADXL345_REG_TIME_INACT      0x26
#define ADXL345_REG_ACT_INACT_CTL   0x27
#define ADXL345_REG_THRESH_FF       0x28
#define ADXL345_REG_TIME_FF         0x29
#define ADXL345_REG_TAP_AXES        0x2A
#define ADXL345_REG_ACT_TAP_STATUS  0x2B
#define ADXL345_REG_BW_RATE         0x2C
#define ADXL345_REG_POWER_CTL       0x2D
#define ADXL345_REG_INT_ENABLE      0x2E
#define ADXL345_REG_INT_MAP         0x2F
#define ADXL345_REG_INT_SOURCE      0x30
#define ADXL345_REG_DATA_FORMAT     0x31
#define ADXL345_REG_DATAX0          0x32
#define ADXL345_REG_DATAX1          0x33
#define ADXL345_REG_DATAY0          0x34
#define ADXL345_REG_DATAY1          0x35
#define ADXL345_REG_DATAZ0          0x36
#define ADXL345_REG_DATAZ1          0x37
#define ADXL345_REG_FIFO_CTL        0x38
#define ADXL345_REG_FIFO_STATUS     0x39


//----------------------------------------------------------------------------------
// Varable Definitions
volatile tGSensor gGSensor = {.isNew = false};
NRF_TWI_MNGR_DEF(m_nrf_twi_mngr, 5, 0);


//----------------------------------------------------------------------------------
// Function Declarations
static void twi_init(void);
static void twi_callback(ret_code_t result, void * p_user_data);
static int32_t reg_write(uint8_t reg, uint8_t val);
static int32_t reg_read(uint8_t Reg, uint8_t * pVal);


/*****************************************************************************
 * @bref    Function for initializing the adxl345 G sensor.
 * @param   none
 * @retval   0: Success
             -: Error
*****************************************************************************/
int32_t ADXL345_init(void)
{
    int32_t ret;
    uint8_t val;

    // Initialize I2C
    twi_init();

    // Initialize INT1 PIN
    nrf_gpio_cfg_input(ADXL345_INT1_PIN, NRF_GPIO_PIN_NOPULL);

    // Check Device ID
    ret = reg_read(ADXL345_REG_DEVID, &val);
    if(ret != 0){
        // I2C Error
        return (-1);
    }
    if(val != 0xE5){
        // Device ID not match
        return (-2);
    }

    // Set normal mode; Set data rate 400Hz
    ret = reg_write(ADXL345_REG_BW_RATE, 0x0C);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Disable selftest; INT high; Full resolution; Data right align; range +-16G
    ret = reg_write(ADXL345_REG_DATA_FORMAT, 0x0B);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set three axis access offset (offset = val * 15.6mg)
    ret = reg_write(ADXL345_REG_OFSX, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }
    ret = reg_write(ADXL345_REG_OFSY, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }
    ret = reg_write(ADXL345_REG_OFSZ, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    //-----------------------------------------------------------

    // Set tap threshold (thresh = val * 62.5mg)
    ret = reg_write(ADXL345_REG_THRESH_TAP, 100);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    ret = reg_write(ADXL345_REG_DUR, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    ret = reg_write(ADXL345_REG_LATENT, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }
    ret = reg_write(ADXL345_REG_WINDOW, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    ret = reg_write(ADXL345_REG_TAP_AXES, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    //-----------------------------------------------------------

    // Set active threshold (thresh = val * 62.5mg)
    ret = reg_write(ADXL345_REG_THRESH_ACT, 30);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set inactive threshold (thresh = val * 62.5mg)
    ret = reg_write(ADXL345_REG_THRESH_INACT, 10);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set inactive time threshold (thresh = val * 1sec)
    ret = reg_write(ADXL345_REG_TIME_INACT, 10);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set ACT detect mode; Enable all axis detect
    ret = reg_write(ADXL345_REG_ACT_INACT_CTL, 0xFF);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // //-----------------------------------------------------------

    // Set free fall threshold (thresh = val * 62.5mg recomand 300ms~600ms)
    ret = reg_write(ADXL345_REG_THRESH_FF, 100);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set free fall time threshold (thresh = val * 5ms recomand 100ms~350ms) 
    ret = reg_write(ADXL345_REG_TIME_FF, 100);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // //-----------------------------------------------------------

    // All interupt map to INT1
    ret = reg_write(ADXL345_REG_INT_MAP, 0x00);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Set FIFO bypass mode
    ret = reg_write(ADXL345_REG_FIFO_CTL, 0);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Enable act & inact interupt
    ret = reg_write(ADXL345_REG_INT_ENABLE, 0x18);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Clear Int
    ret = reg_read(ADXL345_REG_INT_SOURCE, &val);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    // Link disable; Disable auto sleep; Start messure
    ret = reg_write(ADXL345_REG_POWER_CTL, 0x08);
    if(ret != 0){
        // I2C Error
        return (-1);
    }

    return (0);
}

/*****************************************************************************
 * @bref      Function for triger access read
 * @param[in] isBlock Is block mode
 * @retval   0: Success
            -1: Error
*****************************************************************************/
int32_t ADXL345_trig_read(bool isBlock)
{
    static uint8_t buf[6];
    static const uint8_t reg = ADXL345_REG_DATAX0;

    static nrf_twi_mngr_transfer_t const transfers[] =
    {
        NRF_TWI_MNGR_WRITE(ADXL345_SLAVE_ADDR, &reg, 1, NRF_TWI_MNGR_NO_STOP),
        NRF_TWI_MNGR_READ (ADXL345_SLAVE_ADDR, buf, 6, 0)
    };
    static nrf_twi_mngr_transaction_t NRF_TWI_MNGR_BUFFER_LOC_IND transaction =
    {
        .callback            = twi_callback,
        .p_user_data         = buf,
        .p_transfers         = transfers,
        .number_of_transfers = sizeof(transfers) / sizeof(transfers[0])
    };

    if(!isBlock){
        if(NRF_SUCCESS != nrf_twi_mngr_schedule(&m_nrf_twi_mngr, &transaction)){
            return (-1);
        }
    }else{
        nrf_drv_twi_config_t const config = {
           .scl                = ADXL345_SCL_PIN,
           .sda                = ADXL345_SDA_PIN,
           .frequency          = NRF_TWI_FREQ_400K,
           .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
           .clear_bus_init     = false,
           .hold_bus_uninit    = false
        };
        nrf_twi_mngr_perform(&m_nrf_twi_mngr, &config, transfers, 2, NULL);
    }

    return (0);
}

/*****************************************************************************
 * @bref    ADXL345 loop func.
 * @param   none
 * @retval  none
*****************************************************************************/
void ADXL345_loop(void)
{
    uint32_t ret;
    static uint8_t cnt = 0;

    if(nrf_gpio_pin_read(ADXL345_INT1_PIN)){
        if(cnt == 0){
            uint8_t val;
            ret = reg_read(ADXL345_REG_INT_SOURCE, &val);
            if(ret != 0){
                return;
            }
            
            NRF_LOG_INFO("INT source: %X", val);
        }

        if(cnt++ > 200){
            cnt = 0;
        }
    }
}

/*****************************************************************************
 * @bref    Function for initializing twi.
 * @param   none
 * @retval  none
*****************************************************************************/
static void twi_init(void)
{
    uint32_t err_code;

    nrf_drv_twi_config_t const config = {
       .scl                = ADXL345_SCL_PIN,
       .sda                = ADXL345_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .clear_bus_init     = false,
       .hold_bus_uninit    = false
    };

    err_code = nrf_twi_mngr_init(&m_nrf_twi_mngr, &config);
    APP_ERROR_CHECK(err_code);
}

/*****************************************************************************
 * @bref    twi read complete callback
 * @param   none
 * @retval  none
*****************************************************************************/
static void twi_callback(ret_code_t result, void * p_user_data)
{
    uint8_t *buf = (uint8_t*)p_user_data;

    int16_t X = (int16_t)(buf[1]<<8 | buf[0]);
    int16_t Y = (int16_t)(buf[3]<<8 | buf[2]);
    int16_t Z = (int16_t)(buf[5]<<8 | buf[4]);

    gGSensor.x = X;
    gGSensor.y = Y;
    gGSensor.z = Z;
    gGSensor.isNew = true;
}

/*****************************************************************************
 * @bref        Function for write reg value
 * @param[in]   reg: Reg addr
 * @param[in]   val: Value
 * @retval       0: Success
                -1: Error
*****************************************************************************/
static int32_t reg_write(uint8_t reg, uint8_t val)
{
    ret_code_t ret;
    uint8_t buf[2];

    buf[0] = reg;
    buf[1] = val;

    nrf_twi_mngr_transfer_t transfers[] =
    {
        NRF_TWI_MNGR_WRITE(ADXL345_SLAVE_ADDR, buf, 2, 0),
    };

    nrf_drv_twi_config_t config = {
       .scl                = ADXL345_SCL_PIN,
       .sda                = ADXL345_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .clear_bus_init     = false,
       .hold_bus_uninit    = false
    };

    ret = nrf_twi_mngr_perform(&m_nrf_twi_mngr, &config, transfers, 1, NULL);
    if(ret != NRF_SUCCESS){
        return (-1);
    }

    return (0);
}

/*****************************************************************************
 * @bref        Function for read reg value
 * @param[in]   Reg: Reg addr
 * @param[in]   pVal: Value ptr
 * @retval       0: Success
                -1: Error
*****************************************************************************/
static int32_t reg_read(uint8_t Reg, uint8_t * pVal)
{
    ret_code_t ret;

    nrf_twi_mngr_transfer_t transfers[] =
    {
        NRF_TWI_MNGR_WRITE(ADXL345_SLAVE_ADDR, &Reg, 1, NRF_TWI_MNGR_NO_STOP),
        NRF_TWI_MNGR_READ (ADXL345_SLAVE_ADDR, pVal, 1, 0)
    };

    nrf_drv_twi_config_t config = {
       .scl                = ADXL345_SCL_PIN,
       .sda                = ADXL345_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .clear_bus_init     = false,
       .hold_bus_uninit    = false
    };

    ret = nrf_twi_mngr_perform(&m_nrf_twi_mngr, &config, transfers, 2, NULL);
    if(ret != NRF_SUCCESS){
        return (-1);
    }

    return (0);
}

