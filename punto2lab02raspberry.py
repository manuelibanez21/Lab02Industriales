# pico_receptor_punto2_arq.py
# Receptor MicroPython (Raspberry Pi Pico)
# Punto 2: ACK/NACK (ARQ simple) con probabilidad de error simulada.
# Recibe [LEN][SEQ][DATA...][CHK], valida checksum, responde "ACK" o "NACK".

from machine import UART, Pin
import utime
import urandom  # para generar probabilidad de NACK aleatorio

uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))  # UART0 -> GP0(TX), GP1(RX)

def calcular_checksum(len_b, seq_b, data_bytes):
    s = len_b + seq_b
    for b in data_bytes:
        s += b
    return s & 0xFF

# Probabilidad de responder NACK aunque el paquete sea válido (ejemplo: 20%)
PROB_ERROR = 20  

print("Pico receptor punto 2 (ACK/NACK con ARQ). Esperando paquetes...")

while True:
    if uart.any() >= 1:
        len_b = uart.read(1)
        if not len_b:
            continue
        len_v = len_b[0]
        total_expected = 1 + len_v + 1  # seq + data + chk
        t0 = utime.ticks_ms()

        while uart.any() < total_expected and utime.ticks_diff(utime.ticks_ms(), t0) < 500:
            utime.sleep_ms(5)

        if uart.any() < total_expected:
            print("ERROR: trama incompleta (timeout).")
            uart.write(b"NACK\n")
            _ = uart.read()  # limpiar
            continue

        rest = uart.read(total_expected)
        seq_v = rest[0]
        data = rest[1:1+len_v]
        chk_recv = rest[-1]
        chk_calc = calcular_checksum(len_v, seq_v, data)

        if chk_calc == chk_recv:
            # paquete válido → pero simulamos NACK con cierta probabilidad
            if urandom.getrandbits(8) % 100 < PROB_ERROR:
                print("❌ Simulando NACK (error artificial) seq:", seq_v)
                uart.write(b"NACK\n")
            else:
                try:
                    sdata = bytes(data).decode('utf-8')
                except:
                    sdata = str(list(data))
                print("OK - datos:", sdata, "seq:", seq_v, "chk:", chk_recv)
                uart.write(b"ACK\n")
        else:
            print("ERROR CHECKSUM - recibido:", chk_recv, "calc:", chk_calc)
            uart.write(b"NACK\n")
    else:
        utime.sleep_ms(10)
