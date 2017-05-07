void print2Digits(byte thisByte) {
  if (thisByte < 0xF) {
    if (DEBUG) {
      Serial.print("0");
    }
  }
  if (DEBUG) {
    Serial.print(thisByte, HEX);
  }
}
