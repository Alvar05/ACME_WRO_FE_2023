void setDireccionCamara(int grados) {
  commandB.angServo = grados;
  enviarSPIB();
  servoFinished = false;
}

byte camaraSweep(int angInicial, int angFinal,  int tTotal) {
  byte bloque = 0;
  setDireccionCamara(angInicial);
  int direccionCamara = angInicial;
  int periodo = tTotal / abs(angFinal - angInicial);
  while (direccionCamara != angFinal) {
    long tiempoSweep = millis() + periodo;
    while (millis() < tiempoSweep) {
      if (verdeCerca()) bloque = 1;
      else if (rojoCerca()) bloque = 2;
      updateRobot();
    }
    if (angInicial < angFinal) direccionCamara++;
    else if (angFinal < angInicial) direccionCamara--;
    else return bloque;
    setDireccionCamara(direccionCamara);
  }
  return bloque;
}
