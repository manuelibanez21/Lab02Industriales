from machine import UART, Pin
import time

# UART0 en Pico (TX=GP0, RX=GP1)
uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

def check_parity(byte, parity):
    p = 0
    for i in range(8):
        p ^= (byte >> i) & 1
    return p == parity

def calc_lrc(block):
    lrc = 0
    for b in block:
        lrc ^= b
    return lrc

print("Pico receptor listo, esperando datos...")

while True:
    if uart.any() >= 11:  # 5 datos + 5 paridades + 1 LRC
        datos = []
        paridades = []
        
        for i in range(5):
            d = uart.read(1)[0]
            p = uart.read(1)[0]
            datos.append(d)
            paridades.append(p)
            print(f"Dato: {hex(d)} | Paridad: {p} -> {'OK' if check_parity(d,p) else 'ERROR'}")
        
        lrc_rx = uart.read(1)[0]
        lrc_calc = calc_lrc(datos)
        print(f"LRC recibido: {hex(lrc_rx)} | LRC calculado: {hex(lrc_calc)} -> {'OK' if lrc_rx == lrc_calc else 'ERROR'}")
        print("-"*40)

    time.sleep(0.2)
