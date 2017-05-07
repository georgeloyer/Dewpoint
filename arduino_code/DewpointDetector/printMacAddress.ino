void printMacAddress() {
  // the MAC address of your WiFi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  if (DEBUG) {
    Serial.print("MAC: ");
    print2Digits(mac[5]);
    Serial.print(":");
    print2Digits(mac[4]);
    Serial.print(":");
    print2Digits(mac[3]);
    Serial.print(":");
    print2Digits(mac[2]);
    Serial.print(":");
    print2Digits(mac[1]);
    Serial.print(":");
    print2Digits(mac[0]);
  }
}


