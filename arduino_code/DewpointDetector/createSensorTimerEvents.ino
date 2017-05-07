// Create Sensor Timer Events
void createSensorTimerEvents() {
  rightNow = millis();
  if ((rightNow - lastSensorTime) >= sensorTimerSecs*1000) {
    lastSensorTime = rightNow;
    gEM.queueEvent(EventManager::kEventUser0, 0);
  }
}

