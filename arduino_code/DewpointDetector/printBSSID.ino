void printBSSID(byte bssid[]) {
  if (DEBUG) {
    print2Digits(bssid[5]);
    Serial.print(":");
    print2Digits(bssid[4]);
    Serial.print(":");
    print2Digits(bssid[3]);
    Serial.print(":");
    print2Digits(bssid[2]);
    Serial.print(":");
    print2Digits(bssid[1]);
    Serial.print(":");
    print2Digits(bssid[0]);
  }
}


