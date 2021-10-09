void imprimirSensores() {
  Serial.print(String("Yaw=") + sensores.ypr[0] + String("  Pitch=") + sensores.ypr[2] + String("  Roll=") + sensores.ypr[1]);
  Serial.println();
  Serial.print(String("Izquierdo=") + sensores.sr04[0] + String("  Centro=") + sensores.sr04[1]+ String("  Derecho=") + sensores.sr04[2]);
  Serial.println();
  Serial.print(String("EncoderB=") + sensores.encoder[0] + String("  EncoderC=") + sensores.encoder[1]);
  Serial.println();
  for(int i=0; i<pinesAnalogicos;i++){
    Serial.print( String("AV")+ i +String("=") + sensores.analogicos[i]+String(" "));
  }
  Serial.println();
  for(int i=0; i<pinesDigitales;i++){
    Serial.print(String("DV")+ i +String("=") + sensores.digitales[i]+String(" "));
  }
  Serial.println();
  Serial.println();
}
