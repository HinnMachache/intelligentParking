//Esp32 Code
#define RXD2 16
#define TXD2 17

char dataIn;
String message;

void setup() {
  Serial.begin(19200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  // Serial2.print("Esp32 Says: Hallo Uno! This is ESP32\n");
  // delay(200);

  // while (Serial2.available ()) {
  //   Serial.println(Serial2.readString());
  // }
  while (Serial2.available()) {
    dataIn = Serial2.read();

    if (dataIn != '\n') { message += dataIn; }
    else                { break; }

  }

  if (dataIn == '\n') {
    Serial.println(message);
    
    dataIn = 0;
    message = "";
  }
  delay(100);
  
}