#include "definiciones.h"
int errores = 0;
long errorTrama = 0;
long buenaTrama = 0;
long timeOut;
long tiempoLED;
bool motorActivo = false;
long microsMotor = 100000;

void setup() {
  pinMode(LED, OUTPUT);
  motorSetup();
  setupSPI();
  sensorSetup();
  tiempoLED = millis() + 500;
  digitalWrite(LED, 1);
  delay(100);
}

void loop() {
  if (activePID) {
    motorStart(speed);
  }
  else if (activeRampa) {
    updateRampa();
  }
  else if (activeDistance) {
    updateDistancia();
  }
  sensorUpdate();
  updateSPI();
  if (millis() > tiempoLED) {
    digitalWrite(LED, !digitalRead(LED));
    tiempoLED = millis() + 200;
  }
}
