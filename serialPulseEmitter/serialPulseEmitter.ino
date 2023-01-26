// Constants
byte handshake = 'c';
byte pulse     = 's';
byte reset     = 'r';
byte message        ;
int  outputPin =  12;
bool handshakeMade = false;
unsigned long pulseOnTimeMs = 500;

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
    message = SerialUSB.read();
    SerialUSB.println(message);
  }
  
  // Wait for handshake
  if (handshakeMade == false) {
    if (message == handshake) {
      SerialUSB.write(handshake);
      handshakeMade = true;
      return;
    }
  }

  // Wait for command to signal pulse
  else {
    if (message == pulse) {
      digitalWrite(outputPin, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(pulseOnTimeMs);      
      digitalWrite(outputPin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (message == reset) {
      handshakeMade = false;
    }
  }
  
}
