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
#define mediaTOFbloque 600
#define mediaTOFpared 1200
#define mediaTOF0direccion 1300
#define mediaTOF1direccion 1500
const int offsetUS = 120;
byte sectorRonda[4] = {0, 0, 0, 0};
int sentidoRonda = 0;
byte vuelta = 0;
int vMaxSector = 10;
int vMinSector = 10;
#define TIEMPO_BLOQUE 500
int dirAnteriorCurva = 0;

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
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector.
float offsetMPU = 0.0;

// MOTORS
unsigned int offsetD = 0;
unsigned int offsetI = 0;
///////////////////////////SENSORS SLAVE STRUCTURE////////////////
struct datosA { // 10 data bytes (start and end bytes are not counted as data bytes)
  const byte startByte = STARTBYTE;
  byte sr04[3];
  unsigned int encoderI;
  unsigned int encoderD;
  int velocidad;
  bool lastCommandFlag = 0;
  const byte endByte = ENDBYTE;
};
datosA sensor;

bool lastCommandAState = 0;
bool commandFinished = false;
byte buffA[sizeof(sensor)];

struct infoCommandsA { // 10 data bytes
  const byte startByte = STARTBYTE;
  byte motor; //Motor
  byte motorFlags; //Motor
  byte flags = 0; // Every single bit is used as a different flag in the slave microcontroller
  int distance; // Indicates the distance for the car to move
  int vMaxDistance; // Indicates the max velocity for the car to move a distance
  byte masComandos[3] = {2, 3, 4}; // Extra commands to fill the rest of the data bytes
  const byte endByte = ENDBYTE;
};
infoCommandsA commandA;

///////////////////////////I2C SLAVE STRUCTURE////////////////
struct datosB { // 9 data bytes
  const byte startByte = STARTBYTE;
  byte sector;
  unsigned int TOF[2];
  int quitar;
  byte bloqueCerca;
  byte lastCommandFlag;
  const byte endByte = ENDBYTE;
};
datosB i2c;

bool lastCommandBState = 0;
bool servoFinished = false;
byte buffB[sizeof(i2c)] = {0};

struct infoCommandsB { // 9 data bytes
  const byte startByte = STARTBYTE;
  bool activeCam = 1;
  byte angServo = 90;
  byte leds = 0;
  byte filtroCam = 0;
  byte distBloqueCerca = 95;
  byte distBloqueLejos = 145;
  byte masComandos[3] = {24, 7, 0};
  const byte endByte = ENDBYTE;
};
infoCommandsB commandB;
