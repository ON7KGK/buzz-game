/**
 * @file periph_ctrl.h
 * @brief Compatibility shim for ESP-IDF peripheral control
 *
 * This file provides compatibility for Arduino-ESP32 framework version 2.x
 * that doesn't include esp_private headers.
 */

#ifndef _ESP_PRIVATE_PERIPH_CTRL_H_
#define _ESP_PRIVATE_PERIPH_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Include the public peripheral control if available
#include "driver/periph_ctrl.h"

#ifdef __cplusplus
}
#endif

#endif /* _ESP_PRIVATE_PERIPH_CTRL_H_ */
