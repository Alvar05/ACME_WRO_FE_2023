#include <Servo.h>
#include <WS2812.h>
#include <SPI.h>
#include <cRGB.h>
#include <VL53L1X.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include "definiciones.h"

WS2812 TD(2);
WS2812 TI(2);
WS2812 FD(3);
WS2812 FI(3);
MPU6050 mpu(Wire);  //Initiates some objects
VL53L1X TOF_I;
VL53L1X TOF_D;
Servo servoCamara;
long tiempoServo = millis();
int i = 0;

void setup() {
  // put your setup code here, to run once: 
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  setupSPI();
  setupServo();
  setupLeds();
  Wire.setClock(400000);
  Wire.begin();
  setupMPU();
  setupHL();
  setupTOF();
  tiempoLED = millis() + 500;
  tiempoServo = millis() + 2000;
}

void loop() {
  // put your main code here, to run repeatedly:
  updateSPI();
  juntarFiltrarBloquesHL(command.filtroCam, 40);
  if (!flagSPI) {
    updateMPU();
    updateServo();
    updateLeds();
    i2c.TOF[0] = readMedianaTOF('I', 7);
    i2c.TOF[1] = readMedianaTOF('D', 7);
    i2c.bloqueCerca = readMedianaHL(indexBloques[0], 15, 'I');
  if (command.leerSector) i2c.sector = readSectorMediana(15);
  }
  if (millis() > tiempoLED) {
    digitalWrite(LED, !digitalRead(LED));
    tiempoLED = millis() + 200;
  }
}
