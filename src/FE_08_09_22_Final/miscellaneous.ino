//This tab contains all related to showing sensor data on the M5Stack screen
void infoPantalla() { // Shows in the M5 screen all sensor data
  M5.update();
  if (M5.BtnC.wasPressed()) {
    debugPantalla = !debugPantalla;
    M5.Lcd.clear();
    M5.Lcd.setTextSize(2);
    //if (imprimirStart) {
    M5.Lcd.setCursor(135, 220);
    M5.Lcd.print("Start");
    //}
    M5.Lcd.setCursor(230, 220);
    M5.Lcd.print("Debug");
  }
  if (debugPantalla) {
    sensorUpdate();
    loopMPU();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(282, 0);
    M5.Lcd.print(M5.Power.getBatteryLevel() + String("    "));

    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.print(sensor.sr04[0] + String("   "));
    M5.Lcd.setCursor(130, 9);
    M5.Lcd.print(sensor.sr04[1] + String("   "));
    M5.Lcd.setCursor(260, 50);
    M5.Lcd.print(sensor.sr04[2] + String("   "));

    M5.Lcd.setTextSize(6);
    M5.Lcd.setCursor(50, 90);
    if (ypr[0] < 0) M5.Lcd.print(ypr[0] + String("  "));
    else M5.Lcd.print(String(" ") + ypr[0] + String("  "));

    M5.Lcd.setTextSize(3);

    M5.Lcd.setCursor(0, 150);
    M5.Lcd.print(motorGetDistancia("B") + String("   "));
    M5.Lcd.setCursor(260, 150);
    M5.Lcd.print(motorGetDistancia("C") + String("   "));

  }
}

void infoPantallaSetup() { //Shows the buttons to start program and debug
  M5.Lcd.clear();
  M5.Lcd.setTextSize(2);
  //if (imprimirStart) {
  M5.Lcd.setCursor(135, 220);
  M5.Lcd.print("Start");
  //}
  M5.Lcd.setCursor(230, 220);
  M5.Lcd.print("Debug");
}
