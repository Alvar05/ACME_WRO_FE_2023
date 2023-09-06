void setupServo() {
  servoCamara.attach(SERVO_CAM, 700, 2400);   //2550 max
  servoCamara.write(87);
}

void updateServo() {
  static byte angAnterior = 87;  //Centro exacto
  if (command.angServo != angAnterior) {
    servoCamara.write(command.angServo);
    if (command.angServo < 45) camaraGirada = 0;
    else if (command.angServo < 135) camaraGirada = 1;
    else if (command.angServo <= 180) camaraGirada = 2;
    angAnterior = command.angServo;
  }
}
