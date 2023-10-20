void setupSPI() {
  pinMode(MISO, INPUT);
  pinMode(MOSI, INPUT);
  pinMode (PIN_INTERRUPCION, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPCION), SSFalling, FALLING);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // now turn on interrupts
  SPI.attachInterrupt();
  SPDR = *(byte*) &sensor; // Loads the first byte in the register
}

void updateSPI() {
  if (haveData) {
    if (buff[0] == STARTBYTE and buff[sizeof(command) - 1] == ENDBYTE) {
      for (uint8_t i = 0; i < sizeof(command); i++) {
        byte *pointerBuff = (byte*)&buff;
        byte *pointerCommand = (byte*)&command;
        pointerBuff += i;
        pointerCommand += i;
        *pointerCommand = *pointerBuff;
      }
      buenaTrama++;
    }
    else errorTrama++;
    //////////////////////////ULTRASONICS//////////////////////
    for (uint8_t i = 0; i < 3; i++) {
      if (bitRead(command.flags, i)) {
        activeUS[i] = true;
      }
      else if (activeUS[i]) {
        activeUS[i] = false;
        sensor.sr04[i] = 0;
      }
    }
    ///////////////////////////////MOTORS//////////////////////
    if (bitRead(command.motorFlags, 4) != breakBitState) {
      breakBitState = !breakBitState;
      motorBreak();
    }
    if (bitRead(command.motorFlags, 3) != distanceBitState) {
      distanceBitState = !distanceBitState;
      vInicial = motorGetSpeed();
      vFinal = bitClear(command.motor, 7);
      activeDistance = true;
      activeRampa = false;
      activePID = false;
      if (vFinal == 0) {
        desacelBreak = true;
      }
      long distance = command.distance;
      vMaxDist = (100 * distance + MIN_ACCEL_DISTANCE * vInicial + MIN_DESACCEL_DISTANCE * (vFinal + MIN_V_TO_BREAK * desacelBreak)) / (MIN_ACCEL_DISTANCE + MIN_DESACCEL_DISTANCE);
      motorResetSlaveCount("D");
      if (vMaxDist > vInicial) activeDistAcel = true;
      else activeDistAcel = false;
    }
    if (bitRead(command.motorFlags, 2) != rampaBitState) {
      rampaBitState = !rampaBitState;
      bool negative = bitRead(command.motor, 7);
      vInicial = motorGetSpeed();
      vFinal = bitClear(command.motor, 7);
      if (negative) vFinal *= (-1);
      motorResetSlaveCount("D");
      activeRampa = true;
      activePID = false;
      activeDistance = false;
    }
    if (bitRead(command.motorFlags, 1) != PIDBitState) {
      PIDBitState = !PIDBitState;
      bool negative = bitRead(command.motor, 7);
      speed = bitClear(command.motor, 7);
      if (negative) speed *= (-1);
      activePID = true;
      activeRampa = false;
      activeDistance = false;
    }
    if (bitRead(command.motorFlags, 0) != PWMBitState) {
      PWMBitState = !PWMBitState;
      bool negative = bitRead(command.motor, 7);
      if (negative) motorStartPower(bitClear(command.motor, 7) * (-1));
      else motorStartPower(bitClear(command.motor, 7));
      activePID = false;
      activeRampa = false;
      activeDistance = false;
    }
    haveData = false;
  }
}

// SPI bus ISR
ISR (SPI_STC_vect) {
  readWriteSPI();
  haveData = true;
  attachInterrupt(digitalPinToInterrupt(ENC_MA), encoderM, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_I), encoderI, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_D), encoderD, RISING);
  echoCheckFlag = true;
}

void readWriteSPI() {
  byte * r = (byte*) &buff;
  const byte * s = (byte*) &sensor;
  unsigned int i;
  *r++ = SPDR;  // get first byte
  s++;
  for (i = 1; i < sizeof(command); i++) {
    SPDR = *s++;
    /*
       The following NOP introduces a small delay that can prevent the wait
       loop form iterating when running at the maximum speed. This gives
       about 10% more speed, even if it seems counter-intuitive. At lower
       speeds it is unnoticed.
    */
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) {
      if (digitalRead(SS)) { // If any data is lost, exits the transmission
        SPDR = *(byte*) &sensor;
        pinMode(MISO, INPUT);
        return; // wait
      }
    }
    *r++ = SPDR;
  }
  pinMode(MISO, INPUT);
  SPDR = *(byte*) &sensor;
}

void SSFalling() {
  pinMode(MISO, OUTPUT);
  detachInterrupt(digitalPinToInterrupt(ENC_MA));
  detachInterrupt(digitalPinToInterrupt(ENC_I));
  detachInterrupt(digitalPinToInterrupt(ENC_D));
  echoCheckFlag = false;
}
