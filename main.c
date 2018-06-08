/**
 * Copyright (c) 2016 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/** @file
 *
 * @defgroup bootloader_secure main.c
 * @{
 * @ingroup dfu_bootloader_api
 * @brief Bootloader project main file for secure DFU.
 *
 */

#include <stdint.h>
#include "sdk_config.h"
#include "boards.h"
#include "nrf_mbr.h"
#include "nrf_bootloader.h"
#include "nrf_bootloader_app_start.h"
#include "nrf_dfu.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_backend.h"
#include "app_error.h"
#include "app_error_weak.h"
#include "nrf_bootloader_info.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    NRF_LOG_ERROR("received a fault! id: 0x%08x, pc: 0x&08x\r\n", id, pc);
    NVIC_SystemReset();
}

#if 0
void app_error_handler_bare(uint32_t error_code)
{
    (void)error_code;
    NRF_LOG_ERROR("received an error: 0x%08x!\r\n", error_code);
    NVIC_SystemReset();
}
#endif

#if BOOTLOADER_USE_DK_BOARD
/**@brief Function for initialization of LEDs.
 */
static void leds_init(void)
{
    bsp_board_leds_init();
    bsp_board_led_on(BSP_BOARD_LED_2);
}
#endif

/**@brief Function for initializing the button module.
 */
static void buttons_init(void)
{
#if BOOTLOADER_USE_DK_BOARD
    nrf_gpio_cfg_sense_input(BOOTLOADER_BUTTON,
                             BUTTON_PULL,
                             NRF_GPIO_PIN_SENSE_LOW);
#else
	nrf_gpio_cfg_input(BTN_LED_DOWN, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BTN_LED_UP, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(PWR_BUTTON, NRF_GPIO_PIN_PULLUP);

#endif	

}


/**@brief Function for application main entry.
 */
int main(void)
{
    uint32_t ret_val;

    (void) NRF_LOG_INIT(NULL);

    NRF_LOG_INFO("Inside main\r\n");

#if BOOTLOADER_USE_DK_BOARD
    leds_init();
#endif
    buttons_init();

    ret_val = nrf_bootloader_init();
    APP_ERROR_CHECK(ret_val);

    // Either there was no DFU functionality enabled in this project or the DFU module detected
    // no ongoing DFU operation and found a valid main application.
    // Boot the main application.
    nrf_bootloader_app_start(MAIN_APPLICATION_START_ADDR);

    // Should never be reached.
    NRF_LOG_INFO("After main\r\n");
}


static uint32_t dbg_err;
// static char dbg_buf[ 128];
void set_err_code(uint32_t err)
{
	dbg_err = err;
}

void print_error(uint32_t line_num, const uint8_t * p_file_name)
{

	
	nrf_log_frontend_std_2(NRF_LOG_LEVEL_ERROR,
	"ERROR [%s]:%d\r\n", (uint32_t )p_file_name, line_num);
	nrf_log_frontend_std_1(NRF_LOG_LEVEL_ERROR, 
	"err_code=%d\r\n", dbg_err);	
	
while(1);	
//      NRF_LOG_RAW_INFO("ERROR [%s]:%d\r\n", (char *)p_file_name, line_num);


}

/**
 * @}
 */
