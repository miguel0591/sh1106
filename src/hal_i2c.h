/**
 * @file hal_i2c.h
 * @author Miguel Manuel Blanco (miguelmanuelblanco@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-08-15
 *
 * @copyright Copyright (c) 2024
 */
#ifndef INC_HAL_I2C_H
#define INC_HAL_I2C_H

#include "stdint.h"

typedef enum { HAL_OK, HAL_ERROR, HAL_BUSY } status_t;

status_t HAL_I2C_send(uint8_t * data, uint8_t size);

#endif
