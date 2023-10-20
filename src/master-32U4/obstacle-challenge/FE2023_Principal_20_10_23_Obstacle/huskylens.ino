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
  if (distAnterior < 400) {
    commandB.distBloqueCerca = 60;
    byte lectura = 0;
    if (sentidoRonda == 1) lectura = camaraSweep(90, 130, 750);
    else lectura = camaraSweep(90, 50, 750);
    if (lectura == 0) {
      commandB.distBloqueLejos = 90;
      if (sentidoRonda == 1) setDireccionCamara(155);
      else setDireccionCamara(35);
      long tiempoLectura = millis() + 750;
      while (millis() < tiempoLectura) updateRobot();
      if (verdeCerca()) lecturaSector = 3;
      else if (rojoCerca()) lecturaSector = 4;
      else lecturaSector = i2c.sector;
    }
    else {
      if (sentidoRonda == 1) setDireccionCamara(155); 
      else setDireccionCamara(35);
      long tiempoLectura = millis() + 750;
      while (millis() < tiempoLectura) updateRobot();
      if (i2c.sector == 1 or i2c.sector == 3 or i2c.sector == 5 or i2c.sector == 7 or i2c.sector == 8) {
        if (lectura == 1) lecturaSector = 7;
        else lecturaSector = 10;
      }
      else if (i2c.sector == 2 or i2c.sector == 4 or i2c.sector == 6 or i2c.sector == 9 or i2c.sector == 10) {
        if (lectura == 1) lecturaSector = 8;
        else lecturaSector = 9;
      }
      else lecturaSector = lectura;
    }
  }
  else {
    if (distAnterior == 500){
      if (sentidoRonda == 1) setDireccionCamara(155);
      else setDireccionCamara(35);
    }
    else {
      if (sentidoRonda == 1) setDireccionCamara(160);
      else setDireccionCamara(30);
    }
    long tiempo = millis() + 750;
    while (millis() < tiempo) updateRobot();
    lecturaSector = i2c.sector;
  }
  commandB.distBloqueCerca = 95;
  commandB.distBloqueLejos = 145;
  return lecturaSector;
}
