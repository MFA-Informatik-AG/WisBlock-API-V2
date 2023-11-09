#ifndef WISBLOCK_API_RP2040_H
#define WISBLOCK_API_RP2040_H

#include <Arduino.h>
#include <LoRaWan-Arduino.h>
#include "wisblock_cayenne.h"
#include <Wire.h>

#include <mbed.h>
#include <rtos.h>
#include <multicore.h>
#include <time.h>

#include <WisBlock-API-V2.h>

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


using namespace rtos;
using namespace mbed;
using namespace std::chrono_literals;
using namespace std::chrono;

void periodic_wakeup(void);
extern osThreadId loop_thread;
extern TimerEvent_t g_task_wakeup_timer;

bool init_serial_task(void);

#define api_ble_printf(...) \
	\\ RP2040 does not have BLE


#endif