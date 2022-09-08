//ULTRASONICO
#include <NewPing.h>
#define TRIGGER_IZQUIERDO 6 
#define ECHO_IZQUIERDO 7  
#define TRIGGER_DERECHO 4
#define ECHO_DERECHO 5
#define TRIGGER_CENTRO 8
#define ECHO_CENTRO 9 
#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE_CENTRO 200 // Maximum distance (in cm) to ping.
#define MAX_DISTANCE 90 // Maximum distance (in cm) to ping.
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(TRIGGER_IZQUIERDO, ECHO_IZQUIERDO, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(TRIGGER_CENTRO, ECHO_CENTRO, MAX_DISTANCE_CENTRO),
  NewPing(TRIGGER_DERECHO, ECHO_DERECHO, MAX_DISTANCE)
};
//Ground (don't connected)
byte SUELO_IZQIERDO = 0;
byte SUELO_DERECHO = 1;

//Pins
byte pinesDigitales[] = {14, 15, 16};
byte pinesAnalogicos[] = {A0, A1, A2, A3};

//Recieve commands of the M5Stack
char accion = 'X'; // Uses the X char as "without actions"

// I2C
#define PIN_INTERRUPCION 10
#define TIEMPO_TIMEOUT 1000
long timeOut;
struct datos { //Bytes
  byte sr04[3];
  byte suelo[2];
  byte analogico[4];
  byte digital[3];
  char direccion = 'N';
};
datos sensor;
