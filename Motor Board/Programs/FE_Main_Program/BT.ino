 void updateSerial() {
  if (Serial.available() > 0) {
    char dato = Serial.read();
    if (dato == 'P') kp = Serial.parseFloat();
    if (dato == 'I') ki = Serial.parseFloat();
    if (dato == 'D') kd = Serial.parseFloat();
    if (dato == 'V') velocidad = Serial.parseInt();
    if (dato == 'H') Serial.println(String("kp=") + kp + String("  ki=") + ki + String("  kd=") + kd + String("  v=") + velocidad);
    if (dato == 'F') estadoBT = false;
    if (dato == 'T') estadoBT = true;
    if (dato == 'B') motorResetCount("B");
    if (dato == 'C') motorResetCount("C");
    if (dato == '_') beep(Serial.parseInt());
    if (dato == 'U') {/////UPDATE
      Serial.print("< "); //Inicio de trama
      Serial.print(kp + String(" ") + ki + String(" ") + kd + String(" ") + velocidad + String(" "));//PID
      for (int i = 0; i < 8; i++) {     //Valores de los sensores
        Serial.print(sensorReadPercent(i) + String(" "));
      }
      Serial.print(qtrReadPosition() + String(" "));    // Posicion de la línea de 0-5000
      Serial.print(motorGetCount("B") + String(" ") + motorGetCount("C") + String(" ")); // grados de motor
      Serial.print(estadoBT + String(" "));
      Serial.print(">"); //Final de trama
    }
  }
  if (startBeep < millis()) { //ON OFF del beep
    if (estadoBeep == true) {
      Serial.println("OFF_");
      estadoBeep = false;
    }
  }
}
void beep(int tiempo) {    //Envio de pitido
  tiempo = constrain(tiempo, 100, 10000);
  Serial.println(String("ON_") + tiempo);
  startBeep = millis() + tiempo;
  estadoBeep = true;
}
