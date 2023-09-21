void setupServo() {
  pinMode(SERVO_CAM, OUTPUT);
  }

void updateServo() {
  static byte angAnterior = 87;  //Centro exacto
  if (millis() > servoDelay and !servoMoved){ // If para evitar que se siga actualizando el servo cuando ya está en su posición
    dirCam.detach();
    servoMoved = true;
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
