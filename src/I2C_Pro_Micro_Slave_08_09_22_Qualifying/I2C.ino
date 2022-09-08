//This tab contains the methods related to I2C connection between M5Stack(Master) and ProMicro(Slave) controllers
void requestEvent(int bytes) {
  Wire.write((byte*)&sensor, sizeof(sensor));
  timeOut = millis() + TIEMPO_TIMEOUT;
  digitalWrite(PIN_INTERRUPCION, 0);
}
void receiveEvent(int bytes) {
  while (Wire.available()) { // loop through all but the last
    accion = Wire.read(); // receive byte as a character
  }
}
void i2cSetup() {
  Wire.begin(8);
  Wire.setClock(400000);
  Wire.onRequest(requestEvent); // Cuando hace la petición el master
  Wire.onReceive(receiveEvent); // Cuando recibe del master
}
