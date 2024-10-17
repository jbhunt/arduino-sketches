
/*
Author: Josh Hunt
Email: hunt.brian.joshua@gmail.com
Description: This program detects and signals brief flashes of light on a display using a photologic device
*/

// LIGHT-TO-FREQUENY SENSOR

class Stimulus{

  private:
  
  // int sensorLine = 23; // data line of the sensor connected to pin #10
  int sensorLine = 10;
  int ttlOutput = 12;
  int sensorState = 0; // stores the state of the sensor as HIGH or LOW
  int lastSensorState = 0; // stores the sensor state of the previous loop as HIGH or LOW
  unsigned long sensorValue = 0; // keeps track of the time inbetween TTL pulses coming from the sensor
  unsigned long lightThresh = 5; // time inbetween pulses in us used to threshold a transition to light
  unsigned long darkThresh = 20; // time inbetween pulses in us used to threshold a trasition to dark
  unsigned long pulseDuration = 50000; // pulse duration in us
  unsigned long refracDuration = 1000000 * 0.4; // three tenths of a second (approx. 300 ms)
  unsigned long pulseEndTime = 0;
  unsigned long lastTransitionTime = 0;
  unsigned long refracEndTime = 0;
  boolean inPulse = false;
  boolean inRefrac = false;
  boolean overLightThresh = false;
  boolean overDarkThresh = false;
  
  public:

  // constructor
  Stimulus(){
    init();
  }

  void init(){
    pinMode(sensorLine,INPUT);
    pinMode(ttlOutput,OUTPUT);
    lastTransitionTime = micros();  
  }
  
  // check light sensor input each cycle
  
  void update(unsigned long currentTime){
    lastSensorState = sensorState;
    sensorState = digitalRead(sensorLine);
    
    if (sensorState != lastSensorState) {
      if (currentTime < lastTransitionTime) { // There was a 32-bit micros() clock roll-over
        sensorValue = currentTime + (4294967296-lastTransitionTime);
      }
      else {
        sensorValue = currentTime - lastTransitionTime;
      } //end rollover check

      lastTransitionTime = currentTime;
  
      // Set Current Sensor State
      if (sensorValue < lightThresh) {
        overLightThresh = true;
        overDarkThresh = false;
      } 
      else if (sensorValue > darkThresh) {
        overLightThresh = false;
        overDarkThresh = true;
      } 
      else {
        overLightThresh = false;
        overLightThresh = false;
      } // end setting sensor state
    } //end sensor change check
  
    if ((overLightThresh)&&(!inRefrac)){
      if (!inPulse){
        inPulse = true;
        pulseEndTime = currentTime + pulseDuration;
        refracEndTime = pulseEndTime + refracDuration;
        digitalWrite(LED_BUILTIN,HIGH);
        digitalWrite(ttlOutput, HIGH);
      }
    }
  
    if ((inPulse)&&(currentTime>=pulseEndTime)){
      inPulse = false;
      inRefrac = true;
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(ttlOutput,LOW);
    }
  
    if (inRefrac){
      if (currentTime>=refracEndTime){
        inRefrac = false;
      }
    }
  }
};

// MAIN LOOP

Stimulus stim;
unsigned long currentTime = 0;
boolean started = false;
int ledPin = 13;

void setup() {
  // SerialUSB.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(ledPin,OUTPUT);
}

void loop(){
  currentTime = micros();
  stim.update(currentTime);
}