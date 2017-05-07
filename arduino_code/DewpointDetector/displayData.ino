// Displays the humidity, ambient temperature, object temperature, 
//  and dewpoint temperature on the TFT display

void displayData(float h, float t, float o, float d) {
  tft.setCursor(20,30);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setTextSize(2);
  
  tft.print("Humidity:      ");
  tft.println(h);
  tft.setCursor(20,60);
  tft.print("Ambient Temp:  ");
  tft.println(t);
  tft.setCursor(20,90);
  tft.print("Mirror Temp:   ");
  tft.println(o);
  tft.setCursor(20,120);
  tft.print("Dewpoint Temp: ");
  tft.println(d);
  tft.setCursor(20,150);
  tft.println("Dewpoint Detector");
  return;
}


