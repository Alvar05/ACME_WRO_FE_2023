
void pixySetup() {
  pixy.init();
  pixy.changeProg("color_connected_components");
}

void updatePixyPos() {
  pixy.ccc.getBlocks();
  for (int i = 0; i < pixy.ccc.numBlocks; i++) {
    int disRecta = (altBase * disBase) / pixy.ccc.blocks[i].m_height;
    float ejeX = map(pixy.ccc.blocks[i].m_x, 0, 315, -100, 100);
    int angulo = (ejeX * 32) / 100;
    int disDiag = disRecta / cos(angulo * DEG_TO_RAD);
    int disLat = disRecta * tan(angulo * DEG_TO_RAD);

    posData[0][i] = disRecta;
    posData[1][i] = disDiag;
    posData[2][i] = angulo;
    posData[3][i] = disLat;
  }
}

void calibrarPixy(int bloque) {
  int numMuestras = 10;
  int alt[numMuestras];
  Serial.println("Iniciando calibracion");
  for (int i = 0; i < numMuestras; i++) {
    pixy.ccc.getBlocks();
    alt[i] = pixy.ccc.blocks[bloque].m_height;
    Serial.println(String("|Muestra ") + i + String("| = ") + alt[i]);
  }
  altBase = (alt[0]+alt[1]+alt[2]+alt[3]+alt[4]+alt[5]+alt[6]+alt[7]+alt[8]+alt[9]) / numMuestras;
  Serial.println(String("Calibrado: Alt=") + altBase + String(" @ Dis=") + disBase);
}
