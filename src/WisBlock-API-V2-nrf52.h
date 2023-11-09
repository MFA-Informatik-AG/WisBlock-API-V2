#ifndef WISBLOCK_API_NRF52_H
#define WISBLOCK_API_NRF52_H

#include <WisBlock-API-V2-common.h>


#if API_DEBUG > 0
#define API_LOG(tag, ...)         \
	do                            \
	{                             \
		if (tag)                  \
			PRINTF("[%s] ", tag); \
		PRINTF(__VA_ARGS__);      \
		PRINTF("\n");             \
	} while (0)
#else
#define API_LOG(...)
#endif

#include <Arduino.h>
#include <LoRaWan-Arduino.h>
#include "wisblock_cayenne.h"
#include <Wire.h>

#include <nrf_nvic.h>

// Main loop stuff
void periodic_wakeup(TimerHandle_t unused);
extern SemaphoreHandle_t g_task_sem;
// extern SoftwareTimer g_task_wakeup_timer;
extern TimerHandle_t g_task_wakeup_timer;

// BLE
#include <bluefruit.h>
void init_ble(void);
BLEService init_settings_characteristic(void);
void restart_advertising(uint16_t timeout);
extern BLECharacteristic g_lora_data;
extern BLEUart g_ble_uart;
extern bool g_ble_uart_is_connected;
extern bool g_enable_ble;

#define api_ble_printf(...)             \
	if (g_ble_uart_is_connected)        \
	{                                   \
		g_ble_uart.printf(__VA_ARGS__); \
	}

#endif