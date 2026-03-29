#include <WiFi.h>
#include <PubSubClient.h>

//Configuration
const char* ssid        = "Pi4";
const char* password    = "999999999";
const char* mqtt_server = "broker.emqx.io";
const int   mqtt_port   = 1883;

const char* command_topic = "epicure/commands";

// Define hardware pins for UART2 (to STM32)
#define RXp2 16
#define TXp2 17

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

void setup() {

  Serial.begin(115200);
  
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); 
  
  delay(10);
  Serial.println("\n--- ESP32 MQTT to UART Gateway ---");

  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}

//WiFi Setup 
void setup_wifi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP32Gateway-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.subscribe(command_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(command_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(5000);
    }
  }
}

//MQTT Message Callback
// This function fires automatically when a message is published to the subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic [");
  Serial.print(topic);
  Serial.print("]: ");

  // 1. Iterate through the byte array payload
  // (PubSubClient payloads are NOT null-terminated strings)
  for (unsigned int i = 0; i < length; i++) {
    char c = (char)payload[i];
    
    // Print to debug monitor
    Serial.print(c);
    
    // Forward immediately to STM32 via Hardware Serial2
    Serial2.print(c); 
  }
  Serial.println();

  // 2. Add a Framing Delimiter
  // Sending a newline character signals the end of the message to the STM32 parser
  Serial2.print('\n'); 
}