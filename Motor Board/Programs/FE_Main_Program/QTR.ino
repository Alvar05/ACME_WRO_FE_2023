void qtrSetup()
{
  // Configurar sensores
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A1, A2, A3, A4, A5, A6
  }, SensorCount);
  leerEEPROM(); //Inicializar minimos y maximos de los sensores guardados.
  if (buttonsCurrent() == "U") {  //Si después del reset permanece en el botón de arriba pulsado entra en calibracion
    setLEDColor(15, "PINK", "FLASH");
    while (buttonsCurrent() == "U") updateLED();
    delay(1000);
    setLEDColor(15, "OFF", "FLASH");
    qtrCalibrate();
  }
  if (buttonsCurrent() == "D") {  //Si después del reset permanece en el botón de abajo pulsado entra en calibracion
    setLEDColor(15, "BLUE", "FLASH");
    while (buttonsCurrent() == "D") updateLED();
    delay(1000);
    setLEDColor(15, "OFF", "FLASH");
    sensorCalibrate();
  }
}


// analogRead() takes about 0.1 ms on an AVR.
// 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
// * 10 reads per calibrate() call = ~24 ms per calibrate() call.
// Call calibrate() 400 times to make calibration take about 10 seconds.

void qtrCalibrate() {
  setLEDColor(15, "PINK", "FLASH");
  buttonsWait();
  int led = 1;
  for (uint16_t i = 1; i < 400; i++) {
    qtr.calibrate();
    if ((i % 100) == 0) {
      setLEDColor(led, "OFF", "NORMAL");
      led = led << 1;
    }
    updateLED();
  }
  setLEDColor(15, "GREEN", "NORMAL");
  escribirEEPROM();
  buttonsWait();
  setLEDColor(15, "OFF", "NORMAL");
  delay(1000);
}

// print the calibration minimum values measured when emitters were on
void qtrReadMinimum() {

  for (uint8_t i = 0; i < SensorCount; i++)  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();
}

// print the calibration maximum values measured when emitters were on
void qtrReadMaximum() {

  for (uint8_t i = 0; i < SensorCount; i++)  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
}

int qtrReadPosition() {
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  return qtr.readLineBlack(sensorValues);

  /* print the sensor values as numbers from 0 to 1000, where 0 means maximum
    // reflectance and 1000 means minimum reflectance, followed by the line
    // position
    for (uint8_t i = 0; i < SensorCount; i++)
    {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
    }
    Serial.println(position);

    delay(250);*/
}
void escribirEEPROM() {

  for (int i = 0; i < SensorCount; i++) {
    EEPROM.put(i * 2, qtr.calibrationOn.minimum[i]);
    EEPROM.put(((i * 2) + (2 * SensorCount)), qtr.calibrationOn.maximum[i]);
  }
  delay(1000);
}
