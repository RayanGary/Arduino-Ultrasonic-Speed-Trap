#include <Servo.h>
//stopwatch library: https://github.com/thomasfredericks/Chrono 
#include <Chrono.h>
#include <LightChrono.h>
#include "U8glib.h"

Chrono stopWatch;

//sensor pins
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;

const int SPEEDLIMIT = 0.5;

//sensor states for checks
bool sensor1 = false;
bool sensor2 = false;

float timePassed;
double carSpeed; 

//servos
Servo servo1;
Servo servo2;

#define DISP_CS 9
#define DISP_A0 7
#define DISP_RST 6
// 11 SDIN
// 13 SCLK 
  U8GLIB_SH1106_128X64 u8g(DISP_CS, DISP_A0, DISP_RST);

void setup() {
  
  //Sensor 1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  //Sensor 2
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  //Starts serial communication 
  Serial.begin(9600);

  //Servo
  servo1.attach(8);
  servo2.attach(10);

  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on.
}

void loop() {
  /*main program loop
   */
   
  //TODO: ACTIVATE TRAP FUNC.
  //IF SPEED > LIMIT => ACTIVATE TRAP
  getTimePassed();
  
}

float  getTimePassed(){
  //TODO: PICK BETTER NAME / WRITE BETTER DOCS.
  //getSpeed()?
  //TODO: PRINT SPEED ON DISPLAY + CLEAR AFTER FEW SCONDS? 
  /* getTimePassed checks to see if anything has passed infront of our sensors. 
   *  if so it calculates the time passed to cross them both. 
   *  it returns time passed in seconds.
   * 
   * for rapid testing purposes, it also prints out 
   * time passed and speed to the serial console 
   */
   
  timePassed = 0; //reset value of timePassed 
  
  //Check if something has passed by the first sensor.
  //assuming wall distance is higher than 20, means anything less than 15 should be a car.
  //if true, check if sensor 2 was previously activated,
  //meaning that the car is moving backwards. if so, ignore. 
  
  if (calDistance(echoPin1,trigPin1)<15 && sensor1 == false && sensor2 == false){
      //Sensor 1 is activated, restart the stopWatch
      //note: the stopWatch has been running on boot,
      //can't start whenever needed unfortunately
      stopWatch.restart();
      
      Serial.println("SENSOR 1 ACTIVATED"); //indicator for serial
      
      sensor1 = true; //Set first sensor to be true, to avoid constant loops. 
      
  } else {}


  //Check if something has passed by the 2nd sensor.
  //assuming wall distance is higher than 20, means anything less than 15 should be a car.
  //if true, check if sensor 1 was previously activated,
  //meaning that the car is moving backwards. if so, ignore.
    
  if (calDistance(echoPin2,trigPin2)<15 && sensor1 == true){
      
      Serial.println("SENSOR 2 ACTIVATED"); //indicator for serial 

      //Reset status of both sensors.
      sensor2 = false;  
      sensor1 = false; 
      
      timePassed = (stopWatch.elapsed()+500.0)/1000.0; //How much time took the car to pass both sensors.
      carSpeed = 0.5/timePassed; //get car speed assuming distance between sensors is 0.5m
      
      //indicator for serial 
      Serial.print("TIME PASSED:");
      Serial.print(timePassed);
      Serial.println();
      Serial.print("SPEED:");
      Serial.print(carSpeed);
      Serial.println();

      String carSpeedString = String(carSpeed);

  u8g.firstPage();
  do {  
  u8g.setFont(u8g_font_courR14);
  u8g.drawStr(6, 20, "Your speed: ");
  u8g.setFont(u8g_font_courR24);
  u8g.drawStr(24, 60, carSpeedString.c_str());
  } while( u8g.nextPage());
  delay(1000);
  

      /*
       * IF SPEED > LIMIT ACTIVATE TRAPS
       */
      
      return(((stopWatch.elapsed()+500.0)/1000.0)); //return time passed.
  } else {}
  
  }


float calDistance(int echoPin, int trigPin){
  /*calDistance function allows us to calculate the distance read by
   * an ultrasonic sensor. using it's echoPin and trigPin as parameters.
   * this allows us to reuse this function later to check if something
   * passed infront of the sensor.
   */
  delay(100);

  long duration, distance;

  //Trigger a pulse reading using the trigger pin
  //for echo to read later. 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);

  //duration (microseconds) is the time it took the echoPin 
  //to read a high pulse bouncing off an object. 
  duration = pulseIn(echoPin,HIGH);
  
  //get the cm distance via the duration.
  //the signal traveled back and forth, so we take half the duration
  //and divide that by the speed of sound, 29 microseconds/cm
  //giving us the distance in cm 
  distance = (duration/2)/29; 
  return distance ;
  }

void draw(){
}

void activateTrap(){
  servo1.write(180);
  servo2.write(180);
  Serial.println("TRAP ACTIVATED");
  delay(3000);
  resetTrap();
}

void resetTrap(){
  servo1.write(0);
  servo2.write(0);
  Serial.println("TRAP RESET");
}
