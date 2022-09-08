//This tab contains the methods related to I2C connection between M5Stack(Master) and ProMicro(Slave) controllers
void sensorUpdate() { //Updates sensor variables with the new values
  if (sensorInterruptFlag == true) {
    Wire.requestFrom(8, 16);    // request 16 bytes from slave device #8
    if (Wire.available() == 16) {
      int index = 0;
      while (Wire.available()) { // slave may send less than requested
        byte*pointer = (byte*)&sensor;
        pointer = pointer + index;
        *pointer = (byte)Wire.read();
        index++;
      }
    }
    sensorInterruptFlag = false;
  }
}

void enviarI2C(char dato) { // Sends some character to the ProMicro controller
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(dato);
  Wire.endTransmission();    // stop transmitting
}

void sensorInterrupt() {
  sensorInterruptFlag = true;
}
