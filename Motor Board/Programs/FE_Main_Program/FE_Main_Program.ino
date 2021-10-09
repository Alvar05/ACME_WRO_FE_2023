#include "definiciones.h"
#include <Servo.h>


Servo direccion;
datos sensores;
int indicePrograma = -1;
int direccionPID = 0;

void setup() {
  I2CSetup();
  sigueLineasSetup();
  pixySetup(); 
  direccion.attach(A1, 650, 2400);
  direccion.write(30);
  motorInvert("C");



  while (buttonsCurrent() != "U") {
    if (buttonsCurrent() == "D") {
      Serial.println("indicePrograma++");
      indicePrograma++;
      if (indicePrograma > 3) indicePrograma = 0;
      setLEDColor(15, "OFF", "NORMAL");
      setLEDColor(1 << indicePrograma, "BLUE", "NORMAL");
      while (buttonsCurrent() == "D");
    }
  }
  setLEDColor(15, "GREEN", "NORMAL");  delay(100);  setLEDColor(15, "OFF", "NORMAL");  delay(100);  setLEDColor(15, "GREEN", "NORMAL");  delay(100);  setLEDColor(15, "OFF", "NORMAL");
}


void loop() {
  updateLED();
  updatePixyPos();
  updateSerial();
  switch (indicePrograma) {
    case 0:   //PID recto con giroscopio  //Programa LED 1
      Serial.println(sensores.ypr[0]);
      direccion.write(pidGiroscopio(50, 0));
      break;

    case 1:   //PID diagonales con giroscopio //Programa LED 2
      Serial.println("1");
      static long tiempo = 0;
      Serial.println(posData[0][0] + String(" ") + sensores.ypr[0]);
      if (posData[0][0] < 350) {
        if (pixy.ccc.blocks[0].m_signature == 1) direccionPID = -45;
        if (pixy.ccc.blocks[0].m_signature == 2) direccionPID = 45;
      }
      else if (tiempo + 500 < millis()) {
        Serial.println("RECTO");
        direccionPID = 0;
        tiempo = millis();
      }

      direccion.write(pidGiroscopio(50, direccionPID));
      break;

    case 2: 
      //Serial.println("2");
      /*buttonsWait("U");
      calibrarPixy(0);
      buttonsWait("D");*/
      imprimirSensores();
      break;

    case 3:
      Serial.println("3");
      motorStartPower("BC", velocidad);
      break;
  }

}
