#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "time.h"
#include "web_data.h"
#include <PubSubClient.h>

//#define M5_STACK
#ifdef M5_STACK
#include <M5Stack.h>
#endif

#define pinRele 21
#define LED_BUILTIN 4
#define pinOn 22

WiFiClient espClient;
PubSubClient client(espClient);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "NOME DA REDE WIFI";
const char* password = "SENHA DA REDE WIFI";
AsyncWebServer server(80);

const char* mqtt_server = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "ESP32";
const char* mqttPassword = "123";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightoffset_sec = 3600;

bool state_on = true;

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

void onCSSRequest(AsyncWebServerRequest* request) {
  request->send(200, "text/css", style_css);
}

void onTurnHigh(AsyncWebServerRequest* request) {
  digitalWrite(pinRele, HIGH);
  request->send(202, "text/plain", "ON");
}

void onTurnLow(AsyncWebServerRequest* request) {
  digitalWrite(pinRele, LOW);
  request->send(202, "text/plain", "ON");
}

unsigned long epochTime;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.println(messageTemp);

  if (messageTemp == "botao") 
  {
    if (!state_on) 
    {
      digitalWrite(pinRele, LOW); 
      state_on = true;
    }
    else
    {
      digitalWrite(pinRele, HIGH);  
      state_on = false;
    }
  }

  if (messageTemp == "on") {
    Serial.println("LED is on");
    digitalWrite(pinRele, LOW);
  } else if (messageTemp == "off") {
    Serial.println("LED is off");
    digitalWrite(pinRele, HIGH);
  }
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(WiFi.macAddress());
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("test_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pinRele, OUTPUT);
  pinMode(pinOn, OUTPUT);
  digitalWrite(pinOn, HIGH);

  #ifdef M5_STACK
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(
        0, 0);
  M5.Lcd.print("Vamos comecar");
  #endif

  Serial.println("VAMOS COMECAR");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    ESP.restart();
    return;
  } else {
    pinMode(LED_BUILTIN, OUTPUT);
    for (byte i = 0; i < 2; i++) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  Serial.println();
  Serial.print("IP Address: ");
  String ip_config = WiFi.localIP().toString().c_str();
  Serial.println(ip_config);
  #ifdef M5_STACK
  for (uint8_t i = 0; i < 3; i++) {
    M5.Lcd.fillScreen(GREEN);
    vTaskDelay(100);
    M5.Lcd.fillScreen(BLACK);
    vTaskDelay(100);
  }
  M5.Lcd.print("Conectado na rede IP: " + ip_config);
  #endif
  configTime(0, 0, ntpServer);

  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  delay(1000);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/style.css", HTTP_GET, onCSSRequest);
  server.on("/high", HTTP_GET, onTurnHigh);
  server.on("/low", HTTP_GET, onTurnLow);
  server.onNotFound(notFound);
  server.begin();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  #ifdef M5_STACK
  M5.Lcd.setCursor(
      0, 120);
  #endif
  if (!client.connected()) {
    reconnect();
    #ifdef M5_STACK
    M5.Lcd.print("Desconectado do MQTT!!");
    #endif
  }
  #ifdef M5_STACK
  M5.Lcd.print("Conectado MQTT");
  M5.Lcd.setCursor(
      0, 160);
  M5.Lcd.print("broker.emqx.io");
  M5.Lcd.setCursor(
      0, 180);
  M5.Lcd.print("topic: test_topic");
  #endif
  client.loop();
}