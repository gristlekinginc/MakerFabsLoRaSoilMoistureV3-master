#if !defined(CONFIG_H)
#define CONFIG_H

#include <Arduino.h>
#include <lmic.h>
#include <Wire.h>
#include <hal/hal.h>
#include <SPI.h>

#define SKETCH_VERSION "NIK: 2023Feb"

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
#define TX_INTERVAL 300 // 1200;


////////////////////////////////
// PIN CONFIG
////////////////////////////////
#define SENSOR_PIN A2 // sensors pin mapping
#define SENSOR_PWR_PIN 5 // select control pin for switching VCC (sensors)
#define PWM_OUT_PIN 9 //Not sure it's used


////////////////////////////////
// LORAWAN CONFIG
////////////////////////////////

// RFM95 pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {2, 6, 7},
};

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8] = {};
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = {};
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = {};
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }


//////////////////////////
// CONSTANT VALUES
//////////////////////////
#define CAP_AIR 880          // capacitive sensor in the value (maximum value)
#define CAP_WATER 560        // capacitive sensor in water value (minimum value)



#endif // CONFIG_H