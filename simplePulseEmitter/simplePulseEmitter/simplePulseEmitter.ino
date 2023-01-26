// Constants
byte connectCommand = 'c';
byte signalCommand = 's';
byte releaseCommand = 'r';
byte message;
int  outputPin =  12;
bool handshakeMade = false;

// Variables
unsigned long pulseOnTimeMs = 500;

//
void setup() {
  // put your setup code here, to run once:
  pinMode(outputPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  SerialUSB.begin(9600);
}

void loop() {

  //
  if (SerialUSB.available() == false) {
    return;
  }
  else {
    message = SerialUSB.read();
  }
  
  // Wait for handshake
  if (handshakeMade == false) {
    if (message == connectCommand) {
      SerialUSB.write(connectCommand);
      handshakeMade = true;
      return;
    }
  }

  // Wait for command to signal pulse
  else {
    if (message == signalCommand) {
      // pulseOnTimeMs = SerialUSB.readString().toInt();
      digitalWrite(outputPin, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(pulseOnTimeMs);      
      digitalWrite(outputPin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (message == releaseCommand) {
      handshakeMade = false;
    }
  }
  
}
