void motorResetCount(String m) {            //Restarts the encoder counter
  for (int i = 0; i < m.length(); i++) {
    if (m.charAt(i) == 'I') {
      offsetI = sensor.encoderI;
    }
    else if (m.charAt(i) == 'D') {
      offsetD = sensor.encoderD;
    }
  }
}

unsigned int motorGetDistancia(String motor) { //Returns the distance in cm done by the tyre
  if (motor == "D")return sensor.encoderD - offsetD;
  else if (motor == "I")return sensor.encoderI - offsetI;
}

unsigned int motorGetDistanciaTotal(String motor) { //Returns the distance in cm done by the tyre
  if (motor == "D")return sensor.encoderD;
  else if (motor == "I")return sensor.encoderI;
}

void motorStartPower(int vel) { // vel goes from -100 to 100
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7); // Bit 7 en 1 para saber que es negativo
  bitWrite(commandA.motorFlags, 0, !bitRead(commandA.motorFlags, 0)); // Elimina el PID
  enviarSPIA();
}

void motorStart(int vel) { // vel goes from -100 to 100
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7); // Bit 7 en 1 para saber que es negativo
  bitWrite(commandA.motorFlags, 1, !bitRead(commandA.motorFlags, 1)); // Elimina el PID
  enviarSPIA();
}

void setVelocidad(int vel) {
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7); // Bit 7 en 1 para saber que es negativo
  bitWrite(commandA.motorFlags, 2, !bitRead(commandA.motorFlags, 2)); // Activa la Rampa
  enviarSPIA();
}

void moveDistancia(int distancia, int vMax, int vFinal) {
  commandA.motor = constrain(abs(vFinal), 0, 100);
  if (vFinal < 0) bitSet(commandA.motor, 7); // Bit 7 en 1 para saber que es negativo
  commandA.distance = distancia;
  commandA.vMaxDistance = vMax;
  bitWrite(commandA.motorFlags, 3, !bitRead(commandA.motorFlags, 3)); // Activa la distancia
  enviarSPIA();
}

void motorBreak() {
  bitWrite(commandA.motorFlags, 4, !bitRead(commandA.motorFlags, 4)); // Activa el freno
  enviarSPIA();
}

void pidGyro(int dirObj, float kp, float ki, float kd) { //Servo of MiniZ Range: 30º > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
  int grados = 0;                                       //IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)
  grados = getCarAngle();

  error = dirObj - grados;
  if ((error < 0) and (abs(error + 360) < error)) error = error + 360;  //This selects the shortest way (left-negative, right-positive)
  else if ((error > 0) and (abs(error - 360) < error)) error = error - 360;

  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  direccion.write(constrain(90 - OFFSET_SERVO - (int)giro, 30, 144));
}

float getCarAngle() {
  return (i2c.MPU / 100.00) - offsetMPU;
}
