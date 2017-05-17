/*
 * startTMP007()
 * History:
 * 2017-05-12 gel Original
 * 2017-05-16 gel TODO: make it boolean and return true on correct startup, false on failure
 *                allow calling program to gracefully continue working, while reporting failure 
 *                of the sensor. Also could re-try multiple times in order to self-heal.
 * 
 * starts up the thermopile TMP007 sensor. Hangs if it fails to start up.
 */
void startTMP007() { 
  // you can also use tmp007.begin(TMP007_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP007_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)
  if (DEBUG) {
    Serial.println("Starting thermopile tmp007 sensor.");
  }
  if (! tmp007.begin()) {
    if (DEBUG) {
      Serial.println("No thermopile sensor found");
    }
    while (1);
  }
  if (DEBUG) {
    Serial.println("Thermopile tmp007 sensor started.");
  }
}
