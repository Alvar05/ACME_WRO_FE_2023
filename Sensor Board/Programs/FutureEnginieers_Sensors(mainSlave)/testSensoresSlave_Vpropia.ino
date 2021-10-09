#include "I2Cdev.h"
#include <Wire.h>
#include "definiciones.h"

datos sensores;

void setup() {
  Wire.begin(8);
  Wire.setClock(400000);
  Serial.begin(115200);           // start serial for output
  Wire.onReceive(receiveEvent); // Cuando recibe del master
  delay(1000);
}

void loop() {
  imprimirSensores();
  delay(1000);
}
