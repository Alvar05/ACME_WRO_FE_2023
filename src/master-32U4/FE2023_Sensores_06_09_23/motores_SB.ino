void motorSetup() {
  pinMode(ENC_MA, INPUT_PULLUP);
  pinMode(ENC_MB, INPUT_PULLUP);
  pinMode(ENC_D, INPUT_PULLUP);
  pinMode(ENC_I, INPUT_PULLUP);
  pinMode(PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_DIS, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_MA), encoderM, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_D), encoderD, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_I), encoderI, RISING);
}

void encoderM() {
  if (!digitalRead(MOTOR_DIR)) encoderMcontador++;
  else encoderMcontador--;
  incrementoMotor = micros() - microsAnteriorMotor;
  microsAnteriorMotor = micros();
}

void encoderD() {
  encoderDcontador++;
  encoderDslave++;
}

void encoderI() {
  encoderIcontador++;
  encoderIslave++;
}

long motorGetCount(String motor) {                //Returns the pulses done by the tyre
  if (motor == "D")return encoderDcontador;
  else if (motor == "I")return encoderIcontador;
  else if (motor == "M")return encoderMcontador;
}

long motorGetSlaveCount(String motor) {                //Returns the pulses done by the tyre
  if (motor == "D")return encoderDslave;
  else if (motor == "I")return encoderIslave;
}

void motorResetCount(String motor) {            //Restarts the encoder counter
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'D') encoderDcontador = 0;
    else if (motor.charAt(i) == 'I') encoderIcontador = 0;
    else if (motor.charAt(i) == 'M') encoderMcontador = 0;
  }
}

void motorResetSlaveCount(String motor) {            //Restarts the encoder counter
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'D') encoderDslave = 0;
    else if (motor.charAt(i) == 'I') encoderIslave = 0;
  }
}

long motorGetSlaveDistance(String motor) {                //Returns the pulses done by the tyre
  if (motor == "D")return ((encoderDslave / PULSOS_ENC) * PI * diametroRueda);
  else if (motor == "I")return ((encoderIslave / PULSOS_ENC) * PI * diametroRueda);
}

unsigned int motorGetDistancia(String motor) {            //Returns the distance in mm done by the tyre
  if (motor == "D")return ((encoderDcontador / PULSOS_ENC) * PI * diametroRueda);
  else if (motor == "I")return ((encoderIcontador / PULSOS_ENC) * PI * diametroRueda);
}

void updateMotor() {
  long diferenciaMicros = micros() - microsAnteriorMotor;
  if (diferenciaMicros > incrementoMaximoEntrePulos) {
    incrementoMotor = 100000; // Con este valor la velocidad se hace 0
  }
}

void motorStartPower(int vel) {
  vel = constrain(vel, -100, 100);
  int velPwm = map (abs(vel), 0, 100, 0, 255);
  if (vel < 0) digitalWrite(MOTOR_DIR, 1);
  else if (vel > 0)digitalWrite(MOTOR_DIR, 0);
  analogWrite (PWM, velPwm);
}

int motorGetSpeed() {  // devuelve la velocidad de -100 a 100
  int vel = 0;
  if (incrementoMotor > 10000) vel = (incrementoMinimoEntrePulsos * 100) / incrementoMotor;
  else vel = (10000 / (incrementoMotor - 550) + 6);
  vel = constrain(vel, -100, 100);
  if (digitalRead(MOTOR_DIR)) vel = vel * (-1);
  return vel;
}

void motorStart(int velocidad) {
  if (velocidad == 0) {
    motorStartPower(0);
    return;
  }
  float kp = 3, ki = 0.01, kd = 3; // definir de forma local
  velocidad = constrain(velocidad, -100, 100);
  updateMotor();
  error = motorGetSpeed() - velocidad;
  proporcional = error * kp;
  errorSuma = errorSuma + error;
  if (errorAnterior * error < 0) errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  if (velocidad > 0) {
    motorStartPower(constrain(velocidad - (int)giro, 0, 100));
  }
  else if (velocidad < 0) {
    motorStartPower(constrain(velocidad - (int)giro, -100, 0));
  }
}

bool acelExpo(int velInicial, int velFinal) {          //Also returns 0 y 1 as the acelLineal() does
  velInicial = constrain(velInicial, 2, 100);
  velFinal = constrain(velFinal, 0, 100);
  const float distInicial = (velInicial * MIN_ACCEL_DISTANCE) / 100; // Distancia de offset para empezar en la velocidad inicial
  float velocidad = ((distInicial + motorGetSlaveDistance("D")) / MIN_ACCEL_DISTANCE) * 100;
  if (velocidad <= velFinal) {
    if (velocidad >= V_MAX_PID) motorStartPower(velocidad);
    else motorStart(velocidad);
    return 0;
  }
  else return 1;
}

bool desacelExpo(int velInicial, int velFinal) { //Same as acelExpo but to decelerate
  velInicial = constrain(velInicial, 0, 100);
  velFinal = constrain(velFinal, 0, 100);
  if (velFinal == 0) {
    desacelBreak = true;
    velFinal = MIN_V_TO_BREAK;
  }
  const float distInicial = ((100 - velInicial) / 100.00) * MIN_DESACCEL_DISTANCE; // Grados de offset para empezar en la velocidad inicial
  float velocidad = (((motorGetSlaveDistance("D") + distInicial) / MIN_DESACCEL_DISTANCE) * (-100)) + 100;
  if (velocidad >= velFinal) {
    if (velocidad > V_MAX_PID) motorStartPower(velocidad);
    else motorStart(velocidad);
    return 0;
  }
  else {
    if (desacelBreak) motorStartPower(0);
    desacelBreak = false;
    return 1;
  }
}

void motorBreak() {
  digitalWrite(MOTOR_DIR, !digitalRead(MOTOR_DIR));
  analogWrite (PWM, 0);
  activeDistance = false;
  activeRampa = false;
  activePID = false;
}

void updateRampa() {
  if (vFinal > vInicial) {
    if (acelExpo(vInicial, vFinal)) {
      activeRampa = false;
      sensor.lastCommandFlag = !sensor.lastCommandFlag;
      if (vFinal <= V_MAX_PID) {
        speed = vFinal;
        activePID = true;
      }
    }
  }
  else if (vFinal < vInicial) {
    if (desacelExpo(vInicial, vFinal)) {
      activeRampa = false;
      if (vFinal <= V_MAX_PID) {
        speed = vFinal;
        activePID = true;
      }
      sensor.lastCommandFlag = !sensor.lastCommandFlag;
    }
  }
  else {
    activeRampa = false;
    sensor.lastCommandFlag = !sensor.lastCommandFlag;
    if (vFinal <= V_MAX_PID) {
      speed = vFinal;
      activePID = true;
    }
    else motorStartPower(vFinal);
  }
}

bool acelExpoCapado(int velInicial, int velMax, int velFinal) {          //Also returns 0 y 1 as the acelLineal() does
  velInicial = constrain(velInicial, 2, 100);
  const unsigned long distInicial = (velInicial * MIN_ACCEL_DISTANCE) / 100; // Distancia de offset para empezar en la velocidad inicial
  long velocidad = ((distInicial + motorGetSlaveDistance("D") * 100) / MIN_ACCEL_DISTANCE);
  if (velocidad <= velFinal) {
    velocidad = constrain(velocidad, 1, velMax);
    if (velocidad >= V_MAX_PID) motorStartPower(velocidad);
    else motorStart(velocidad);
    return 0;
  }
  else return 1;
}

bool desacelExpoCapado(int velInicial, int velMax, int velFinal) { //Same as acelExpo but to decelerate
  velFinal = constrain(velFinal, 0, 100);
  if (velFinal == 0) {
    desacelBreak = true;
    velFinal = MIN_V_TO_BREAK;
  }
  const float distInicial = ((100 - velInicial) / 100.00) * MIN_DESACCEL_DISTANCE; // Grados de offset para empezar en la velocidad inicial
  float velocidad = (((motorGetSlaveDistance("D") + distInicial) / MIN_DESACCEL_DISTANCE) * (-100)) + 100;
  if (velocidad >= velFinal) {
    velocidad = constrain(velocidad, 0, velMax);
    if (velocidad > V_MAX_PID) motorStartPower(velocidad);
    else motorStart(velocidad);
    return 0;
  }
  else {
    if (desacelBreak) motorBreak();
    desacelBreak = false;
    return 1;
  }
}

void updateDistancia() {
  Serial.println(sensor.lastCommandFlag);
  if (activeDistAcel) {
    if (acelExpoCapado(vInicial, command.vMaxDistance, vMaxDist)) {
      //Serial.println(motorGetSlaveDistance("D"));
      motorResetSlaveCount("D");
      activeDistAcel = false;
    }
  }
  else {
    if (desacelExpoCapado(vMaxDist, command.vMaxDistance, vFinal)) {
      Serial.println("ACABADO");
      Serial.println(sensor.lastCommandFlag);
      activeDistance = false;
      sensor.lastCommandFlag = !(sensor.lastCommandFlag);
      Serial.println(sensor.lastCommandFlag);
      if (vFinal <= V_MAX_PID) {
        speed = vFinal;
        activePID = true;
      }
      else motorStartPower(vFinal);
      //Serial.println(motorGetSlaveDistance("D"));
    }
  }
}
