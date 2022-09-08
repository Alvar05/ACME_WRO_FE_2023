//Definición M5Stack para FE 2021-2022

//Way of the laps
int sentido = 0; //if its CW is 0 and if its CCW is 180

//Driver ESC Motor
#define ESC 16
#define ENC_C 34 // encoder tyre C (left)  **has outside PULL_UP **
#define ENC_B 35 // encoder tyre B (right) **has outside PULL_UP **
float diametroRueda = 2.9; //Diameter of the tyres in cm
float distanciaEntreRuedas = 7.5; // Distance between tyres in cm
float distanciaEntreEjes = 10; //Distance between axles in cm
bool sentidoB = true;
bool sentidoC = true;
volatile long encoderBcontador = 0;
volatile long encoderBtotal = 0;
volatile long encoderCcontador = 0;
volatile long encoderCtotal = 0;
int velocidad;

//Ultrasonic
#define TRIGGER_PIN  26  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     36 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
int arrayUltrasonico[5];
bool arrayLlena = false;
int indiceLlenado = 0;
long millisLectura = 0;

//PID
float error, proporcional, integral, derivativo, errorSuma, giro, errorAnterior, offsetMPU;
int anguloMaximo = 30;

//ARCHS
int anguloDesplazamiento = 0;

//I2C
#define PIN_INTERRUPCION 5
volatile bool sensorInterruptFlag = false;
struct datos { //16 bytes
  byte sr04[3]; //Ultrasonics
  byte suelo[2]; //Some extra variables for possible new sensors with I2C
  byte analogico[4]; // """"
  byte digital[3]; // """"
  char direccion; /////Predet. to 'N', changes to 'H'(CW) or 'A'(CCW)
};
datos sensor;

//SPEAKER
#define TONE_PIN_CHANNEL 3
int duracionBeep = 0;
int frecuenciaBeep = 0;

//SCREEN
bool debugPantalla = true;

//MPU6050
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 13
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
