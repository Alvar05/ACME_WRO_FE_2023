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

int readMedianaHL(int index, int numBloqueGuardado, int nIteraciones, char varOut) {     //maxIterations: 15, maxBlocks: 4 (numBloqueGuardado: 0, 1, 2, 3)
  if (!camaraIniciada) return -1;
  static int xRaw[60];
  static int yRaw[60];
  static int wRaw[60];
  static int hRaw[60];
  static int idRaw[60];
  int arrOut[nIteraciones];

  HUSKYLENSResult bloque = huskylens.get(index);

  switch (varOut) {
    case 'X':
      xRaw[indiceMedianaHL[numBloqueGuardado][0] + (numBloqueGuardado * 15)] = bloque.xCenter;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = xRaw[i + (numBloqueGuardado * 15)];
      indiceMedianaHL[numBloqueGuardado][0]++;
      break;
    case 'Y':
      yRaw[indiceMedianaHL[numBloqueGuardado][1] + (numBloqueGuardado * 15)] = bloque.yCenter;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = yRaw[i + (numBloqueGuardado * 15)];
      indiceMedianaHL[numBloqueGuardado][1]++;
      break;
    case 'W':
      if (camaraGirada == 1) wRaw[indiceMedianaHL[numBloqueGuardado][2] + (numBloqueGuardado * 15)] = bloque.width;    // To transponse the image if it is rotated
      else wRaw[indiceMedianaHL[numBloqueGuardado][2] + (numBloqueGuardado * 15)] = bloque.height;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = wRaw[i + (numBloqueGuardado * 15)];
      indiceMedianaHL[numBloqueGuardado][2]++;
      break;
    case 'H':
      if (camaraGirada == 1) hRaw[indiceMedianaHL[numBloqueGuardado][3] + (numBloqueGuardado * 15)] = bloque.height;   // To transponse the image if it is rotated
      else hRaw[indiceMedianaHL[numBloqueGuardado][3] + (numBloqueGuardado * 15)] = bloque.width;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = hRaw[i + (numBloqueGuardado * 15)];
      indiceMedianaHL[numBloqueGuardado][3]++;
      break;
    case 'I':
      idRaw[indiceMedianaHL[numBloqueGuardado][4] + (numBloqueGuardado * 15)] = bloque.ID;
      for (int i = 0; i < nIteraciones; i++) arrOut[i] = idRaw[i + (numBloqueGuardado * 15)];
      indiceMedianaHL[numBloqueGuardado][4]++;
      break;
  }

  quickSort(arrOut, 0, nIteraciones - 1);
  for (int k = 0; k < 5; k++) {
    if (indiceMedianaHL[numBloqueGuardado][k] == nIteraciones) indiceMedianaHL[numBloqueGuardado][k] = 0;
  }

  return arrOut[int(nIteraciones / 2)];
}

void quickSort(int* arr, const int left, const int right) { // Descendant QuickSort
  int i = left, j = right;
  int tmp;
  int pivot = arr[(left + right) / 2];

  while (i <= j) {
    while (arr[i] > pivot) i++;
    while (arr[j] < pivot) j--;
    if (i <= j) {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };

  if (left < j) quickSort(arr, left, j);
  if (i < right) quickSort(arr, i, right);
}

void filtrarHL(int filtroSuperior, int w[2], int h[2]) {        // It sorts the blocks by distance
  if (!camaraIniciada) return;                                  // w[0] = widthMin, w[1] = widthMax; h[0] = heightMin, h[1] = heightMax
  huskylens.requestBlocks();
  int numBlocks = huskylens.countBlocks();
  if (numBlocks < 1) numBlocks = 1;
  
  int hValido[numBlocks];
  int hOrdenado[numBlocks];
  
  for (int i = 0; i < huskylens.countBlocks(); i++) {
    hValido[i] = 999;
    HUSKYLENSResult bloque = huskylens.get(i);
    if (camaraGirada == 1) {
      if ((bloque.yCenter < 240 - filtroSuperior) and bloque.width >= w[0] and bloque.width <= w[1] and bloque.height >= h[0] and bloque.height <= h[1]) {
        hValido[i] = bloque.height;
      }
    }
    else if (camaraGirada == 0) {
      if ((bloque.xCenter < 320 - filtroSuperior) and bloque.width >= h[0] and bloque.width <= h[1] and bloque.height >= w[0] and bloque.height <= w[1]) {
        hValido[i] = bloque.width;
      }
    }
    else if (camaraGirada == 2) {
      if ((bloque.xCenter > filtroSuperior) and bloque.width >= h[0] and bloque.width <= h[1] and bloque.height >= w[0] and bloque.height <= w[1]) {
        hValido[i] = bloque.width;
      }
    }
    hOrdenado[i] = hValido[i];
  }
  quickSort(hOrdenado, 0, abs(huskylens.countBlocks()) - 1);     //it sorts the copy of the array by descendant height
  int arrOut[numBlocks];
  for (int i = 0; i < huskylens.countBlocks(); i++) {
    int index;
    for (int j = 0; j < huskylens.countBlocks(); j++) {
      if (hOrdenado[i] == hValido[j]) {
        index = j;
        break;
      }
    }
    arrOut[i] = index;
  }
  int indiceArr = 0;
  for (indiceArr = 0; indiceArr < huskylens.countBlocks() and indiceArr < MAX_BLOCKS; indiceArr++) indexBloques[indiceArr] = arrOut[indiceArr];
  if (MAX_BLOCKS - indiceArr + 1 > 0) {
    for (int i = indiceArr; i < MAX_BLOCKS - indiceArr + 1; i++) indexBloques[i] = 999;
  }
}

void juntarID(int margen) {       // It joins two IDs if they are the same block
  int numBlocks = huskylens.countBlocks();
  if (numBlocks < 1) numBlocks = 1;
  
  int indexBloquesID[numBlocks];
  int indice = 0;
  bool igualAnterior = false;

  for (int i = 0; i < huskylens.countBlocks(); i++) {
    indexBloquesID[i] = 999;
    if (!igualAnterior) {
      indexBloquesID[indice] = indexBloques[i];
      indice++;
      if (mismoIndex(indexBloques[i], indexBloques[i + 1], margen)) igualAnterior = true;
    }
    else if (!mismoIndex(indexBloques[i], indexBloques[i + 1], margen)) igualAnterior = false;
  }
  for (int i = 0; i < huskylens.countBlocks() and i < MAX_BLOCKS; i++) indexBloques[i] = indexBloquesID[i];
}

bool mismoIndex (int index1, int index2, int margen) {
  HUSKYLENSResult b0 = huskylens.get(index1);
  HUSKYLENSResult b1 = huskylens.get(index2);
  if ((b0.xCenter > b1.xCenter - margen) and (b0.xCenter < b1.xCenter + margen) and (b0.yCenter > b1.yCenter - margen) and (b0.yCenter < b0.yCenter + margen)) {
    return true;
  }
  else return false;
}

byte readSector(int nIterLectura) {
  if (!camaraIniciada) return -1;
  long distBloqueC = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[0], 0, nIterLectura, 'H');
  long distBloqueL = (alturaBloque * distanciaBloque) / readMedianaHL(indexBloques[1], 1, nIterLectura, 'H');
  int ID[2] = {readMedianaHL(indexBloques[0], 0, nIterLectura, 'I'), readMedianaHL(indexBloques[1], 1, nIterLectura, 'I')};   // ID[0] = close, ID[1] = far
  
  for (int i = 0; i < NUM_ID; i++) {        // Converts the differents IDs: 1 Green, 2 Red
    if (ID[0] == ID_VERDE[i]) ID[0] = 1;
    else if (ID[0] == ID_ROJO[i]) ID[0] = 2;

    if (ID[1] == ID_VERDE[i]) ID[1] = 1;
    else if (ID[1] == ID_ROJO[i]) ID[1] = 2;
  }

  if (ID[0] == -1 and ID[1] == -1) return 11;           // No blocks
  else if (ID[0] != -1 and ID[1] == -1) {               // 1 block
    if (distBloqueC < command.distBloqueCerca * 10) return 0 + ID[0];         // close Block
    else if (distBloqueC > command.distBloqueLejos * 10) return 4 + ID[0];    // far Block
    else return 2 + ID[0];                              // Center block
  }
  else {                                                // 2 blocks
    if (ID[0] != ID[1] and ID[0] == 2) return 10;       // Red - Green
    else return 5 + ID[0] + ID[1];
  }
}

byte readSectorMediana(int nIteraciones) {    // maxIterations = 15
  if (!camaraIniciada) return -1;
  static byte sectorArray[15];
  sectorArray[indiceSector] = readSector(nIteraciones);
  indiceSector++;

  int arrOut[nIteraciones];
  for (int i = 0; i < nIteraciones; i++) arrOut[i] = sectorArray[i];
  quickSort(arrOut, 0, nIteraciones - 1);
  if (indiceSector == nIteraciones) indiceSector = 0;

  return arrOut[byte(nIteraciones / 2)];
}
