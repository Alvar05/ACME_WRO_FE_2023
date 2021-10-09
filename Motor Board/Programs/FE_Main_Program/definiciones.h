//Definición placa controladora Arduino NANO para Lego NXT Siguelineas 2020 v.1.0 "ORANGE IS THE NEW BLACK"

//Driver TB6612FNG Motor
#define AIN1 4  // AIN2 está negado respecto a la entrada AIN1
#define BIN1 7 // BIN2 está negado respecto a la entrada BIN1
#define PWMA 5 // 
#define PWMB 6  // RJ12 para motoress: Buscar en Fritzing por RJ11, os aparecerá con 6 pines, para seguir con la compatibilidad de Lego,
//el pin 1 y 2 de cada conector irán conectadas las salidas Y del driver. En pin 3 GND, 4 Vcc, 5 ENCA, 6 ENCB. Unicamente se utiliza el encoderB
#define ENC_B 2 // encoder Motor B
#define ENC_C 3 // encoder Motor C
bool sentidoB = true;
bool sentidoC = true;
volatile long encoderBcontador = 0;
volatile long encoderCcontador = 0;
unsigned long incrementoB = micros();
unsigned long incrementoC = micros();
const long incrementoMinimoEntrePulsos = 40000;

#include<EEPROM.h>

//Sensores analogicos NXT
#define P2 A7 // Puerto analogico nº2 NXT. Reflejado activado siempre
#define P3 A0 // Puerto analogico nº3 NXT
int sensorEntorno [] = {0, 1023,  0, 1023,  0, 1023,  0, 1023,  0, 1023,  0, 1023,  0, 1023,  0, 1023}; //blanco,negro
// La array QTR-MD-A-8 están conectados a los puertos A0-A7. Existe un jumper de seleccion para decidir el tipo de sensor utilizado en A0 y A7(NXT o QTR)

//Serie WS2812b
#include <FastLED.h>
#define NUM_LEDS 4
#define DATA_PIN 8
CRGB leds[NUM_LEDS];

//Botones
#define BOTON_UP 10
#define BOTON_DOWN 9

//Pixy
#include <Pixy2.h>
Pixy2 pixy;
int altBase = 140;
int disBase = 350;
int numBloque = 0;
int posData [4][4]; //Fila: [0]=disRecta [1]=disDiagonal [2]=angulo [3]=disLat //Columna: numBloque

//QTRA
#include <QTRSensors.h>
QTRSensors qtr;
const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

//BT
bool estadoBT = false;
unsigned long startBeep = 0;
bool estadoBeep = false;

//MPU6050
#include "I2Cdev.h"
#include <Wire.h>

#define pinesAnalogicos 4
#define pinesDigitales 3

// I2C
struct datos {
  int ypr[3];
  int sr04[3];
  unsigned long encoder[2];
  int analogicos[pinesAnalogicos];
  int digitales[pinesDigitales];
};

//PID
float error, proporcional, integral, derivativo, errorSuma, giro, errorAnterior;
float kp = 1, ki = 0.0, kd = 5;
int velocidad = 0;

int outMode[] = {AIN1, BIN1, DATA_PIN}; //Listas para la configuración del pinMode aplicadas en el setupSiguelineas()
int inMode[] = {BOTON_UP, BOTON_DOWN};// Como pull-up

String colorLed[4] = {"OFF", "OFF", "OFF", "OFF"};
String modeLed[4] = {"NORMAL", "NORMAL", "NORMAL", "NORMAL"};

// Funciones de control de Motor:
// void motorB(int velocidad), void motorC(int velocidad),void motorBreak(char motor)
// Funciones de Sensores:
// int sensorAnalog(int puerto), int sensorDigital(int puerto), boolean leerContacto(int puerto), int leerReflejado(int puerto), int leerAmbiente(int puerto), int leerUltrasonico(int puerto)
