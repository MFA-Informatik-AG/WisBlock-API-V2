/**
 * @file WisBlock-API-V2.h
 * @author Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * @brief Includes and global declarations
 * @version 0.2
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef WISBLOCK_API_H
#define WISBLOCK_API_H

#ifdef NRF52_SERIES
#include <WisBlock-API-V2-nrf52.h>
#endif
#ifdef ARDUINO_ARCH_RP2040 
#include <WisBlock-API-V2-rp2040.h>
#endif
#ifdef ESP32
#include <WisBlock-API-V2-esp32.h>
#endif 

#endif						   // WISBLOCK_API_H
