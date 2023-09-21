void setupTOF() {
  pinMode(XSHUT_D, OUTPUT);
  pinMode(XSHUT_I, INPUT);
  digitalWrite(XSHUT_D, LOW);
  TOF_I.setTimeout(500);
  while (!TOF_I.init()) {
    Serial.println("Error TOF izquierdo!\nReintentando...");
    pinMode(XSHUT_I, OUTPUT);
    digitalWrite(XSHUT_I, LOW);
    delay(100);
    pinMode(XSHUT_I, INPUT);
  }
  TOF_I.setAddress((uint8_t)42);
  pinMode(XSHUT_D, INPUT);
  TOF_D.setTimeout(500);
  while (!TOF_D.init()) {
    Serial.println("Error TOF derecho!\nReintentando...");
    delay(100);
  }
  TOF_D.setDistanceMode(VL53L1X::Long);
  TOF_I.setDistanceMode(VL53L1X::Long);
  TOF_D.setMeasurementTimingBudget(50000);
  TOF_I.setMeasurementTimingBudget(50000);
  TOF_D.setROICenter(199);
  TOF_I.setROICenter(199);
  TOF_D.startContinuous(50);
  TOF_I.startContinuous(50);
  TOF_D.setROISize(4, 16);
  TOF_I.setROISize(4, 16);
}

int readTOF(char lado) {
  static int distanciaTOF_I;
  static int distanciaTOF_D;
  if (lado == 'D') {
    if (TOF_D.dataReady()) distanciaTOF_D = TOF_D.read(false);
    return distanciaTOF_D;
  }
  else if (lado == 'I') {
    if (TOF_I.dataReady()) distanciaTOF_I = TOF_I.read(false);
    return distanciaTOF_I;
  }
}

int readMedianaTOF(char lado, int nIteraciones){
  static int tofI[15];
  static int tofD[15];
  int arrOut[nIteraciones];

  if (lado == 'D') {
    tofD[indiceMedianaTOF_D] = readTOF('D');
    for (int i = 0; i < nIteraciones; i++) arrOut[i] = tofD[i];
    indiceMedianaTOF_D++;
  }
  else if (lado == 'I') {
    tofI[indiceMedianaTOF_I] = readTOF('I');
    for (int i = 0; i < nIteraciones; i++) arrOut[i] = tofI[i];
    indiceMedianaTOF_I++;
  }
  quickSort(arrOut, 0, nIteraciones - 1);
  if (indiceMedianaTOF_D == nIteraciones) indiceMedianaTOF_D = 0;
  if (indiceMedianaTOF_I == nIteraciones) indiceMedianaTOF_I = 0;
  
  return arrOut[int(nIteraciones / 2)];
}
