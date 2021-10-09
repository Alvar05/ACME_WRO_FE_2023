// Recoger datos
void updateSensores() {
  //MPU 6050 YPR
  for (int i = 0; i < 3; i++) {
    sensores.ypr[i] = ypr[i] * 180 / M_PI;
  }
  //SR04
  for (int i = 0; i < 3; i++) {
    sensores.sr04[i] = sonar[i].ping_cm();
  }
  //ENCODERS
  sensores.encoder[0] = gradosB;
  sensores.encoder[1] = gradosC;
  //Pines Analogicos
  for (int i = 0; i < sizeof(pinesAnalogicos); i++) {
    pines.analogicos[i] = analogRead(pinesAnalogicos[i]);
  }
  //Pines Digitales
  for (int i = 0; i < sizeof(pinesDigitales); i++) {
    pines.digitales[i] = digitalRead(pinesDigitales[i]);
  }
}

void imprimirSensores() {
  Serial.print(String("Yaw=") + sensores.ypr[0] + String("  Pitch=") + sensores.ypr[2] + String("  Roll=") + sensores.ypr[1]);
  Serial.println();
  Serial.print(String("Izquierdo=") + sensores.sr04[0] + String("  Centro=") + sensores.sr04[1]+ String("  Derecho=") + sensores.sr04[2]);
  Serial.println();
  Serial.print(String("EncoderB=") + sensores.encoder[0] + String("  EncoderC=") + sensores.encoder[1]);
  Serial.println();
  for(int i=0; i<sizeof(pinesAnalogicos);i++){
    Serial.print(pinesAnalogicos[i]+String("=") + pines.analogicos[i]+String(" "));
  }
  Serial.println();
  for(int i=0; i<sizeof(pinesDigitales);i++){
    Serial.print(pinesDigitales[i]+String("=") + pines.digitales[i]+String(" "));
  }
  Serial.println();
  Serial.println();
}
