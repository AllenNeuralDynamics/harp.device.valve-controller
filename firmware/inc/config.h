#ifndef CONFIG_H
#define CONFIG_H


#define UART_TX_PIN (0)
#define HARP_SYNC_RX_PIN (5)
#define HARP_CORE_LED_PIN (2)

#define VALVE_PIN_BASE (6)
#define GPIO_PIN_BASE (22)

#define VALVES_MASK (0x0000FFFF)
#define GPIOS_MASK (0x000000FF)

#define HARP_DEVICE_ID (1406)
#define HW_VERSION_MAJOR (1)
#define HW_VERSION_MINOR (0)
#define HW_ASSEMBLY_VERSION (0)

#define FW_VERSION_MAJOR (0)
#define FW_VERSION_MINOR (0)
#define FW_VERSION_PATCH (0)

#define UNUSED_SERIAL_NUMBER (0)  // Deprecated in favor of R_UUID

// Doesnt work yet:
//#define USBD_MANUFACTURER "The Allen Institute for Neural Dynamics"
//#define USBD_PRODUCT "Harp.Device.Lickety-Split"

#endif // CONFIG_H
