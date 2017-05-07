// Sensor Timer Listener
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

  displayData(h, t, objt, d);

}

