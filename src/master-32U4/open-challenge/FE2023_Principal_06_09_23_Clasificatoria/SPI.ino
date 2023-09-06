void setupSPI() {
  SPI.begin ();
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  // SS en nivel alto, garantiza que el esclavo esta inactivo
  pinMode(SSA, OUTPUT);
  pinMode(SSB, OUTPUT);
  digitalWrite(SSA, HIGH);
  digitalWrite(SSB, HIGH);
}

void enviarSPIA() {
  digitalWrite(SSA, LOW);    // SS is pin 10
  const byte * s = (const byte*) &commandA;
  byte * r = (const byte*) &buffA;
  unsigned int i;
  delayMicroseconds(50);
  for (i = 0; i < sizeof(commandA); i++) *r++ = transferAndWait(*s++);
  digitalWrite(SSA, HIGH);
  if (buffA[0] == STARTBYTE and buffA[sizeof(sensor) - 1] == ENDBYTE) {
    for (uint8_t i = 0; i < sizeof(sensor); i++) {
      byte *pointerBuff = (byte*)&buffA;
      byte *pointerSensor = (byte*)&sensor;
      pointerBuff += i;
      pointerSensor += i;
      *pointerSensor = *pointerBuff;
      *pointerBuff = 0;
    }
    buenaTramaA++;
  }
  else errorTramaA++;
}

void enviarSPIB() {
  digitalWrite(SSB, LOW);    // SS is pin 10
  const byte * s = (const byte*) &commandB;
  byte * r = (const byte*) &buffB;
  unsigned int i;
  delayMicroseconds(50);
  for (i = 0; i < sizeof(commandB); i++) *r++ = transferAndWait(*s++);
  digitalWrite(SSB, HIGH);
  if (buffB[0] == STARTBYTE and buffB[sizeof(i2c) - 1] == ENDBYTE) {
    for (uint8_t i = 0; i < sizeof(i2c); i++) {
      byte *pointerBuff = (byte*)&buffB;
      byte *pointerI2C = (byte*)&i2c;
      pointerBuff += i;
      pointerI2C += i;
      *pointerI2C = *pointerBuff;
      *pointerBuff = 0;
    }
    buenaTramaB++;
  }
  else errorTramaB++;
}

byte transferAndWait (const byte data) {
  byte a = SPI.transfer (data);
  delayMicroseconds (10);
  return a;
}

void imprimirSensores() { //Prints by Serial the sensor data
  Serial.println(String("Izquierdo=") + sensor.sr04[0] + String("  Centro=") + sensor.sr04[1] + String("  Derecho=") + sensor.sr04[2]);
  Serial.println(String("ENC IZQ: ") + motorGetDistancia("I") + String("  ENC DER: ") + motorGetDistancia("D") + String("  VELOCIDAD: ") + sensor.velocidad);
  Serial.println(sensor.direccion);
  Serial.println("\n\n");
}
