/*
  crossExerciseBench
  
  Created by James Connor
  This reads in two analog values from potentiometers to determine 
  the position of the exercise bench arms. These positions are used
  to increase and the volume of the speakers attached to two digital 
  pots (stereo). 
  The volume starts at level 1, quiet with the bench at rest (arms down)
  with no user input it remains at this volume.
  Should the user move the bench arms up past the threshold of one 
  motion the volume will increase to 2.
  If the user repeats the motion after going to rest position within 
  time X the volume will increase 3...4... and a maximum of 5.
  If the user does not repeat a full motion within time X the volume 
  will decrease. Should time X expire again and no motion has been 
  performed the volume will decrease again to a minimum of level 1.
 
 4131 library
 https://github.com/declanshanaghy/ArduinoLibraries/tree/master/MCP4161
 https://github.com/johnnyonthespot/MapleLibraries/tree/master/MCP4131
 
 This example code is in the public domain.
 */

#include <SPI.h>
#include <MCP4131.h>


#define DEBUG //uncomment for debugging
//#define DEBUGSENSOR //uncomment for debugging

#define SENSOR0PIN A0
#define SENSOR1PIN A1
#define POT0PIN 10
#define POT1PIN 9

// values used for the minimum and maximum range of the sensor to 
// determine a motion on the bench
#define SENSOR0MIN 200
#define SENSOR0MAX 800
#define SENSOR1MIN 200
#define SENSOR1MAX 800

#define deltaReduce 5000 // 5 seconds

// time
unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long deltaTime = 0;
unsigned long deltaTimeSensor = 0;

// volume
int currentVolume = 0;
#define STEPCOUNT 5
//127 is minimum, 1 is max
int volume[5] = {127, 115, 80, 30, 1}; //initial working values

//input and outputs
int sensor0Val, sensor1Val;
MCP4131 pot0(POT0PIN); 
MCP4131 pot1(POT1PIN); 

// arm position
boolean hasRested = true;

/**
 * Measure critical sensors
 */
void measureSensors() {
  // read the inputs:
  // read once to set the ADC correctly, wait for the capacitor to charge the read again
  sensor0Val = analogRead(SENSOR0PIN);
  delay(10);
  sensor0Val = analogRead(SENSOR0PIN);
  
  sensor1Val = analogRead(SENSOR1PIN);
  delay(10);
  sensor1Val = analogRead(SENSOR1PIN);
  
  #ifdef DEBUGSENSOR
  Serial.print("Sensor0: ");
  Serial.print(sensor0Val);
  Serial.print("  Sensor1: ");
  Serial.println(sensor1Val);
  #endif
}

/**
 * Setup routine
 */
void setup() {
  // set pin directions
  pinMode(SENSOR0PIN, INPUT);
  pinMode(SENSOR1PIN, INPUT);
  
  //set pots to initial volume level
  pot0.setTap(volume[currentVolume]);
  pot1.setTap(volume[currentVolume]);
  
  //setup DEBUG
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

/**
 * Loop
 */
void loop() {

  currentTime = millis();
  deltaTime = currentTime - previousTime;
  
  // measure input sensors
  measureSensors();
  
  // check sensors for bot at max to increase volume
  if (sensor0Val > SENSOR0MAX && sensor1Val > SENSOR1MAX) {
    //make sure they have gone back down first
	if (hasRested) {
	  // both arms have done a full motion so we can increase volume
	  currentVolume++;
	
	  // ensure it doesn't go above maximum
	  if (currentVolume >= STEPCOUNT) {
	    currentVolume = 4;
	  }
	  
	  //send to pots
	  pot0.setTap(volume[currentVolume]);
	  pot1.setTap(volume[currentVolume]);
	  
	  
      #ifdef DEBUG
      Serial.print("Pot Volume Increase: ");
      Serial.println(volume[currentVolume]);
      #endif
	  
	  // reset the previous time the volume changed
	  previousTime = currentTime;
	  
	  // we have now no longer rested so
	  hasRested = false;
	}
  }
  
  // check sensors for rested state
  if (sensor0Val < SENSOR0MIN && sensor1Val < SENSOR1MIN) {
    hasRested = true;
  }
  
  // check time to reduce volume
  if (deltaTime > deltaReduce) {
	// reduce volume level
    currentVolume--;
	
	// ensure it doesn't go below 0
	if (currentVolume < 0) {
	  currentVolume = 0;
	}
	
	//send to pots
	pot0.setTap(volume[currentVolume]);
	pot1.setTap(volume[currentVolume]);
	
    #ifdef DEBUG
    Serial.print("Pot Volume Decrease: ");
    Serial.println(volume[currentVolume]);
    #endif
	
	// reset the previous time the volume changed
	previousTime = currentTime;
  }
  
  // delay for stability
  delay(10);
}
