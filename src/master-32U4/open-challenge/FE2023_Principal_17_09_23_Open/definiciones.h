#define BTN 6
#define BUZZ 5
#define DIR A5
#define LED 13
#include <Servo.h>
#define OFFSET_SERVO -6
float error, proporcional, integral, derivativo, errorSuma, giro, errorAnterior;
long beepTime = 0;
bool beepDone = false;
int distAnterior = 0;
#define mediaTOFbloque 500
#define mediaTOFpared 1200
#define mediaTOF0direccion 1300
#define mediaTOF1direccion 1500
byte sectorRonda[4] = {0, 0, 0, 0};
int sentidoRonda = 0;
byte vuelta = 0;
int vMaxSector;
int vMinSector;

//SPI
#include <SPI.h>
#define SSA A4
#define SSB A3
#define STARTBYTE 251 // Only as a start flag
#define ENDBYTE 252 // // Only as an end flag
long SPIA_Interval = 5; // Delay in milliseconds
long lastTimeSPIA = millis();
long SPIB_Interval = 5; // Delay in milliseconds
long lastTimeSPIB = millis();

//MPU6050
#include <MPU6050_light.h>
#include "Wire.h"
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float offsetMPU = 0.0;

// MOTORES
unsigned int offsetD = 0;
unsigned int offsetI = 0;
///////////////////////////ESCTRUCTURAS SLAVE SENSORES////////////////
struct datosA { // 10 data bytes
  const byte startByte = STARTBYTE;
  byte sr04[3]; //Ultrasonics
  unsigned int encoderI; //Encoders
  unsigned int encoderD;
  int velocidad;
  bool lastCommandFlag = 0; /////Predet. to 'N', changes to 'H'(CW) or 'A'(CCW)
  const byte endByte = ENDBYTE;
};
datosA sensor;

bool lastCommandState = 0;
bool commandFinished = false;
byte buffA[sizeof(sensor)];

struct infoCommandsA { // 10 data bytes
  const byte startByte = STARTBYTE;
  byte motor; //Motor
  byte motorFlags; //Motor
  byte flags = 7; // Every single bit is used as a different flag in the slave microcontroller
  // 3 bits for ultrasonic switch, 2 bits for encoder reset.
  // Starts at 7, which is 00000111, to start all the ultrasonics
  int distance; // Indicates the distance for the car to move
  int vMaxDistance; // Indicates the max velocity for the car to move a distance
  byte masComandos[3] = {2, 3, 4};
  const byte endByte = ENDBYTE;
};
infoCommandsA commandA;

///////////////////////////ESCTRUCTURAS SLAVE I2C////////////////
struct datosB { // 8 data bytes
  const byte startByte = STARTBYTE;
  byte sector;
  unsigned int TOF[2];
  int quitar;
  byte bloqueCerca;
  const byte endByte = ENDBYTE;
};
datosB i2c;

byte buffB[sizeof(i2c)] = {0};

struct infoCommandsB { // 8 data bytes
  const byte startByte = STARTBYTE;
  bool activeCam = 1;
  byte angServo = 90;
  byte leds = 0;
  byte filtroCam = 0;
  byte distBloqueCerca = 95;
  byte distBloqueLejos = 145;
  byte masComandos[2] = {24, 7};
  const byte endByte = ENDBYTE;
};
infoCommandsB commandB;

//////////////////////////////////ESTRUCTURA BT
struct BT {
  unsigned int encoderI = 0;
  unsigned int encoderD = 0;
  int velocidad = 0;
  byte flag = 0;
  byte sr04[3] = {0}; //Ultrasonics
  int mpu = 0;
  int TOF[2] = {0};
  byte sectorCam = 0;
};
BT bt;
