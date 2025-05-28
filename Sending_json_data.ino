#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN A0

const char* ssid = "bidur";
const char* password = "11223344";

ESP8266WebServer server(80);  // Web server on port 80
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi.");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());

   server.on("/", []() {
    float tempC = dht.readTemperature();
    float humidity = dht.readHumidity();
    int soilRaw = analogRead(SOIL_PIN);
    float moisture = map(soilRaw, 1023, 0, 0, 100);

    String json = "{";
    json += "\"temperature\": " + String(tempC, 2) + ",";
    json += "\"humidity\": " + String(humidity, 2) + ",";
    json += "\"soil_moisture\": " + String(moisture, 2);
    json += "}";

    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
