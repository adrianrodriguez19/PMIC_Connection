#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define I2C_NODE DT_NODELABEL(i2c0)  // or i2c1 depending on your board
#define PMIC_ADDR 0x30

const struct device *i2c_dev;

void pmic_write_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    int ret = i2c_write(i2c_dev, buf, sizeof(buf), PMIC_ADDR);
    if (ret < 0) {
        LOG_ERR("Failed to write to PMIC");
    } else {
        LOG_INF("Wrote 0x%02x to reg 0x%02x", val, reg);
    }
}

uint8_t pmic_read_reg(uint8_t reg) {
    uint8_t val = 0;
    int ret;

    ret = i2c_write_read(i2c_dev, PMIC_ADDR, &reg, 1, &val, 1);
    if (ret < 0) {
        LOG_ERR("Failed to read from PMIC");
    } else {
        LOG_INF("Read 0x%02x from reg 0x%02x", val, reg);
    }

    return val;
}

void main(void) {
    i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    pmic_write_reg(0x01, 0x55);
    k_sleep(K_MSEC(10));
    uint8_t read_val = pmic_read_reg(0x01);

    // You can breakpoint here or loop to observe read_val
    while (1) {
        k_sleep(K_SECONDS(1));
    }
}
