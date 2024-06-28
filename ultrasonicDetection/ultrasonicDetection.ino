#define trigPin 6
#define echoPin 7

float duration_us, distance_cm, distance, filteredDistance, filterArray[20];

void setup() {
  Serial.begin(19200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  filteredDistance = filterNoise();
  Serial.print("Distance: ");
  Serial.print(filteredDistance);
  Serial.println("cm");
  
  parkingInfo();
}

float measureDistance() {
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
  int parkingSpaces = 5;
  int carCounter = 0;
  bool carParked = false;
  int distance;

  distance = filterNoise();

  if (distance > 6) {
    carParked = false;
    carCounter -= 1;
  }

  if (distance < 4) {
    carParked = true;
    carCounter += 1;
  }

  if (carCounter <= 0) { carCounter = 0; }

  parkingSpaces = parkingSpaces - carCounter;

  Serial.println(parkingSpaces);
  Serial.println(carCounter);
  Serial.println(carParked);

}