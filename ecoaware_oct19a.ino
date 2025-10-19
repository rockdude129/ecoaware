#include "arduino_secrets.h"
#include "thingProperties.h"
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

DHT dht(23, DHT11);

// Pins
const int REDLED = 18;
const int GREENLED = 19;
const int relay = 17;
const int trigPin = 22;
const int echoPin = 21;

float previousDistance = 0;
const int motionThreshold = 30;

// Helper function to determine season
String getSeason(int month) {
  if (month == 12 || month <= 2) return "Winter";
  if (month >= 3 && month <= 5) return "Spring";
  if (month >= 6 && month <= 8) return "Summer";
  return "Fall";
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  dht.begin();

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Arduino Cloud connection
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  Serial.print("Connecting to Arduino Cloud");
  while (!ArduinoCloud.connected()) {
    ArduinoCloud.update();
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to Arduino Cloud!");

  // Fetch weather data once on startup
  fetchWeatherData();
}

void loop() {
  ArduinoCloud.update();

  // --- Ultrasonic motion detection ---
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (previousDistance != 0 && abs(distance - previousDistance) > motionThreshold) {
    motionDetected = true;
    Serial.println("🚨 Motion detected!");
    relayState = true; // optional: trigger relay when motion
    onRelayStateChange(); // ensure hardware matches cloud
  } else {
    motionDetected = false;
  }
  previousDistance = distance;

  // --- Inside temperature ---
  insideTempF = dht.readTemperature(true);
  if (isnan(insideTempF)) {
    Serial.println("Failed to read DHT sensor!");
    delay(100);
    return;
  }

  // --- Comfort logic ---
  bool warmSeason = (season == "Spring" || season == "Summer");
  bool coldSeason = (season == "Fall" || season == "Winter");
  bool comfortOK = false;

  if (coldSeason) comfortOK = (insideTempF > outsideTempF);
  else if (warmSeason) comfortOK = (insideTempF < outsideTempF);
  else comfortOK = true;

  if (comfortOK) {
    digitalWrite(GREENLED, HIGH);
    digitalWrite(REDLED, LOW);
    Serial.println("✅ Comfortable for the season (GREEN)");
  } else {
    digitalWrite(GREENLED, LOW);
    digitalWrite(REDLED, HIGH);
    Serial.println("⚠️ Temperature imbalance (RED)");
  }

  // Push live updates to the cloud
  ArduinoCloud.update();

  Serial.print("Inside: "); Serial.print(insideTempF);
  Serial.print(" | Outside: "); Serial.print(outsideTempF);
  Serial.print(" | Season: "); Serial.print(season);
  Serial.print(" | Motion: "); Serial.println(motionDetected);

  delay(100);
}

void fetchWeatherData() {
  const char* apiKey = "8135f20f92cc4802923235059251810";
  const char* city = "San%20Francisco";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  String url = String("https://api.weatherapi.com/v1/current.json?key=") + apiKey + "&q=" + city + "&aqi=no";

  http.begin(client, url);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0 && httpResponseCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
    if (!error) {
      outsideTempF = doc["current"]["temp_f"];
      String localtime = doc["location"]["localtime"];
      int month = localtime.substring(5, 7).toInt();
      season = getSeason(month);

      Serial.println("==== WEATHER DATA ====");
      Serial.print("Outside Temp (F): "); Serial.println(outsideTempF);
      Serial.print("Month: "); Serial.println(month);
      Serial.print("Season: "); Serial.println(season);
      Serial.println("======================");
    } else {
      Serial.println("Failed to parse weather JSON!");
    }
  } else {
    Serial.print("HTTP Error: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

// Triggered when Google Home / Arduino Cloud toggles the relay
void onRelayStateChange() {
  digitalWrite(relay, relayState ? HIGH : LOW);
  Serial.print("Relay turned ");
  Serial.println(relayState ? "ON" : "OFF");
}
