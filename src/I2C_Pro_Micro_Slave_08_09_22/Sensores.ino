//This tab contains methods to update and print in serial all data from different sensors (Mainly ultrasonics)
void sensorSetup() {

}
void sensorUpdate() { //Updates to the I2C variables the sensor values
  if (millis() > timeOut) digitalWrite(PIN_INTERRUPCION, 0);
  /*
    //Pines Analogicos
    for (int i = 0; i < sizeof(pinesAnalogicos); i++) {
      sensor.analogico[i] = analogRead(pinesAnalogicos[i]);
    }
    //Pines Digitales
    for (int i = 0; i < sizeof(pinesDigitales); i++) {
      sensor.digital[i] = digitalRead(pinesDigitales[i]);
    }
    }*/
  //SR04
  for (int i = 0; i < 3; i++) {
    int distancia = sonar[i].ping_cm();
    sensor.sr04[i] = distancia;
    if (distancia == 0) sensor.sr04[i] = 255;
    delay(30);

    digitalWrite(PIN_INTERRUPCION, 1);
    delay(5);
  }
}
void imprimirSensores() { //Prints by Serial the sensor data

  Serial.print(String("Izquierdo=") + sensor.sr04[0] + String("  Centro=") + sensor.sr04[1] + String("  Derecho=") + sensor.sr04[2]);
  Serial.println();
  Serial.print(String("SueloIzquierdo=") + sensor.suelo[0] + String("  SueloDerecho=") + sensor.suelo[1]);
  Serial.println();
  for (int i = 0; i < 4; i++) {
    Serial.print( String("AV") + i + String("=") + sensor.analogico[i] + String(" "));
  }
  Serial.println();
  for (int i = 0; i < 3; i++) {
    Serial.print(String("DV") + i + String("=") + sensor.digital[i] + String(" "));
  }
  Serial.println();
  Serial.println();
}
