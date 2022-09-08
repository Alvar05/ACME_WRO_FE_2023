#include <ESP32Servo.h>
#include <math.h>
Servo esc;

void setupESC() {
  esc.setPeriodHertz(50);
  esc.attach(ESC, 1000, 2000);  //El attach es como cualquier servo
  esc.writeMicroseconds(2000); // | Los writeMicros de 2000 y 1200 son para programar el esc y se tiene que hacer siempre asi y en este orden y
  delay(50);                   // | con un pequeño delay entre medio
  esc.writeMicroseconds(1200); // | Y antes de poder controlar el esc se tiene que dejar otro delay (200 milis van bien)
  delay(200);
}

void motorInvert() { //Inverts the motor direction
  Serial.println("PLEASE, SWITCH THE JUMPER, DON'T DO THIS TO ME");
}

void motorStartPower(int potencia) { //Runs the motor at certain power
  potencia = constrain (potencia, -100, 100);
  if (potencia > 0) esc.writeMicroseconds(map(potencia, 0, 100, 1200, 2000));
  else esc.writeMicroseconds(map(abs(potencia), 0, 100, 1200, 1000));
}

void motorStop(bool freno) { //Motor break (don't recommended because it can break plastic gears, is better using motorStartPower(0);)
  esc.writeMicroseconds(1200);
  if (freno == true) {
    delay(5);
    if (map(esc.read(), 0, 180, 1000, 2000) > 1200) esc.writeMicroseconds(1190);
    else esc.writeMicroseconds(11960);
    delay(5);
    esc.writeMicroseconds(1200);
  }
}

long motorGetCount(String motor) {                //Returns the angle done by the tyre
  if (motor == "C")return encoderCcontador;
  else if (motor == "B")return encoderBcontador;
}

int motorGetDistancia(String motor) {            //Returns the distance in cm done by the tyre
  if (motor == "C")return ((encoderCcontador / 360.00) * PI * diametroRueda);
  else if (motor == "B")return ((encoderBcontador / 360.00) * PI * diametroRueda);
}

int motorGetDistanciaTotal(String motor) {            //Same as motorGetDistancia() but used to have an absolut value
  if (motor == "C")return ((encoderCtotal / 360.00) * PI * diametroRueda);
  else if (motor == "B")return ((encoderBtotal / 360.00) * PI * diametroRueda);
}

void motorResetCount(String motor) {            //Restarts the encoder counter
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'C') encoderCcontador = 0;
    else if (motor.charAt(i) == 'B') encoderBcontador = 0;
  }
}

void motorResetCountTotal(String motor) {            //Restarts the absolut encoder counter
  for (int i = 0; i < motor.length(); i++) {
    if (motor.charAt(i) == 'C') encoderCtotal = 0;
    else if (motor.charAt(i) == 'B') encoderBtotal = 0;
  }
}

void encoderB () {                     //ISR to increment encoderB counter (if motor is in reverse, counts negative) 
  if (map(esc.read(), 0, 180, 1000, 2000) < 1200) {
    encoderBcontador -= 120;
    encoderBtotal -= 120;
  }
  else  {
    encoderBcontador += 120;
    encoderBtotal += 120;
  }
}

void encoderC () {                  //ISR to increment encoderC counter (if motor is in reverse, counts negative) 
  if (map(esc.read(), 0, 180, 1000, 2000) < 1200) {
    encoderCcontador -= 120;
    encoderCtotal -= 120;
  }
  else  {
    encoderCcontador += 120;
    encoderCtotal += 120;
  }
}

void motorMoveDistancia(int velocidad, int distancia) { // Moves the robot a certain distance
  motorResetCount("B");
  motorStartPower(velocidad);
  while (abs(motorGetDistancia("B")) < distancia);
  motorStartPower(0);
}

int pidGyro(int dirObj, float kp, float ki, float kd) { //Servo of MiniZ Range: 30º > Servo Max Left | 87º > Servo Center | 144 > Servo Max Right
  loopMPU();
  int grados = 0;                                       //IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)
  grados = ypr[0];

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
  return constrain(90 - (int)giro, 30, 144);
}

void setRadioGiro(float radio) { //Selects the maximum angle of the servo in pidGyroCapado() to steer with the selected radius
  if (radio == 54) anguloMaximo = 65;
  if (radio == 33.5) anguloMaximo = 50; //These two are the most common used
  if (radio == 24.5) anguloMaximo = 30; //"""""""
}

int pidGyroCapado(int dirObj, float kp, float ki, float kd) {//IMPORTANT: To use this PID we ahve to use setRadioGiro() before to set correctly the max angle
  loopMPU();
  int grados = 0;                                       ///IMPORTANT: Direction input has to be (0º, 90º, 180º, 270º)
  grados = ypr[0];

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
  return constrain(90 - (int)giro, anguloMaximo, (174 - anguloMaximo));
}

bool acelLineal(float kAcel, int velInicial, int velFinal) {  // kAcel is the accel constant increase of vel(%) by second)
  static float velAcel = velInicial;                        // Function returns 1 if the accel is finished (max velocity)
  velAcel += kAcel / 100;                                   // and 0 if its running (keeps accelerating)
  if (velAcel <= velFinal) {
    int velocidadEsc = map(velAcel, 0, 100, 1200, 2000);
    velocidadEsc = constrain(velocidadEsc, 1200, 2000);
    esc.writeMicroseconds(velocidadEsc);
    Serial.println(velAcel + String(" ") + velocidadEsc);
    delay(10);
    return 0;
  }
  else return 1;
}

bool acelExpo(int aceleracion, int velInicial, int velFinal) {          //Also returns 0 y 1 as the acelLineal() does 
  float grados = ((float)aceleracion * 360.00) / (diametroRueda * PI);  // aceleracion is the distance you want to accelerate
  float velocidad = ((motorGetCount("B") / grados) * (velFinal - velInicial)) + velInicial;
  if (velocidad <= velFinal) {
    int velocidadEsc = map(velocidad, 0, 100, 1200, 2000);
    velocidadEsc = constrain(velocidadEsc, 1200, 2000);
    esc.writeMicroseconds(velocidadEsc);
    return 0;
  }
  else return 1;
}

bool desacelExpo(int aceleracion, int velInicial, int velFinal) { //Same as acelExpo but to decelerate
  float grados = ((float)aceleracion * 360.00) / (diametroRueda * PI);
  float velocidad = (((grados - motorGetCount("B")) / grados) * (velInicial - velFinal)) + velFinal;
  if (velocidad >= velFinal) {
    int velocidadEsc = map(velocidad, 0, 100, 1200, 2000);
    velocidadEsc = constrain(velocidadEsc, 1200, 2000);
    esc.writeMicroseconds(velocidadEsc);
    return 0;
  }
  else return 1;
}

void desplazamientoLateral(float desplazamiento, float radioGiro) { //Returns the angle in wich the robot moves X distance(cm) laterally
  anguloDesplazamiento = RAD_TO_DEG * acos((1 - (desplazamiento / radioGiro))) ;
}

void rectaAcel (int distancia, int vInicial, int vCrucero, int vFinal, int direccionPid) { // Makes a straigth accel, cruising speed, and decel in a certain distance
  motorResetCount("B");
  while (acelExpo(50, vInicial, vCrucero) == 0) {
    direccion.write(pidGyro(direccionPid, 1, 0, 0));
  }
  while (motorGetDistancia("B") < (distancia - 50)) {
    direccion.write(pidGyro(direccionPid, 1, 0, 0));
    //if (sensor.direccion != 'N') break;
  }
  while (desacelExpo(distancia, vCrucero, vFinal) == 0) {
    direccion.write(pidGyro(direccionPid, 1, 0, 0));
    //if (sensor.direccion != 'N') break;
  }
}
