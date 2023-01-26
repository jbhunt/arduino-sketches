// Constants
byte          connectCommand = 'c';
byte          signalCommand  = 's';
byte          releaseCommand = 'r';
byte          message             ; 
int           outputPin      =  12;
unsigned long pulseOnTimeMs  = 500;

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

  // Send byte back to Python
  if (message == connectCommand) {
    SerialUSB.write(connectCommand);
    return;
  }

  // Emit a pulse
  if (message == signalCommand) {
    digitalWrite(outputPin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(pulseOnTimeMs);      
    digitalWrite(outputPin, LOW);
    digitalWrite(LED_BUILTIN, LOW); 
  }
}
