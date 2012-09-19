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


  #define SENSOR0PIN A0
  #define SENSOR1PIN A1

  unsigned long currentTime = 0;
  unsigned long previousTime = 0;
  unsigned long deltaTime = 0;
  int sensor0Val, sensor0Val;

  /**
   * Measure critical sensors
   */
  void measureSensors() {
    if (deltaTime >= 10) { //10ms
      // read the inputs:
      sensor0Val = analogRead(SENSOR0PIN);
      sensor1Val = analogRead(SENSOR1PIN);
    }
  }

// the setup routine runs once when you press reset:
void setup() {
  pinMode(SENSOR0PIN, INPUT);
  pinMode(SENSOR1PIN, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {

  currentTime = millis();
  deltaTime = currentTime - previousTime;
  
  // measure input sensors
  measureSensors();
  
  
  delay(1);        // delay in between reads for stability
  
  previousTime = currentTime;
}