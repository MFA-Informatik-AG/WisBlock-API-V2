#ifdef ARDUINO_ARCH_ESP32

#include "WisBlock-API-V2.h"

/**
 * @brief System reset
 *
 */
void api_reset(void)
{
	esp_restart();
}

/**
 * @brief Waits for a trigger to wake up
 *    On FreeRTOS the trigger is release of a semaphore
 *    On mbed the trigger is an OS signal
 *
 */
void api_wait_wake(void)
{
	// Wait until semaphore is released (FreeRTOS)
	xSemaphoreTake(g_task_sem, portMAX_DELAY);
}

/**
 * @brief Wake up loop task
 *
 * @param reason for wakeup
 */
void api_wake_loop(uint16_t reason)
{
	g_task_event_type |= reason;
	API_LOG("API", "Waking up loop task");

	if (g_task_sem != NULL)
	{
		// Wake up task to send initial packet
		xSemaphoreGive(g_task_sem);
	}
}

/**
 * @brief Initialize LoRa transceiver
 *
 * @return uint32_t result of initialization, 0 = success
 */
uint32_t api_init_lora(void)
{
	return lora_rak13300_init();
}

/**
 * @brief Initialize the timer for frequent sending
 *
 */
void api_timer_init(void)
{
	// Nothing to do for ESP32
}

/**
 * @brief Start the timer for frequent sending
 *
 */
void api_timer_start(void)
{
	g_task_wakeup_timer.attach_ms(g_lorawan_settings.send_repeat_time, periodic_wakeup);
}

/**
 * @brief Stop the timer for frequent sending
 *
 */
void api_timer_stop(void)
{
	g_task_wakeup_timer.detach();
}

/**
 * @brief Restart the time with a new time
 *
 * @param new_time
 */
void api_timer_restart(uint32_t new_time)
{
	// g_task_wakeup_timer.stop();
	api_timer_stop();

	g_task_wakeup_timer.detach();

	if ((g_lorawan_settings.send_repeat_time != 0) && (g_lorawan_settings.auto_join))
	{
		g_task_wakeup_timer.attach_ms(g_lorawan_settings.send_repeat_time, periodic_wakeup);
	}
}


#endif