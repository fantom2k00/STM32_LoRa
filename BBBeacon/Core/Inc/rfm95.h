#pragma once

#include <stdbool.h>
#include "main.h"

#ifndef RFM95_SPI_TIMEOUT
#define RFM95_SPI_TIMEOUT 10
#endif

#ifndef RFM95_WAKEUP_TIMEOUT
#define RFM95_WAKEUP_TIMEOUT 10
#endif

#ifndef RFM95_SEND_TIMEOUT
#define RFM95_SEND_TIMEOUT 100
#endif

#ifndef RFM95_RECEIVE_TIMEOUT
#define RFM95_RECEIVE_TIMEOUT 1000
#endif

#define RFM95_EEPROM_CONFIG_MAGIC 0xab67

typedef struct {

	uint32_t frequency;

} rfm95_channel_config_t;

typedef struct {

	uint16_t magic;
	uint16_t rx_frame_count;
	uint16_t tx_frame_count;
	uint8_t rx1_delay;
	rfm95_channel_config_t channels[16];
	uint16_t channel_mask;

} rfm95_eeprom_config_t;

typedef void (*rfm95_on_after_interrupts_configured)();

typedef bool (*rfm95_load_eeprom_config)(rfm95_eeprom_config_t *config);
typedef void (*rfm95_save_eeprom_config)(const rfm95_eeprom_config_t *config);

typedef uint32_t (*rfm95_get_precision_tick)();
typedef void (*rfm95_precision_sleep_until)(uint32_t ticks_target);

typedef uint8_t (*rfm95_random_int)(uint8_t max);
typedef uint8_t (*rfm95_get_battery_level)();

typedef enum
{
	RFM95_INTERRUPT_DIO0,
	RFM95_INTERRUPT_DIO1,
	RFM95_INTERRUPT_DIO5

} rfm95_interrupt_t;

typedef enum
{
	RFM95_RECEIVE_MODE_NONE,
	RFM95_RECEIVE_MODE_RX1_ONLY,
	RFM95_RECEIVE_MODE_RX12,
} rfm95_receive_mode_t;

#define RFM95_INTERRUPT_COUNT 3

typedef struct {

	SPI_HandleTypeDef *spi_handle;
	GPIO_TypeDef *nss_port;
	uint16_t nss_pin;

	GPIO_TypeDef *nrst_port;
	uint16_t nrst_pin;

	uint8_t device_address[4];
	uint8_t network_session_key[16];
	uint8_t application_session_key[16];

	uint32_t precision_tick_frequency;
	uint32_t precision_tick_drift_ns_per_s;
	rfm95_receive_mode_t receive_mode;
	rfm95_get_precision_tick get_precision_tick;
	rfm95_precision_sleep_until precision_sleep_until;

	rfm95_random_int random_int;
	rfm95_get_battery_level get_battery_level;

	rfm95_load_eeprom_config reload_config;
	rfm95_save_eeprom_config save_config;
	rfm95_on_after_interrupts_configured on_after_interrupts_configured;
	rfm95_eeprom_config_t config;
	volatile uint32_t interrupt_times[RFM95_INTERRUPT_COUNT];

} rfm95_handle_t;

bool rfm95_init(rfm95_handle_t *handle);
bool rfm95_set_power(rfm95_handle_t *handle, int8_t power);
bool rfm95_send_receive_cycle(rfm95_handle_t *handle, const uint8_t *send_data, size_t send_data_length);

void rfm95_on_interrupt(rfm95_handle_t *handle, rfm95_interrupt_t interrupt);
