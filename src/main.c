#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_twi.h"
#include "app_error.h"
#include "nrf_delay.h"

// TWI instance ID
#define TWI_INSTANCE_ID     0

// PMIC I2C Address (7-bit)
#define PMIC_ADDRESS        0x30

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    // Not used in blocking mode
}

void twi_init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_config = {
       .scl                = 27,       // or whichever pin you're using
       .sda                = 26,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}

void pmic_write_register(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi, PMIC_ADDRESS, data, sizeof(data), false);
    APP_ERROR_CHECK(err_code);
}

uint8_t pmic_read_register(uint8_t reg)
{
    uint8_t value;
    ret_code_t err_code;

    // Send the register address
    err_code = nrf_drv_twi_tx(&m_twi, PMIC_ADDRESS, &reg, 1, true); // repeated start
    APP_ERROR_CHECK(err_code);

    // Read the data
    err_code = nrf_drv_twi_rx(&m_twi, PMIC_ADDRESS, &value, 1);
    APP_ERROR_CHECK(err_code);

    return value;
}

int main(void)
{
    twi_init();

    // Example: Write 0x55 to register 0x01
    pmic_write_register(0x01, 0x55);

    nrf_delay_ms(10);

    // Read back from register 0x01
    uint8_t read_val = pmic_read_register(0x01);

    while (true)
    {
        // Breakpoint here or use RTT logging
    }
}
