//BlindDistanceDetector2
//
//ESP32 C3 supermini black port USB C 
//
//C++ code
//
#include <ESP32Servo.h>
#include "Adafruit_VL53L0X.h" 

// HC-SR 04
#define triggerPin 21
#define echoPin 3

// active buzzer
#define soundPin 4

// leds
#define lightPin 5

// buttons
#define buttonDownPin 6
#define buttonUpPin  7

//servo
#define servoPin 10

//I2C com  (VL53L0X)
#define SDApin 8
#define SCLpin 9

//
#define offSetDistanceActivation 70
#define offSetDistanceDeactivation 30

//Constants

const unsigned int minimumTime = 15;//ms
const unsigned int maxSoundLightONWaitingTime = 1700;//ms
const unsigned int lightONTimeFactor = 10;
const unsigned int ONTime = 150; //ms






unsigned long distanceHCSR04 = 500;//mm
unsigned long distanceVL53L0X = 500;//mm

long currentDistance = 500;//mm
unsigned long midDistance = 0;//mm

long oldDistance = 500;//mm

int autoOff=15;//ms
unsigned long autoOffTime=0;//ms

unsigned long currentTime=0;//ms
unsigned long medTime=0;//ms



long positionObjective=1500;
long  currentPosition=1500;
unsigned long positionRefreshTime=0;
unsigned long currentMicroTime=0;

int counter=0;
int N=0;
int M=0;

int activation=0;


// Tactil wheel output
Servo servoWheel;

// VL53L0X
Adafruit_VL53L0X VL53sensor = Adafruit_VL53L0X();

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

  //inicialization
  digitalWrite(triggerPin, LOW);
  digitalWrite(lightPin,LOW);
  digitalWrite(soundPin,LOW);


  distanceHCSR04 = 500; //mm
  distanceVL53L0X = 500; //mm
  currentDistance = 1000; //mm
  oldDistance = 500; //mm

  currentTime=millis();
  medTime=currentTime+30;
  soundLightOnTime+=currentTime+minimumTime;


  Serial.begin(115200);


   delay(150);

   if (!VL53sensor.begin()) {
    Serial.println("Error: VL53L0X no encontrado");
}
    
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
        soundLightOnTime=minimumTime+oldDistance*lightONTimeFactor;
        if(soundLightOnTime>maxSoundLightONWaitingTime) soundLightOnTime=maxSoundLightONWaitingTime;
        soundLightOnTime+=currentTime;
      digitalWrite(lightPin,LOW);
      digitalWrite(soundPin,LOW);
    }
   else   if(soundLightState==0 && currentTime>soundLightOnTime){
      soundLightState=1;
        soundLightOffTime=currentTime+ONTime;
      digitalWrite(lightPin,HIGH);
      digitalWrite(soundPin,HIGH);
    }

}


void gotoObjective(){

  long difference = positionObjective - currentPosition;
  unsigned long timePosition = 100; // microseconds
  
  if (difference >= 0) {
    if(difference<2001)timePosition = map(difference, 0, 2000, 1000, 50);
    else timePosition = 50;
  }
  else {
    if(difference > (-2001) )timePosition = map(-difference, 0, 2000, 1000, 50);
    else timePosition = 50;

  }

  currentMicroTime = micros();
  
  //  (overflow test)
  if (currentMicroTime - positionRefreshTime >= timePosition) {
    positionRefreshTime = currentMicroTime; 

    // moving by steps 
    if (currentPosition > 9 && currentPosition < 2491) {
      currentPosition += difference / 10;
      currentPosition += constrain(difference, -10, 10);
      currentPosition = constrain(currentPosition,500,2500);
    }

    if (!(servoWheel.attached())) servoWheel.attach(servoPin, 500, 2500);
    
    if (currentPosition > 499 && currentPosition < 2501) {
      servoWheel.writeMicroseconds(currentPosition);
    }
  }
}


VL53L0X_RangingMeasurementData_t measure;


void loop()
{
  
 
  //it will only woprks with a button pressed
  if(!digitalRead(buttonDownPin) || !digitalRead(buttonUpPin)){
   
        


    //average distance 
    
    currentTime= millis();


    if(currentTime>medTime){

      medTime=currentTime+30;
      
      if (counter < 8 ) {
        counter++;
        
        distanceHCSR04 = constrain( (unsigned long )(0.1723 * readUltrasonicDistance(triggerPin,echoPin)),0UL,5000UL) ;//mm
        if(distanceHCSR04>3999) distanceHCSR04=0;

        VL53sensor.rangingTest(&measure, false); //take a measure from VL53L0X
        distanceVL53L0X =constrain( ((unsigned long) measure.RangeMilliMeter),0UL,2100UL) ;
        if(distanceVL53L0X>1999)distanceVL53L0X=0;


        
        if(distanceVL53L0X>0 && distanceHCSR04>0 && distanceHCSR04<distanceVL53L0X){
          if(N<5 ) {
            midDistance += distanceHCSR04;

            N++;          
            }

        }
        else if(M<5 && distanceVL53L0X>0) {
            midDistance += distanceVL53L0X;

            M++;          
            }
        else if(N<5 && distanceHCSR04>0) {
            midDistance += distanceHCSR04;

            N++;          
            }

        
        
        


        
      }
      else{


       
        if((N+M)>0)midDistance=midDistance/(N+M);
        else midDistance=3000;
        currentDistance = constrain((long)midDistance,0,3000);
        midDistance=0;
        counter=0;
        N=0;
        M=0;
        triggerSoundLight();

      }
    
    }
    //end of average distance

  




  if (!activation && (currentDistance < oldDistance-offSetDistanceActivation || currentDistance > oldDistance+offSetDistanceActivation)){
    activation=1;
    oldDistance=currentDistance;
  }
  
  else if (activation && currentDistance > oldDistance-offSetDistanceDeactivation && currentDistance < oldDistance+offSetDistanceDeactivation){
    activation=0;

  }
    

    if (activation){

      
      
            
      if (currentDistance <= 500) {

      
        // from 0 a 500 mm servo goes from 180 to 90 degrees
        positionObjective=(map(currentDistance,0,500,2500,1501));
        
      } else {
        // from 501 a 3000 mm servo goes from 90 to 0 degrees
        positionObjective=(map(currentDistance,501,3000,1500,500));
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
      autoOff=2000;
      soundLightState=0;
      soundLightOnTime=currentTime+1;
      digitalWrite(lightPin,LOW);
      digitalWrite(soundPin,LOW);
    }
    delay(10); 
  } 
}