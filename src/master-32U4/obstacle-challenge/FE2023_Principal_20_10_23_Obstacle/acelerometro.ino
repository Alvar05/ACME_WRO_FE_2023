//This tab includes the functions to initalize MPU 6050 and update its data
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setupMPU() {
  Wire.setClock(400000);
  Wire.begin();
  mpu.begin();
  delay(1000);
  mpu.calcGyroOffsets();
  delay(3000);
}

// ================================================================
// ===                    UPDATE MPU DATA                       ===
// ================================================================

void updateMPU() { //Updates MPU(angle) data
  mpu.update();
  ypr[0] = mpu.getAngleZ() * (-1) - offsetMPU;
  ypr[0] = ((int)ypr[0] % 360) + (ypr[0] - (int)ypr[0]);
  if (ypr[0] > 180) {
    ypr[0] = (ypr[0] - 360);
  }
  if (ypr[0] < -180) {
    ypr[0] = (ypr[0] + 360);
  }///// Forces the angle to 180 <---> -180
}
