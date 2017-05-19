/*
 * createSensorTimerEvents()
 * 
 * record the current time and compare to the lastSensorTime 
 * when an event was queued. If millis() rolls over, it's handled
 * because we are doing unsigned long instead of unsigned int
 * math on the millis time base.
 */
void createSensorTimerEvents() {
  rightNow = millis();
  if (rolled) {                             // if we rolled nextSensorTime
    if (rightNow <= nextSensorTime) {      // and rightNow has rolled over
      rolled = false;                       // we're not in rolled state
    }                                       // else we stay in rolled state, and
    return;                                 // return without setting an event
  }
  
  if (rightNow >= nextSensorTime) {         // passed the time increment for sensorTime
    if (DEBUG) {
      Serial.print("millis for sensorTimer: ");
      Serial.print(rightNow);
      Serial.print("  millis for nextSensorTime: ");
      Serial.println(nextSensorTime);
    }
    nextSensorTime = rightNow + sensorTimerSecs*1000;   // set the next sensorTime epoch
    if (nextSensorTime < rightNow) {        // overflow of unsigned long
      rolled = true;
    } 
    gEM.queueEvent(EventManager::kEventUser0, 0);
  }
}
