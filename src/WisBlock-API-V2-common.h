#ifndef WISBLOCK_API_COMMON_H
#define WISBLOCK_API_COMMON_H

#define WISBLOCK_API_VER 2
#define WISBLOCK_API_VER2 0
#define WISBLOCK_API_VER3 10
#ifndef NO_BLE_LED

// Set usage of BLE connection LED (blue). Comment the line to enable LED
#define NO_BLE_LED 1
#endif

// Debug output set to 0 to disable app debug output
#ifndef API_DEBUG
#define API_DEBUG 0
#endif

extern volatile uint16_t g_task_event_type;

/** Wake up events, more events can be defined in app.h */
#define NO_EVENT 0
#define STATUS 0b0000000000000001
#define N_STATUS 0b1111111111111110
#define BLE_CONFIG 0b0000000000000010
#define N_BLE_CONFIG 0b1111111111111101
#define BLE_DATA 0b0000000000000100
#define N_BLE_DATA 0b1111111111111011
#define LORA_DATA 0b0000000000001000
#define N_LORA_DATA 0b1111111111110111
#define LORA_TX_FIN 0b0000000000010000
#define N_LORA_TX_FIN 0b1111111111101111
#define AT_CMD 0b0000000000100000
#define N_AT_CMD 0b1111111111011111
#define LORA_JOIN_FIN 0b0000000001000000
#define N_LORA_JOIN_FIN 0b1111111110111111

/** Wake signal for RAK11310 */
#define SIGNAL_WAKE 0x001

// LoRa
int8_t init_lora(void);
int8_t init_lorawan(bool region_change = false);
int8_t re_init_lorawan(void);
bool send_p2p_packet(uint8_t *data, uint8_t size);
lmh_error_status send_lora_packet(uint8_t *data, uint8_t size, uint8_t fport = 0);

extern bool g_lpwan_has_joined;
extern bool g_rx_fin_result;
extern bool g_join_result;
extern uint32_t otaaDevAddr;

#define LORAWAN_DATA_MARKER 0x55
struct s_lorawan_settings
{
	uint8_t valid_mark_1 = 0xAA;				// Just a marker for the Flash
	uint8_t valid_mark_2 = LORAWAN_DATA_MARKER; // Just a marker for the Flash
												// OTAA Device EUI MSB
	uint8_t node_device_eui[8] = {0x00, 0x0D, 0x75, 0xE6, 0x56, 0x4D, 0xC1, 0xF3};
	// OTAA Application EUI MSB
	uint8_t node_app_eui[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x01, 0xE1};
	// OTAA Application Key MSB
	uint8_t node_app_key[16] = {0x2B, 0x84, 0xE0, 0xB0, 0x9B, 0x68, 0xE5, 0xCB, 0x42, 0x17, 0x6F, 0xE7, 0x53, 0xDC, 0xEE, 0x79};
	// ABP Device Address MSB
	uint32_t node_dev_addr = 0x26021FB4;
	// ABP Network Session Key MSB
	uint8_t node_nws_key[16] = {0x32, 0x3D, 0x15, 0x5A, 0x00, 0x0D, 0xF3, 0x35, 0x30, 0x7A, 0x16, 0xDA, 0x0C, 0x9D, 0xF5, 0x3F};
	// ABP Application Session key MSB
	uint8_t node_apps_key[16] = {0x3F, 0x6A, 0x66, 0x45, 0x9D, 0x5E, 0xDC, 0xA6, 0x3C, 0xBC, 0x46, 0x19, 0xCD, 0x61, 0xA1, 0x1E};
	// Flag for OTAA or ABP
	bool otaa_enabled = true;
	// Flag for ADR on or off
	bool adr_enabled = false;
	// Flag for public or private network
	bool public_network = true;
	// Flag to enable duty cycle
	bool duty_cycle_enabled = false;
	// Default is off
	uint32_t send_repeat_time = 0;
	// Number of join retries
	uint8_t join_trials = 5;
	// TX power 0 .. 10
	uint8_t tx_power = 0;
	// Data rate 0 .. 15 (validity depnends on Region)
	uint8_t data_rate = 3;
	// LoRaWAN class 0: A, 2: C, 1: B is not supported
	uint8_t lora_class = 0;
	// Subband channel selection 1 .. 9
	uint8_t subband_channels = 1;
	// Flag if node joins automatically after reboot
	bool auto_join = false;
	// Data port to send data
	uint8_t app_port = 2;
	// Flag to enable confirmed messages
	lmh_confirm confirmed_msg_enabled = LMH_UNCONFIRMED_MSG;
	// Fixed LoRaWAN lorawan_region (depends on compiler option)
	uint8_t lora_region = 1;
	// Flag for LoRaWAN or LoRa P2P
	bool lorawan_enable = true;
	// Frequency in Hz
	uint32_t p2p_frequency = 916000000;
	// Tx power 0 .. 22
	uint8_t p2p_tx_power = 22;
	// Bandwidth 0: 125, 1: 250, 2: 500, 3: 62.5, 4: 41.67, 5: 31.25, 6: 20.83, 7: 15.63, 8: 10.4, 9: 7.8
	uint8_t p2p_bandwidth = 0;
	// Spreading Factor SF7..SF12
	uint8_t p2p_sf = 7;
	// Coding Rate 1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8
	uint8_t p2p_cr = 1;
	// Preamble length
	uint8_t p2p_preamble_len = 8;
	// Symbol timeout
	uint16_t p2p_symbol_timeout = 0;
	// Command from BLE to reset device
	bool resetRequest = true;
};

// int size = sizeof(s_lorawan_settings);
extern s_lorawan_settings g_lorawan_settings;
extern uint8_t g_rx_lora_data[];
extern uint8_t g_rx_data_len;
extern uint8_t g_tx_lora_data[];
extern uint8_t g_tx_data_len;
extern bool g_lorawan_initialized;
extern int16_t g_last_rssi;
extern int8_t g_last_snr;
enum P2P_RX_MODE
{
	RX_MODE_NONE = 0,
	RX_MODE_RX = 1,
	RX_MODE_RX_TIMED = 2,
	RX_MODE_RX_WAIT = 3
};
extern uint8_t g_lora_p2p_rx_mode;
extern uint32_t g_lora_p2p_rx_time;

#define LORAWAN_COMPAT_MARKER 0x57
struct s_loracompat_settings
{
	uint8_t valid_mark_1 = 0xAA;				  // Just a marker for the Flash
	uint8_t valid_mark_2 = LORAWAN_COMPAT_MARKER; // Just a marker for the Flash

	// Flag if node joins automatically after reboot
	bool auto_join = false;
	// Flag for OTAA or ABP
	bool otaa_enabled = true;
	// OTAA Device EUI MSB
	uint8_t node_device_eui[8] = {0x00, 0x0D, 0x75, 0xE6, 0x56, 0x4D, 0xC1, 0xF3};
	// OTAA Application EUI MSB
	uint8_t node_app_eui[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x01, 0xE1};
	// OTAA Application Key MSB
	uint8_t node_app_key[16] = {0x2B, 0x84, 0xE0, 0xB0, 0x9B, 0x68, 0xE5, 0xCB, 0x42, 0x17, 0x6F, 0xE7, 0x53, 0xDC, 0xEE, 0x79};
	// ABP Network Session Key MSB
	uint8_t node_nws_key[16] = {0x32, 0x3D, 0x15, 0x5A, 0x00, 0x0D, 0xF3, 0x35, 0x30, 0x7A, 0x16, 0xDA, 0x0C, 0x9D, 0xF5, 0x3F};
	// ABP Application Session key MSB
	uint8_t node_apps_key[16] = {0x3F, 0x6A, 0x66, 0x45, 0x9D, 0x5E, 0xDC, 0xA6, 0x3C, 0xBC, 0x46, 0x19, 0xCD, 0x61, 0xA1, 0x1E};
	// ABP Device Address MSB
	uint32_t node_dev_addr = 0x26021FB4;
	// Send repeat time in milliseconds: 2 * 60 * 1000 => 2 minutes
	uint32_t send_repeat_time = 0;
	// Flag for ADR on or off
	bool adr_enabled = false;
	// Flag for public or private network
	bool public_network = true;
	// Flag to enable duty cycle (validity depends on Region)
	bool duty_cycle_enabled = false;
	// Number of join retries
	uint8_t join_trials = 5;
	// TX power 0 .. 15 (validity depends on Region)
	uint8_t tx_power = 0;
	// Data rate 0 .. 15 (validity depends on Region)
	uint8_t data_rate = 3;
	// LoRaWAN class 0: A, 2: C, 1: B is not supported
	uint8_t lora_class = 0;
	// Subband channel selection 1 .. 9
	uint8_t subband_channels = 1;
	// Data port to send data
	uint8_t app_port = 2;
	// Flag to enable confirmed messages
	lmh_confirm confirmed_msg_enabled = LMH_UNCONFIRMED_MSG;
	// Command from BLE to reset device
	bool resetRequest = true;
	// LoRa region
	uint8_t lora_region = 0;
};

// Flash
void init_flash(void);
bool save_settings(void);
void log_settings(void);
void flash_reset(void);
extern bool init_flash_done;

// Battery
void init_batt(void);
float read_batt(void);
uint8_t get_lora_batt(void);
uint8_t mv_to_percent(float mvolts);

// AT command parser
#include "at_cmd.h"
typedef struct atcmd_s
{
	const char *cmd_name;		   // CMD NAME
	const char *cmd_desc;		   // AT+CMD?
	int (*query_cmd)(void);		   // AT+CMD=?
	int (*exec_cmd)(char *str);	   // AT+CMD=value
	int (*exec_cmd_no_para)(void); // AT+CMD
	const char *permission;		   // "R" or "RW"
} atcmd_t;
void at_serial_input(uint8_t cmd);
extern char *region_names[];
extern char g_at_query_buf[];
bool user_at_handler(char *user_cmd, uint8_t cmd_size) __attribute__((weak));
// extern atcmd_t g_user_at_cmd_list[] __attribute__((weak));
extern atcmd_t *g_user_at_cmd_list __attribute__((weak));
extern uint8_t g_user_at_cmd_num __attribute__((weak));
extern bool has_custom_at;

void at_settings(void);

extern char g_ble_dev_name[];

/** Application build time */
const unsigned char app_build_time[] =
	{
		BUILD_DAY_CH0,
		BUILD_DAY_CH1,
		'.',
		BUILD_MONTH_CH0,
		BUILD_MONTH_CH1,
		'.',
		BUILD_YEAR_CH0,
		BUILD_YEAR_CH1,
		BUILD_YEAR_CH2,
		BUILD_YEAR_CH3};

#define WISBLOCK_API_BUILD_TIME app_build_time

// API stuff
void setup_app(void);
bool init_app(void);
void app_event_handler(void);
void ble_data_handler(void) __attribute__((weak));
void lora_data_handler(void);

void api_set_version(uint16_t sw_1 = 1, uint16_t sw_2 = 0, uint16_t sw_3 = 0);
void api_read_credentials(void);
void api_set_credentials(void);
void api_reset(void);
void api_wait_wake(void);
void api_wake_loop(uint16_t reason);
uint32_t api_init_lora(void);
void api_timer_init(void);
void api_timer_start(void);
void api_timer_stop(void);
void api_timer_restart(uint32_t new_time);
void api_log_settings(void);
void api_set_alias(String alias_string);
extern char g_alias[];
extern bool g_has_alias;
extern const char settings_name[];

// Read/Write for WisBlock-API, module independent
enum
{
	WB_FILE_READ = 0,
	WB_FILE_WRITE = 1,
};

extern uint16_t g_sw_ver_1;	   // major version increase on API change / not backwards compatible
extern uint16_t g_sw_ver_2;	   // minor version increase on API change / backward compatible
extern uint16_t g_sw_ver_3;	   // patch version increase on bugfix, no affect on API
extern String g_device_pid;	   // Product PID (if applicable)
extern String g_custom_fw_ver; // Custom firmware version

#endif