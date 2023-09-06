#include "definiciones.h"

bool robotActivo = false;
unsigned long buenaTramaA = 0;
unsigned long errorTramaA = 0;
unsigned long buenaTramaB = 0;
unsigned long errorTramaB = 0;
bool estadoFreno = 1;
long tiempoBeep = 0;
bool beepDone = false;
long tiempoLED;

Servo direccion;

void setup(void) {
  Serial.begin(115200);
  direccion.attach(DIR);
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  setupSPI();
  tiempoLED = millis() + 500;
  digitalWrite(LED, 1);
  digitalWrite(BUZZ, 1);
  direccion.write(84);
  delay(100);
  digitalWrite(BUZZ, 0);
  delay(6000);
  luzFrontal('C');
  digitalWrite(BUZZ, 1);
  delay(100);
  digitalWrite(BUZZ, 0);
  delay(100);
  digitalWrite(BUZZ, 1);
  delay(100);
  digitalWrite(BUZZ, 0);
  while (digitalRead(BTN)) {
    enviarSPIA();
    enviarSPIB();
    updateLed;
    if (sensor.sr04[0] != 0 and sensor.sr04[2] != 0 and disPared[0] == 255) {
      leerInicio();
      if (disPared[0] == 1) {
        if (sentido == 0) intermitente('D');
        else intermitente('I');
      }
    }
  }
  delay(1000);
  motorResetCount("ID");
  enviarSPIB();
  offsetMPU = i2c.MPU;
  if (disPared[0] == 0) {
    moveDistancia(sensor.sr04[1] - 60, 10, 0);
    while (motorGetDistancia("D") < sensor.sr04[1] - 60) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(0, 1, 0, 0);
      if (posInicial == 2) { // OPCIONES SI AUN NO PODEMOS SABER EL SENTIDO
        if (sensor.sr04[0] > 100)sentido = 180;
        else if (sensor.sr04[2] > 100) sentido = 0;
      }
      else if (posInicial == 1) {
        if (sensor.sr04[0] < 100) sentido = 180;
        else sentido = 0;
      }
      else {
        if (sensor.sr04[2] < 100) sentido = 0;
        else sentido = 180;
      }
    }
    motorStart(2);
    while (sentido == -1) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(0, 1, 0, 0);
      if (sensor.sr04[0] > 100)sentido = 180;
      else if (sensor.sr04[2] > 100) sentido = 0;
    }
    motorStartPower(0);
  }
  else moveDistancia(sensor.sr04[1] - 60, 10, 10);
  setVelocidad(5);
  if (sentido == 180) {
    while (getCarAngle() > -80) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(270, 1, 0, 0);
    }
  }
  else {
    while (getCarAngle() < 80) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(270, 1, 0, 0);
    }
  }
  if (((sentido == 0) and (posInicial == 1)) or ((sentido == 180) and (posInicial == 3))) {
    motorResetCount("D");
    while (motorGetDistancia("D") < 80) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(90 + sentido, 1, 0, 0);
    }
  }
  else if (posInicial == 2) {
    motorResetCount("D");
    while (motorGetDistancia("D") < 50) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(90 + sentido, 1, 0, 0);
    }
  }
  else {
    motorResetCount("D");
    while (motorGetDistancia("D") < 30) {
      enviarSPIA();
      enviarSPIB();
      pidGyro(90 + sentido, 1, 0, 0);
    }
  }
  while (((sensor.sr04[0] > 100) or (sensor.sr04[2] > 100))) {
    enviarSPIA();
    enviarSPIB();
    pidGyro(90 + sentido, 1, 0, 0);
  }
  if (sentido == 0) TOFsentido = 1; // Si no ya está predeterminado a 0
  if ((sensor.sr04[0] + sensor.sr04[2]) > 70) disPared[1] = 0;
  else disPared[1] = 1;
  motorStartPower(0);
  intermitente('E');
}

void loop(void) {
  enviarSPIA();
  enviarSPIB();
}

void updateLed() {
  if (millis() > tiempoLED) {
    digitalWrite(LED, !digitalRead(LED));
    tiempoLED = millis() + 200;
  }
}
