\# Laboratorio 02 – Comunicaciones Industriales



Este repositorio contiene el informe, códigos y evidencias del \*\*Laboratorio 02\*\* de la asignatura de \*\*Comunicaciones Industriales\*\* (Septiembre de 2025), realizado por \*\*Manuel Ibañez, Nicol Vargas y Laura Cardenas\*\*.



\## 📋 Resumen del Informe

Se implementaron y compararon diferentes mecanismos de detección y corrección de errores en comunicación UART entre \*\*Arduino Uno\*\* (emisor) y \*\*Raspberry Pi Pico\*\* (receptor):



\- \*\*Checksum (suma módulo 256)\*\* para detección básica de errores.  

\- \*\*Protocolo ARQ (ACK/NACK + retransmisión)\*\* para asegurar entrega.  

\- \*\*Comparación VRC vs LRC\*\*, evaluando cobertura de errores y overhead.  



Se observaron los límites de cada técnica, su eficiencia y robustez, además del papel del MAX3232 en la adaptación entre estándares electrónicos.



\## ⚙️ Contenido del Repositorio

\- `Lab02Industriales.pdf` → Informe completo.  

\- `punto1lab02Arduino/punto1lab02Arduino.ino` → Código Arduino con Checksum.  

\- `punto1lab02.py` → Código en Python (Raspberry, receptor con Checksum).  

\- `punto2lab02arduino/punto2lab02arduino.ino` → Arduino emisor con ARQ.  

\- `punto2lab02raspberry.py` → Receptor en Python con ARQ.  

\- `punto3.py` → Simulación en Python (comparación VRC vs LRC).  

\- `punto3lab02Arduino/punto3lab02Arduino.ino` → Código Arduino adicional para pruebas.  



\## 🧪 Resultados Clave

\- El \*\*Checksum\*\* identificó errores simples, pero no patrones complejos.  

\- El \*\*ARQ\*\* mejoró confiabilidad, aunque generó mayor retransmisión en condiciones ruidosas.  

\- El \*\*LRC\*\* detectó más fallos que el \*\*VRC\*\*, al costo de mayor overhead.  

\- El \*\*MAX3232\*\* fue clave para asegurar compatibilidad física entre dispositivos.  



\## 📚 Conclusiones

El laboratorio dejó claro que garantizar la integridad de los datos en entornos industriales requiere protocolos confiables.  

Se sugiere seguir explorando técnicas como \*\*CRC\*\*, \*\*códigos de Hamming\*\* o protocolos industriales como \*\*Modbus\*\* o \*\*Profibus\*\* para aplicaciones más complejas en la Industria 4.0.



\## 👩‍💻 Autores

\- Manuel Ibañez  

\- Nicol Vargas  

\- Laura Cardenas



