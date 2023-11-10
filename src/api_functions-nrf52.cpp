#ifdef NRF52_SERIES

#include "WisBlock-API-V2.h"
#include "api_functions.h"

/**
 * @brief System reset
 *
 */
void api_reset(void)
{
	sd_nvic_SystemReset();
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
#ifdef _VARIANT_ISP4520_
	return lora_isp4520_init(SX1262);
#else
	return lora_rak4630_init();
#endif
}

/**
 * @brief Initialize the timer for frequent sending
 *
 */
void api_timer_init(void)
{
	g_task_wakeup_timer = xTimerCreate(NULL, mypdMS_TO_TICKS(g_lorawan_settings.send_repeat_time), true, NULL, periodic_wakeup);
}

/**
 * @brief Start the timer for frequent sending
 *
 */
void api_timer_start(void)
{
	// g_task_wakeup_timer.start();
	if (isInISR())
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xTimerChangePeriodFromISR(g_task_wakeup_timer, mypdMS_TO_TICKS(g_lorawan_settings.send_repeat_time), &xHigherPriorityTaskWoken);
		xTimerStartFromISR(g_task_wakeup_timer, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xTimerChangePeriod(g_task_wakeup_timer, mypdMS_TO_TICKS(g_lorawan_settings.send_repeat_time), 0);
		xTimerStart(g_task_wakeup_timer, 0);
	}

}

/**
 * @brief Stop the timer for frequent sending
 *
 */
void api_timer_stop(void)
{
	// g_task_wakeup_timer.stop();
	if (isInISR())
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xTimerStopFromISR(g_task_wakeup_timer, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xTimerStop(g_task_wakeup_timer, 0);
	}

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

	if ((g_lorawan_settings.send_repeat_time != 0) && (g_lorawan_settings.auto_join))
	{
		// g_task_wakeup_timer.setPeriod(new_time);
		// g_task_wakeup_timer.start();

		if (isInISR())
		{
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xTimerChangePeriodFromISR(g_task_wakeup_timer, mypdMS_TO_TICKS(new_time), &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		else
		{
			xTimerChangePeriod(g_task_wakeup_timer, mypdMS_TO_TICKS(new_time), 0);
		}
	}
}


#endif