#include "definiciones.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector

#include <NewPing.h>
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(TRIGGER_IZQUIERDO, ECHO_IZQUIERDO, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_CENTRO, ECHO_CENTRO, MAX_DISTANCE),
  NewPing(TRIGGER_DERECHO, ECHO_DERECHO, MAX_DISTANCE)
};

// Objeto de la estructura para pasar los datos
datos sensores;
pin pines ;


void setup() {
  Serial.begin(115200);
  //while (!Serial);
  //Encoders
  pinMode(ENCODERB, INPUT_PULLUP);
  pinMode(ENCODERC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODERB), encoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODERC), encoderC, CHANGE);
  //Pines digitales.  Ir a definiciones para cambiar
  for (int i=0; i<sizeof(pinesDigitales);i++){
    pinMode(pinesDigitales[i],INPUT);
  }
  //MPU
  setupMPU();
}

void loop() {
  loopMPU(); // Necesario para actualizar el YPR
  //testUltra();
  //testEncoder();
  updateSensores(); //Actualiza la información en la estructura sensores
  imprimirSensores();
  enviarSensores(); // envia la información por i2c
  gradosB++;
  gradosC--;
  delay(1000);
}
