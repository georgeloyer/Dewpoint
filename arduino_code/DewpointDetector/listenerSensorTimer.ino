/*
 * listenerSensorTimer(int, int)
 * 
 * Sensor Timer Listener is a listener on the Event Queue for specific events signalling
 * that it's time for a sensor read out, including display on the TFT screen, writing the
 * data to the IOT service, and logging locally to the SD card
 */
 
void listenerSensorTimer (int eventCode, int eventParam) {
  // read the TMP007 thermopile sensor
  objt = tmp007.readObjTempC();
  if (DEBUG) {
    Serial.print("Object Temperature: "); Serial.print(objt); Serial.print("*C  ");
  }
  diet = tmp007.readDieTempC();
  if (DEBUG) {
    Serial.print("Die Temperature: "); Serial.print(diet); Serial.print("*C  ");
  }
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    if (DEBUG) {
      Serial.println("Error reading temperature!");
    }
  }
  else {
    t = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    if (DEBUG) {
      Serial.println("Error reading temperature!");
    }
  }
  else {
    h = event.relative_humidity;
  }
  d = dewpointC(t, h);

  if (DEBUG) {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Dewpoint temp: ");
    Serial.print(d);
    Serial.println(" *C ");
  } 

  // write data to the TFT display
  displayData(h, t, objt, d);

  // build log struct
  dewlog.fullPathLogFile = fullPathLogFile; 
  dewlog.dateTime = "2107-05-14 10:57:45";
  dewlog.humidity = h;
  dewlog.temperature = t;
  dewlog.mirrorTemp = objt;
  dewlog.dewpointTemp = d;
  dewlog.heaterStatus = false;
   
  // write data to the IOT service
  // sendDataIot(dewlog);

  // write data to the SD card log
  fullPathLogFile = writeToLogFile(dewlog);

  return;
}

