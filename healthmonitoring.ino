//#include < PulseSensorPlayground.h > <br>

#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#define LM35_PIN A0
#define PULSE_SENSOR_PIN 4

#define SECONDS_PER_UPDATE 10  // Update every 10 seconds

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char ssid[] = "Googly";
char pass[] = "";
#define EspSerial Serial1
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);

PulseSensorPlayground pulseSensor;

int lastUpdate = 0;

void setup()
{
  Serial.begin(115200);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  // You can also specify the server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  pulseSensor.begin();  // Initialize the pulse sensor
}

void loop()
{
  Blynk.run();
  int currentMillis = millis();
  if (currentMillis - lastUpdate >= SECONDS_PER_UPDATE * 1000) {
    lastUpdate = currentMillis;

    float temperature = readLM35Temperature();
    int heartRate = readPulseSensor();
    Serial.println(temperature);
    Serial.println(heartRate);
    Blynk.virtualWrite(V1, temperature); // Assuming V1 is a Value Display widget for temperature
    Blynk.virtualWrite(V2, heartRate);    // Assuming V2 is a Value Display widget for heart rate
  }
}

float readLM35Temperature()
{
  int sensorValue = analogRead(LM35_PIN);
  float voltage = (sensorValue / 1024.0) * 5000; // LM35 produces 10mV per degree Celsius
  float temperature = voltage / 10.0; // Convert to Celsius
  return temperature;
}

int readPulseSensor()
{

  
  int heartRate = pulseSensor.getBeatsPerMinute();
  // int heartRate = analogRead(PULSE_SENSOR_PIN); // Use this line if you still have issues with the pulse sensor library

  return heartRate;
}
