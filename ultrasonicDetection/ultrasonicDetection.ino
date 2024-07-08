#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define trigPin 5
#define echoPin 6
#define carPark1 2
#define carPark2 3
#define carPark3 4

SoftwareSerial softSerial (10, 11);  //RX,TX

float filteredDistance;

void setup() {
  Serial.begin(19200);
  softSerial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(carPark1, INPUT);
  pinMode(carPark2, INPUT);
  pinMode(carPark3, INPUT);
}

void loop() {
  // filteredDistance = filterNoise();
  // Serial.print("Distance: ");
  // Serial.print(filteredDistance);
  // Serial.println("cm");
  
  parkingInfo();
  //softSerial.println(filteredDistance);

}

float measureDistance() {
  float duration_us, distance_cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration_us = pulseIn(echoPin, HIGH);
  distance_cm = 0.017 * duration_us;

  return (distance_cm);
}

float filterNoise() {
  float distance, filterArray[20];
  //1. Take 20 measurements and store in an array
  for (int sample = 0; sample < 20; sample++) {
    filterArray[sample] = measureDistance();
    delay(30);
  }

  //2. Sort Array in Ascending order
  for (int currentSample = 0; currentSample < 19; currentSample++) {
    for (int nextSample = currentSample + 1; nextSample < 20; nextSample++) {
      if (filterArray[currentSample] > filterArray[nextSample]) {
        float swap = filterArray[currentSample];
        filterArray[currentSample] = filterArray[nextSample];
        filterArray[nextSample] = swap;
      }
    }
  }

  //3. Filter Array
  double sum = 0;
  for (int counter = 5; counter < 15; counter++) {
    sum += filterArray[counter];
  }
  distance = sum / 10;

  return (distance);

}

void parkingInfo() {
  int parkingSpaces = 4;
  int emptySpaces = 0;
  int carCounter = 0, slot1 = 0, slot2 = 0, slot3 = 0, slot4 = 0;
  String space1 = "empty", space2 = "empty", space3 = "empty", space4 = "empty";
  String carParked = "", availableSpaces = "";
  int distance;
  String output;

  int carStatus1 = digitalRead(carPark1);
  int carStatus2 = digitalRead(carPark2);
  int carStatus3 = digitalRead(carPark3);
  distance = filterNoise();

  if (carStatus1 == 0) {
    slot1 = 1;
    space1 = "occupied";
    // softSerial.println("Slot 1 is occupied");
  }
  else {
    slot1 = 0;
    space1 = "empty";
    //softSerial.println("Slot 1 is empty!");
  }
  if (carStatus2 == 0) {
    slot2 = 1;
    space2 = "occupied";
    //softSerial.println("Slot 2 is occupied");
  }
  if (carStatus2 == 1) {
    slot2 = 0;
    space2 = "empty";
    //softSerial.println("Slot 2 is empty!");
  }
  if (carStatus3 == 0) {
    slot3 = 1;
    space3 = "occupied";
    //softSerial.println("Slot 3 is occupied");
  }
  else {
    slot3 = 0;
    space3 = "empty";
    //softSerial.println("Slot 3 is empty!");   
  }

  if (distance > 6) {
    slot4 = 0;
    space4 = "empty";
    //softSerial.println("Slot 4 is empty!");
  }

  if (distance < 5) {
    slot4 = 1;
    space4 = "occupied";
    //softSerial.println("Slot 4 is occupied!");
  }

  carCounter = slot1 + slot2 + slot3 + slot4;
  emptySpaces = parkingSpaces - carCounter;
  
  JsonDocument doc;

  doc["empty Spaces"] = emptySpaces;
  doc["cars parked"] = carCounter;
  doc["slot 1"] = space1;
  doc["slot 2"] = space2;
  doc["slot 3"] = space3;
  doc["slot 4"] = space4;

  doc.shrinkToFit();  // optional

  serializeJson(doc, output);

  // availableSpaces = String(emptySpaces) + String("\n");
  // carParked = String(carCounter) + String("\n");
  // softSerial.print("Empty Spaces: ");
  // softSerial.print(availableSpaces);
  // softSerial.print("Cars parked: ");
  // softSerial.print(carParked);
  softSerial.println(output);
  Serial.println(output);
  delay(100);
  

}