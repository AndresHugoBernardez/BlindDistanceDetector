// C++ code
//
#include <Servo.h>

int distance = 0;

int currentDistance = 0;

int oldDistance = 0;

int autoOff=15;

const int button=2;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

int counter;

Servo servo_9;

void setup()
{
  servo_9.attach(9, 500, 2500);
  pinMode(button,INPUT_PULLUP);

  distance = 50;
  currentDistance = 600;
  oldDistance = 605;
}

void loop()
{
  
 
  
  if(!digitalRead(button)){
    currentDistance = 600;
    
    for (counter = 0; counter < 5; ++counter) {
      distance = 0.01723 * readUltrasonicDistance(4, 3);
      if (distance < currentDistance) {
        currentDistance = distance;
        
      }
      delay(15); // Wait for 15 millisecond(s)
    }

    currentDistance = constrain(currentDistance,0,200);
  

    if (currentDistance < oldDistance - 5 || currentDistance > oldDistance + 5) {
      if (currentDistance < 50) {

        if(!servo_9.attached())servo_9.attach(9, 500, 2500);
        // de 0 a 50 cm corresponde de 180 a 90 grados
        servo_9.write(map(currentDistance,0,50,180,90));
        delay(150); // Wait for 150 millisecond(s)
      } else {
        // de 50 a 200 cm corresponde de 90 a 0 grados
        servo_9.write(map(currentDistance,50,200,90,0));
        delay(150); // Wait for 150 millisecond(s)
      }
      autoOff=15;
      oldDistance = currentDistance;
    }
    else{
      if(autoOff>0){
        autoOff--;
      }
      else{
        if(servo_9.attached())servo_9.detach();
      }

    }
    
    
  }
  else{
    if(servo_9.attached())servo_9.detach();
    delay(50);
  }
  
  delay(20); // Wait for 20 millisecond(s)
}