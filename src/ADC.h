#include <Arduino.h>
#include <helpers.h>

void setupADC()
{
  pinMode(PWM_OUT_PIN, OUTPUT); // digitalWrite(PWM_OUT_PIN, LOW);

  TCCR1A = bit(COM1A0);            // toggle OC1A on Compare Match
  TCCR1B = bit(WGM12) | bit(CS10); // CTC, scale to clock
  OCR1A = 1;                       // compare A register value (5000 * clock speed / 1024).When OCR1A == 1, PWM is 2MHz

  // ADC2  AVCC as reference voltage
  ADMUX = _BV(REFS0) | _BV(MUX1);

  // ADC2  internal 1.1V as ADC reference voltage
  // ADMUX = _BV(REFS1) |_BV(REFS0) | _BV(MUX1);

  // 8  分频
  ADCSRA = _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);
}

// read capacitive sensor value
void getADC(int *mappedValue, int *rawValue)
{
  setupADC();
  sensorPowerOn(); //
  delay(100);
  for (int i = 0; i < 3; i++)
  {
    // start ADC conversion
    ADCSRA |= (1 << ADSC);
    delay(10);

    if ((ADCSRA & 0x40) == 0)
    {
      *rawValue = (ADCH << 8) + ADCL;
      ADCSRA |= 0x40;
#if DEBUG_OUT_ENABLE
      Serial.print(F("ADC:"));
      Serial.println(rawValue);
#endif
    }
    ADCSRA |= (1 << ADIF); // reset as required
    delay(50);
  }
  delay(200);
  *mappedValue = (int)map(*rawValue, CAP_AIR, CAP_WATER, 0, 100);
}

unsigned long getVDD()
{ // Returns device voltage

  // measures voltage by band gap voltage
  unsigned int vdd = 0;

  int x = 0;
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  while (((vdd == 0) && (x <= 10)) || isnan(vdd))
  {
    x++;
    ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    delay(50);             // Wait for Vref to settle
    ADCSRA |= (1 << ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC))
      ;        // wait until done
    vdd = ADC; // Vcc in millivolts
               // mcu dependend calibration
  }
  return 1122475UL / (unsigned long)vdd; // 1126400 = 1.1*1024*1000
}