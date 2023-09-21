void beep(int t) {
  beepDone = false;
  beepTime = millis() + t;
  digitalWrite(BUZZ, 1);
}

void updateBeep() {
  if ((millis() > beepTime) and !beepDone){
    beepDone = true;
    digitalWrite(BUZZ, 0);
  }
}
