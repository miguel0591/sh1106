/**
 * @file test_sh1106.c
 * @author Miguel Manuel Blanco (miguelmanuelblanco@gmail.com)
 * @brief Pruebas Formales sobre el driver sh1106
 *
 * En este archivo se detallan todas las pruebas ralizadas para el controlador de pantalla OLED
 * SH1106, la libreria se encuentra actualmente en desarrollo, faltan escribir varias funciones, por
 * lo que se realizaron las pruebas sobre las funciones existentes hasta la fechar. En la siguiente
 * lista puede verse el detalle de las pruebas realiadas y las pruebas pendientes falta de avance en
 * el desarrollo.
 *
 * <b>PRUEBAS REALIZADAS</b>
 * <ul>
 *   <li>Test 1: Enviar un comando cualquiera y que retorne status OK.</li>
 *   <li>Test 2: Enviar un comando cualquiera y que retorne status ERROR.</li>
 *   <li>Test 3: Probar funcion de contraste y que retorno OK.</li>
 *   <li>Test 4: Probar funcion de contraste y que retorno ERROR.</li>
 *   <li>Test 5: Probar la funcion de de actualizacion de pantalla y retorne OK.</li>
 *   <li>Test 6: Probar la funcion de de actualizacion de pantalla y retorne error.</li>
 *   <li>Test 7: Probar la funcion para llenar el buffer (que se carga en la DDRAM) de valores en
 * 0xFF (WHITE) funciona correctamente.</li>
 *   <li>Test 8: Probar la funcion de inicializacion de la pantalla, simulando una
 * inicializacion correcta (status devuelve OK).</li>
 *   <li>Test 9: Probar la funcion de inicializacion de la pantalla, simulando una inicializacion
 * incorrecta (status devuelve ERROR).</li>
 *   <li>Test 10: Probar la funcion para modificar 1 pixel "valido" en el buffer.</li>
 *   <li>Test 11: Probar la funcion para modificar 1 pixel "NO valido" en el buffer.</li>
 * </ul>
 *
 * <b>PRUEBAS PENDIENTES</b>
 * <ul>
 *   <li>Probar funciones pendientes de desarrollo. (ver sh1106.h)</li>
 * </ul>
 * @version 0.1
 * @date 2024-08-15
 * @copyright Copyright (c) 2024
 */

#include "unity.h"
#include "mock_hal_i2c.h"
#include "sh1106.h"

/**
 * @brief Buffer de la librea a probar (variable extern).
 *
 */
extern uint8_t SH1106_Buffer[];

/**
 * @brief Variable para almacenar el estatus de las pruebas.
 *
 */
sh1106_status_t status;

/**
 * @brief Buffer de comparacion para el test 10.
 *
 */
uint8_t buffer_test10[BUFFER_SIZE];

/**
 * @brief Funcion que se ejecuta antes de cada prueba.
 *
 */
void setUp(void) {
    status = 0x03;                         // ponemos el estatus en un valor fuera de lo definido
    memset(SH1106_Buffer, 0, BUFFER_SIZE); // Inicializa el buffer a ceros antes de cada prueba

    memset(buffer_test10, 0, BUFFER_SIZE);
    buffer_test10[4] = 0x10;
}

/**
 * @brief Test 1: Enviar un comando cualquiera y que retorne status OK
 *
 * La prueba tiene una funcion mockeada correspondiente a la comunicacion I2C (que no se probara en
 * este momento), dicha comunicacion retorna un status si se ralizo correctamente, en esta prueba
 * hacemos que dicha funcion retorne 0 (HAL_OK) cada vez que se llame, guardamos en una variable el
 * retorno del comando a probar y luego verificamos si se modifico su valor al valor correcto.
 *
 */
void test_enviar_un_comando_a_el_driver_por_I2C_y_que_retorne_status_ok(void) {
    HAL_I2C_send_fake.return_val = 0;    // valor de retorno status HAL
    status = sh1106_SendCmd(DISPLAY_ON); // almacemos el status para evaluarlo
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 2: Enviar un comando cualquiera y que retorne status ERROR
 *
 * La prueba tiene una funcion mockeada correspondiente a la comunicacion I2C (que no se probara en
 * este momento), dicha comunicacion retorna un status si se ralizo correctamente, en esta prueba
 * hacemos que dicha funcion retorne 1 (HAL_ERROR) cada vez que se llame, guardamos en una variable
 * el retorno del comando a probar y luego verificamos si se modifico su valor al valor correcto.
 */
void test_enviar_un_comando_a_el_driver_por_I2C_y_que_retorne_status_error(void) {
    HAL_I2C_send_fake.return_val = 1;
    status = sh1106_SendCmd(0x01);
    TEST_ASSERT_EQUAL(SH1106_ERROR, status);
}

/**
 * @brief Test 3: Probar funcion de contraste y que retorno OK.
 *
 * La prueba envia el comando de configuracion de contraste y el argumento con la intensidad del
 * contraste deseado. Como se llama a la HAL y queremos indicar un caso donde la comunicacion es
 * correcta, hacemos que la fucion mockeada retorne 0, creamos e incialziamos una variable status en
 * 0x02, despues de aplicar la funcion de contraste chequeamos si se modifico el valor de status. Se
 * corrobora que tenga el valor correspondiente.
 *
 */
void test_probar_la_funcion_de_seteo_de_contraste_y_que_retorno_ok(void) {
    HAL_I2C_send_fake.return_val = 0;
    status = sh1106_ContrasSet(0x80);
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 4: Probar funcion de contraste y que retorno ERROR.
 *
 * La prueba envia el comando de configuracion de contraste y el argumento con la intensidad del
 * contraste deseado. Como se llama a la HAL y queremos indicar un caso donde la comunicacion es
 * erronea, hacemos que la fucion mockeada retorne 1, creamos e incialziamos una variable status en
 * 0x02, despues de aplicar la funcion de contraste chequeamos si se modifico el valor de status. Se
 * corrobora que tenga el valor correspondiente.
 *
 */
void test_probar_la_funcion_de_seteo_de_contraste_y_que_retorno_error(void) {
    HAL_I2C_send_fake.return_val = 1;
    status = sh1106_ContrasSet(0x80);
    TEST_ASSERT_EQUAL(SH1106_ERROR, status);
}

/**
 * @brief Test 5: Probar la funcion de de actualizacion de pantalla y retorne OK.
 *
 * La actualizacion de pantalla va tomando los diferentes partes del buffer y los envia a la DDRAM.
 * El propósito de este test es ver si una vez ejecuatada la funcion retorna el valor de ERROR.
 */
void test_probar_la_funcion_de_actualiacion_de_pantalla_y_que_retorne_ok(void) {
    HAL_I2C_send_fake.return_val = 0;
    status = sh1106_UpdateScreen();
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 6: Probar la funcion de de actualizacion de pantalla y retorne error.
 *
 * La actualizacion de pantalla va tomando los diferentes partes del buffer y los envia a la DDRAM.
 * El propósito de este test es ver si una vez ejecuatada la funcion retorna el valor de ERROR.
 */
void test_probar_la_funcion_de_actualiacion_de_pantalla_y_que_retorne_error(void) {
    HAL_I2C_send_fake.return_val = 1;
    status = sh1106_UpdateScreen();
    TEST_ASSERT_EQUAL(SH1106_ERROR, status);
}

/**
 * @brief Test 7: Probar la funcion para llenar el buffer (que se carga en la DDRAM) de valores en
 * 0xFF (WHITE) funciona correctamente.
 *
 */
void test_llenar_el_buffer_de_valores_correspondientes_a_todos_los_bit_encendidos(void) {
    uint8_t buffer_esperado[BUFFER_SIZE];
    memset(buffer_esperado, 0xFF, BUFFER_SIZE);
    status = sh1106_Fill(WHITE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_esperado, SH1106_Buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 8: Probar la funcion de inicializacion de la pantalla, simulando una inicializacion
 * correcta (status devuelve OK).
 *
 */

void test_probar_la_funcion_init_simulando_una_exito(void) {
    HAL_I2C_send_fake.return_val = 0;
    uint8_t buffer_esperado[BUFFER_SIZE];
    memset(buffer_esperado, 0x00, BUFFER_SIZE);
    status = sh1106_Init();
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_esperado, SH1106_Buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 9: Probar la funcion de inicializacion de la pantalla, simulando una inicializacion
 * incorrecta (status devuelve OERROR).
 */

void test_probar_la_funcion_init_simulando_una_error(void) {
    HAL_I2C_send_fake.return_val = 1;
    uint8_t buffer_esperado[BUFFER_SIZE];
    memset(buffer_esperado, 0x00, BUFFER_SIZE);
    status = sh1106_Init();
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_esperado, SH1106_Buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(SH1106_ERROR, status);
}

/**
 * @brief Test 10: Probar la funcion para modificar 1 pixel "valido" en el buffer.
 */
void test_probar_la_funcion_para_modificar_un_pixel_valido_del_buffer(void) {
    status = sh1106_DrawPixel(4, 4, WHITE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_test10, SH1106_Buffer, BUFFER_SIZE);
    TEST_ASSERT_EQUAL(SH1106_OK, status);
}

/**
 * @brief Test 11: Probar la funcion para modificar 1 pixel "NO valido" en el buffer.
 */
void test_probar_la_funcion_para_modificar_un_pixel_NO_valido_del_buffer(void) {
    status = sh1106_DrawPixel(129, 4, WHITE);
    TEST_ASSERT_EQUAL(SH1106_ERROR, status);
}
