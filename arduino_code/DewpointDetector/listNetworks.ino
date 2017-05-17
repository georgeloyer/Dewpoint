/*
 * listNetworks()
 * 
 * prints a list of networks for user choice on which to connect to
 */

void listNetworks() {
  // scan for nearby networks:
  if (DEBUG) {
    Serial.println("** Scan Networks **");
  }
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    if (DEBUG) {
      Serial.println("Couldn't get a WiFi connection");
    }
    while (true);
  }

  // print the list of networks seen:
  if (DEBUG) {
    Serial.print("number of available networks: ");
    Serial.println(numSsid);
  }
  
  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    if (DEBUG) {
      Serial.print(thisNet + 1);
      Serial.print(") ");
      Serial.print("Signal: ");
      Serial.print(WiFi.RSSI(thisNet));
      Serial.print(" dBm");
      Serial.print("\tChannel: ");
      Serial.print(WiFi.channel(thisNet));
      byte bssid[6];
      Serial.print("\t\tBSSID: ");
      printBSSID(WiFi.BSSID(thisNet, bssid));
      Serial.print("\tEncryption: ");
      printEncryptionType(WiFi.encryptionType(thisNet));
      Serial.print("\t\tSSID: ");
      Serial.println(WiFi.SSID(thisNet));
      Serial.flush();
    }
  }
  Serial.println();
}


