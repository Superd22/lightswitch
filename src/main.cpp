#include <Arduino.h>
#include <./config.h>
#include <button-controller.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <./hue.h>

const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiClient client;
const int led = 13;
std::vector<ButtonController> buttons; 

ESPHue hueApi = ESPHue(client, HUE_API_KEY, HUE_BRIDGE_IP, 80);

void setup() {  
  Serial.begin(115200);
  pinMode(D1, INPUT);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 

  Serial.println("Creating groups");
  buttons.push_back(ButtonController(hueApi, "Main", D1));
  Serial.println(sizeof(buttons));
}

void loop() {
  buttons[0].handleButton();
}

void handleButtonPress() {

}