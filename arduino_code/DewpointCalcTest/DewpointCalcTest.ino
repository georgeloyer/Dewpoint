/*******************************************************
 * DewpointCalculator
 * 4/11/2017 
 * Written by George Loyer
 * Writes a table of dewpoints based on ambient t and humidity
 *******************************************************/

#include "DHT.h"
#define DHTPIN 10     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// Connect pin 1 (on the left) of the sensor to +3.3V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#include <Wire.h>
#include "Adafruit_TMP007.h"
// Connect VCC to +3.3V 
// Gnd -> Gnd
// SCL connects to the I2C clock pin.
// SDA connects to the I2C data pin. 

DHT dht(DHTPIN, DHTTYPE);
Adafruit_TMP007 tmp007; //Start with the default i2c address 0x40

float dewpointC(float c, float h);

void setup() {
  Serial.begin(9600);
  Serial.println("Dewpoint Calculator Test");

  // you can also use tmp007.begin(TMP007_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP007_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)
  if (! tmp007.begin()) {
    Serial.println("No thermopile sensor found");
    while (1);
  }

  dht.begin(); 

}

void loop() {
  int t, h;
  float d;

  Serial.println("    10     20      30      40      50      60      70      80      90      100");
  Serial.println("    -----  ------  ------  ------  ------  ------  ------  ------  ------- --------");

  for (t=-10; t< 50; t+=5) {
    Serial.print(t);
    Serial.print("  ");
    for (h=10; h<=100; h+=10) {
      d = dewpointC(t, h);
      Serial.print(d);
      Serial.print("  ");
    }
    Serial.println();
  }
  delay(10000);
}

float dewpointC(float c, float h) {
  float d, es, e;

  es = 0.611*exp(5423.0*(1.0/273.15 - 1.0/(c-273.15)));
  e = h/100.0*es;
  d = (5423.0/(19.854 - log(e/0.611))) + 273.15;
  return d;
}

