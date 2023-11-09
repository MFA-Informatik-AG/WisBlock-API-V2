#ifndef WISBLOCK_API_ESP32_H
#define WISBLOCK_API_ESP32_H

#include <Arduino.h>
#include <LoRaWan-Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include <Preferences.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

// BLE
#include <NimBLEUtils.h>
#include <NimBLEServer.h>
#include <NimBLEDevice.h>
#include <NimBLEAdvertising.h>
#include <ArduinoJson.h>

#include "USB.h"

#include <WisBlock-API-V2-common.h>
#include "wisblock_cayenne.h"

#if API_DEBUG > 0
#define API_LOG(tag, ...)                \
	do                                   \
	{                                    \
		if (tag)                         \
			Serial.printf("[%s] ", tag); \
		Serial.printf(__VA_ARGS__);      \
		Serial.printf("\n");             \
	} while (0)
#else
#define API_LOG(...)
#endif



void periodic_wakeup(void);
extern SemaphoreHandle_t g_task_sem;
// extern SoftwareTimer g_task_wakeup_timer;
extern Ticker g_task_wakeup_timer;



void init_ble(void);
BLEService init_settings_characteristic(void);
void restart_advertising(uint16_t timeout);
extern BLECharacteristic *lora_characteristic;
extern BLECharacteristic *uart_tx_characteristic;
extern bool g_ble_uart_is_connected;
extern bool g_enable_ble;

// WiFi
void init_wifi(void);
void get_wifi_prefs(void);
extern bool g_has_credentials;
extern bool g_conn_status_changed;
extern volatile bool g_wifi_connected;
extern char g_ap_name[];
extern String g_ssid_prim;
extern String g_ssid_sec;
extern String g_pw_prim;
extern String g_pw_sec;



void usb_rx_cb(void);
void stop_ble_adv(void);

#define api_ble_printf(...)                                             \
	if (g_ble_uart_is_connected)                                        \
	{                                                                   \
		char buff[255];                                                 \
		int len = sprintf(buff, __VA_ARGS__);                           \
		uart_tx_characteristic->setValue((uint8_t *)buff, (size_t)len); \
		uart_tx_characteristic->notify(true);                           \
	}

#endif
