void desactivarHL() {
  commandB.activeCam = 0;
  enviarSPIB();
}

void activarHL() {
  commandB.activeCam = 1;
  enviarSPIB();
}

byte leerSector() {
  byte lecturaSector = 11;
  if (sentidoRonda == 1) {
    if (distAnterior < 400) {
      commandB.distBloqueCerca = 60;
      byte lectura = camaraSweep(90, 130, 2000);
      if (lectura == 0) {
        commandB.distBloqueLejos = 90;
        setDireccionCamara(160);
        long tiempoLectura = millis() + 2000;
        while (millis() < tiempoLectura) updateRobot();
        if (verdeCerca()) lecturaSector = 3;
        else if (rojoCerca()) lecturaSector = 4;
        else lecturaSector = i2c.sector;
      }
      else {
        intermitente('E');
        lecturaSector = lectura;
      }
    }
    else {
      if (distAnterior == 500) setDireccionCamara(155);
      else setDireccionCamara(160);
      long tiempo = millis() + 2000;
      while (millis() < tiempo) updateRobot();
      lecturaSector = i2c.sector;
    }
  }
  else {
    if (distAnterior < 400) {
      commandB.distBloqueCerca = 60;
      byte lectura = camaraSweep(90, 50, 2000);
      if (lectura == 0) {
        commandB.distBloqueLejos = 90;
        setDireccionCamara(30);
        long tiempoLectura = millis() + 2000;
        while (millis() < tiempoLectura) updateRobot();
        if (verdeCerca()) lecturaSector = 3;
        else if (rojoCerca()) lecturaSector = 4;
        else lecturaSector = i2c.sector;
      }
      else {
        intermitente('E');
        lecturaSector = lectura;
      }
    }
    else {
      if (distAnterior == 500) setDireccionCamara(25);
      else setDireccionCamara(30);
      long tiempo = millis() + 2000;
      while (millis() < tiempo) updateRobot();
      lecturaSector = i2c.sector;
    }
  }
  desactivarHL();
  commandB.distBloqueCerca = 95;
  commandB.distBloqueLejos = 145;
  return lecturaSector;
}
