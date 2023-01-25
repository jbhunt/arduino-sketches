// Constants
byte handshake = 'h';
byte pulse     = 'p';
byte input     = 'z';
byte reset     = 'r';
int  outputPin =  8;
bool handshakeMade = false;
unsigned long pulseOnTimeMs = 5;

// Variables


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
    input = SerialUSB.read();
  }
  
  // Wait for handshake
  if (handshakeMade == false) {
    if (input == handshake) {
      SerialUSB.write(handshake);
      handshakeMade = true;
      return;
    }
  }

  // Wait for command to signal pulse
  else {
    if (input == pulse) {
      digitalWrite(outputPin, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(pulseOnTimeMs);      
      digitalWrite(outputPin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (input == reset) {
      handshakeMade = false;
    }
  }
  
}
