/**
 * @file sh1106.c
 * @author Miguel Manuel Blanco (miguelmanuelblanco@gmail.com)
 * @brief Source File - Driver para SH1106 - Pantalla OLED
 * @version 0.1
 * @date 2024-08-15
 *
 * @copyright Copyright (c) 2024
 */

/* === Inclusion de archivos de cabecera  ====================================================== */
#include "sh1106.h"

/* === Public variable declarations ============================================================ */
uint8_t SH1106_Buffer[BUFFER_SIZE] = {0};
uint8_t SH1106_Buffer1[] = {2, 4, 5, 6};

/* === Public function declarations ============================================================ */
sh1106_status_t sh1106_SendCmd(uint8_t cmd) {
    return (HAL_I2C_send(&cmd, 1));
};

sh1106_status_t sh1106_SendData(uint8_t * data, size_t size) {
    return (HAL_I2C_send(data, size));
};

sh1106_status_t sh1106_ContrasSet(uint8_t contrast) {
    uint8_t cmd[] = {SET_CONSTRAS, contrast};
    for (uint8_t i = 0; i < sizeof(cmd); i++) {
        if (sh1106_SendCmd(i) == SH1106_ERROR) {
            return SH1106_ERROR;
        };
    };
    return HAL_OK;
};

sh1106_status_t sh1106_UpdateScreen(void) {
    for (uint8_t i = 0; i < SH1106_HEIGHT / 8; i++) {
        uint8_t command[] = {(FIRT_PAGE_ADD + i), FIRT_COLUM_ADD_L, FIRT_COLUM_ADD_H};
        for (uint8_t j = 0; j < (sizeof(command)); j++) {
            if (sh1106_SendCmd(command[j]) == SH1106_ERROR) {
                return SH1106_ERROR;
            }
        }
        if (sh1106_SendData(&SH1106_Buffer[SH1106_WHIDTH * i], SH1106_WHIDTH) == SH1106_ERROR) {
            return SH1106_ERROR;
        }
    }
    return SH1106_OK;
};

sh1106_status_t sh1106_Fill(sh1106_color_t color) {
    memset(SH1106_Buffer, (color == BLACK) ? 0x00 : 0xFF, sizeof(SH1106_Buffer));
    return SH1106_OK;
};

sh1106_status_t sh1106_init(void) {
    sh1106_SendCmd(FIRT_PAGE_ADD);      // Nos posicionamos sobre la primera pagina
    sh1106_SendCmd(COM_OUT_SCAN_INVER); // setemos el sentido de los comunes
    // seteamos la primera direccion de las columnas.
    sh1106_SendCmd(FIRT_COLUM_ADD_L);
    sh1106_SendCmd(FIRT_COLUM_ADD_H);
    sh1106_ContrasSet(0x80);
    sh1106_SendCmd(RE_MAP_SEG_NORMAL); // Set segment re-map 0 to 127 - CHECK
    sh1106_SendCmd(DISPLAY_NORMAL);    // los bit 1 encendido bit 0 apagado.
    // Set multiplex ratio.
    sh1106_SendCmd(MUX_RATIO_CONFIG);
    if (32 == SH1106_HEIGHT) {
        sh1106_SendCmd(MUX_RATIO_32HEIGHT);
    } else if (64 == SH1106_HEIGHT) {
        sh1106_SendCmd(MUX_RATIO_64HEIGHT);
    } else {
        return SH1106_ERROR;
    }
    sh1106_SendCmd(RAT_OSC_FREQ_CONF);  // Set display clock divide ratio/oscillator frequency
    sh1106_SendCmd(0xF0);               // f = 150%*fo, ratio = 1
    sh1106_SendCmd(CHARG_DISCHAR_PERI); // Ser pre-charge period
    sh1106_SendCmd(0x22);               // 2 DCLKs precharge, 2 DCLKs discharger
    sh1106_SendCmd(PADS_HARD_CONFIG);   // PADs Configuration
    if (32 == SH1106_HEIGHT) {
        sh1106_SendCmd(PADS_HARD_SEQUEN);
    } else if (64 == SH1106_HEIGHT) {
        sh1106_SendCmd(PADS_HARD_ALTERNA);
    } else {
        return SH1106_ERROR;
    }
    sh1106_SendCmd(SET_VCOM_DESEL_LEV);
    sh1106_SendCmd(0x20);              // 0x20,0.77xVcc
    sh1106_SendCmd(SET_DC_DC_CONTROL); // Configurar DC/DC
    sh1106_SendCmd(DC_DC_ENABLE);      // Activar DC/DC
    sh1106_SendCmd(DISPLAY_ON);        // Encender Display
    sh1106_Fill(BLACK);                // Limpiar Pantalla (buffer)
    sh1106_UpdateScreen();
    return SH1106_OK;
}
