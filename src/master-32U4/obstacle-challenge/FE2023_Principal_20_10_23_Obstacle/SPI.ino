void setupSPI() {
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  // SS in high level, guarantees the slaves are unactive in the bus
  pinMode(SSA, OUTPUT);
  pinMode(SSB, OUTPUT);
  digitalWrite(SSA, HIGH);
  digitalWrite(SSB, HIGH);
}

void updateSPI() {
  if (millis() > (lastTimeSPIA + SPIA_Interval)){
    enviarSPIA();
    lastTimeSPIA = millis();
  }
  if (millis() > (lastTimeSPIB + SPIB_Interval)){
    enviarSPIB();
    lastTimeSPIB = millis();
  }
}

void enviarSPIA() {
  digitalWrite(SSA, LOW);
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
  lastCommandACheck();
}

void enviarSPIB() {
  digitalWrite(SSB, LOW);
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
  lastCommandBCheck();
}

byte transferAndWait (const byte data) {
  byte a = SPI.transfer (data);
  delayMicroseconds (10);
  return a;
}

void lastCommandACheck() {
  if (sensor.lastCommandFlag != lastCommandAState) {
    lastCommandAState = sensor.lastCommandFlag;
    commandFinished = true;
  }
}

void lastCommandBCheck() {
  if (i2c.lastCommandFlag != lastCommandBState) {
    lastCommandBState = i2c.lastCommandFlag;
    servoFinished = true;
  }
}
