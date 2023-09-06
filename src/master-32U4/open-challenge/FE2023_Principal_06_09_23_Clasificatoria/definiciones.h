#define BTN 6
#define BUZZ 5
#define DIR A5
#define LED 13
#define OFFSET_SERVO -6
#include <Servo.h>
float error, proporcional, integral, derivativo, errorSuma, giro, errorAnterior, offsetMPU;
byte sentido = -1;
byte disPared[4] = {255, 255, 255, 255};
byte vuelta = 0;
bool TOFsentido = 0;
int posInicial = -1;

//SPI
#include <SPI.h>
#define SSA A4
#define SSB A3
#define STARTBYTE 251 // Only as a start flag
#define ENDBYTE 252 // // Only as an end flag

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
  char direccion; /////Predet. to 'N', changes to 'H'(CW) or 'A'(CCW)
  const byte endByte = ENDBYTE;
};
datosA sensor;

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
struct datosB{ // 8 data bytes
  const byte startByte = STARTBYTE;
  byte sector;
  unsigned int TOF[2];
  int MPU;
  byte bloqueCerca;
  const byte endByte = ENDBYTE;
};
datosB i2c;

byte buffB[sizeof(i2c)] = {0};

struct infoCommandsB{ // 8 data bytes
  const byte startByte = STARTBYTE;
  bool leerSector = 0;
  byte angServo = 87;
  byte leds = 0;
  byte filtroCam = 70;
  byte masComandos[4] = {3, 78, 24, 7};
  const byte endByte = ENDBYTE;
};
infoCommandsB commandB;

//////////////////////////////////ESTRUCTURA BT
struct BT{
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
