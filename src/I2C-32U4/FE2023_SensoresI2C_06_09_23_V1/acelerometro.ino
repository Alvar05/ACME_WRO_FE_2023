//This tab includes the functions to initalize MPU 6050 and update its data
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setupMPU() {
  mpu.begin();
  delay(1000);
  mpu.setGyroOffsets(-1.75, 0.97, -0.27);
}

// ================================================================
// ===                    UPDATE MPU DATA                       ===
// ================================================================

void updateMPU() { //Updates MPU(angle) data    //En honor a "loopMPU()" o7
  mpu.update();
  ypr[0] = mpu.getAngleZ() * (-1) - offsetMPU;
  ypr[0] = ((int)ypr[0] % 360) + (ypr[0] - (int)ypr[0]);
  if (ypr[0] > 180) {
    ypr[0] = (ypr[0] - 360);
  }
  if (ypr[0] < -180) {
    ypr[0] = (ypr[0] + 360);
  }///// Forces the angle to 180 <---> -180
  i2c.MPU = int(ypr[0] * 100);
}
