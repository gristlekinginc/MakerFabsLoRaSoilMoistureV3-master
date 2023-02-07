#if !defined(HELPERS_H)
#define HELPERS_H

#include <Arduino.h>
#include <config.h>

// switch VCC on (sensors on)
void sensorPowerOn(void)
{
  digitalWrite(SENSOR_PWR_PIN, HIGH); // Sensor power on
}

// switch VCC off (sensor off)
void sensorPowerOff(void)
{
  digitalWrite(SENSOR_PWR_PIN, LOW); // Sensor power off
}

void printHex2(unsigned v)
{
  v &= 0xff;
  if (v < 16){
    Serial.print('0');
  }
  Serial.print(v, HEX);
}

void packingPayload(uint8_t payload[10], int moisturePercent, int moistureRaw, float temperature, float humidity, int vdd){
    // prepare payload for TX
    byte csmLow = lowByte(moisturePercent);
    byte csmHigh = highByte(moisturePercent);
    // place the bytes into the payload
    payload[0] = csmLow;
    payload[1] = csmHigh;

    // float -> int
    // note: this uses the sflt16 datum (https://github.com/mcci-catena/arduino-lmic#sflt16)
    // used range for mapping type float to int:  -1...+1, -> value/100
    uint16_t payloadTemp = 0;
    if (temperature != 0)
      payloadTemp = LMIC_f2sflt16(temperature / 100);
    // int -> bytes
    byte tempLow = lowByte(payloadTemp);
    byte tempHigh = highByte(payloadTemp);
    // place the bytes into the payload
    payload[2] = tempLow;
    payload[3] = tempHigh;

    // used range for mapping type float to int:  -1...+1, -> value/100
    uint16_t payloadHumid = 0;
    if (humidity != 0)
      payloadHumid = LMIC_f2sflt16(humidity / 100);
    // int -> bytes
    byte humidLow = lowByte(payloadHumid);
    byte humidHigh = highByte(payloadHumid);
    payload[4] = humidLow;
    payload[5] = humidHigh;

    // int -> bytes
    byte battLow = lowByte(vdd);
    byte battHigh = highByte(vdd);
    payload[6] = battLow;
    payload[7] = battHigh;

    // prepare payload for TX
    byte svLow = lowByte(moistureRaw);
    byte svHigh = highByte(moistureRaw);
    // place the bytes into the payload
    payload[8] = svLow;
    payload[9] = svHigh;
}

#endif // HELPERS_H