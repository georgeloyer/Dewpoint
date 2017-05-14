void startTouchScreen() {
  if (DEBUG) {
    Serial.println("Starting touchscreen controller.");
  }
  if (!ts.begin()) {
    if (DEBUG) {
      Serial.println("Couldn't start touchscreen controller.");
    }
    while (1); 
  }
  if (DEBUG) {
    Serial.println("Touchscreen started.");
    Serial.println("Starting TFT device.");
  }
  tft.begin();
  yield();
  if (DEBUG) {
    Serial.println("Started Touch Screen and Screen Driver.");
  }
}
