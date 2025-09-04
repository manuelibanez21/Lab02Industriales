/* punto1_checksum_error.ino
   Arduino UNO - Emisor con errores aleatorios
   Envía paquetes: [LEN][SEQ][DATA...][CHK]
   En 20% de los casos se altera el checksum para forzar error.
*/

#include <SoftwareSerial.h>

const int RX_PIN = 10;   // pin RX (no usado en este punto)
const int TX_PIN = 11;   // pin TX hacia RX de la Pico
SoftwareSerial mySerial(RX_PIN, TX_PIN);

const unsigned long BAUD = 9600;
const uint8_t SEQ = 0; // en punto 1 fijo a 0

void setup() {
  Serial.begin(BAUD);       // Monitor serie con la PC
  mySerial.begin(BAUD);     // Canal software hacia la Pico
  randomSeed(analogRead(0)); // Semilla aleatoria
  Serial.println("Punto 1 - Emisor con errores aleatorios (20%).");
}

uint8_t calcular_checksum(uint8_t len, uint8_t seq, uint8_t *data) {
  uint16_t s = 0;
  s += len;
  s += seq;
  for (uint8_t i = 0; i < len; i++) s += data[i];
  return (uint8_t)(s & 0xFF);
}

void enviar_paquete(uint8_t *data, uint8_t len) {
  uint8_t chk = calcular_checksum(len, SEQ, data);

  // Con probabilidad del 20% alteramos el checksum
  if (random(100) < 20) {
    chk = chk + 1; // forzar error
    Serial.println("⚠️ Paquete con ERROR forzado.");
  }

  // Enviar en orden: LEN, SEQ, DATA..., CHK
  mySerial.write(len);
  mySerial.write(SEQ);
  for (uint8_t i = 0; i < len; i++) mySerial.write(data[i]);
  mySerial.write(chk);

  // Imprimir en monitor para evidencias
  Serial.print("Enviado paquete len=");
  Serial.print(len);
  Serial.print(" chk=");
  Serial.println(chk);
}

void loop() {
  // Ejemplo: enviar la cadena "ABC"
  uint8_t datos[] = { 'A', 'B', 'C' };
  enviar_paquete(datos, 3);
  delay(1000);
}
