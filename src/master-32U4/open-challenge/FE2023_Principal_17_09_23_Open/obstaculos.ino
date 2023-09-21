void moveSector(int direccion, byte sector, int sentido, int numSector, bool leer) { // Direccion (del sector a realizar): 0, 90, 180, 270 Sentido: 1 -> CW, -1 -> CCW
  vMaxSector = 100;
  vMinSector = 25;
  desactivarHL();
  if (leer) {
    vMaxSector = 100;
    vMinSector = 15;
  }
  if (sentido == 1) { //////////////////////////////////////////////////////////////////////////////////////////////////////////// CW
    byte casoBreak = 5; // CASOS---> 5: Correcto, 0: Recto Pared, 1: Recto Bloque, 2: Cambio a Pared, 3: Cambio a bloque
    switch (sector) {
      case 1:
        //D450,10,10 A90 D200,10,10 O600 V5 J0 D100,10,10
        setDireccionCamara(120);
        giroEnlace(450, direccion);
        activarHL();
        beep(100);
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector); // 200 es el inicio 0 teórico, por eso se resta
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        beep(100);
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 1;
            sectorRonda[numSector] = 7;
            beep(100);
            break;
          }
          else if (rojoCerca()) {
            beep(100);
            sectorRonda[numSector] = 8;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        desplazamientoLateral(angPID, 600);
        setVelocidad(5);
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 2:
        // A90 D600,10,5 J0 D100,10,10
        setDireccionCamara(60);
        giroEnlace(0, direccion);
        activarHL();
        beep(100);
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 3;
            beep(100);
            sectorRonda[numSector] = 10;
            break;
          }
          else if (rojoCerca()) {
            beep(100);
            sectorRonda[numSector] = 9;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(400, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 3:
        // D450,10,10 A90 D100,10,10 J1 D550,10,10
        setDireccionCamara(160);
        giroEnlace(450, direccion);
        intermitente('A');
        moveDistancia(100 + distAnterior - 200, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        moveDistancia(400, vMinSector, vMinSector);
        intermitente('D');
        distAnterior = 800;
        while (!commandFinished) updateRobot();
        break;
      /////////////////////////////////
      case 4:
        // A90 D600,10,5 J0 D100,10,10
        setDireccionCamara(90);
        giroEnlace(0, direccion);
        intermitente('A');
        moveDistancia(600 + distAnterior - 200, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 5:
        //D450,10,10 A90 D550,10,5 J1 D150,10,10
        setDireccionCamara(120);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 2;
            beep(100);
            sectorRonda[numSector] = 6;
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(160);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 6:
        // A90 D600,10,5 J0 D100,10,10
        setDireccionCamara(60);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 3;
            sectorRonda[numSector] = 5;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 7:
        // D450,10,10 A90 D550,10,5 J1 D150,10,10
        setDireccionCamara(120);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 2;
            sectorRonda[numSector] = 8;
            beep(100);
            break;
          }
          else if (!verdeCerca()) {
            casoBreak = 2;
            sectorRonda[numSector] = 1;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(160);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 8:
        // D450,10,10 A90 D200,10,10 O600 V5 J0 D100,10,10
        setDireccionCamara(120);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector); // 200 es el inicio 0 teórico, por eso se resta
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 1;
            sectorRonda[numSector] = 7;
            beep(100);
            break;
          }
          else if (!rojoCerca()) {
            sectorRonda[numSector] = 1;
            beep(100);
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        desplazamientoLateral(angPID, 600);
        setVelocidad(5);
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 9:
        // A90 D600,10,5 J0 D100,10,10
        setDireccionCamara(60);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 3;
            sectorRonda[numSector] = 10;
            beep(100);
            break;
          }
          else if (!rojoCerca()) {
            sectorRonda[numSector] = 2;
            beep(100);
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(400, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 10:
        // A90 D200,10,10 O-600 V5 J1 D150,10,10
        setDireccionCamara(60);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 0;
            sectorRonda[numSector] = 9;
            beep(100);
            break;
          }
          else if (!verdeCerca()) {
            casoBreak = 0;
            sectorRonda[numSector] = 2;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(160);
        desplazamientoLateral(angPID, -600);
        setVelocidad(5);
        while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('D');
        distAnterior = 800;
        break;
      /////////////////////////////////
      case 11:
        // A90 D600,10,5 J0 D100,10,10
        setDireccionCamara(60);
        giroEnlace(0, direccion);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(250 + distAnterior - 200, vMaxSector, 5);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        setDireccionCamara(90);
        moveDistancia(600, vMaxSector, vMinSector);
        while (!commandFinished) {
          updateMPU();
          pidGyro(angPID, 1.5, 0, 0);
          enviarSPIA();
          enviarSPIB();
          updateLed();
          updateBeep();
        }
        while (i2c.TOF[1] < mediaTOFpared) updateRobot();
        intermitente('D');
        distAnterior = 200;
        break;
        /////////////////////////////////
    }
    desactivarHL();
    if (casoBreak == 0) {
      setDireccionCamara(90);
      moveDistancia(400, vMaxSector, 5);
      while (!commandFinished) updateRobot();
      while (i2c.TOF[1] < mediaTOFpared) updateRobot();
      intermitente('D');
      distAnterior = 200;
    }
    else if (casoBreak == 1) {
      setDireccionCamara(160);
      moveDistancia(350, vMaxSector, 5);
      while (!commandFinished) updateRobot();
      while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
      moveDistancia(50, vMinSector, vMinSector);
      while (!commandFinished) updateRobot();
      intermitente('D');
      distAnterior = 800;
    }
    else if (casoBreak == 2) {
      setDireccionCamara(90);
      desplazamientoLateral(angPID, 600);
      setVelocidad(5);
      while (i2c.TOF[1] < mediaTOFpared) updateRobot();
      intermitente('D');
      distAnterior = 200;
    }
    else if (casoBreak == 3) {
      setDireccionCamara(160);
      desplazamientoLateral(angPID, -600);
      setVelocidad(5);
      while (i2c.TOF[1] > mediaTOFbloque) updateRobot();
      moveDistancia(50, vMinSector, vMinSector);
      while (!commandFinished) updateRobot();
      intermitente('D');
      distAnterior = 800;
    }
  }
  else if (sentido == -1) { //////////////////////////////////////////////////////////////////////////////////////////////////////////// CCW
    byte casoBreak = 5; // CASOS---> 5: Correcto, 0: Recto Pared, 1: Recto Bloque, 2: Cambio a Pared, 3: Cambio a bloque
    switch (sector) { // CCW
      case 1:
        setDireccionCamara(120);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 3;
            beep(100);
            sectorRonda[numSector] = 8;
            break;
          }
          else if (verdeCerca()) {
            beep(100);
            sectorRonda[numSector] = 7;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(400, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      /////////////////////////////////
      case 2:
        setDireccionCamara(60);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector); // 200 es el inicio 0 teórico, por eso se resta
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 1;
            sectorRonda[numSector] = 9;
            beep(100);
            break;
          }
          else if (verdeCerca()) {
            beep(100);
            sectorRonda[numSector] = 10;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        desplazamientoLateral(angPID, -600);
        setVelocidad(5);
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 3:
        setDireccionCamara(90);
        giroEnlace(0, direccion);
        intermitente('A');
        moveDistancia(600 + distAnterior - 200, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        intermitente('I');
        distAnterior = 200;
        break;
      case 4:
        setDireccionCamara(30);
        giroEnlace(450, direccion);
        intermitente('A');
        moveDistancia(100 + distAnterior - 200, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        moveDistancia(400, vMinSector, vMinSector);
        intermitente('I');
        distAnterior = 800;
        while (!commandFinished) updateRobot();
        break;
      case 5:
        setDireccionCamara(120);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 3;
            sectorRonda[numSector] = 6;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 6:
        setDireccionCamara(60);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 2;
            beep(100);
            sectorRonda[numSector] = 5;
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(30);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 7:
        setDireccionCamara(120);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 3;
            sectorRonda[numSector] = 8;
            beep(100);
            break;
          }
          else if (!verdeCerca()) {
            sectorRonda[numSector] = 1;
            beep(100);
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(400, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 8:
        setDireccionCamara(120);
        giroEnlace(0, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 0;
            sectorRonda[numSector] = 7;
            beep(100);
            break;
          }
          else if (!rojoCerca()) {
            casoBreak = 0;
            sectorRonda[numSector] = 1;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(30);
        desplazamientoLateral(angPID, 600);
        setVelocidad(5);
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 9:
        setDireccionCamara(60);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (verdeCerca()) {
            casoBreak = 2;
            sectorRonda[numSector] = 10;
            beep(100);
            break;
          }
          else if (!rojoCerca()) {
            casoBreak = 2;
            sectorRonda[numSector] = 2;
            beep(100);
            break;
          }
        }
        desactivarHL();
        setDireccionCamara(30);
        moveDistancia(350, vMaxSector, 5);
        while (!commandFinished) updateRobot();
        while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
        moveDistancia(50, vMinSector, vMinSector);
        while (!commandFinished) updateRobot();
        intermitente('I');
        distAnterior = 800;
        break;
      case 10:
        setDireccionCamara(60);
        giroEnlace(450, direccion);
        activarHL();
        intermitente('A');
        moveDistancia(200 + distAnterior - 200, vMinSector, vMinSector); // 200 es el inicio 0 teórico, por eso se resta
        while (!commandFinished) updateRobot();
        if (numSector == 0 and vuelta == 2) robotStop();
        if (leer or vuelta == 0) {
          if (rojoCerca()) {
            casoBreak = 1;
            sectorRonda[numSector] = 9;
            beep(100);
            break;
          }
          else if (!verdeCerca()) {
            sectorRonda[numSector] = 2;
            beep(100);
          }
        }
        desactivarHL();
        setDireccionCamara(90);
        desplazamientoLateral(angPID, -600);
        setVelocidad(5);
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
      case 11:
        setDireccionCamara(120);
        giroEnlace(0, direccion);
        intermitente('A');
        if (numSector == 0 and vuelta == 2) {
          moveDistancia(250 + distAnterior - 200, vMaxSector, 5);
          while (!commandFinished) updateRobot();
          robotStop();
        }
        desactivarHL();
        setDireccionCamara(90);
        moveDistancia(600, vMaxSector, vMinSector);
        while (!commandFinished) {
          updateMPU();
          pidGyro(angPID, 1.5, 0, 0);
          enviarSPIA();
          enviarSPIB();
          updateLed();
          updateBeep();
        }
        while (i2c.TOF[0] < mediaTOFpared) updateRobot();
        intermitente('I');
        distAnterior = 200;
        break;
    }
    desactivarHL();
    if (casoBreak == 0) {
      setDireccionCamara(90);
      moveDistancia(400, vMaxSector, vMinSector);
      while (!commandFinished) updateRobot();
      while (i2c.TOF[0] < mediaTOFpared) updateRobot();
      intermitente('I');
      distAnterior = 200;
    }
    else if (casoBreak == 1) {
      setDireccionCamara(30);
      moveDistancia(350, vMaxSector, vMinSector);
      while (!commandFinished) updateRobot();
      while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
      moveDistancia(50, vMinSector, vMinSector);
      while (!commandFinished) updateRobot();
      intermitente('I');
      distAnterior = 800;
    }
    else if (casoBreak == 2) {
      setDireccionCamara(90);
      desplazamientoLateral(angPID, -600);
      setVelocidad(vMinSector);
      while (i2c.TOF[0] < mediaTOFpared) updateRobot();
      intermitente('I');
      distAnterior = 200;
    }
    else if (casoBreak == 3) {
      setDireccionCamara(30);
      desplazamientoLateral(angPID, 600);
      setVelocidad(vMinSector);
      while (i2c.TOF[0] > mediaTOFbloque) updateRobot();
      moveDistancia(50, vMinSector, vMinSector);
      while (!commandFinished) updateRobot();
      intermitente('I');
      distAnterior = 800;
    }
  }
  if (leer) {
    activarHL();
    motorStartPower(0);
    if (numSector == 3) sectorRonda[0] = leerSector();
    else sectorRonda[numSector + 1] = leerSector();
  }
  //moveDistancia(100, vMinSector, vMinSector);
  desactivarHL();
  //while (!commandFinished)updateRobot();
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
    moveDistancia(dist, vMaxSector, vMaxSector);
    while (!commandFinished) updateRobot();
  }
  angPID = direccion;
  if (angPID == 270) while (abs(abs(getCarAngle()) - 90) > 10) updateRobot();
  else while (abs(abs(getCarAngle()) - abs(angPID)) > 10) updateRobot();
}
