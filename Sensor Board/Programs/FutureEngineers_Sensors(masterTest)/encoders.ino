void encoderB() {
  gradosB++;
}
void encoderC() {
  gradosC++;
}
void testEncoder(){
  Serial.println(String("ENCODERB=") + gradosB + String("\t") + String("ENCODERC=") + gradosC);
  Serial.println();
}
