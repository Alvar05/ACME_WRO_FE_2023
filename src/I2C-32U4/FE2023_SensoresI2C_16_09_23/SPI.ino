void setupSPI() {
  pinMode(MISO, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(PIN_INTERRUPCION, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPCION), SSFalling, FALLING);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  // now turn on interrupts
  SPI.attachInterrupt();
  SPDR = *(byte*) &i2c; // Carga el primer byte de la trama
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
        *pointerBuff = 0;
      }
      buenaTrama++;
    }
    else errorTrama++;

    //Serial.println(errorTrama + String("   ") + buenaTrama);
    //Serial.println(command.camara + String(" ") + command.leds + String(" ") + command.filtroCam + String(" ") + command.masComandos[0] + String(" ") + command.masComandos[1] + String(" ") + command.masComandos[2] + String(" ") + command.masComandos[3]);
    //Serial.println(command.leds, BIN);
    ///////////////////////LEDS
    luzFreno(bitRead(command.leds, 2));
    if (bitRead(command.leds, 0) and bitRead(command.leds, 1)) intermitente('E');
    else if (bitRead(command.leds, 0)) intermitente('D');
    else if (bitRead(command.leds, 1)) intermitente('I');
    else intermitente('A');
    if (bitRead(command.leds, 3)) luzFrontal('L');
    else if (bitRead(command.leds, 4)) luzFrontal('C');
    else luzFrontal('A');
  }
  haveData = false;
}

ISR (SPI_STC_vect) {
  readWriteSPI();
  haveData = true;
  flagSPI = false;
}

void readWriteSPI() {
  byte * r = (byte*) &buff;
  const byte * s = (byte*) &i2c;
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
        SPDR = *(byte*) &i2c;
        pinMode(MISO, INPUT);
        return; // wait
      }
    }
    *r++ = SPDR;
  }
  SPDR = *(byte*) &i2c;
  pinMode(MISO, INPUT);
}

void SSFalling() {
  pinMode(MISO, OUTPUT);
  flagSPI = true;
}
