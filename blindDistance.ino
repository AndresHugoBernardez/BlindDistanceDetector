//Placa ESP32 C3 supermini negro puerto USB C 
//
//C++ code
//
#include <ESP32Servo.h>

#define triggerPin 21
#define echoPin 3
#define soundPin 4
#define lightPin 5
#define buttonDownPin 6
#define buttonUpPin  7
#define servoPin 10

#define offSetAngle 3
#define offSetPosition 15

unsigned long distance = 50;

long currentDistance = 50;
long medDistance = 0;

long oldDistance = 50;

int autoOff=15;
unsigned long autoOffTime=0;

unsigned long currentTime=0;
unsigned long medTime=0;



long positionObjective=1500;
long  currentPosition=1500;
unsigned long positionRefreshTime=0;
unsigned long currentMicroTime=0;

int counter=0;
int N=0;

Servo servoWheel;

int soundLightState=0;
unsigned long soundLightOffTime=0;
unsigned long soundLightOnTime=0;





void setup()
{
  servoWheel.attach(servoPin, 500, 2500);
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

  currentTime=millis();
  medTime=currentTime+30;


  Serial.begin(115200);

   delay(150);
    
  Serial.println("Ready!"); // This won't print until monitor is open

 
}




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





void triggerSoundLight(){

   if(soundLightState==1 && currentTime>soundLightOffTime){
      soundLightState=0;
        soundLightOnTime=15+oldDistance*10;
        if(soundLightOnTime>1700) soundLightOnTime=1700;
        soundLightOnTime+=currentTime;
      digitalWrite(lightPin,LOW);
      digitalWrite(soundPin,LOW);
    }
   else   if(soundLightState==0 && currentTime>soundLightOnTime){
      soundLightState=1;
        soundLightOffTime=currentTime+150;
      digitalWrite(lightPin,HIGH);
      digitalWrite(soundPin,HIGH);
    }

}


void gotoObjective(){

  long difference = positionObjective - currentPosition;
  unsigned long timePosition = 100; // Mejor usar unsigned long para microsegundos
  
  if (difference >= 0) timePosition = map(difference, 0, 2000, 1000, 50);
  else timePosition = map(-difference, 0, 2000, 1000, 50);

  currentMicroTime = micros();
  
  // Condición de tiempo a prueba de desbordamientos (overflow)
  if (currentMicroTime - positionRefreshTime >= timePosition) {
    positionRefreshTime = currentMicroTime; 

    // Las matemáticas AHORA están dentro del temporizador
    if (currentPosition > 9 && currentPosition < 2491) {
      currentPosition += difference / 10;
      currentPosition += constrain(difference, -10, 10);
    }

    if (!(servoWheel.attached())) servoWheel.attach(servoPin, 500, 2500);
    
    if (currentPosition > 499 && currentPosition < 2501) {
      servoWheel.writeMicroseconds(currentPosition);
    }
  }
}


void loop()
{
  
 
  
  if(!digitalRead(buttonDownPin) || !digitalRead(buttonUpPin)){
   
        


    //Promedio de distancias
    
    currentTime= millis();


    if(currentTime>medTime){

      medTime=currentTime+30;
      
      if (counter < 8 ) {
        counter++;
        
        distance = (unsigned long )(0.01723 * readUltrasonicDistance(triggerPin,echoPin)) ;
      
        if(N<5){
        if(distance>0) {
          medDistance += distance;
          N++;          
          }
      
        }

        
      }
      else{


       
        if(N>0)medDistance=medDistance/N;
        else medDistance=300;
        currentDistance = constrain(medDistance,0,300);
        medDistance=0;
        counter=0;
        N=0;
        triggerSoundLight();

      }
    
    }
    //fin promedio de distancias

  




  
    

    if (currentDistance < oldDistance-offSetAngle || currentDistance > oldDistance+offSetAngle){
       oldDistance = currentDistance;
            
            
      if (currentDistance <= 50) {

      
        // de 0 a 50 cm corresponde de 180 a 90 grados
        positionObjective=(map(currentDistance,0,50,2500,1501));
        
      } else {
        // de 51 a 300 cm corresponde de 90 a 0 grados
        positionObjective=(map(currentDistance,51,300,1500,500));
      }
      autoOff=2000;

     //Serial.print("distance:");
     //Serial.println(oldDistance);

     gotoObjective();
      
    }
    else{
      if(autoOff>0){
        if((currentTime-autoOffTime)>=1) {
          autoOff--;
          autoOffTime=currentTime;
        }

         gotoObjective();
      }
      else{
        if(servoWheel.attached())servoWheel.detach();
      }

    }
    
    


  }
  else{
    if(servoWheel.attached())servoWheel.detach();

    if(soundLightState==1){
      soundLightState=0;
      soundLightOnTime=currentTime+1;
      digitalWrite(lightPin,LOW);
      digitalWrite(soundPin,LOW);
    }
    delay(10); 
  } 
}
