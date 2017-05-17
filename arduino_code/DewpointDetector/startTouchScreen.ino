/*
 * startTouchScreen()
 * History:
 * 2017-05    gel Original
 * 2017-05-17 gel TODO: change to boolean return value and return true when startup is successful, false when it fails
 *                allowing the calling setup() to gracefully continue working even if TFT is not working. This may need
 *                to return an int and let the return code take one of two negative values for the two failure cases, plus
 *                a third one when both fail.
 *                OR, break this into two start functions one for TS, one for TFT, so that both can return booleans and the
 *                graceful failure can be coded in the setup()
 * 
 */
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
