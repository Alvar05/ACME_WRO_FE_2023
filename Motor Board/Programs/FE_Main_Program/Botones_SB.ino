String buttonsCurrent() {
  String current = "";
  if (digitalRead(BOTON_UP) == 0) current.concat("U");
  if (digitalRead(BOTON_DOWN) == 0) current += "D";
  return current;
}
void buttonsWait() {
  while (buttonsCurrent() == "");
}
void buttonsWait(String boton) {
  while (buttonsCurrent() != boton);
}
