//Main program for the Qualifying round. This program needs I2C communication with the ProMicro controller 
// Includes the other tabs and some libraries needed
#include <VL53L1X.h>
#include <math.h>
#include <ESP32Servo.h>
#include <M5Stack.h>
#include "I2Cdev.h"
#include "Wire.h"
#include "definiciones.h"
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);  //Initiates some objects

Servo direccion;
VL53L1X tof;

int vueltas = 0; // Lap counter
int distanciaTof = 0;

void setup() {
  // Initializes the robot
  M5.begin();
  M5.Lcd.begin();
  M5.Power.begin();

  Serial.begin(115200);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Speaker setup...");
  speakerSetup();
  M5.Lcd.print(" OK\nMPU setup y offsets...");
  setupMPU();
  M5.Lcd.print(" OK\nServo y ESC setup...");
  setupServo();
  setupESC();
  M5.Lcd.print(" OK\nInterrupciones...");

  pinMode(ENC_B, INPUT);
  pinMode(ENC_C, INPUT);
  pinMode(PIN_INTERRUPCION, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoderB, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_C), encoderC, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPCION), sensorInterrupt, RISING);
  M5.Lcd.print(" OK\nListo");

  Wire.setClock(400000); // use 400 kHz I2C
  tof.setTimeout(500);
  if (!tof.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
  tof.setDistanceMode(VL53L1X::Long);
  tof.setMeasurementTimingBudget(50000);

  tof.startContinuous(50);

  infoPantallaSetup();

  M5.Lcd.setTextSize(5);
  while (!M5.BtnB.isPressed()) { //Waits for central button to be pressed and start the round
    M5.update();
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
  }
  delay(1000);
  loopMPU();
  offsetMPU = ypr[0]; //Takes angle offsets
  M5.Lcd.clear();

  motorResetCount("BC");
  motorResetCountTotal("BC");

  sensorUpdate();
  infoPantallaSetup();
  debugPantalla = false;
  ////////Here starts the round/////////
  enviarI2C('S'); //Sends a flag to ProMicro to return the way of the round ('A' for CCW and 'H' for CW) 
  motorStartPower(10);
}


void loop() {
  // put your main code here, to run repeatedly:
  sensorUpdate();
  if (vueltas == 0) { //Checks the direction in the first corner
    rectaAcel(sensor.sr04[1] - 60, 5, 15, 5, 0);
    speakerBeep(1200, 50);
    while (sensor.direccion == 'N') { //Waits until ProMicro returns the direction character
      motorStartPower(5);
      sensorUpdate();
      speakerUpdate();
    }
    if (sensor.direccion == 'A') { //If direction is CCW, sets a variable to change the way
      sentido = 180;
    }
  }
  else {
    while (distanciaTof < 700) { ////First turn (2nd and 3rd laps)
      if (tof.dataReady()) distanciaTof = tof.read(false);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.print(distanciaTof + String("   "));
      sensorUpdate();
      direccion.write(pidGyro(0, 1, 0, 0));
    }
  }
  motorResetCount("B");
  while (motorGetDistancia("B") < 80) {
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    speakerUpdate();
    direccion.write(pidGyro(90 + sentido, 1, 0, 0));
  }

  
  sensorUpdate();
  while (distanciaTof < 700) { ////Second turn
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    sensorUpdate();
    direccion.write(pidGyro(90 + sentido, 1, 0, 0));
  }
  speakerBeep(1200, 100);
  motorResetCount("B");
  while (motorGetDistancia("B") < 80) {
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    speakerUpdate();
    direccion.write(pidGyro(180, 1, 0, 0));
  }



  sensorUpdate();
  while (distanciaTof < 700) {////Third turn
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    sensorUpdate();
    direccion.write(pidGyro(180, 1, 0, 0));
  }
  speakerBeep(1200, 100);
  motorResetCount("B");
  while (motorGetDistancia("B") < 80) {
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    speakerUpdate();
    direccion.write(pidGyro(270 - sentido, 1, 0, 0));
  }



  sensorUpdate();
  while (distanciaTof < 700) {////Fourth turn
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    sensorUpdate();
    direccion.write(pidGyro(270 - sentido, 1, 0, 0));
  }
  speakerBeep(1200, 100);
  motorResetCount("B");
  while (motorGetDistancia("B") < 80) {
    if (tof.dataReady()) distanciaTof = tof.read(false);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(distanciaTof + String("   "));
    speakerUpdate();
    direccion.write(pidGyro(0, 1, 0, 0));
  }
  vueltas++;
  if (vueltas == 3) {
    while (1) motorStartPower(0); //Stops the robot and ends the round
  }
}
