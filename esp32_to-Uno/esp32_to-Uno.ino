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

void setup() {
  Serial.begin(19200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  wifi_connect();
  firebase_auth();

}

void loop() {
  // Serial2.print("Esp32 Says: Hallo Uno! This is ESP32\n");
  // delay(200);

  // while (Serial2.available ()) {
  //   Serial.println(Serial2.readString());
  // }
  while (Serial2.available()) {
    dataIn = Serial2.read();

    if (dataIn != '\n' && dataIn != '\r') { message += dataIn; }
    else                { break; }

  }

  if (dataIn == '\n') {
    //Serial.println(message);
    message.trim();
    // String input;
    String jsonStr = message;

    int emptySpaces = jsonStr.substring(jsonStr.indexOf("\"empty Spaces\":") + 14, jsonStr.indexOf(",", jsonStr.indexOf("\"empty Spaces\":"))).toInt();
    int carsParked = jsonStr.substring(jsonStr.indexOf("\"cars parked\":") + 13, jsonStr.indexOf(",", jsonStr.indexOf("\"cars parked\":"))).toInt();
    String slot1 = jsonStr.substring(jsonStr.indexOf("\"slot 1\":\"") + 9, jsonStr.indexOf("\"", jsonStr.indexOf("\"slot 1\":\"") + 9));
    String slot2 = jsonStr.substring(jsonStr.indexOf("\"slot 2\":\"") + 9, jsonStr.indexOf("\"", jsonStr.indexOf("\"slot 2\":\"") + 9));
    String slot3 = jsonStr.substring(jsonStr.indexOf("\"slot 3\":\"") + 9, jsonStr.indexOf("\"", jsonStr.indexOf("\"slot 3\":\"") + 9));
    String slot4 = jsonStr.substring(jsonStr.indexOf("\"slot 4\":\"") + 9, jsonStr.indexOf("\"", jsonStr.indexOf("\"slot 4\":\"") + 9));

    Serial.println("Empty Spaces: " + String(emptySpaces));
    Serial.println("Cars parked: " + String(carsParked));
    Serial.println("Slot 1: " + slot1);
    Serial.println("Slot 2: " + slot2);
    Serial.println("Slot 3: " + slot3);
    Serial.println("Slot 4: " + slot4);
    // JsonDocument doc;

    // DeserializationError error = deserializeJson(doc, message);

    // if (error) {
    //   Serial.print("deserializeJson() failed: ");
    //   Serial.println(error.c_str());
    //   return;
    // }

    // int empty_Spaces = doc["empty Spaces"]; // 89
    // int cars_parked = doc["cars parked"]; // 29
    // const char* slot_1 = doc["slot 1"]; // "occupied"
    // const char* slot_2 = doc["slot 2"]; // "occupied"
    // const char* slot_3 = doc["slot 3"]; // "occupied"
    // const char* slot_4 = doc["slot 4"]; // "occupied"

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();
      if(Firebase.RTDB.setString(&fbdo, "ParkingInfo/Availabilty", WIFI_SSID)) {
        Serial.println();
        Serial.print(" - Succesfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }else {
        Serial.println("FAILED: " + fbdo.errorReason());
      }
    }
    
    dataIn = 0;
    message = "";
  }
  
  delay(100);
  
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