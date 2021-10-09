void receiveEvent(int bytes) {
  uint8_t index = 0;
  if (bytes == 20)index = 0;
  else if (bytes == 14)index = 20;
  else return;
  //Serial.println(bytes);

  while (Wire.available()) {
    byte*pointer = (byte*)&sensores;
    pointer = pointer + index;
    *pointer = (byte)Wire.read();
    index++;
  }
}
