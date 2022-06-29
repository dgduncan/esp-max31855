#ifndef _MAX31855_H
#define _MAX31855_H

/**
  * @brief Error states from the max31855
  *
  */
typedef enum {
    ERROR_OK = 0,
    ERROR_OPEN_CIRCUIT,
    ERROR_SHORT_GROUND,
    ERROR_SHORT_VOLTAGE_REFERENCE
} max31855_error_t;

max31855_error_t max31855_read_thermocouple_temperature(float *thermocouple_buffer, float *cold_junction_buffer);
esp_err_t max31855_initialize_device(void);
esp_err_t max31855_configure_spi(void);

#endif