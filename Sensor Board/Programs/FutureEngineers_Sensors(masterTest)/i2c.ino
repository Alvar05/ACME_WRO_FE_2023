void enviarSensores() {
  //cogemos la primera posición de memoria de la estuctura y la enviamos byte a byte
  Wire.beginTransmission(8);
  Wire.write((byte*)&sensores, sizeof(sensores));
  /* ptrYaw = ptrYaw + 1;
    Wire.write(*ptrYaw);
    delay(10);*/
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(8);
  Wire.write((byte*) & (pines), sizeof(pines));
  /* ptrYaw = ptrYaw + 1;
    Wire.write(*ptrYaw);
    delay(10);*/
  Wire.endTransmission();
}
