/*
Recibir de Master (SPI):
(byte) lado a mirar con la camara, instruccion de mandar el sector al Master                                        | esto se puede juntar en un byte
(byte) LEDs del coche (000abcde): a = int izq, b = int der, c = freno, d = frontales cortas, e = frontales largas   |
(byte) lado camara y sector + LEDs del coche
       (xyzabcde): x = cam lado izq, y = cam lado der, z = leer sector, a = int izq, b = int der, c = freno, d = frontales cortas, e = frontales largas
(byte) pixeles del filtro de la camara (se pueden añadir bytes para meter mas filtros)

Enviar a Master (SPI):
(byte) sector
(int) TOF izquierdo
(int) TOF derecho
(int) MPU

Enviar a Master (Pin Digital) (opcional):
(4 pines) sector en binario
(1 pin) TOF izquierdo por encima / debajo de threshold (tal vez programable por el Master por SPI)
(1 pin) TOF derecho por encima / debajo de threshold (tal vez programable por el Master por SPI)
*/
