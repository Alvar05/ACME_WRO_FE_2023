void activarUS(String us){
  for (int i = 0; i < us.length(); i++) {
    if (us.charAt(i) == 'L') bitSet(commandA.flags, 0);
    else if (us.charAt(i) == 'F') bitSet(commandA.flags, 1);
    else if (us.charAt(i) == 'R') bitSet(commandA.flags, 2);
  }
  enviarSPIA();
}

void desactivarUS(String us){
  for (int i = 0; i < us.length(); i++) {
    if (us.charAt(i) == 'L') bitClear(commandA.flags, 0);
    else if (us.charAt(i) == 'F') bitClear(commandA.flags, 1);
    else if (us.charAt(i) == 'R') bitClear(commandA.flags, 2);
  }
  enviarSPIA();
}
