/*
 * createSensorTimerEvents()
 * 
 * record the current time and compare to the lastSensorTime 
 * when an event was queued. If millis() rolls over, it's handled
 * because we are doing unsigned long instead of unsigned int
 * math on the millis time base.
 */
void createSensorTimerEvents() {
  rightNow = (unsigned long)millis();
  if ((rightNow - nextSensorTime) >= 0) {
    nextSensorTime = rightNow + sensorTimerSecs*1000;
    gEM.queueEvent(EventManager::kEventUser0, 0);
  }
}

