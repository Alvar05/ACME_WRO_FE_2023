void updateSerial() {
  while (Serial.available()) {
    char accion = Serial.read();
    /*if (accion == 'D') intermitente('D');
    if (accion == 'L') luzFrontal('L');
    if (accion == 'C') luzFrontal('C');
    if (accion == 'A') {
      intermitente('A');
      luzFrontal('A');
    }
    if (accion == 'G') command.angServo = Serial.parseInt();*/
  }
}

void imprimirSensores () {
  Serial.print(String("SECTOR: ") + i2c.sector);
  Serial.print(String("   Bloque Delante:") + i2c.bloqueCerca);
  Serial.print(String("   TOF I: ") + i2c.TOF[0]);
  Serial.print(String("   TOF D: ") + i2c.TOF[1]);
  Serial.print(String("   MPU: ") + i2c.MPU);
  Serial.println(String("    AngServo: ") + command.angServo);
}
