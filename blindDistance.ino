// C++ code
//
#include <ESP32Servo.h>

#define triggerPin 21
#define echoPin 3
#define soundPin 4
#define lightPin 5
#define buttonDownPin 6
#define buttonUpPin  7
#define servoPin 10

unsigned long distance = 200;

unsigned long currentDistance = 200;

unsigned long oldDistance = 200;

int autoOff=15;

unsigned long currentTime=0;


long readUltrasonicDistance(int trigger, int echo)
{
  
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echo, HIGH,30000);
}
int counter;

Servo servo_9;

int soundLightState=0;
unsigned long soundLightTime=0;


int triggerSoundLight(){


   if(currentTime>soundLightTime){
        soundLightTime=currentTime+15+oldDistance*8;
     

    if(soundLightState==0){
      soundLightState=1;
      digitalWrite(lightPin,HIGH);
      digitalWrite(soundPin,HIGH);
      delay(150);
      digitalWrite(lightPin,LOW);
      digitalWrite(soundPin,LOW);

     }
    else{
     soundLightState=0;
            delay(150);
     
    }
    return(0);
}
    return(1);

}


void setup()
{
  servo_9.attach(servoPin, 500, 2500);
  pinMode(buttonDownPin,INPUT_PULLUP);
  pinMode(buttonUpPin,INPUT_PULLUP);
  pinMode(soundPin,OUTPUT);
  pinMode(lightPin,OUTPUT);
    pinMode(triggerPin, OUTPUT);  // Clear the trigger
    
    pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);

  digitalWrite(lightPin,LOW);
  digitalWrite(soundPin,LOW);


  distance = 50;
  currentDistance = 100;
  oldDistance = 50;
  //Serial.begin(115200);
}

void loop()
{
  
 
  
  if(!digitalRead(buttonDownPin) || !digitalRead(buttonUpPin)){
    currentTime= millis();
        
        currentDistance=0;
    
    for (counter = 0; counter < 5; ++counter) {
      distance = (unsigned long )(0.01723 * readUltrasonicDistance(triggerPin,echoPin)) ;
      /*Serial.print("distance:");
      Serial.println(distance);*/
      
      currentDistance += distance;
        
      
      delay(5); // Wait for 15 millisecond(s)
    }
        currentDistance=currentDistance/5;
    triggerSoundLight();

    currentDistance = constrain(currentDistance,0,200);
  

    if (currentDistance < oldDistance - 5 || currentDistance > oldDistance + 5){
       oldDistance = currentDistance;
            
            
      if (currentDistance < 50) {

        if(!servo_9.attached())servo_9.attach(servoPin, 500, 2500);
        // de 0 a 50 cm corresponde de 180 a 90 grados
        servo_9.write(map(currentDistance,0,50,180,90));
        if(triggerSoundLight())delay(10); // Wait for 150 millisecond(s)
        
      } else {
        // de 50 a 200 cm corresponde de 90 a 0 grados
        servo_9.write(map(currentDistance,50,200,90,0));
        if(triggerSoundLight())delay(10);
      }
      autoOff=15;
      
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
    
  }

  
  
  delay(10); // Wait for 10 millisecond(s)
}