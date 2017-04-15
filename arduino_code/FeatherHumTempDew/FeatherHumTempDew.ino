/*
 * FeatherHumTempDew
 * History: 2016-04-09  gel Initial draft code
 * 
 * Description: 
 * This program runs on a Feather Huzzah wifi enabled Arduino platform. Using the Arduino IDE (instead of the ESP 12 Lua platform), 
 * we take humidity and temperature readings for ambient levels from DHT22 module, and remote temperature readings (non-contact) from
 * a TMP007 module. The purpose of the system is to provide an IOT monitor of humiidty, ambient temperature, mirror temperature and 
 * the effective dew point of the environment for the Project 40 telescope.
 * 
 * Based on: 
 * DHTServer - ESP8266 Webserver with a DHT sensor as an input
 * Based on ESP8266Webserver, DHTexample, and BlinkWithoutDelay (thank you)
 * Version 1.0  5/3/2014  Version 1.0   Mike Barela for Adafruit Industries
 * 
 * And:
 *  This is an example for the TMP007 Barometric Pressure & Temp Sensor
 *  Designed specifically to work with the Adafruit TMP007 Breakout 
 *  ----> https://www.adafruit.com/products/2023
 *  These displays use I2C to communicate, 2 pins are required to interface
 *  Adafruit invests time and resources providing this open source code, 
 *  please support Adafruit and open-source hardware by purchasing 
 *  products from Adafruit!
 *  Written by Limor Fried/Ladyada for Adafruit Industries.  
 *  BSD license, all text above must be included in any redistribution
 *  
 *  And:
 *  Adafruit MQTT Library ESP8266 Example
 *  Written by Tony DiCola for Adafruit Industries.
 *  MIT license, all text above must be included in any redistribution
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFiClient.h>
#include <DHT.h>
#include <Wire.h>
#include "Adafruit_TMP007.h"

#define DHTTYPE DHT22
#define DHTPIN  12

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "NETGEAR31"
#define WLAN_PASS       "ayodele7"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "georgel1"
#define AIO_KEY         "bf5ae028f12029bee60d467afe56674c92e1b08d"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
// WiFiClient client;
// or... use WiFiFlientSecure for SSL
WiFiClientSecure client;

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'Temp', 'Humidity', 'Mirror Temp' and 'DewPoint' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char TEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/Temp";
const char HUMIDITY_FEED[] PROGMEM = AIO_USERNAME "/feeds/Humidity";
const char MIRRORTEMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/Mirror Temp";
const char DEWPOINT_FEED[] PROGMEM = AIO_USERNAME "/feeds/DewPoint";
Adafruit_MQTT_Publish temp_feed = Adafruit_MQTT_Publish(&mqtt, TEMP_FEED);
Adafruit_MQTT_Publish humidity_feed = Adafruit_MQTT_Publish(&mqtt, HUMIDITY_FEED);
Adafruit_MQTT_Publish mirrortemp_feed = Adafruit_MQTT_Publish(&mqtt, MIRRORTEMP_FEED);
Adafruit_MQTT_Publish dewpoint_feed = Adafruit_MQTT_Publish(&mqtt, DEWPOINT_FEED);

// Initialize DHT sensor 
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01 
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

Adafruit_TMP007 tmp007(0x40);  // If you are using a different i2c address, it goes here

float humidity, temp_f;  // values read from dht22 sensor
float objtemp, dietemp; // value read from tmp007 sensor
float dewpoint; //calculated value
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 5000;              // interval at which to read sensor
 
 
void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  delay(10);

  Serial.println(F("Project 40 Humidity, Temperature, Mirror Temperature and DewPoint Monitor"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  // Connect to WiFi network
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Write some startup information to the console
  Serial.println("");
  Serial.println("HumTempDew Monitor at Project 40");
  Serial.print("Connected to ");
  Serial.println(WLAN_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // initialize the dht22 humidity and temperature sensor
  dht.begin();           

  // initialize the tmp007 temperature sensor
  // you can also use tmp007.begin(TMP007_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP007_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)
  if (! tmp007.begin()) {
    Serial.println("No TMP007 sensor found");
    while (1);
  }
}
 
void loop(void)
{
  // Now we can publish stuff!
  // Wait at least <interval> seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   

    // connect to MQTT at adafruit.io
    //MQTT_connect();
    
    // read the humidity and temperature in F from DHT22
    gettemperature(); 
  
    // publish the ambient temperature
    Serial.print(F("\nSending Ambient Temperature value "));
    Serial.print(temp_f);
    Serial.print("...");
    //if (! temp_feed.publish(temp_f)) {
    //  Serial.println(F("Failed"));
    //} else {
    //  Serial.println(F("OK!"));
    //}
  
    // publish the humidity
    Serial.print(F("\nSending Humidity value "));
    Serial.print(humidity);
    Serial.print("...");
    //if (! humidity_feed.publish(humidity)) {
    //  Serial.println(F("Failed"));
    //} else {
    //  Serial.println(F("OK!"));
    //}
  
    // read the mirror temperature and chip temperature from tmp007 in F
    getmirrortemp();
    
    // publish the mirror temperature
    Serial.print(F("\nSending Mirror Temperature value "));
    Serial.print(temp_f);
    Serial.print("...");
    //if (! mirrortemp_feed.publish(objtemp)) {
    //  Serial.println(F("Failed"));
    //} else {
    //  Serial.println(F("OK!"));
    //}
  
    // checking die temp against ambient temp 
    if (abs(dietemp - temp_f) > 2.0) {
      Serial.print("Die Temp (");
      Serial.print(dietemp);
      Serial.print(") and Ambient Temp (");
      Serial.print(temp_f);
      Serial.println(") are more than 2.0 degrees apart.");
    }
  
    // publish the dewpoint
    calculatedewpoint();
    Serial.print(F("\nSending Dewpoint Temperature val "));
    Serial.print(dewpoint);
    Serial.print("...");
    //if (! dewpoint_feed.publish(dewpoint)) {
    //  Serial.println(F("Failed"));
    //} else {
    //  Serial.println(F("OK!"));
    //}
  
    // ping the server to keep the mqtt connection alive
    // NOT required if you are publishing once every KEEPALIVE seconds
    //if(! mqtt.ping()) {
    //  mqtt.disconnect();
    //}
  }
} 

// Function to get the mirror temperature and the tmp007 die temperature
void getmirrortemp() {
  dietemp = tmp007.readDieTempC();
  dietemp = dietemp * 9 / 5 + 32;
  objtemp = tmp007.readObjTempC();
  objtemp = objtemp * 9 / 5 + 32;
  return;
}

// Function to calculate dew point temperature based on ambient temperature
// and humidity. If the mirror temperature is below the dew point temperature
// then dew will form on the optical surface.
// This first version uses an approximation that is good for humidity > 50%
void calculatedewpoint() {
  // the approximation uses Celsius
  float temp_c = (temp_f - 32) * 5 / 9;
  dewpoint = temp_c - ((100 - humidity)/5.);
  dewpoint = dewpoint * 9 / 5 + 32;
  return;
}

// Function to get humidity and Farenheit temperature from DHT22
void gettemperature() {
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  humidity = dht.readHumidity();          // Read humidity (percent)
  temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp_f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
