void moveSector(int direccion, byte sector, int sentido, int numSector, bool leer) { // Direction of the sector: 0, 90, 180, 270  Way: 1 -> CW, -1 -> CCW
  vMaxSector = 40;
  vMinSector = 10;
  desactivarHL();
  if (leer) {
    vMaxSector = 20;
    vMinSector = 10;
  }
  if (sentido == 1) { //////////////////////////////////////////////////////////////////////////////////////////////////////////// CW
    switch (sector) {
      case 1:
        giroEnlace(distAnterior, 550, direccion, sentido);
        beep(100);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        beep(100);
        setDireccionCamara(90);
        despLateral(angPID, 1);
        setVelocidad(vMinSector);
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 2:
        giroEnlace(distAnterior, 0, direccion, sentido);
        beep(100);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 3:
        setDireccionCamara(160);
        giroEnlace(distAnterior, 550, direccion, sentido);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          robotStop();
        }
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        moveDistancia(450, vMinSector, vMinSector);
        intermitente('D');
        distAnterior = 800;
        while (!commandFinished) updateRobot();
        break;
      /////////////////////////////////
      case 4:
        setDireccionCamara(90);
        giroEnlace(distAnterior, 0, direccion, sentido);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMaxSector, 5);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 5:
        giroEnlace(distAnterior, 550, direccion, sentido);
        intermitente('A');
        setDireccionCamara(160);
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(550, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 6:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 7:
        giroEnlace(distAnterior, 550, direccion, sentido);
        intermitente('A');
        setDireccionCamara(160);
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(550, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 8:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        despLateral(angPID, 1);
        setVelocidad(vMinSector);
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 9:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 10:
        beep(100);
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(160);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        despLateral(angPID, -1);
        setVelocidad(vMinSector);
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 11:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
        /////////////////////////////////
    }
    offsetMPU -= 0.5;
  }
  else if (sentido == -1) { //////////////////////////////////////////////////////////////////////////////////////////////////////////// CCW
    switch (sector) { // CCW
      case 1:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 2:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        despLateral(angPID, -1);
        setVelocidad(vMinSector);
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 3:
        setDireccionCamara(90);
        giroEnlace(distAnterior, 0, direccion, sentido);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMaxSector, 5);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        intermitente('I');
        distAnterior = 200;
        break;
      case 4:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(30);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          robotStop();
        }
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        moveDistancia(450, vMinSector, vMinSector);
        intermitente('I');
        distAnterior = 800;
        while (!commandFinished) updateRobot();
        break;
      case 5:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 6:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(30);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(550, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 7:
        giroEnlace(distAnterior, 0, direccion, sentido);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        setDireccionCamara(90);
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 8:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(30);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        despLateral(angPID, 1);
        setVelocidad(vMinSector);
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 9:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(30);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(550, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 10:
        giroEnlace(distAnterior, 550, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(100, vMinSector, vMinSector); // 200 es el inicio 0 teórico, por eso se resta
          while (!commandFinished) updateRobot();
          robotStop();
        }
        despLateral(angPID, -1);
        setVelocidad(vMinSector);
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 11:
        giroEnlace(distAnterior, 0, direccion, sentido);
        setDireccionCamara(90);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(200, vMinSector, vMinSector);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        moveDistancia(500, vMaxSector, vMinSector);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
    }
  }
  if (leer) {
    activarHL();
    motorBreak(-15);
    long tiempo = millis() + 100;
    while (millis() < tiempo) updateRobot();
    if (numSector == 3) sectorRonda[0] = leerSector();
    else sectorRonda[numSector + 1] = leerSector();
    desactivarHL();
  }
}

bool verdeCerca() {
  if (i2c.sector == 1 or i2c.sector == 7 or i2c.sector == 8) return true;
  else return false;
}

bool rojoCerca() {
  if (i2c.sector == 2 or i2c.sector == 9 or i2c.sector == 10) return true;
  else return false;
}

void giroEnlace(int dist, int direccion) {
  if (dist > 0) {
    moveDistancia(dist, vMinSector, vMinSector);
    while (!commandFinished) updateRobot();
  }
  angPID = direccion;
  if (angPID == 270) while (abs(abs(getCarAngle()) - 90) > 10) updateRobot();
  else while (abs(abs(getCarAngle()) - abs(angPID)) > 10) updateRobot();
}

void giroEnlace(int distInicial, int distFinal, int dir, int sentido) {
  int distPared = sensor.sr04[1] * 10;
  if (vuelta > 0) distPared -= offsetUS;
  if (distPared == 0) distPared = 900;
  int desvDist = 850 - distPared;
  int radio = 150;
  bool moverFinal = false;

  if ((distInicial < 400 and distFinal < 400) or (distInicial > 400 and distFinal < 400)) {
    if (desvDist >= 50 or desvDist < 0) {
      motorStart(-10 * (desvDist / abs(desvDist)));
      waitDistancia(abs(desvDist));
      motorStart(vMinSector);
    }
    else motorStart(10);
    if (distInicial > 400 and distFinal < 400) {
      moverFinal = true;
    }
  }
  else if (distInicial < 400 and distFinal > 400) {
    moveDistancia(distFinal - desvDist, vMinSector, vMinSector);
    while (!commandFinished) updateRobot();
  }
  else {
    setVelocidad(20);
    radio = 700;
  }
  radio *= sentido;
  angPID = dir;
  if (dir > 180 and dir < 360) dir -= 360;
  motorResetCount("DI");
  activePID = false;
  while (abs(abs(getCarAngle()) - abs(dir)) > 10) {
    pidCurva(radio, 2, 0, 0);
    updateRobot();
  }
  activePID = true;
  if (moverFinal) {
    moveDistancia(distInicial - 200, vMinSector, vMinSector);
    while (!commandFinished) updateRobot();
  }
  dirAnteriorCurva = dir;
}

void despLateral(int dirPID, int lado) {  // lado: -1 left, 1 right
  setVelocidad(10);
  int radio = 270 * lado;
  activePID = false;
  if (dirPID > 180 and dirPID <= 360) dirPID -= 360;
  int dirTemp = dirPID + (90 * lado);
  if (dirTemp > 180 and dirTemp <= 360) dirTemp -= 360;
  motorResetCount("DI");
  while (abs(abs(getCarAngle()) - abs(dirTemp)) > 10) {
    pidCurva(radio, 2, 0, 0);
    updateRobot();
  }
  beep(100);
  dirAnteriorCurva = dirTemp;
  motorResetCount("DI");
  radio *= -1;
  while (abs(abs(getCarAngle()) - abs(dirPID)) > 10) {
    pidCurva(radio, 2, 0, 0);
    updateRobot();
  }
  beep(100);
  activePID = true;
  dirAnteriorCurva = dirPID;
}
