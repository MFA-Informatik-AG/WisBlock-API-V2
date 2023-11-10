#ifdef ARDUINO_ARCH_RP2040

#include "WisBlock-API-V2.h"
#include "api_functions.h"

/**
 * @brief System reset
 *
 */
void api_reset(void)
{
	NVIC_SystemReset();
}

/**
 * @brief Waits for a trigger to wake up
 *    On FreeRTOS the trigger is release of a semaphore
 *    On mbed the trigger is an OS signal
 *
 */
void api_wait_wake(void)
{
	bool got_signal = false;
	while (!got_signal)
	{
		osEvent event = osSignalWait(0, osWaitForever);

		if (event.status == osEventSignal)
		{
			got_signal = true;
		}
	}
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

	if (loop_thread != NULL)
	{
		osSignalSet(loop_thread, SIGNAL_WAKE);
	}
}

/**
 * @brief Initialize LoRa transceiver
 *
 * @return uint32_t result of initialization, 0 = success
 */
uint32_t api_init_lora(void)
{
	return lora_rak11300_init();
}

/**
 * @brief Initialize the timer for frequent sending
 *
 */
void api_timer_init(void)
{
	g_task_wakeup_timer.oneShot = false;
	g_task_wakeup_timer.ReloadValue = g_lorawan_settings.send_repeat_time;
	TimerInit(&g_task_wakeup_timer, periodic_wakeup);
	TimerSetValue(&g_task_wakeup_timer, g_lorawan_settings.send_repeat_time);
}

/**
 * @brief Start the timer for frequent sending
 *
 */
void api_timer_start(void)
{
	TimerStart(&g_task_wakeup_timer);
}

/**
 * @brief Stop the timer for frequent sending
 *
 */
void api_timer_stop(void)
{
	TimerStop(&g_task_wakeup_timer);
}

/**
 * @brief Restart the time with a new time
 *
 * @param new_time
 */
void api_timer_restart(uint32_t new_time)
{
	TimerStop(&g_task_wakeup_timer);

	if ((g_lorawan_settings.send_repeat_time != 0) && (g_lorawan_settings.auto_join))
	{
		TimerSetValue(&g_task_wakeup_timer, new_time);
		TimerStart(&g_task_wakeup_timer);
	}
}

#endif