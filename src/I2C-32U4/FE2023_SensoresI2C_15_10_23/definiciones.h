long tiempoLED;
#define LED 13

//TOF
#define XSHUT_D A4
#define XSHUT_I A3
int indiceMedianaTOF_I = 0;
int indiceMedianaTOF_D = 0;
//HuskyLens
#define SERVO_CAM A5
#define MAX_BLOCKS 10 // Maximum number of blocks
#define NUM_ID 2      // Number of IDs per block 
const int ID_VERDE[NUM_ID] = {1, 3};
const int ID_ROJO[NUM_ID] = {2, 4};
const int alturaBloque = 30; //(px)    31px a 90cm
const long distanciaBloque = 900; //(mm)
const int wLimits[2] = {0, 1000}; // Min and max limits (width)
const int hLimits[2] = {0, 1000}; // Min and max limits (height)

long servoDelay = millis();
bool servoMoved = true;
int indiceMedianaHL[4][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}; // x, y, w, h, ID (*4)
int camaraGirada = 1; // Camera position  0: left, 1: center, 2: right
int iteracion = 0;
int indexBloques[MAX_BLOCKS];
int indiceSector = 0;
bool camaraIniciada = false;

//Leds
#define TI_PIN 6
#define TD_PIN 12
#define FI_PIN 5
#define FD_PIN 4
const int tiempoBlink = 200;

cRGB BLACK;
cRGB ledColorFI[3];
cRGB ledColorFD[3];
cRGB ledColorTI[2];
cRGB ledColorTD[2];
int ledModeFI[3];
int ledModeFD[3];
int ledModeTI[2];
int ledModeTD[2];


//SPI
#define STARTBYTE 251
#define ENDBYTE 252
#define PIN_INTERRUPCION 7

volatile bool haveData = false;
unsigned long buenaTrama = 0;
unsigned long errorTrama = 0;

volatile bool flagSPI = false;

struct datos { // 9 data bytes
  const byte startByte = STARTBYTE;
  byte sector = 255;
  unsigned int TOF[2] = {0, 0};
  int MPU = 0;
  byte bloqueCerca = 255;
  byte lastCommandFlag = 0;
  const byte endByte = ENDBYTE;
};
volatile datos i2c;

struct infoCommands { // 9 data bytes
  const byte startByte = STARTBYTE;
  bool leerSector;
  byte angServo;
  byte leds;
  byte filtroCam;
  byte distBloqueCerca;
  byte distBloqueLejos;
  byte masComandos[3];
  const byte endByte = ENDBYTE;
};
volatile infoCommands command;
volatile byte buff[sizeof(command)];
