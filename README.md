# BlindDistanceDetector 
Detector de distancias para ciegos

#Componentes sugeridos:
* Esp32 c3 super mini
* Servo sg90
* HC-SR04
* regulador step-down a 5V ej: mini560 o mp1584, etc
* 2 baterías 18650
* portabaterías 2s 18650
* cargador-regulador de baterías 2s para 18650
* 2 resistencias para divisor de tensión para pin echo del HC-SR04
* 1 pulsador 

#Funcionamiento

Al presionar el botón se mide la distancia 5 veces y a la menor distancia la transforma de la siguiente forma:
* Si es menor a 50cm marca de 180 para 0 y 90 para 50cm
* Si es mayor a 50cm marca de 90 para 50cm y 0 para 200cm o más.


Tinkercad: https://www.tinkercad.com/things/elehajj8ofC-detector-de-distancias-para-ciegos

