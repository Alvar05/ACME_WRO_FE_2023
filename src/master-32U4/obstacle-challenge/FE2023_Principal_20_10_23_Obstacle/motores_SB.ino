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

unsigned int motorGetDistancia(String motor) { //Returns the distance in mm done by the tyre
  if (motor == "D")return sensor.encoderD - offsetD;
  else if (motor == "I")return sensor.encoderI - offsetI;
}

unsigned int motorGetDistanciaTotal(String motor) { //Returns the distance in mm done by the tyre
  if (motor == "D")return sensor.encoderD;
  else if (motor == "I")return sensor.encoderI;
}

void motorStartPower(int vel) { // vel goes from -100 to 100
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7); // Bit 7 high to indicate that it is negative
  bitWrite(commandA.motorFlags, 0, !bitRead(commandA.motorFlags, 0)); // Activates his flag
  enviarSPIA(); // Sends the command to the sensors slave
}

void motorStart(int vel) { // PID control for velocity
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7);  // Bit 7 high to indicate that it is negative
  bitWrite(commandA.motorFlags, 1, !bitRead(commandA.motorFlags, 1)); // Activates his flag
  enviarSPIA();
}

void setVelocidad(int vel) { // Makes an acceleration to the new velocity
  commandA.motor = constrain(abs(vel), 0, 100);
  if (vel < 0) bitSet(commandA.motor, 7); // Bit 7 high to indicate that it is negative
  bitWrite(commandA.motorFlags, 2, !bitRead(commandA.motorFlags, 2)); // Activates his flag
  enviarSPIA();
  commandFinished = false; // This variable contains if the command has been finished in the slave or not
}

void moveDistancia(int distancia, int vMax, int vFinal) {
  commandA.motor = constrain(abs(vFinal), 0, 100);
  if (vFinal < 0) bitSet(commandA.motor, 7); // Bit 7 high to indicate that it is negative
  commandA.distance = distancia;
  commandA.vMaxDistance = vMax;
  bitWrite(commandA.motorFlags, 3, !bitRead(commandA.motorFlags, 3)); // Activates his flag
  enviarSPIA();
  commandFinished = false; // This variable contains if the command has been finished in the slave or not
}

void waitDistancia (int distancia) {    // Waits a certain distance
  int distInicial = motorGetDistancia("D");
  while (motorGetDistancia("D") < distInicial + distancia) updateRobot();
}

void motorBreak(int vel) { // Breakes the car motor
  motorStartPower(vel);
  long tiempo = millis() + 200;
  while (millis() < tiempo) updateRobot();
  motorStartPower(0);
}

void pidGyro(int dirObj, float kp, float ki, float kd) { //Servo of MiniZ Range: 24 > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
  int grados = getCarAngle();                            //IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)

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
}

void pidGyro(int dirObj, float kp, float ki, float kd, int maxAngleDiff) { //Servo of MiniZ Range: 24 > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
  int grados = getCarAngle();                                              //IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)
  maxAngleDiff = constrain(maxAngleDiff, 0, 60);

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
  if (sensor.velocidad >= 0) direccion.write(constrain(90 + OFFSET_SERVO - (int)giro, 90 + OFFSET_SERVO - maxAngleDiff, 90 + OFFSET_SERVO + maxAngleDiff));
}

void pidCurva(int radio, float kp, float ki, float kd) { //Servo of MiniZ Range: 24 > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
  float dist;
  if (radio > 0) dist = motorGetDistancia("D") + 10;
  else dist = motorGetDistancia("I") + 10;
  int grados = getCarAngle();                                       //IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)
  error = (RAD_TO_DEG * (dist / radio)) - grados + dirAnteriorCurva;
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

void desplazamientoLateral(int direccionPID, int distancia) { // Direction: 0, 90, 180, 270
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


void paralelo(int direccionPID, bool direccion) { // direction == 0 CW, direction == 1 CCW
  int direccionMPU;
  if (direccionPID == 270) direccionMPU = -90;
  else direccionMPU = direccionPID;
  if (direccion == false) {
    while (abs(abs(getCarAngle()) - abs(direccionMPU)) < 45) {
      updateMPU();
      pidGyro(direccionPID + 50, 2, 0, 0, 45);
      enviarSPIA();
      enviarSPIB();
      updateLed();
    }
    moveDistancia(300, vMinSector, vMinSector);
    while (!commandFinished) {
      updateMPU();
      pidGyro(direccionPID + 45, 2, 0, 0);
      enviarSPIA();
      enviarSPIB();
      updateLed();
    }
  }
  else {
    while (abs(abs(getCarAngle()) - abs(direccionMPU)) < 45) {
      updateMPU();
      if (direccionPID == 0) pidGyro(310, 2, 0, 0, 45);
      else pidGyro(direccionPID - 50, 2, 0, 0, 45);
      enviarSPIA();
      enviarSPIB();
      updateLed();
    }
    moveDistancia(300, vMinSector, vMinSector);
    while (!commandFinished) {
      updateMPU();
      if (direccionPID == 0) pidGyro(315, 2, 0, 0);
      else pidGyro(direccionPID - 45, 2, 0, 0);
      enviarSPIA();
      enviarSPIB();
      updateLed();
    }
  }
  while (abs(abs(getCarAngle()) - abs(direccionMPU)) > 5) {
    updateMPU();
    pidGyro(direccionPID, 2, 0, 0, 45);
    enviarSPIA();
    enviarSPIB();
    updateLed();
  }
  beep(100);
}
