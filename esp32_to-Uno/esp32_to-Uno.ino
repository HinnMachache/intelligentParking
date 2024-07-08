//Esp32 Code
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "GW_Ki-Near"
#define WIFI_PASSWORD "IMachache@123"
#define API_KEY "AIzaSyAY5hEfz5BOqqGRLM0tp7Z8BoQcfi8HFJE"
#define DATABASE_URL "https://esp32-project00-default-rtdb.firebaseio.com/"

#define RXD2 16
#define TXD2 17

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

char dataIn;
String message;
unsigned long lastReadTime = 0;
const unsigned long readTimeout = 2000; // Timeout for reading the message


void setup() {
  Serial.begin(19200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  wifi_connect();
  firebase_auth();

}

void loop() {
  while (Serial2.available()) {
    dataIn = Serial2.read();

    if (dataIn != '\n') {
      message += dataIn;
      lastReadTime = millis(); // Update the last read time
    } else {
      processMessage();
    }
  }

  // Check for timeout
  if (millis() - lastReadTime > readTimeout && message.length() > 0) {
    Serial.println("Read timeout, processing message");
    processMessage();
  }
  
  delay(100);
  
}

void processMessage() {
  message.trim(); // Remove any extraneous whitespace or newline characters
  Serial.println("Received JSON: " + message);

  // Check if the received message is a valid JSON string
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("Invalid JSON received: ");
    Serial.println(error.c_str());
  } else {
    int empty_Spaces = doc["empty Spaces"]; // 89
    int cars_parked = doc["cars parked"]; // 29
    const char* slot_1 = doc["slot 1"]; // "occupied"
    const char* slot_2 = doc["slot 2"]; // "occupied"
    const char* slot_3 = doc["slot 3"]; // "occupied"
    const char* slot_4 = doc["slot 4"]; // "occupied"

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();
      if (Firebase.RTDB.setInt(&fbdo, "ParkingInfo/Availability", empty_Spaces)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setInt(&fbdo, "ParkingInfo/cars_parked", cars_parked)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setString(&fbdo, "ParkingInfo/Slot_1", slot_1)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setString(&fbdo, "ParkingInfo/Slot_2", slot_2)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setString(&fbdo, "ParkingInfo/Slot_3", slot_3)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setString(&fbdo, "ParkingInfo/Slot_4", slot_4)) {
        Serial.println();
        Serial.print(" - Successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      } else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }
    }
  }

  // Clear message buffer after processing
  message = "";
}

void wifi_connect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void firebase_auth () {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("SignUp Successful");
    signupOK = true;
  }else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}