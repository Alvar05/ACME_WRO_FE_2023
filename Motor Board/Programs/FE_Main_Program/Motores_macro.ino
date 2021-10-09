
void iniciarPid() {
  kp = 0.70;
  ki = 0.00;
  kd = 5.00;


}

void  pidArray(int velocidad) {
  velocidad = constrain(velocidad, -100, 100);
  error = 2500 - qtrReadPosition();
  error = map(error, -2500, 2500, -100, 100);
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("B", velocidad + (int)giro);
  motorStartPower("C", velocidad - (int)giro);
}

void  pidSensor(String sensor, int velocidad) {
  velocidad = constrain(velocidad, -100, 100);
  int numerico = sensor.toInt();
  int sensorD = numerico / 10;
  int sensorI = numerico % 10;
  /*if(sensor.length()>1) */error = sensorReadPercent(sensorD) - sensorReadPercent(sensorI);
  //else sensorReadPercent(sensorI) - 50;
  //Serial.println(error);
  // delay(100);
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("B", velocidad + (int)giro);
  motorStartPower("C", velocidad - (int)giro);
}

void pidSensor(byte sensor, int velocidad, int lado) {
  velocidad = constrain(velocidad, -100, 100);
  error = sensorReadPercent(sensor) - 50; 
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("B", velocidad + (lado*(int)giro));
  motorStartPower("C", velocidad - (lado*(int)giro));
}

int pidGiroscopio(int velocidad, int direccion) {
  velocidad = constrain(velocidad, -100, 100);
  error = direccion - sensores.ypr[0]; 
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("C", velocidad);
  //Serial.println(30 - (int)giro);
  return constrain(30 - (int)giro, 0, 60);
}

int pidUltrasonico(int velocidad, int puertoSR04, int distObjetivo) {
  kp = 1;
  ki = 0;
  kd = 0;
  velocidad = constrain(velocidad, -100, 100);
  error = distObjetivo - sensores.sr04[puertoSR04]; 
  proporcional = error * kp;
  errorSuma = error + errorSuma;
  if (error > -20 and error < 20)  errorSuma = 0;
  integral = errorSuma * ki;
  derivativo = (error - errorAnterior) * kd;
  errorAnterior = error;
  giro = proporcional + integral + derivativo;
  motorStartPower("C", velocidad);
  //Serial.println(constrain(30 - (int)giro, 0, 60) + String(" ") + sensores.sr04[puertoSR04]);
  return constrain(30 - (int)giro, 0, 60);
}
