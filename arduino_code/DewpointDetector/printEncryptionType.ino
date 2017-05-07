void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      if (DEBUG) {
        Serial.print("WEP");
      }
      break;
    case ENC_TYPE_TKIP:
      if (DEBUG) {
        Serial.print("WPA");
      }
      break;
    case ENC_TYPE_CCMP:
      if (DEBUG) {
        Serial.print("WPA2");
      }
      break;
    case ENC_TYPE_NONE:
      if (DEBUG) {
        Serial.print("None");
      }
      break;
    case ENC_TYPE_AUTO:
      if (DEBUG) {
        Serial.print("Auto");
      }
      break;
  }
}



