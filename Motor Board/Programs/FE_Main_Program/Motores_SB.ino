
void motorInvert(String motor) { //Inversion de giro motor
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'B') {
      sentidoB = false;
    }
    else {
      sentidoC = false;
    }
  }
}

void motorStartPower(String motor, int potencia) { //Bloque de potencia
  potencia = constrain (potencia, -100, 100);
  for (int i = 0; i < motor.length(); i++) {

    if (motor.charAt(i) == 'B') {
      if (potencia < 0) digitalWrite(AIN1, !sentidoB);
      else digitalWrite(AIN1, sentidoB);
      analogWrite(PWMA, map(abs(potencia), 0, 100, 0, 255));
    }
    else {
      if (potencia < 0)digitalWrite(BIN1, !sentidoC);
      else digitalWrite(BIN1, sentidoC);
      analogWrite(PWMB, map(abs(potencia), 0, 100, 0, 255));
    }
  }
}

void motorStop(String motor, bool freno) {
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'B') {
      analogWrite(PWMA, 0);
      if (freno == true) {
        delay(5);
        digitalWrite(AIN1, !digitalRead(AIN1));
        analogWrite(PWMA, 127);
        delay(5);
        analogWrite(PWMA, 0);
      }
    }

    else {
      analogWrite(PWMB, 0);
      if (freno == true) {
        delay(5);
        digitalWrite(BIN1, !digitalRead(BIN1));
        analogWrite(PWMB, 127);
        delay(5);
        analogWrite(PWMB, 0);
      }
    }
  }
}

long motorGetCount(String motor) {                //Leer grados de motor
  if (motor == "B")return encoderBcontador;
  else return encoderCcontador;
}
void motorResetCount(String motor) {            //Reiniciar Grados Motor
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'B') encoderBcontador = 0;
    else encoderCcontador = 0;
  }
}
void encoderC () {                //Incremento del contador ISR grados en C
  if (sentidoC == true) {
    if ( digitalRead(BIN1) == 1) {
      encoderCcontador += 2;
    }
    else  encoderCcontador -= 2;
  }
  else {
    if ( digitalRead(BIN1) == 1) {
      encoderCcontador -= 2;
    }
    else  encoderCcontador += 2;
  }
}
void encoderB () {                     //Incremento del contador ISR grados en B
  if (sentidoB == true) {
    if ( digitalRead(AIN1) == 1) {
      encoderBcontador = encoderBcontador + 2;
    }
    else  encoderBcontador = encoderBcontador - 2;
  }
  else {
    if ( digitalRead(AIN1) == 1) {
      encoderBcontador = encoderBcontador - 2;
    }
    else  encoderBcontador = encoderBcontador + 2;
  }
}
/*void encoderC() {
  static unsigned long microsAnteriorC = 0;
  incrementoC = micros() - microsAnteriorC;
  microsAnteriorC = micros();
  if (sentidoC != digitalRead(BIN1)) encoderCcontador += 30;
  else encoderCcontador -= 30;
}

void encoderB() {
  static unsigned long microsAnteriorB = 0;
  incrementoB = micros() - microsAnteriorB;
  microsAnteriorB = micros();
  if (sentidoB != digitalRead(AIN1)) encoderBcontador += 30;
  else encoderBcontador -= 30;
}*/

void motorMove(String motor, int velocidad, long grados, bool freno) {
  motorResetCount(motor);
  motorStartPower(motor, velocidad);
  if (motor.charAt(0) == 'B') while (abs(motorGetCount("B")) < grados);
  else while (abs(motorGetCount("C")) < grados);
  motorStop(motor, freno);
}

int motorGetPower(String motor) {  // devuelve la velocidad de -100 a 100
  int vel = 0;
  if (motor == "B") vel = (incrementoMinimoEntrePulsos * 100) / incrementoB;
  if (motor == "C") vel = (incrementoMinimoEntrePulsos * 100) / incrementoC;
  if (vel > 100) return 0;
  if (sentidoB == digitalRead(AIN1)) vel = vel * (-1) ;
  if (sentidoC == digitalRead(BIN1)) vel = vel * (-1) ;
  else return vel;
}

void motorStart(String motor, int velocidad) {
  float kp = 1.6, ki = 0.00, kd = 2.00; // definir de forma local
  error = motorGetPower(motor) - velocidad;
  proporcional = error * kp;
  errorSuma = errorSuma + error;
  if (error > -10 and error < 10) errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) *kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower(motor, velocidad - (int)giro);
}

void motorStartSync (String motor, int velocidad) {
  const float kp = 0.20, ki = 0.00, kd = 2.00; // definir de forma local
  error = motorGetCount("B") - motorGetCount("C");
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -10 and error < 10)errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("B", velocidad - (int)giro);
  motorStartPower("C", velocidad + (int)giro);
}
