/*
 * listenerSensorTimer(int, int)
 * 
 * Sensor Timer Listener is a listener on the Event Queue for specific events signalling
 * that it's time for a sensor read out, including display on the TFT screen, writing the
 * data to the IOT service, and logging locally to the SD card
 */
 
void listenerSensorTimer (int eventCode, int eventParam) {
  String logLine;
  
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

  heaterStatus = false;

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

  // write data to the IOT service
  // sendDataIot(h, t, objt, d, heaterStatus);
  
  if (sdAvailable) {
    // create the log line
    logLine = String("2107-05-14 10:57:45") + "\t";
    logLine += String(h, 2) + "\t";
    logLine += String(t, 2) + "\t";
    logLine += String(objt, 2) + "\t";
    logLine += String(d, 2) + "\t";
    if (heaterStatus) {
      logLine += String("Heater ON") + "\n";
    } else {
      logLine += String("Heater OFF") + "\n";
    }
     
    // write data to the SD card log
    if (!writeToLogFile(logLine)) {
      if (DEBUG) {
      Serial.println("ERROR: listenerSensorTimer failed to write data to log.");
      }
    }
  } else {
    if (DEBUG) {
      Serial.println("No logging to SD card.");
    }
  }
  
  return;
}

