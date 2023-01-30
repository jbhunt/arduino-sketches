byte          sensorState         = 0;
byte          lastSensorState     = 0;
uint16_t      lightThresh         = 170; // lights on = 10; lights off = 10
uint16_t      darkThresh          = 30;  // lights on = 30; lights off = 150
const byte    sensorLine          = 10;  // 10
const byte    signalLine          = A0;  // 12
unsigned long currentTime         = 0;
unsigned long lastTransitionTime  = 0;
unsigned long samplingDuration    = 4 * 1000; // Interval for checking mean (in us)
unsigned long samplingStopTime    = 0;
unsigned long sensorValue         = 0;
unsigned long ipiSum              = 0;
unsigned long ipiCount            = 0;
float         ipiAverage          = 0.0;
unsigned long lastCheckTime       = 0;
bool          printInterval       = true;
int           ipiAverageMin       = 0;
int           ipiAverageMax       = 100;
int           ipiAverageRounded   = 0;
int           ipiAverageRescaled  = 0;

void setup() {
  pinMode(sensorLine, INPUT);
  // pinMode(signalLine, OUTPUT);
  currentTime = micros();
  SerialUSB.begin(9600);
  analogWriteResolution(10);
}

void loop() {

  // Update dynamic variables
  currentTime = micros();

  // Sampling period has elapsed
  // Check the value of mean IPI
  if (currentTime - samplingStopTime > samplingDuration) {
    
    // Compute average pulse interval
    ipiAverage = (float) ipiSum / (float) ipiCount;
    ipiAverageRescaled = rescale(ipiAverage, ipiAverageMin, ipiAverageMax, 0, 1024);
    
    if (printInterval) {
      SerialUSB.println(ipiAverage);
    }

    // TODO: Write the scaled value to a PWM pin
    analogWrite(signalLine, ipiAverageRescaled);

    //
    if (ipiAverage < lightThresh && digitalRead(signalLine) == LOW) {
      // digitalWrite(signalLine, HIGH);
    }

    //
    if (ipiAverage > darkThresh && digitalRead(signalLine) == HIGH) {
      // digitalWrite(signalLine, LOW);
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

//
int rescale(float m, int rMin, int rMax, int tMin, int tMax) {
  float rescaled;
  float numerator;
  float denominator;
  float factor;
  if (m > rMax) {
    m = rMax;
  }
  if (m < rMin) {
    m = rMin;
  }
  numerator = m - (float) rMin;
  denominator = (float) rMax - (float) rMin;
  factor = (float) tMax - (float) tMin;
  rescaled = numerator / denominator * factor + (float) tMin;
  rescaled = round(rescaled);
  return rescaled;
}
