# BlindDistanceDetector 
Detector de distancias para ciegos

#Componentes sugeridos:
*ESP32-C3 Super Mini
*Sensor ultrasónico HC-SR04
*Microservo SG90
*Regulador Step-Down 5 V (Mini560, MP1584 o similar)
*2 baterías 18650
*Portabaterías 2S para 18650
*Módulo cargador/protector para baterías 18650 en configuración 2S
*2 resistencias para divisor de tensión en la señal ECHO del HC-SR04 (adaptación a 3,3 V del ESP32)
*1 pulsador
*Carcasa impresa en 3D
*Tapita giratoria solidaria al eje del servo con indicador táctil

#Funcionamiento

Al presionar el botón se mide la distancia 5 veces y a la menor distancia la transforma de la siguiente forma:
* Si es menor a 50cm marca de 180 para 0 y 90 para 50cm
* Si es mayor a 50cm marca de 90 para 50cm y 0 para 200cm o más.


Tinkercad: https://www.tinkercad.com/things/elehajj8ofC-detector-de-distancias-para-ciegos

