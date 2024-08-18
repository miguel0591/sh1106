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

sh1106_status_t sh1106_Init(void) {
    uint8_t cmd1[] = {FIRT_PAGE_ADD,      COM_OUT_SCAN_INVER,
                      FIRT_COLUM_ADD_L,   FIRT_COLUM_ADD_H,
                      RE_MAP_SEG_NORMAL,  DISPLAY_NORMAL,
                      RAT_OSC_FREQ_CONF,  0xF0,
                      CHARG_DISCHAR_PERI, 0x22,
                      SET_VCOM_DESEL_LEV, 0x20,
                      SET_DC_DC_CONTROL,  DC_DC_ENABLE,
                      DISPLAY_ON};

    for (uint8_t i = 0; i < sizeof(cmd1); i++) {
        if (sh1106_SendCmd(cmd1[i]) == SH1106_ERROR) {
            return SH1106_ERROR;
        }
    }

    if (SH1106_HEIGHT == 32) {
        uint8_t cmd2[] = {MUX_RATIO_CONFIG, MUX_RATIO_32HEIGHT, PADS_HARD_CONFIG, PADS_HARD_SEQUEN};
        for (uint8_t i = 0; i < sizeof(cmd2); i++) {
            if (sh1106_SendCmd(cmd2[i]) == SH1106_ERROR) {
                return SH1106_ERROR;
            }
        }
    } else if (SH1106_HEIGHT == 64) {
        uint8_t cmd3[] = {MUX_RATIO_CONFIG, MUX_RATIO_64HEIGHT, PADS_HARD_CONFIG,
                          PADS_HARD_ALTERNA};
        for (uint8_t i = 0; i < sizeof(cmd3); i++) {
            if (sh1106_SendCmd(cmd3[i]) == SH1106_ERROR) {
                return SH1106_ERROR;
            }
        }
    } else {
        return SH1106_ERROR;
    }

    if (sh1106_ContrasSet(0x80) == SH1106_ERROR) {
        return SH1106_ERROR;
    }

    if (sh1106_Fill(BLACK) == SH1106_ERROR) {
        return SH1106_ERROR;
    }

    if (sh1106_UpdateScreen() == SH1106_ERROR) {
        return SH1106_ERROR;
    }

    return SH1106_OK;
}

sh1106_status_t sh1106_DrawPixel(uint8_t x, uint8_t y, sh1106_color_t color) {
}
