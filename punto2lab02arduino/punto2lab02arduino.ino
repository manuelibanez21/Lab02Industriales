/* punto2_arq_simple.ino
   Arduino UNO - Emisor con retransmisión (ARQ Simple)
   Envía paquetes: [LEN][SEQ][DATA...][CHK]
   Espera respuesta del receptor (ACK/NACK).
   Si recibe NACK, retransmite el mismo paquete.
*/

#include <SoftwareSerial.h>

const int RX_PIN = 10;   // pin RX (Arduino recibe respuesta de Pico)
const int TX_PIN = 11;   // pin TX hacia RX de la Pico
SoftwareSerial mySerial(RX_PIN, TX_PIN);

const unsigned long BAUD = 9600;
uint8_t seq = 0; // número de secuencia, se incrementa en cada envío

void setup() {
  Serial.begin(BAUD);       // Monitor serie con la PC
  mySerial.begin(BAUD);     // Canal software hacia la Pico
  randomSeed(analogRead(0)); // Semilla aleatoria
  Serial.println("Punto 2 - Emisor con retransmisión (ARQ Simple).");
}

uint8_t calcular_checksum(uint8_t len, uint8_t seq, uint8_t *data) {
  uint16_t s = 0;
  s += len;
  s += seq;
  for (uint8_t i = 0; i < len; i++) s += data[i];
  return (uint8_t)(s & 0xFF);
}

void enviar_paquete(uint8_t *data, uint8_t len) {
  uint8_t chk = calcular_checksum(len, seq, data);

  // Con probabilidad del 20% se fuerza error en el checksum
  if (random(100) < 20) {
    chk = chk + 1;
    Serial.println("⚠️ Paquete con ERROR forzado.");
  }

  // Enviar trama: LEN, SEQ, DATA..., CHK
  mySerial.write(len);
  mySerial.write(seq);
  for (uint8_t i = 0; i < len; i++) mySerial.write(data[i]);
  mySerial.write(chk);

  // Mostrar en consola de Arduino
  Serial.print("Enviado paquete seq=");
  Serial.print(seq);
  Serial.print(" len=");
  Serial.print(len);
  Serial.print(" chk=");
  Serial.println(chk);
}

bool esperar_respuesta(unsigned long timeout_ms) {
  unsigned long t0 = millis();
  String respuesta = "";

  while (millis() - t0 < timeout_ms) {
    if (mySerial.available()) {
      char c = mySerial.read();
      if (c == '\n') break; // fin de mensaje
      respuesta += c;
    }
  }

  if (respuesta == "ACK") {
    Serial.println("✅ Recibido ACK");
    return true; // correcto
  } else if (respuesta == "NACK") {
    Serial.println("❌ Recibido NACK");
    return false; // error, retransmitir
  } else {
    Serial.println("⚠️ Sin respuesta / desconocida: " + respuesta);
    return false; // asumir error
  }
}

void loop() {
  uint8_t datos[] = { 'A', 'B', 'C' };
  bool entregado = false;
  int reintentos = 0;

  while (!entregado) {
    enviar_paquete(datos, 3);
    entregado = esperar_respuesta(500); // espera 500 ms máx
    if (!entregado) {
      reintentos++;
      Serial.println("↻ Reintentando... intento #" + String(reintentos));
    }
  }

  Serial.print("Paquete confirmado con ");
  Serial.print(reintentos);
  Serial.println(" reintentos.");

  seq++; // aumentar número de secuencia
  delay(2000); // espera 2 seg antes de siguiente paquete
}

