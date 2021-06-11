#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN            15         // Pin which is connected to the sensor.

// Using humidity and temp sensor to measure
// Relaying information to dashboard on IBM Watson IoT platform

#define DHTTYPE           DHT11     

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> 


//-------- wifi -----------
const char* ssid = " ";
const char* password = " ";

#define ORG "0bo118"
#define DEVICE_TYPE "ESP32-WROOM-32"
#define DEVICE_ID "887E3CB267AC"
#define TOKEN "!8@F_5_ceFGRJQV-jc"


//-------- information extracted from internet of things service by IBM cloud to build up MQTT message -------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubTopic[] = "iot-2/evt/status/fmt/json";
char subTopic[] = "iot-2/cmd/test/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

const char ledPin = 2;
const char led = 4;

void receivedCallback(char* pubTopic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(pubTopic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == '1') {
    digitalWrite(led, HIGH); 
  } else {
    /* we got '0' -> on */
    digitalWrite(led, LOW);
  }
}

void setup() {
    Serial.begin(115200);
    Serial.println();
    pinMode(ledPin, OUTPUT);
    Serial.println("IBM Watson IoT ESP32 Temperature, Humidity With DHT11");
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
    
    Serial.print("Connecting to "); 
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    } 
    Serial.println("");
    
    Serial.print("WiFi connected, IP address: "); 
    Serial.println(WiFi.localIP());

    if (!client.connected()) {
        Serial.print("Reconnecting client to ");
        Serial.println(server);
        while (!client.connect(clientId, authMethod, token)) {
            Serial.print(".");
            delay(500);
        }
        client.setCallback(receivedCallback);
        if (client.subscribe(subTopic)) {
            Serial.println("subscribe to cmd OK");
        } else {
            Serial.println("subscribe to cmd FAILED");
        }
        Serial.println("IBM Watson IoT connected");
    }
}

long lastMsg = 0;
long temperature = 0;
long humidity = 0;

void loop() {
    delay(delayMS);
    sensors_event_t event;  
    client.loop();
    long now = millis();
    if (now - lastMsg > 3000) {
        lastMsg = now;
        dht.temperature().getEvent(&event);
        temperature = (event.temperature);
        dht.humidity().getEvent(&event);
        humidity = (event.relative_humidity);
        
        String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
               payload += ",\"temperature\":";
               payload += temperature;
               payload += ",\"humidity\":";
               payload += humidity;
               payload += "}}";
        Serial.print("Sending payload: ");
        Serial.println(payload);

        if (client.publish(pubTopic, (char*) payload.c_str())) {
            Serial.println("Publish ok");
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        delay(1000);
        } else {
            Serial.println("Publish failed");
        }
    }
}
