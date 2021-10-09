void sigueLineasSetup () {
  Serial.begin(57600);
  for (int i = 0; i < (sizeof(outMode) / 2); i++) {
    pinMode(outMode[i], OUTPUT);
    //Serial.println(outMode[i]);
  }
  for (int i = 0; i < (sizeof(inMode) / 2); i++) {
    pinMode(inMode[i], INPUT_PULLUP);
    // Serial.println(inMode[i]);
  }
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  attachInterrupt(digitalPinToInterrupt(ENC_C),encoderC,RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_B),encoderB,RISING);
  //qtrSetup();
}


void leerEEPROM() {
  qtr.calibrate(); // Necesario para poder cambiarlas. Lectura de maximos y minimos solo para la array.
  uint16_t dato = 0;
  for (int i = 0; i < SensorCount; i++) {
    EEPROM.get(i * 2, dato);
    qtr.calibrationOn.minimum[i] = dato;
    //Serial.println(dato);
    EEPROM.get(((i * 2) + (2 * SensorCount)), dato);
    qtr.calibrationOn.maximum[i] = dato;
    //Serial.println(dato);
  }
  for (int i = 0; i < 16; i++) { //Lectura para inicializar los sensores analogicos en su maximo y minimo
    int dato;
    EEPROM.get(((i * 2) + 20), dato);
    sensorEntorno[i] = dato;
    //Serial.println(sensorEntorno[i]);
  }
}
