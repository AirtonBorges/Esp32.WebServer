#include <WiFi.h>
#include <LittleFS.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>

#define LED_BUILTIN 8

const char* ssid = "***";
const char* password = "***";
bool ledState = false;

AsyncWebServer server(80);

void handleLedToggle(AsyncWebServerRequest *request) {
  Serial.println("Toggling LED");
  ledState = !ledState;

  digitalWrite(LED_BUILTIN, ledState ? LOW : HIGH);

  String message = ledState ? "LED is ON" : "LED is OFF";
  request->send(200, "application/json", "{\"status\":\"" + message + "\"}");

  Serial.println(message);
}

String processor(const String& var){
  return var;
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  while(!Serial && millis() < 5000){
    delay(10);
  }

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  Serial.println("LittleFS mounted successfully");
  WiFi.begin(ssid, password);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/app.js", String(), false, processor);
  });

  server.on("/led", HTTP_POST, handleLedToggle);

  server.begin();
}

void loop(){
  Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}
