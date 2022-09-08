//This tab includes the functions to initalize MPU 6050 and update its data
// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setupMPU() {
  Wire.begin();
  mpu.begin();
  mpu.setGyroOffsets(-1.88, 0.84, 0.31);
}

// ================================================================
// ===                    UPDATE MPU DATA                       ===
// ================================================================

void loopMPU() { //Updates MPU(angle) data
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
