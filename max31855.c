#include <stdio.h>
#include "max31855.h"

max31855_error_t max31855_check_error(uint8_t error_bits) {
    ESP_LOGI(TAG, "ERROR BITS %d", (error_bits & 7));
    switch (error_bits & 7)
    {
    case 0:
        return ERROR_OK;
    case 1:
        return ERROR_OPEN_CIRCUIT;
    case 2:
        return ERROR_SHORT_GROUND;
    case 4:
        return ERROR_SHORT_VOLTAGE_REFERENCE;
    default:
        ESP_LOGE(TAG, "Weird error bits detected");
        return ERROR_OK;
    }
}

max31855_error_t max31855_read_thermocouple_temperature(float *thermcouple_buffer, float *cold_junction_buffer) {
    spi_transaction_t t = {
        .cmd = 0x0000,
        .rxlength = 32,
        .flags = SPI_TRANS_USE_RXDATA,
    };
    esp_err_t err = spi_device_polling_transmit(spi, &t);
    if (err!= ESP_OK) return err;

    max31855_error_t check_err = max31855_check_error(t.rx_data[3]);
    if (check_err != ERROR_OK) {
        return check_err;
    }

    uint16_t wd = ((uint16_t)t.rx_data[0] << 8) | t.rx_data[1];
    if(t.rx_data[0] >> 7) {
        wd = ~wd;
        *thermcouple_buffer = (float)-((wd >> 2) / 4.000000);
    } else {
        *thermcouple_buffer = (float)((wd >> 2) / 4.000000);
    }

    uint16_t wd2 = ((uint16_t)t.rx_data[2] << 8) | t.rx_data[3];
    if(t.rx_data[2] >> 7) {
        wd2 = ~wd2;
        *cold_junction_buffer = (float)-((wd2 >> 4) / 16.0000);
    } else {
        *cold_junction_buffer = (float)((wd2 >> 4) / 16.0000);
    }

    return ERROR_OK;
}