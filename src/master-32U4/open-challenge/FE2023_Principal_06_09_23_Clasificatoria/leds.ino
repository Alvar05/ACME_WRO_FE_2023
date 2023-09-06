void intermitente(char lado) {
  if (lado == 'I') {
    bitSet(commandB.leds, 1);
  }
  else if (lado == 'D') {
    bitClear(commandB.leds, 1);
    bitSet(commandB.leds, 0);
  }
  else if (lado == 'E') {
    bitSet(commandB.leds, 1);
    bitSet(commandB.leds, 0);
  }
  else {
    bitClear(commandB.leds, 1);
    bitClear(commandB.leds, 0);
  }
  enviarSPIB();
}

void luzFreno(bool estado) {
  bitWrite(commandB.leds, 2, estado);
  enviarSPIB();
}

void luzFrontal(char modo) {
  if (modo == 'L') {
    bitClear(commandB.leds, 4);
    bitSet(commandB.leds, 3);
  }
  else if (modo == 'C') {
    bitClear(commandB.leds, 3);
    bitSet(commandB.leds, 4);
  }
  else {
    bitClear(commandB.leds, 3);
    bitClear(commandB.leds, 4);
  }
  enviarSPIB();
}

void apagarLeds() {
  bitClear(commandB.leds, 0);
  bitClear(commandB.leds, 1);
  bitClear(commandB.leds, 2);
  bitClear(commandB.leds, 3);
  bitClear(commandB.leds, 4);
}
