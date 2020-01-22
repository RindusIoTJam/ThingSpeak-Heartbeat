#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "main.h"

WiFiClientSecure client;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.mode(WIFI_STA); // Ensure AP is off  

  client.setInsecure();
}

void ICACHE_RAM_ATTR inline ISR_timer0() {
  if(alarm) {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN));
  }
  
  if(++seconds>119) {
    seconds = 0;
    heartbeat = true;
  }
  timer0_write(ESP.getCycleCount() + 40000000L); // 40M/80M = 500ms
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);     // LED setup
  digitalWrite(LED_BUILTIN, 0);     // LED on

  setup_wifi();

  digitalWrite(LED_BUILTIN, 1);     // LED off

  noInterrupts();

    // interupt to increment counter every second
    timer0_isr_init();
    timer0_attachInterrupt(ISR_timer0);
    timer0_write(ESP.getCycleCount() + 20000000L); // start in 250ms (80M/20M=1/4)

  interrupts();
}

void loop() {
  if(heartbeat) {
    heartbeat = false;
    digitalWrite(LED_BUILTIN, 0);   // LED on

    if (WiFi.status() == WL_CONNECTED) {
      if (!client.connect("api.thingspeak.com", 443)) {
        Serial.println("ERROR: connecting to api.thingspeak.com failed");
      } else  {
        client.print(String("GET /update?api_key=") + thingspeakApiKey +
                    "&field1=1" +
                    " HTTP/1.1\r\n" +
                    "Host: api.thingspeak.com\r\n" +
                    "User-Agent: HeartbeatESP8266\r\n" +
                    "Connection: close\r\n\r\n");

        if (client.connected()) {
          status = client.readStringUntil('\n');
          status.replace("\r", " ");
        }
        if (client.connected()) {
          client.readStringUntil(' ');
          date = client.readStringUntil('\n');
          date.replace("\r", " ");
        }
        while (client.connected()) {
          client.read();
        }
        Serial.print(date);
        Serial.print("- ");
        Serial.println(status);
        alarm = false;
        digitalWrite(LED_BUILTIN, 1);     // LED off
      }
    } else {
      alarm = true;
      Serial.println("WARN: WiFi not connected!");
      WiFi.begin(wifi_ssid, wifi_pass);
    }
  }
}