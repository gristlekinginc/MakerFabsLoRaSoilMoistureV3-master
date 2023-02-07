/*******************************************************************************
 * Moisture/T/H LoraWan
 * for Nik Hawks
 *******************************************************************************/
#include <config.h>
#include <helpers.h>
#include <lora.h>

//Public Library for AHT
#include <AHTxx.h>
AHTxx humiditySensor(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

// Lightweight low power library for Arduino, Version: 1.81, Date: 21-01-2020
#include <LowPower.h>

//ADC functions
#include <ADC.h>

// payload to send to TTN gateway
static osjob_t sendjob;


void do_send(osjob_t *j)//Function call at every TX_INTERVAL
{
  uint8_t payload[10];         // payload for TX

  float temperature = 0.0;     // temperature
  float humidity = 0.0;        // humidity
  int moisturePercent = 0; // spoil moisture humidity
  int moistureRaw = 0;         // capacitive sensor
  uint vdd; //device voltage 

  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
  }
  else
  {
    //That's where we call functions to get data

    // read capacitive sensor value
    getADC(&moisturePercent,&moistureRaw);
    
    //get voltage
    vdd=getVDD();

    //Why??
    sensorPowerOff();
    delay(100);
    sensorPowerOn();
    delay(300);

    //Get Humidity sensor
    while (humiditySensor.begin() != true) // for ESP-01 use aht10.begin(0, 2);
    {
      Serial.println(F("AHT1x not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
      delay(3000);
    }

    uint8_t x=0;
    // Get the new temperature and humidity value
    while ((humiditySensor.getStatus() != AHTXX_NO_ERROR) && (x < 10))
    {
      x++;
      delay(300);
    }

    temperature = humiditySensor.readTemperature();
    humidity = humiditySensor.readHumidity();

    if (humidity == 0)
      Serial.println(F("Failed to read from AHT sensor (zero values)!"));

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature) || humidity == AHTXX_ERROR || temperature == AHTXX_ERROR)
    {
      Serial.println(F("Failed to read from AHT sensor (value NaN)!"));
      temperature = 0.0;
      humidity = 0.0;
    }

    // measurement completed, power down sensors
    sensorPowerOff();

#ifdef PRINT
    // Print the results
    Serial.print(F("Temperature: "));
    Serial.print(temperature, 2);
    Serial.print(F(" C\t"));
    Serial.print(F("Humidity: "));
    Serial.print(humidity, 2);
    Serial.println(F("% RH\t"));

    Serial.print(F("Voltage: "));
    Serial.print(getVDD);
    Serial.println(F("mV \t"));

    Serial.print(F("Moisture ADC  : "));
    Serial.print(moisturePercent);
    Serial.println(F("% \t"));

    Serial.print(F("Moisture (raw): "));
    Serial.print(moistureRaw);
    Serial.println(F(" \t"));

#endif


    // PACKING PAYLOAD
    packingPayload(payload, moisturePercent, moistureRaw,  temperature,  humidity,vdd);

    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.println(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void txCycle(){//Done after each completion of TX SEND. See lora.h
    // Use library from https://github.com/rocketscream/Low-Power
    for (int i = 0; i < int(TX_INTERVAL / 8); i++)
    {
      // low power sleep mode
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
    do_send(&sendjob);
}


void setup()
{
  Serial.begin(9600);
  Serial.println(F("Starting"));
  Serial.print(F("Sketch version: "));
  Serial.println(F(SKETCH_VERSION));

  // set control pin for VCC as Output
  pinMode(SENSOR_PWR_PIN, OUTPUT);
  sensorPowerOn();

  delay(200);

  Wire.begin(); // Join I2C bus
  // Check if the AHT10 will acknowledge
  while (humiditySensor.begin() == false)
  {
    Serial.println(F("AHT10 not detected. Please check wiring. Freezing."));
    delay(5000);
    // while (1);
  }
  Serial.println(F("AHT10 acknowledged."));

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  LMIC_selectSubBand(1); // for USA
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void loop()
{
  os_runloop_once();
}
