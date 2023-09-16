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
  commandFinished = false;
}

void moveDistancia(int distancia, int vMax, int vFinal) {
  commandA.motor = constrain(abs(vFinal), 0, 100);
  if (vFinal < 0) bitSet(commandA.motor, 7); // Bit 7 en 1 para saber que es negativo
  commandA.distance = distancia;
  commandA.vMaxDistance = vMax;
  bitWrite(commandA.motorFlags, 3, !bitRead(commandA.motorFlags, 3)); // Activa la distancia
  enviarSPIA();
  commandFinished = false;
}

void motorBreak() {
  bitWrite(commandA.motorFlags, 4, !bitRead(commandA.motorFlags, 4)); // Activa el freno
  enviarSPIA();
  commandFinished = false;
}

int pidGyro(int dirObj, float kp, float ki, float kd) { //Servo of MiniZ Range: 24 > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
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
  if (sensor.velocidad >= 0) direccion.write(constrain(90 + OFFSET_SERVO - (int)giro, 24, 144));
  //else direccion.write(constrain(90 + OFFSET_SERVO + (int)giro, 24, 144));
}

float getCarAngle() {
  return ypr[0];
}

void resetCarAngle() {
  offsetMPU = ypr[0];
}

int distToAngle(int distancia) { // Min distance: 120, Max distance: 600
  int angle = 1;
  if (distancia >= 0) distancia = constrain(distancia, 120, 600);
  else {
    distancia = constrain(distancia, -600, -120);
    angle = -1;
  }
  if (abs(distancia) < 145) angle *= 40;
  else if (abs(distancia) < 235) angle *= 50;
  else if (abs(distancia) < 335) angle *= 70;
  else if (abs(distancia) < 420) angle *= 80;
  else if (abs(distancia) < 535) angle *= 87;
  else angle *= 89;
  return angle;
}

void desplazamientoLateral(int direccionPID, int distancia) { // Direccion: 0, 90, 180, 270
  int angle = distToAngle(distancia);
  int direccionMPU;
  if (direccionPID == 270) direccionMPU = -90;
  else direccionMPU = direccionPID;
  if (angle > 0) intermitente('D');
  else intermitente('I');
  while (abs(abs(getCarAngle()) - abs(direccionMPU)) < abs(angle)) {
    updateMPU();
    if (angle > 0) {
      if (direccionPID == 270) pidGyro(5, 2, 0, 0);
      else pidGyro(direccionPID + 90, 2, 0, 0);
    }
    else {
      if (direccionPID == 0) pidGyro(268, 2, 0, 0);
      else pidGyro(direccionPID - 92, 2, 0, 0);
    }
    enviarSPIA();
    enviarSPIB();
    updateLed();
    updateBeep();
  }
  if (angle > 0) intermitente('I');
  else intermitente('D');
  while (abs(abs(getCarAngle()) - abs(direccionMPU)) > 10) {
    updateMPU();
    pidGyro(direccionPID, 2, 0, 0);
    enviarSPIA();
    enviarSPIB();
    updateLed();
    updateBeep();
  }
  intermitente('A');
}
