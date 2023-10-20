void setupServo() {
  pinMode(SERVO_CAM, OUTPUT);
}

void updateServo() {
  static byte angAnterior = 87;  // Exact center
  if (millis() > servoDelay and !servoMoved) { // If to stop the servo actualization when it is in the right position
    dirCam.detach();
    servoMoved = true;
    i2c.lastCommandFlag = !i2c.lastCommandFlag;
  }
  if (command.angServo != angAnterior) {
    dirCam.attach(SERVO_CAM, 590, 2400);
    dirCam.write(command.angServo);
    servoMoved = false;
    if (command.angServo < 45) camaraGirada = 0;
    else if (command.angServo < 135) camaraGirada = 1;
    else if (command.angServo <= 180) camaraGirada = 2;
    angAnterior = command.angServo;
    servoDelay = millis() + 200;
  }
}
