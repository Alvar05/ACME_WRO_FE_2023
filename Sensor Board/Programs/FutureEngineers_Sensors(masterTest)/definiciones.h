//ULTRASONICO
#define ENCODERC 0
#define ENCODERB 1
#define TRIGGER_IZQUIERDO 14
#define ECHO_IZQUIERDO 15
#define TRIGGER_DERECHO A0
#define ECHO_DERECHO A1
#define TRIGGER_CENTRO 6
#define ECHO_CENTRO 8
#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

//ENCODERS
volatile unsigned long gradosB = 0;
volatile unsigned long gradosC = 0;

//MPU6050
#define OUTPUT_READABLE_YAWPITCHROLL
#define INTERRUPT_PIN 7

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
bool debugYPR = false;  // Imprime YPR

// pines
byte pinesDigitales[] = {14, 15, 16};
byte pinesAnalogicos[] = {A0, A1, A2, A3};

// I2C
struct datos {
  int ypr[3]; 
  int sr04[3];
  unsigned long encoder[2];
};
struct pin {
  int analogicos[sizeof(pinesAnalogicos)];
  int digitales[sizeof(pinesDigitales)];
};
