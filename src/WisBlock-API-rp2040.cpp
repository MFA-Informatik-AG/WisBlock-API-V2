#ifdef ARDUINO_ARCH_RP2040

#include "WisBlock-API-V2.h"

/** Flag if data flash was initialized */
bool init_flash_done;

/** Loop thread ID */
osThreadId loop_thread = NULL;

/** Timer for periodic sending */
TimerEvent_t g_task_wakeup_timer;

/** Flag for the event type */
volatile uint16_t g_task_event_type = NO_EVENT;

/**
 * @brief Timer event that wakes up the loop task frequently
 *
 * @param unused
 */
void periodic_wakeup(void)
{
#ifndef _CUSTOM_BOARD_
	// Switch on LED to show we are awake
	digitalWrite(LED_GREEN, HIGH);
#endif
	api_wake_loop(STATUS);
}


/**
 * @brief Arduino setup function. Called once after power-up or reset
 *
 */
void setup()
{

#ifndef _CUSTOM_BOARD_
	// Initialize the built in LED
	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

	// Initialize the connection status LED
	pinMode(LED_BLUE, OUTPUT);
	digitalWrite(LED_BLUE, HIGH);
#endif

#if API_DEBUG > 0
	// Initialize Serial for debug output
	Serial.begin(115200);

	time_t serial_timeout = millis();
	// On nRF52840 the USB serial is not available immediately
	while (!Serial)
	{
		if ((millis() - serial_timeout) < 5000)
		{
			delay(100);
#ifndef _CUSTOM_BOARD_
			digitalWrite(LED_GREEN, !digitalRead(LED_GREEN));
#endif
		}
		else
		{
			break;
		}
	}
#endif

#ifndef _CUSTOM_BOARD_
	digitalWrite(LED_GREEN, HIGH);
#endif

	// Call app setup for special settings
	setup_app();

	API_LOG("API", "====================");
	API_LOG("API", "WisBlock API LoRaWAN");
	API_LOG("API", "====================");

	// Initialize background task for Serial port handling
	init_serial_task();

	// Initialize battery reading
	init_batt();

	// Get LoRa parameter
	init_flash();

#ifndef _CUSTOM_BOARD_
	// RAK11310 does not have BLE, switch off blue LED
	digitalWrite(LED_BLUE, LOW);
#endif

	// If P2P mode, override auto join setting
	if (!g_lorawan_settings.lorawan_enable)
	{
		g_lorawan_settings.auto_join = true;
	}

	// Check if auto join is enabled
	if (g_lorawan_settings.auto_join)
	{
		// Initialize LoRa and start join request
		int8_t lora_init_result = 0;
		if (g_lorawan_settings.lorawan_enable)
		{
			API_LOG("API", "Auto join is enabled, start LoRaWAN and join");
			lora_init_result = init_lorawan();
		}
		else
		{
			API_LOG("API", "Auto join is enabled, start LoRa P2P listen");
			lora_init_result = init_lora();
		}

		if (lora_init_result != 0)
		{
			API_LOG("API", "Init LoRa failed");
			API_LOG("API", "Get your LoRa stuff in order");
		}
		else
		{
			API_LOG("API", "LoRa init success");
		}
	}
	else
	{
		// Put radio into sleep mode
		api_init_lora();
		Radio.Sleep();

		API_LOG("API", "Auto join is disabled, waiting for connect command");
		delay(100);
	}

	// Initialize application
	if (!init_app())
	{
		// Without working application we give a warning message
		API_LOG("API", "Get your application stuff in order");
	}
}

/**
 * @brief Arduino loop task. Called in a loop from the FreeRTOS task handler
 *
 */
void loop()
{
	loop_thread = osThreadGetId();

	// Sleep until we are woken up by an event
	api_wait_wake();
	{

#ifndef _CUSTOM_BOARD_
		// Switch on green LED to show we are awake
		digitalWrite(LED_GREEN, HIGH);
#endif
		while (g_task_event_type != NO_EVENT)
		{
			// Application specific event handler (timer event or others)
			app_event_handler();

			if (ble_data_handler != NULL)
			{
				// Handle BLE UART events
				ble_data_handler();
			}

			// Handle LoRa data events
			lora_data_handler();

			// Serial input event
			if ((g_task_event_type & AT_CMD) == AT_CMD)
			{
				g_task_event_type &= N_AT_CMD;
				while (Serial.available() > 0)
				{
					at_serial_input(uint8_t(Serial.read()));
					delay(5);
				}
			}
		}
		// Skip this log message when USB data is received
		if ((g_task_event_type & AT_CMD) == AT_CMD)
		{
			API_LOG("API", "Loop goes to sleep");
		}
		Serial.flush();
		g_task_event_type = 0;

#ifndef _CUSTOM_BOARD_
		// Switch off blue LED to show we go to sleep
		digitalWrite(LED_GREEN, LOW);
		delay(10);
#endif
		// Go back to sleep
	}
}

#endif