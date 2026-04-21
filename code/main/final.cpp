/************ BLYNK TEMPLATE INFO ************/
#define BLYNK_TEMPLATE_ID "TMPL3pVsLUQbG"
#define BLYNK_TEMPLATE_NAME "Environment Monitoring"
#define BLYNK_AUTH_TOKEN "Mj16Tatez9uD0ENZ1-ImsSK0B9zkU5M"

/************ LIBRARIES ************/
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

/************ SENSOR PINS ************/
#define DHTPIN D4
#define DHTTYPE DHT11
#define SOIL_PIN D3
#define LDR_PIN A0

DHT dht(DHTPIN, DHTTYPE);

/************ WIFI DETAILS ************/
char ssid[] = "Devraj";
char pass[] = "18451845";

BlynkTimer timer;

/************ SENSOR FUNCTION ************/
void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  int soilValue     = digitalRead(SOIL_PIN);
  int ldrValue      = analogRead(LDR_PIN);

  // Send to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilValue);
  Blynk.virtualWrite(V3, ldrValue);

  // Serial Print for debug
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" *C | Humidity: "); Serial.print(humidity);
  Serial.print(" % | Soil: "); Serial.print(soilValue);
  Serial.print(" | Light: "); Serial.println(ldrValue);
}

/************ SETUP ************/
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(SOIL_PIN, INPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Non-blocking Blynk connection
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();

  // Timer interval (every 2 seconds)
  timer.setInterval(2000, sendSensorData);
}

/************ LOOP ************/
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!Blynk.connected()) {
      Blynk.connect();
    }
    Blynk.run();
  }
  timer.run();
}