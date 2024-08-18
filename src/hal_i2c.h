/**
 * @file hal_i2c.h
 * @author Miguel Manuel Blanco (miguelmanuelblanco@gmail.com)
 * @brief Header File (Fake) - Archivo de cabecera para utiizar FFF.
 *
 * La funcion de este archivo es brindar las estructura de las funciones de un supuesta HAL para el
 * modulo que se quiere probar. La funciones Aqui declaradas estan mockeadas.
 * @version 0.1
 * @date 2024-08-15
 *
 * @copyright Copyright (c) 2024
 */
#ifndef INC_HAL_I2C_H
#define INC_HAL_I2C_H
/* === Inclusion de archivos de cabecera  ====================================================== */
#include "stdint.h"

/* === Typedef declarations ==================================================================== */
typedef enum { HAL_OK, HAL_ERROR, HAL_BUSY } status_t;

/* === Public function declarations ============================================================ */
status_t HAL_I2C_send(uint8_t * data, uint8_t size);

#endif
