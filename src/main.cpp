#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define DHT11_PIN D4
DHT dht(DHT11_PIN, DHT11);

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);

String page = "";
float humidity = 0.0;
float temperature = 0.0;

void setup() {
  dht.begin();

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi network..");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", [](){
    page = "";
    page += "Temperature ";
    page += temperature;
    page += "\n";
    page += "Humidity ";
    page += humidity;
    page += "\n";
    server.send(200, "text/plain", page);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  server.handleClient();
  delay(1000);
}