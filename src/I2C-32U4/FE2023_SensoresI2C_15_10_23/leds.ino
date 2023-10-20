void setupLeds() {
  TD.setOutput(TD_PIN);
  TI.setOutput(TI_PIN);
  FD.setOutput(FD_PIN);
  FI.setOutput(FI_PIN);
  BLACK.r = 0; BLACK.g = 0; BLACK.b = 0;
  for (int i = 0; i < 3; i++) {
    setLed(i, 0, 0, 0, 0, "FI");
    setLed(i, 0, 0, 0, 0, "FD");
    ledModeFI[i] = 0;
    ledModeFD[i] = 0;
    ledColorFI[i] = BLACK;
    ledColorFD[i] = BLACK;
  }
  for (int i = 0; i < 2; i++) {
    setLed(i, 0, 0, 0, 0, "TI");
    setLed(i, 0, 0, 0, 0, "TD");
    ledModeTI[i] = 0;
    ledModeTD[i] = 0;
    ledColorTI[i] = BLACK;
    ledColorTD[i] = BLACK;
  }
}

void updateLeds() {
  static long tiempo = 0;
  static bool encendido = true;
  if (tiempo + tiempoBlink < millis()) {
    for (int ledFI = 0; ledFI < 3; ledFI++) {
      if (ledModeFI[ledFI] == 2) {
        if (encendido) FI.set_crgb_at(ledFI, ledColorFI[ledFI]);
        else FI.set_crgb_at(ledFI, BLACK);
      }
      else if (ledModeFI[ledFI] == 1) FI.set_crgb_at(ledFI, ledColorFI[ledFI]);
      else FI.set_crgb_at(ledFI, BLACK);
    }
    for (int ledFD = 0; ledFD < 3; ledFD++) {
      if (ledModeFD[ledFD] == 2) {
        if (encendido) FD.set_crgb_at(ledFD, ledColorFD[ledFD]);
        else FD.set_crgb_at(ledFD, BLACK);
      }
      else if (ledModeFD[ledFD] == 1) FD.set_crgb_at(ledFD, ledColorFD[ledFD]);
      else FD.set_crgb_at(ledFD, BLACK);
    }
    for (int ledTI = 0; ledTI < 2; ledTI++) {
      if (ledModeTI[ledTI] == 2) {
        if (encendido) TI.set_crgb_at(ledTI, ledColorTI[ledTI]);
        else TI.set_crgb_at(ledTI, BLACK);
      }
      else if (ledModeTI[ledTI] == 1) TI.set_crgb_at(ledTI, ledColorTI[ledTI]);
      else TI.set_crgb_at(ledTI, BLACK);
    }
    for (int ledTD = 0; ledTD < 2; ledTD++) {
      if (ledModeTD[ledTD] == 2) {
        if (encendido) TD.set_crgb_at(ledTD, ledColorTD[ledTD]);
        else TD.set_crgb_at(ledTD, BLACK);
      }
      else if (ledModeTD[ledTD] == 1) TD.set_crgb_at(ledTD, ledColorTD[ledTD]);
      else TD.set_crgb_at(ledTD, BLACK);
    }
    encendido = !encendido;
    tiempo = millis();
    FI.sync();
    FD.sync();
    TI.sync();
    TD.sync();
  }
}

void setLed(int num, int modo, int r, int g, int b, String moduloLed) {  //num = number of led; modo: 0 = OFF, 1 = ON, 2 = Blink
  cRGB color;
  color.r = r;
  color.g = g;
  color.b = b;
  if (moduloLed == "FI") {
    FI.set_crgb_at(num, color);
    ledColorFI[num] = color;
    ledModeFI[num] = modo;
  }
  else if (moduloLed == "FD") {
    FD.set_crgb_at(num, color);
    ledColorFD[num] = color;
    ledModeFD[num] = modo;
  }
  else if (moduloLed == "TI") {
    TI.set_crgb_at(num, color);
    ledColorTI[num] = color;
    ledModeTI[num] = modo;
  }
  else if (moduloLed == "TD") {
    TD.set_crgb_at(num, color);
    ledColorTD[num] = color;
    ledModeTD[num] = modo;
  }
}

void intermitente(char lado) {
  if (lado == 'I') {
    setLed(0, 0, 0, 0, 0, "FD");
    setLed(1, 0, 0, 0, 0, "TD");
    setLed(2, 2, 255, 186, 0, "FI");
    setLed(0, 2, 255, 186, 0, "TI");
  }
  else if (lado == 'D') {
    setLed(2, 0, 0, 0, 0, "FI");
    setLed(0, 0, 0, 0, 0, "TI");
    setLed(0, 2, 255, 186, 0, "FD");
    setLed(1, 2, 255, 186, 0, "TD");
  }
  else if (lado == 'E') {
    setLed(2, 2, 255, 186, 0, "FI");
    setLed(0, 2, 255, 186, 0, "FD");
    setLed(1, 2, 255, 186, 0, "TD");
    setLed(0, 2, 255, 186, 0, "TI");

  }
  else {
    setLed(2, 0, 0, 0, 0, "FI");
    setLed(0, 0, 0, 0, 0, "FD");
    setLed(1, 0, 0, 0, 0, "TD");
    setLed(0, 0, 0, 0, 0, "TI");
  }
}

void luzFreno(bool estado) {
  if (estado) {
    setLed(0, 1, 255, 0, 0, "TD");
    setLed(1, 1, 255, 0, 0, "TI");
  }
  else {
    setLed(0, 0, 0, 0, 0, "TD");
    setLed(1, 0, 0, 0, 0, "TI");
  }
}

void luzFrontal(char modo) {
  if (modo == 'L') {
    setLed(0, 1, 255, 255, 255, "FI");
    setLed(1, 1, 255, 255, 255, "FI");
    setLed(1, 1, 255, 255, 255, "FD");
    setLed(2, 1, 255, 255, 255, "FD");
  }
  else if (modo == 'C') {
    setLed(0, 1, 70, 70, 70, "FI");
    setLed(1, 1, 70, 70, 70, "FI");
    setLed(1, 1, 70, 70, 70, "FD");
    setLed(2, 1, 70, 70, 70, "FD");
  }
  else {
    setLed(0, 0, 0, 0, 0, "FI");
    setLed(1, 0, 0, 0, 0, "FI");
    setLed(1, 0, 0, 0, 0, "FD");
    setLed(2, 0, 0, 0, 0, "FD");
  }
}
