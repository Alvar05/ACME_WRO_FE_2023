//This tab contains all variables and defined pins needed in the rest of the program
#define LED 13
// Motor & Encoders
#define ENC_MA 0
#define PWM 11
#define MOTOR_DIR 9
#define MOTOR_DIS 10
#define ENC_MB 1
#define ENC_D 2
#define ENC_I 3
#define PULSOS_ENC 28.00
#define MIN_ACCEL_DISTANCE 800 // Minimum distance to go 0-100 safely
#define MIN_DESACCEL_DISTANCE 1200 // Minimum distance to go 100-0 safely
#define V_MAX_PID 80
#define MIN_V_TO_BREAK 6
volatile long encoderMcontador = 0;
volatile long encoderDcontador = 0;
volatile long encoderIcontador = 0;
volatile long encoderDslave = 0;
volatile long encoderIslave = 0;
//volatile long tiempoAnterior = micros();
//volatile long tiempoNuevo = micros();
const long incrementoMinimoEntrePulsos = 550;
const long incrementoMaximoEntrePulos = 60000;
const float diametroRueda = 29; // En milímetros
float error, proporcional, integral, derivativo, errorSuma, giro, errorAnterior;
unsigned long incrementoMotor = micros();
bool motorInvertido = false; // True es sentido positivo y false sentido negativo
unsigned long microsAnteriorMotor = micros();
int speed = 0;
bool activePID = false;
bool activeRampa = false;
bool activeDistance = false;
bool activeDistAcel = false;
bool breakBitState = false;
bool distanceBitState = false;
bool rampaBitState = false;
bool PIDBitState = false;
bool PWMBitState = false;
long vInicial;
long vFinal;
unsigned long vMaxDist;
bool desacelBreak = false;

//Ultrasonics
#include <NewPing.h>
#define TRIGGER_IZQUIERDO A0 
#define ECHO_IZQUIERDO A1
#define TRIGGER_DERECHO A4
#define ECHO_DERECHO A5
#define TRIGGER_CENTRO A3
#define ECHO_CENTRO A2 
#define SONAR_NUM 3
#define NO_ECHO 255
#define ITERATIONS     5 // Number of iterations.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).
#define MAX_DISTANCE_CENTRO 200 // Maximum distance (in cm) to ping.
#define MAX_DISTANCE 90 // Maximum distance (in cm) to ping.
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(TRIGGER_IZQUIERDO, ECHO_IZQUIERDO, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_CENTRO, ECHO_CENTRO, MAX_DISTANCE_CENTRO),
  NewPing(TRIGGER_DERECHO, ECHO_DERECHO, MAX_DISTANCE)
};
unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
volatile uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
bool newLecture = false;
volatile uint8_t currentIteration = 0;        // Keeps track of iteration step.
volatile unsigned int timeRAW[SONAR_NUM][ITERATIONS];
bool activeUS[3] = {true, true, true};
unsigned long lastPing = millis();
volatile bool echoCheckFlag = true; 

//Recieve commands of the Master
char accion = 'X'; // Uses the X char as "without actions"

// SPI
#include <SPI.h>
#define STARTBYTE 251
#define ENDBYTE 252
#define PIN_INTERRUPCION 7
volatile bool haveData = false;
struct datos { // 10 data bytes
  const byte startByte = STARTBYTE;
  byte sr04[3] = {13, 57, 131} ; //Ultrasonics
  unsigned int encoderI = 0; //Encoders
  unsigned int encoderD = 0;
  int velocidad = 0;
  bool lastCommandFlag = 0;
  const byte endByte = ENDBYTE;
};
volatile datos sensor;

struct infoCommands { // 10 data bytes
  const byte startByte = STARTBYTE;
  byte motor; //Motor
  byte motorFlags; //Motor
  byte flags; // Every single bit is used as a different flag in the slave microcontroller
  // 3 bits for ultrasonic switch, 2 bits for encoder reset.
  // Starts at 7, which is 00000111, to start all the ultrasonics
  int distance; // Indicates the distance for the car to move
  int vMaxDistance; // Indicates the max velocity for the car to move a distance
  byte masComandos[3] = {2, 3, 4};
  const byte endByte = ENDBYTE;
};
volatile infoCommands command;
volatile byte buff[sizeof(command)];
