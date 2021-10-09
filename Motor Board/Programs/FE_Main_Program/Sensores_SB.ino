byte sensorReadPercent(int sensor) {
  int normalizado = sensorReadRawValue(sensor);
  normalizado = constrain(normalizado, sensorEntorno[sensor * 2], sensorEntorno[(sensor * 2) + 1]);
  normalizado = map(normalizado, sensorEntorno[sensor * 2], sensorEntorno[(sensor * 2) + 1], 100, 0);
  return normalizado;
}

int sensorReadRawValue(int puerto) { //Puertos disponibles 0-7

  puerto = 14 + puerto;
  return analogRead(puerto);
}

void sensorCalibrate() {
  setLEDColor(15, "BLUE", "FLASH");
  buttonsWait();
  setLEDColor(15, "OFF", "NORMAL");
  for (int j = 0; j < 8; j++) {
    setLEDColor(j + 1, "BLUE", "FLASH");
    int maximo = 0;
    int minimo = 1023;
    for (long i = 0; i < 30000; i++) {
      int lectura = analogRead(14 + j);
      if (lectura < minimo) {
        minimo = lectura;
        sensorEntorno [(j * 2)]  = minimo;
      }
      if (lectura > maximo) {
        maximo = lectura;
        sensorEntorno [((j * 2) + 1)]  = maximo;
      }
      updateLED();
    }
    setLEDColor(j + 1, "OFF", "NORMAL");
  }
  for (int i = 0; i < 16; i++) {
    EEPROM.put((i * 2) + 20, sensorEntorno [i] );
    Serial.print(sensorEntorno[i]);
    Serial.print(' ');
  }
  Serial.println();
  setLEDColor(15, "GREEN" , "NORMAL");
  buttonsWait();
  setLEDColor(15, "OFF", "NORMAL");
  delay(1000);
}
