/**
 * @file sh1106.h
 * @author Blanco Miguel Manuel
 * @brief Header File - Driver para SH1106 - Pantalla OLED
 *
 * Este archivo tiene todas las definiciones necesararias para trabajar con la pantalla oled con
 * controlador sh1106. Actualmente solo funcion usando I2C sobre la HAL de STM32, pero se ira
 * haciendo crecer la libreria para funcionar usando otros protocolos de comunicacion, otras
 * plataformas como ESP. Se incorpora el uso de DMA (Acceso Directo a Memoria) en futuras
 * versiones.
 */

#ifndef INC_SH1106_H_
#define INC_SH1106_H_

/* === Inclusion de archivos de cabecera  ====================================================== */
#include "stdint.h"
#include "stddef.h"
#include "hal_i2c.h"

/* === Definicion de los macros publicos ======================================================= */
#define SH1106_WHIDTH (128) ///< @brief Ancho de la pantalla en pixeles
#define SH1106_HEIGHT (64)  ///< @brief Alto de la pantalla en pixeles

/**
 * @brief Tamaño del buffer para almacenar datos de la DDRAM, que se nviaran a la pantalla
 */
#define BUFFER_SIZE (SH1106_WHIDTH * SH1106_HEIGHT / 8)

// Comandos.
#define DISPLAY_ON  (0xAF) ///< @brief Enviar este comando enciende la pantalla
#define DISPLAY_OFF (0xAE) ///< @brief Enviar este comando apaga la pantalla

/**
 * @brief Se usa en la fucion sh1106_ContrasSet para setear el constraste de la pantalla
 */
#define SET_CONSTRAS     (0x81)

#define FIRT_PAGE_ADD    (0xB0) ///< @brief Comando para poscionarse en la pagina 0.
#define FIRT_COLUM_ADD_H (0X10) ///< @brief Bit mas signficativos de la primera columna
#define FIRT_COLUM_ADD_L (0X00) ///< @brief Bit menos signficativos de la primera columna

#define DISPLAY_NORMAL   (0xA6) ///< @brief Los bit en 1 en la DDRAM, son led encendidos
#define DISPLAY_INVERTED (0xA7) ///< @brief Los bit en 1 en la DDRAM, son led apagados

/**
 * @brief Este comando establece la dirección de escaneo de la salida común, lo que permite
 * flexibilidad de diseño en el módulo OLED. Además, la pantalla tendrá un efecto inmediato una vez
 * que se emita este comando. Es decir, si este comando se cambia por COM_OUT_SCAN_INVER la pantalla
 * se invertira verticalmente
 */
#define COM_OUT_SCAN_NORMA (0xC0)

/**
 * @brief Este comando establece la dirección de escaneo de la salida común, lo que permite
 * flexibilidad de diseño en el módulo OLED. Además, la pantalla tendrá un efecto inmediato una vez
 * que se emita este comando. Es decir, si este comando se cambia por COM_OUT_SCAN_NORMA la pantalla
 * se invertira verticalmente
 */
#define COM_OUT_SCAN_INVER (0xC8)

/**
 * @brief La relación entre la dirección de la columna de RAM y el controlador de segmento es
 * Normal, las columnas se cuentas de izquieda a derecha.
 */
#define RE_MAP_SEG_NORMAL (0xA0)

/**
 * @brief La relación entre la dirección de la columna de RAM y el controlador de segmento es
 * Invertido, las columnas se cuentas de derecha a izquierda.
 */
#define RE_MAP_SEG_INVERT (0xA1)

/**
 * @brief Este comando sirve para configurar los del pad de señales COMMON (secuencial
 * o alternativo) para que coincida con el diseño del hardware del panel OLED, este comando se envia
 * seguido de unos los siguientes comando: PADS_HARD_SEQUEN o PADS_HARD_ALTERNA para el modo
 * secuencial o alternativo respectivamente.
 */
#define PADS_HARD_CONFIG  (0xDA)
#define PADS_HARD_SEQUEN  (0x02) ///< @brief Configuracion de pads en modo secuencia.
#define PADS_HARD_ALTERNA (0x12) ///< @brief Configuracion de pads en modo alternativo.

/**
 * @brief Este comando cambia los 64 modos multiplex predeterminados a cualquier relación multiplex
 * de 1 a 64. Los pads de salida COM0-COM63 se cambiarán a la señal común correspondiente.
 */
#define MUX_RATIO_CONFIG   (0xA8)
#define MUX_RATIO_32HEIGHT (0x1F) ///< @brief Modo de multiplex. usado para una pantalla de 32pixel
#define MUX_RATIO_64HEIGHT (0x3F) ///< @brief Modo de multiplex. usado para una pantalla de 64pixel

/**
 * @brief Este comando se utiliza para configurar la frecuencia de los relojes internos del display
 * (DCLK). Se define como la relación de división (valor de 1 a 16) utilizada para dividir la
 * frecuencia del oscilador. Por defecto el driver lo tiene configurado en 1. La frecuencia de trama
 * se determina mediante la relación de división, la cantidad de relojes de visualización por fila,
 * la relación MUX y la frecuencia del oscilador. Seguido de este comando se debe enviar un byte de
 * configuracion donde 4 bits mas significativos corresponden a la relacion de divicion (Ratio
 * Divide)  los 4 bits menos significativos a la frecuencia del oscilador. Para mas informacion ver
 * pag. 25 datasheet SH1106.
 */
#define RAT_OSC_FREQ_CONF (0XD5)

/**
 * @brief Este comando se utiliza para establecer la duración del período de precarga. El intervalo
 * se cuenta en número de DCLK. Por defecto viene configurado en es de 2 DCLK. Este comando debe
 * enviase seguido de un comando de un byte para establecer los valores, dichos varalores se
 * encuentran en pag. 16 del datasheet del sh1106.
 */
#define CHARG_DISCHAR_PERI (0xD9)

/**
 * @brief Este comando sirve para establecer el nivel de voltaje de salida del pad común en la etapa
 * de deselección. Este comando debe enviarse seguido de un comando para seleccionar el nivel
 * deseado, los niveles se encuentan detallados en la pag. 27 del datasheet del 1106.
 */
#define SET_VCOM_DESEL_LEV (0xDB)

/**
 * @brief Este comando sirve para controlar el convertidor de voltaje CC-CC. El convertidor se
 * encenderá emitiendo este comando y luego el comando de encendido de la pantalla. La pantalla del
 * panel debe estar apagada mientras se emite este comando.
 */
#define SET_DC_DC_CONTROL (0xAD)
#define DC_DC_DISABLE     (0x8A) ///< @brief habilita el convertidor DC-DD (Vpp interno).
#define DC_DC_ENABLE      (0x8A) ///< @brief deshabilita el convertidor DC-DD (Vpp externo).

/* === Typedef declarations ==================================================================== */
/**
 * @brief Valor de retorno de las funciones implementadas en este modulo de codigo. Siendo las
 * mismas, OK, ERROR o BUSY.
 *
 */
typedef enum {
    SH1106_OK,    // completo OK
    SH1106_ERROR, // no pudo completar
    SH1106_BUSY   // esta trabajando
} sh1106_status_t;

/**
 * @brief Valores que toman los pixeles en la DDRAM en condiones normales de funcionamiento.
 *
 */
typedef enum {
    BLACK = 0x00, // pixel apagado
    WHITE = 0x01  // pixel encendido
} sh1106_color_t;

/* === Public function declarations ============================================================ */
/**
 * @brief Envia un comando determinado al sh1106.
 * @param cmd - Comando a enviar (ver #defines de sh1106.h).
 * @note Nota 1: para I2C se usa HAL_I2C_Mem_Write de ST, 0x00 es el bit de control con Co=0 D/C=0.
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_SendCmd(uint8_t cmd);

/**
 * @brief Envia un stream de datos a la DDRAM
 * @param data - puntero al buffer de datos a enviar
 * @param size - tamaño de buffer a envair
 * @note Nota 1: para para I2C se usa HAL_I2C_Mem_Write de ST, 0x40 es el bit de control con Co=0
 * D/C=1.
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_SendData(uint8_t * data, size_t size);

/**
 * @brief
 *
 * @param contrast puede tomar valores entre 0 y 255, siendo:
 * <ul>
 *   <li>0 --> valor de contraste minimo.</li>
 *   <li>255 --> valor de contraste maximo.</li>
 * </ul>
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_ContrasSet(uint8_t contrast);

/**
 * @brief Actualizacion de pantalla.
 *
 * Envia de el contenido del buffer (por I2C) a la DDRAM del display. El controlador divide el
 * "alto" de la pantalla en bloques de 8 pixeles (de arriba hacia abajo), cada uno de estos grupo
 * corresponde a una "pagina" y cuando nos posicionamos en una pagina debemos pasar la direccion de
 * la columna (en dos partes) desde la que queremos empezar a escribir. El controlador incrementa la
 * direccion de forma automatica hasta completar el "ancho" de la pantalla seteado, luego de esto
 * debemos pasar a una nueva pagina incrementando en 1 el comando FIRT_PAGE_ADD, se debe setear la
 * direccion de las columnas nuevamente. Este proceso se realiza hasta cubrir todas las paginas del
 * alto seteado.
 *
 * @note Nota 1: Esta version no tiene en cuenta offset que puede existir en el conexionado fisico
 * de los segmentos y el tamaño de la pantalla oled. Si la pantalla conectada al driver es de un
 * ancho menor al ancho maximo del driver, y se encuentra fisicamente conectada a un segmento
 * distiton de 0 (es decir esta despalzada), puede ser que la infomarcion que se vea en pantalla
 * apareza desplazada e incompleta. Esto se solucionara en futuras versiones.
 *
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_UpdateScreen(void);

/**
 * @brief Configuracion de parametros.
 *
 * La funcion de inicializacion, envia de formar secuencial los comandos de configuracion necesario
 * para poder empezar a utilizar el display.
 *
 * <ul>
 *   <li>Indicar la direccion de pagina 0.</li>
 *   <li>Indicar la direccion de columna 0 (comando doble).</li>
 *   <li>Indicar el sentido de incremento de las direcciones de los pines COMMON.</li>
 *   <li>Setear el contraste.</li>
 *   <li>Indicar el sentido de incremento de las direcciones de los pines SEGMENT.</li>
 *   <li>Indicar la relacion entre el estado del pixel y el valor de dato de la DDRAM.</li>
 *   <li>Configurar el Ratio Multiplexacion.</li>
 *   <li>Configurar el divisor de clock y la frecuencia del oscilador interno.</li>
 *   <li>Seterar los periodos de pre-carga y descarga.</li>
 *   <li>Configurar los PADs.</li>
 *   <li>Setear el valor de VCOM.</li>
 *   <li>Habilitar el convertidor DC/DC interno (para no usar Vpp externo).</li>
 *   <li>Encender el display.</li>
 *   <li>Limpiar el buffer donde se cargan los datos a enviar a la DDRAM</li>
 *   <li>Enviar los datos a la pantalla</li>
 * </ul>
 *
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_init(void);

/**
 * @brief LLena la variable buffer con los bits del color correspondiente.
 *
 * @param color: Es el color que se desea usar para rellenar la pantalla, pudiendo ser blanco o
 * negro.
 * @return sh1106_status_t: Estado de la operacion.
 */
sh1106_status_t sh1106_Fill(sh1106_color_t color);
#endif /* INC_SH1106_H_ */
