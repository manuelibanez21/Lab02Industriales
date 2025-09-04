/*
 * Arduino UNO - Emisor UART con VRC y LRC + errores aleatorios
 * Comunicación con Raspberry Pi Pico usando SoftwareSerial
 * 
 * Pines:
 *   - Pin 11: TX hacia Pico (con divisor resistivo si se conecta directo)
 *   - Pin 10: RX desde Pico
 * 
 * Serial (USB): depuración en el Monitor Serie
 * PicoSerial: envío real al Pico
 */

#include <SoftwareSerial.h>
SoftwareSerial PicoSerial(10, 11); // RX=10, TX=11

byte dataBlock[5] = {0x12, 0x34, 0x56, 0x78, 0x9A};

// Calcular paridad par (VRC)
byte calcParity(byte data) {
  byte parity = 0;
  for (int i = 0; i < 8; i++) {
    parity ^= (data >> i) & 0x01;
  }
  return parity;
}

// Calcular LRC (XOR)
byte calcLRC(byte *block, int len) {
  byte lrc = 0x00;
  for (int i = 0; i < len; i++) {
    lrc ^= block[i];
  }
  return lrc;
}

void setup() {
  Serial.begin(9600);      // Monitor Serie
  PicoSerial.begin(9600);  // UART hacia Pico
  randomSeed(analogRead(0)); // Inicializar generador aleatorio
  Serial.println("Arduino emisor con errores aleatorios listo!");
}

void loop() {
  Serial.println("Enviando bloque...");

  // Enviar 5 datos con sus paridades
  for (int i = 0; i < 5; i++) {
    byte d = dataBlock[i];
    byte p = calcParity(d);

    // Probabilidad de introducir error (20%)
    if (random(100) < 20) {
      // Decidir aleatoriamente si corromper dato o paridad
      if (random(2) == 0) {
        d ^= (1 << random(8)); // Cambiar un bit aleatorio del dato
        Serial.println("⚠️ Error introducido en dato!");
      } else {
        p ^= 1; // Invertir paridad
        Serial.println("⚠️ Error introducido en paridad!");
      }
    }

    // Enviar al Pico
    PicoSerial.write(d);
    PicoSerial.write(p);

    // Mostrar en consola
    Serial.print("Dato enviado: ");
    Serial.print(d, HEX);
    Serial.print(" Paridad: ");
    Serial.println(p);
  }

  // Enviar LRC (sin errores, para ver si bloque se detecta mal por un byte corrupto)
  byte lrc = calcLRC(dataBlock, 5);
  PicoSerial.write(lrc);

  Serial.print("LRC enviado: ");
  Serial.println(lrc, HEX);
  Serial.println("-------------------------");

  delay(3000); // Espera entre bloques
}

