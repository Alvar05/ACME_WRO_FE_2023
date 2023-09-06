void leerInicio() {
  if ((sensor.sr04[0] + sensor.sr04[2]) > 70) { //PARED NORMAL
    disPared[0] = 0;
    if (sensor.sr04[0] < 30)posInicial = 1;
    else if (sensor.sr04[2] < 30) posInicial = 3;
    else posInicial = 2;
  }
  else { //PARED ENSANCHADA
    disPared[0] = 1;
    if (sensor.sr04[0] < 10) { // Implica sentido antihorario y posición central
      posInicial = 2;
      sentido = 180;
    }
    else if (sensor.sr04[2] < 10) { // Implica sentido horario y posición central
      posInicial = 2;
      sentido = 0;
    }
    // A partir de aqui implica que está en el lateral
    else if (sensor.sr04[0] > 25) {
      posInicial = 3;
      sentido = 180;
    }
    else{
      posInicial = 1;
      sentido = 0;
    }
  }
}
