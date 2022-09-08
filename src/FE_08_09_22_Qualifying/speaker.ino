void speakerSetup() { //Initiates speaker
  ledcSetup(TONE_PIN_CHANNEL, 0, 13);
  ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
}

void speakerTone(int frecuencia) { // Starts speaker at a certain frequency
  ledcWriteTone(TONE_PIN_CHANNEL, frecuencia);
}

void speakerBeep(int frecuencia, int duracion) { // Makes a beep at a certain frequency a certain time ()
  frecuenciaBeep = frecuencia;
  duracionBeep = millis() + duracion;
  ledcWriteTone(TONE_PIN_CHANNEL, frecuenciaBeep);
}

void speakerMute() { // Mutes the speaker
  ledcWriteTone(TONE_PIN_CHANNEL, 0);
  digitalWrite(SPEAKER_PIN, 0);
}

void speakerUpdate() { //Updates the state of speakerBeep()
  if (frecuenciaBeep != 0) {
    if (millis() > duracionBeep) {
      frecuenciaBeep = 0;
      duracionBeep = 0;
      speakerMute();
    }
  }
}
