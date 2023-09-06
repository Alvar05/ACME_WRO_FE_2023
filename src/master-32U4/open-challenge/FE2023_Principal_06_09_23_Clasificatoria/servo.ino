void setDireccionCamara(int grados){
  commandB.angServo = grados;
  enviarSPIB();
}
