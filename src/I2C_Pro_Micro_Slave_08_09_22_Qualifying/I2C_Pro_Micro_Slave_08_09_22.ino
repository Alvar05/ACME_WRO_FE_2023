// Program for the ProMicro in Slave mode to communicate by I2C with the Main controller (M5Stack)
#include<Wire.h>
#include"definiciones.h"


void setup() {
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  i2cSetup();
  sensorSetup();
  timeOut = millis() + TIEMPO_TIMEOUT;
}

void loop() {
  if (accion == 'S') { ////Establishes the way of the round when 'S' recieved from M5Stack
    if (sensor.sr04[0] > 100 or sensor.sr04[2] > 100) {
      if (sensor.sr04[0] > 100) sensor.direccion = 'A';
      else sensor.direccion = 'H';
      accion = 'X';
    }
  }
  if (accion == 'N') { ////Delete the way configuration when 'N' recieved from M5Stack
    sensor.direccion = 'N';
    accion = 'X';
  }
  sensorUpdate();

  //imprimirSensores(); 
  //delay(1000);
}
