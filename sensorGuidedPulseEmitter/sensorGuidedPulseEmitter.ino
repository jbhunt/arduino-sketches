byte          sensorState         = 0;
byte          lastSensorState     = 0;
uint16_t      lightThresh         = 170; // lights on = 10; lights off = 10
uint16_t      darkThresh          = 30;  // lights on = 30; lights off = 150
const byte    sensorLine          = 10;  // 10
const byte    signalLine          = 12;  // 12
unsigned long currentTime         = 0;
unsigned long lastTransitionTime  = 0;
unsigned long samplingDuration    = 5 * 1000; // Interval for checking mean (in us)
unsigned long samplingStopTime    = 0;
unsigned long sensorValue         = 0;
unsigned long ipiSum              = 0;
unsigned long ipiCount            = 0;
float         ipiAverage          = 0.0;
unsigned long lastCheckTime       = 0;
bool          printInterval       = true;
int           ipiAverageMin       = 0;
int           ipiAverageMax       = 200;
int           ipiAverageScaled    = 0;

void setup() {
  pinMode(sensorLine, INPUT);
  pinMode(signalLine, OUTPUT);
  currentTime = micros();
  SerialUSB.begin(9600);
}

void loop() {

  // Update dynamic variables
  currentTime = micros();

  // Sampling period has elapsed
  // Check the value of mean IPI
  if (currentTime - samplingStopTime > samplingDuration) {
    
    // Compute average pulse interval
    ipiAverage = (float) ipiSum / (float) ipiCount;
    ipiAverageScaled = map(round(ipiAverage), ipiAverageMin, ipiAverageMax, 0, 255);
    
    if (printInterval) {
      SerialUSB.println(ipiAverageScaled);
    }

    // TODO: Write the scaled value to a PWM pin

    //
    if (ipiAverage < lightThresh && digitalRead(signalLine) == LOW) {
      digitalWrite(signalLine, HIGH);
    }

    //
    if (ipiAverage > darkThresh && digitalRead(signalLine) == HIGH) {
      digitalWrite(signalLine, LOW);
    }

    // Reset the end time point
    samplingStopTime = micros();
    ipiSum = 0;
    ipiCount = 0;
  }

  // Keep sampling
  else {

    //
    lastSensorState = sensorState;
    sensorState = digitalRead(sensorLine);

    if (sensorState != lastSensorState) {

      // Get the new sensor value
      if (currentTime < lastTransitionTime) { // There was a 32-bit micros() clock roll-over
        sensorValue = currentTime + (4294967296 - lastTransitionTime);
      }
      else {
        sensorValue = currentTime - lastTransitionTime;
      }
  
      //
      lastTransitionTime = currentTime;
      ipiSum += sensorValue;
      ipiCount += 1;
    }
  }
}
