#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "Zerk";
const char *password = "beer4pass";
const long utcOffsetInSeconds = -25200;
#define NEOPIXEL_PIN (D4)
#define NUM_PIXELS (60)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;

bool LightsOn = false;

unsigned long lastNTPUpdate = 0;       // Stores the last time we updated NTP time
const long NTPUpdateInterval = 60000;  // Check NTP every 60 seconds

unsigned long lastFlash = 0;           // Stores the last time we triggered lightning
unsigned long nextFlashInterval = 0;   // Random time for next lightning

bool isConnectedToInternet() {
  return client.connect("google.com", 80);
}

const unsigned long wifiConnectionTimeout = 15000;  // 15 seconds

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  unsigned long wifiConnectionStart = millis();  // Record the time when connection attempt started

  while (WiFi.status() != WL_CONNECTED && millis() - wifiConnectionStart < wifiConnectionTimeout) {
      delay(500);
      Serial.println(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
  } else {
      Serial.println("Failed to connect to WiFi after 15 seconds.");
  }

  // Check for internet connection by attempting to connect to google.com
  if (isConnectedToInternet()) {
    timeClient.begin();
  } else {
    LightsOn = true; // If no internet, turn on the light effect immediately
    Serial.println("No internet, starting program without time.");
  }

  pixels.begin();
  pixels.setBrightness(25);
}
void loop() {
  unsigned long currentMillis = millis();

  // NTP time update
  if (isConnectedToInternet() && currentMillis - lastNTPUpdate > NTPUpdateInterval) {
    timeClient.update();
    if (timeClient.getHours() < 23 && timeClient.getHours() > 17) {
      LightsOn = true;
      Serial.println("Spooky");
    } else {
      LightsOn = false;
      Serial.println("Not Spooky");
    }
    lastNTPUpdate = currentMillis;
    Serial.println(timeClient.getHours());
  }

  // Lightning effect
  if (LightsOn && currentMillis - lastFlash > nextFlashInterval) {
      int numberOfFlashes = random(0, 4); // 0 to 3 flashes
      for (int i = 0; i < numberOfFlashes; i++) {
          all_on(random(0, 100));
          delay(random(50, 200));  // It's a short delay, so we can afford to use delay here
      }
      nextFlashInterval = random(0, random(0, random(0, random(0, 20000))));
      lastFlash = currentMillis;
  } else {
      turn_off_all();
  }
}

void all_on(int Length) {
  int brightness = random(50, 256);  // vary intensity from half to full
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }
  pixels.show();
  delay(Length);
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}

void turn_off_all() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
}
