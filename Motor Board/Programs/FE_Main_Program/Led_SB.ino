void setLEDColor(int ledBin, String color, String mode) {
  constrain(ledBin, 1, 15);
  for (int i = 0; i < 4; i++) {
    if ((ledBin & 1) == 1) {
      //led = led - 1;
      colorLed[i] = color;                      //Color can be "OFF", "GREEN", "RED" or "ORANGE" in SmallBasic
      modeLed[i] = mode;                        // Can be "NORMAL", "FLASH" or  "PULSE".
      colores(color, i);
      FastLED.show(20);
    }
    ledBin = ledBin >> 1;
  }
}

void colores(String color, int led) {
  if (color == "WHITE")leds[led] = CRGB::White;
  if (color == "YELLOW")leds[led] = CRGB::Yellow;
  if (color == "RED")leds[led] = CRGB::Red;
  if (color == "GREEN")leds[led] = CRGB::Green;
  if (color == "BLUE")leds[led] = CRGB::Blue;
  if (color == "ORANGE")leds[led] = CRGB::Orange;
  if (color == "PINK")leds[led] = CRGB::HotPink;
  if (color == "OFF")leds[led] = CRGB::Black;
  if (color == "BLACK")leds[led] = CRGB::Black;
}

void updateLED() {            //Necesario para realizar el flash
  static long tiempo = 0;
  static bool estado = true;
  if ( tiempo + 250 < millis()) {
    for (int led = 0; led < 4; led++) {
      if (modeLed[led] != "NORMAL") {
        if (estado == true) {
          String color = colorLed[led];
          colores(color, led);
        }
        else {
          leds[led] = CRGB::Black;
        }
        FastLED.show(20);
      }
    }
    estado = !estado;
    tiempo = millis();
  }
}
