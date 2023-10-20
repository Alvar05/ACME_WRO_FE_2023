#include <Servo.h>
#include <WS2812.h>
#include <SPI.h>
#include <cRGB.h>
#include <VL53L1X.h>
#include "Wire.h"
#include "definiciones.h"
#include <HUSKYLENS.h>

WS2812 TD(2);
WS2812 TI(2);
WS2812 FD(3);
WS2812 FI(3);
VL53L1X TOF_I;
VL53L1X TOF_D;
long lecturasCam = 0;
Servo dirCam;
HUSKYLENS huskylens;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //while (!Serial);
  //Serial.println("Iniciando...");
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  setupSPI();
  setupServo();
  setupLeds();
  Wire.setClock(400000);
  Wire.begin();
  setupHL();
  setupTOF();
  //Serial.println("TOF: OK\nInicio completado");
  tiempoLED = millis() + 500;

}

void loop() {
  // put your main code here, to run repeatedly:
  updateSPI();
  if (!flagSPI) {
    updateServo();
    updateLeds();
    i2c.TOF[0] = readMedianaTOF('I', 3);
    i2c.TOF[1] = readMedianaTOF('D', 3);
    if (command.leerSector) {
      filtrarHL(command.filtroCam, wLimits, hLimits);
      juntarID(15);   // Descomentar si se usan varios ID por bloque (no hay limite de IDs por bloque en teoria)
      i2c.sector = readSectorMediana(7);
      lecturasCam++;
      huskylens.customText(i2c.sector + String("  "), 0, 0);
    }
  }
  if (micros() > tiempoLED) {
    digitalWrite(LED, !digitalRead(LED));
    tiempoLED = micros() + 200000;
  }
}
