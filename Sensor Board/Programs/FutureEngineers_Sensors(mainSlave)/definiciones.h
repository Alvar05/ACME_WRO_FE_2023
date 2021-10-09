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
