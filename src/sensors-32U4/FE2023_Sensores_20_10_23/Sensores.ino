void sensorSetup() {
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting
  lastPing = pingTimer[0];
  for (uint8_t i = 1; i < SONAR_NUM; i++) { // Set the starting time for each sensor
    pingTimer[i] = lastPing + PING_INTERVAL;
    lastPing = pingTimer[i];
  }
}

void sensorUpdate() { // Updates to the SPI structure the sensor values
  sensor.encoderI = motorGetDistancia("I");
  sensor.encoderD = motorGetDistancia("D");
  updateMotor();
  sensor.velocidad = motorGetSpeed();
  ultrasonicUpdate();
}

void ultrasonicUpdate() {
  if (!(activeUS[0]) and !(activeUS[1]) and !(activeUS[2])) lastPing = millis();
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors
    if (millis() >= pingTimer[i] && activeUS[i]) {  // Is it this sensor active and time to ping?
      int cycle = -1;
      if (activeUS[0]) cycle = 0;
      else if (activeUS[1]) cycle = 1;
      else if (activeUS[2]) cycle = 2;
      pingTimer[i] = lastPing + PING_INTERVAL;  // Set next time this sensor will be pinged
      lastPing += PING_INTERVAL;
      if (i == cycle) oneSensorCycle(); // Sensor ping cycle complete
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping
      currentSensor = i;                          // Sensor being accessed
      if (currentIteration == ITERATIONS - 1) currentIteration = 0;
      else currentIteration++;
      timeRAW[currentSensor][currentIteration] = 14535; // Time which divided by US_ROUNDTRIP_CM equals 255. Make distance 255 in case there's no ping echo for this sensor
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo every 24 µS).
    }
  }
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (echoCheckFlag) {
    if (sonar[currentSensor].check_timer()) {
      timeRAW[currentSensor][currentIteration] = sonar[currentSensor].ping_result;
    }
  }
}

void oneSensorCycle() { // Sensor ping cycle complete. Sorts the distances and takes the median value
  unsigned int cm[SONAR_NUM][ITERATIONS];
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    for (uint8_t j = 0; j < ITERATIONS; j++)cm[i][j] = timeRAW[i][j] / US_ROUNDTRIP_CM;
  }
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (activeUS[i]) {
      quickSort(cm[i], 0, ITERATIONS - 1);
      sensor.sr04[i] = cm[i][ITERATIONS / 2];
    }
  }
  newLecture = true;
}

void quickSort(int* arr, const int left, const int right) { // Ascendant QuickSort
  int i = left, j = right;
  int tmp;

  int pivot = arr[(left + right) / 2];
  while (i <= j)
  {
    while (arr[i] < pivot) i++;
    while (arr[j] > pivot) j--;
    if (i <= j)
    {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };

  if (left < j)
    quickSort(arr, left, j);
  if (i < right)
    quickSort(arr, i, right);
}
