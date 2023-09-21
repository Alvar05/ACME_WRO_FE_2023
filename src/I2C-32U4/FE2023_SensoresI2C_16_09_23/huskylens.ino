void setupHL() {
  for (int i = 0; i < 4 and camaraIniciada == false; i++) {
    camaraIniciada = huskylens.begin(Wire);   // T: SDA, R: SCL
    if (!camaraIniciada) {
      Serial.println("HuskyLens: Error");
      delay(100);
    }
    else Serial.println("HuskyLens: OK");
  }
}

int readMedianaHL(int index, int nIteraciones, char varOut) {     //maxIteraciones: 15, maxBloques: 4 (se puede  cambiar)
  if (!camaraIniciada) return -1;
  static int xRaw[60];
  static int yRaw[60];
  static int wRaw[60];
  static int hRaw[60];
  static int idRaw[60];
  int arrOut[nIteraciones];

  huskylens.requestBlocks();
  HUSKYLENSResult bloque = huskylens.get(index);
  if (index == 98) index = 0;
  if (index == 99) index = 1;

  switch (varOut) {
    case 'X':
      xRaw[indiceMedianaHL[index][0] + (index * 15)] = bloque.xCenter;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = xRaw[i + (index * 15)];
      indiceMedianaHL[index][0]++;
      break;
    case 'Y':
      yRaw[indiceMedianaHL[index][1] + (index * 15)] = bloque.yCenter;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = yRaw[i + (index * 15)];
      indiceMedianaHL[index][1]++;
      break;
    case 'W':
      if (camaraGirada == 1) wRaw[indiceMedianaHL[index][2] + (index * 15)] = bloque.width;    // Por si la imagen esta transpuesta por girar el espejo
      else wRaw[indiceMedianaHL[index][2] + (index * 15)] = bloque.height;                 // Aqui se transpone si es el caso
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = wRaw[i + (index * 15)];
      indiceMedianaHL[index][2]++;
      break;
    case 'H':
      if (camaraGirada == 1) hRaw[indiceMedianaHL[index][3] + (index * 15)] = bloque.height;   // Por si la imagen esta transpuesta por girar el espejo
      else hRaw[indiceMedianaHL[index][3] + (index * 15)] = bloque.width;                  // Aqui se transpone si es el caso
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = hRaw[i + (index * 15)];
      indiceMedianaHL[index][3]++;
      break;
    case 'I':
      idRaw[indiceMedianaHL[index][4] + (index * 15)] = bloque.ID;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = idRaw[i + (index * 15)];
      indiceMedianaHL[index][4]++;
      break;
  }

  quickSort(arrOut, 0, nIteraciones - 1);
  for (int k = 0; k < 10; k++) {
    if (indiceMedianaHL[index][k] == nIteraciones) indiceMedianaHL[index][k] = 0;
  }

  return arrOut[int(nIteraciones / 2)];
}

void quickSort(int* arr, const int left, const int right) {

  int i = left, j = right;
  int tmp;

  int pivot = arr[(left + right) / 2];
  while (i <= j) {
    while (arr[i] < pivot) i++;
    while (arr[j] > pivot) j--;
    if (i <= j) {
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

void juntarFiltrarBloquesHL(int pxFiltrados, int margenPx) {    // Para cuando se usan 2 ID por color, filtra los index y los junta
  if (!camaraIniciada) return;
  huskylens.requestBlocks();
  int bValido[2][4] = {{ -1, -1, -1, -1}, {96, 97, 98, 99}}; // Height, Index (predeterminado, no es bonito lo se)
  int numIndices = 0;
  for (int i = 0; i < constrain(huskylens.countBlocks(), 0, 2); i++) {
    HUSKYLENSResult bloque = huskylens.get(i);
    if (camaraGirada == 1 and bloque.yCenter < 240 - pxFiltrados) bValido[0][i] = bloque.height;
    else if (camaraGirada == 0 and bloque.xCenter < 320 - pxFiltrados) bValido[0][i] = bloque.width;
    else if (camaraGirada == 2 and bloque.xCenter > pxFiltrados) bValido[0][i] = bloque.width;
    else continue;
    bValido[1][i] = i;
    numIndices++;
  }
  int indexValido[4];
  for (int i = 0; i < 4; i++) indexValido[i] = bValido[1][i];
  quickSort(indexValido, 0, 3);

  int indexReal[2] = {98, 99};

  if (numIndices == 1) indexReal[0] = indexValido[0];
  else if (numIndices == 2) {
    if (mismoBloque(indexValido[0], indexValido[1], margenPx)) indexReal[0] = indexValido[0];
    else for (int i = 0; i < 2; i++) indexReal[i] = indexValido[i];
  }
  else {
    if (mismoBloque(indexValido[0], indexValido[1], margenPx)) {
      indexReal[0] = indexValido[0];
      indexReal[1] = indexValido[2];
    }
    else if (mismoBloque(indexValido[1], indexValido[2], margenPx)) {
      indexReal[0] = indexValido[0];
      indexReal[1] = indexValido[1];
    }
    else if (mismoBloque(indexValido[0], indexValido[2], margenPx)) {
      indexReal[0] = indexValido[0];
      indexReal[1] = indexValido[1];
    }
  }

  HUSKYLENSResult b0 = huskylens.get(indexReal[0]);
  HUSKYLENSResult b1 = huskylens.get(indexReal[1]);

  indexBloques[0] = indexReal[0];
  indexBloques[1] = indexReal[1];

  if (b1.height >= b0.height) {
    indexBloques[0] = indexReal[1];
    indexBloques[1] = indexReal[0];
  }
}

bool mismoBloque (int index1, int index2, int margenPx) {
  HUSKYLENSResult b0 = huskylens.get(index1);
  HUSKYLENSResult b1 = huskylens.get(index2);
  if ((b0.xCenter > b1.xCenter - margenPx) and (b0.xCenter < b1.xCenter + margenPx) and (b0.yCenter > b1.yCenter - margenPx) and (b0.yCenter < b0.yCenter + margenPx)/* and ((b0.ID == b1.ID + 2) or (b0.ID + 2 == b1.ID))*/) {
    return true;
  }
  else return false;
}

void filtrarBloquesHL(int pxFiltrados) {
  if (!camaraIniciada) return;
  huskylens.requestBlocks();
  int bValido[2][huskylens.countBlocks()] = {{ -1, -1}, {98, 99}}; // Height, Index (predeterminado, no es bonito lo se)
  int indiceBueno = 0;
  for (int i = 0; i < huskylens.countBlocks(); i++) {
    HUSKYLENSResult bloque = huskylens.get(i);
    if (camaraGirada == 1 and bloque.yCenter < 240 - pxFiltrados) bValido[0][indiceBueno] = bloque.height;
    else if (camaraGirada == 0 and bloque.xCenter < 320 - pxFiltrados) bValido[0][indiceBueno] = bloque.width;
    else if (camaraGirada == 2 and bloque.xCenter > pxFiltrados) bValido[0][indiceBueno] = bloque.width;
    else continue;
    bValido[1][indiceBueno] = i;
    indiceBueno++;
  }
  indexBloques[0] = bValido[1][0];
  indexBloques[1] = bValido[1][1];
  if (bValido[0][1] >= bValido[0][0]) {
    indexBloques[0] = bValido[1][1];
    indexBloques[1] = bValido[1][0];
  }
}

/*byte readSector() {
  if (!camaraIniciada) return -1;
  long distBloqueC = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[0], 15, 'H');
  long distBloqueL = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[1], 15, 'H');
  int ID_C = readMedianaHL(indexBloques[0], 15, 'I');
  int ID_L = readMedianaHL(indexBloques[1], 15, 'I');
  if (ID_C == 3) ID_C = 1;
  if (ID_C == 4) ID_C = 2;
  if (ID_L == 3) ID_L = 1;
  if (ID_L == 4) ID_L = 2;

  if (ID_C == -1 and ID_L == -1) return 11;         // No hay bloques
  else if (ID_C != -1 and ID_L == -1) {             // Hay 1 bloque
    if (distBloqueC < 700) return 0 + ID_C;           // Bloque Cerca
    else if (distBloqueC > 1200) return 4 + ID_C;       // Bloque Lejos
    else return 2 + ID_C;                             // Bloque Centro
  }
  else {                                            // Hay 2 bloques
    if (ID_C != ID_L and ID_C == 2) return 10;        // Rojo - Verde
    else return 5 + ID_C + ID_L;
  }
  }*/

byte readSectorLejos() {
  if (!camaraIniciada) return -1;
  long distBloqueC = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[0], 15, 'H');
  long distBloqueL = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[1], 15, 'H');
  int ID_C = readMedianaHL(indexBloques[0], 15, 'I');
  int ID_L = readMedianaHL(indexBloques[1], 15, 'I');
  if (ID_C == 3) ID_C = 1;
  if (ID_C == 4) ID_C = 2;
  if (ID_L == 3) ID_L = 1;
  if (ID_L == 4) ID_L = 2;

  if (ID_C == -1 and ID_L == -1) return 11;         // No hay bloques
  else if (ID_C != -1 and ID_L == -1) {             // Hay 1 bloque
    if (distBloqueC < command.distBloqueCerca * 10) return 0 + ID_C;         // Bloque Cerca
    else if (distBloqueC > command.distBloqueLejos * 10) return 4 + ID_C;     // Bloque Lejos
    else return 2 + ID_C;                             // Bloque Centro
  }
  else {                                            // Hay 2 bloques
    if (ID_C != ID_L and ID_C == 2) return 10;        // Rojo - Verde
    else return 5 + ID_C + ID_L;
  }
}

byte readSectorMedianaLejos(int nIteraciones) {    // maxIteraciones = 15
  if (!camaraIniciada) return -1;
  static byte sectorArray[15];
  sectorArray[indiceSector] = readSectorLejos();
  indiceSector++;

  int arrOut[nIteraciones];
  for (int i = 0; i < nIteraciones; i++) arrOut[i] = sectorArray[i];
  quickSort(arrOut, 0, nIteraciones - 1);
  if (indiceSector == nIteraciones) indiceSector = 0;

  return arrOut[byte(nIteraciones / 2)];
}
