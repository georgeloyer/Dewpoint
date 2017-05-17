/*
 * drawAppScreen()
 * simple app screen setup function
 */

void drawAppScreen() {
  tft.fillScreen(ILI9341_RED);
  tft.setRotation(3);
  tft.fillRect(10, 10, 300, 220, ILI9341_BLACK);
}

