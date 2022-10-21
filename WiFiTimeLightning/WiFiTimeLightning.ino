#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Zerk";
const char *password = "beer4pass";

const long utcOffsetInSeconds = -25200;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

bool LightsOn = false;
int TimeTimer = 90;

#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN (D4)
#define NUM_PIXELS (60)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  timeClient.begin();
  pixels.begin();
  pixels.setBrightness(25);
}

void loop() {
  TimeTimer++;
  if (TimeTimer > 100) {
    timeClient.update();
    Serial.println(timeClient.getHours());
    if (timeClient.getHours() < 23 && timeClient.getHours() > 17) {
      LightsOn = true;
    }else{
      LightsOn = false;
    }
    TimeTimer = 0;
  }
  if (LightsOn) {
    Serial.println("Spooky");
    all_on(random(0, 100));
    delay(random(0, random(0, random(0, random(0, 20000)))));
  }else{
    Serial.println("Not Spooky");
    delay(1000);
  }
  Serial.println(TimeTimer);
}

void all_on(int Length) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.show();
  delay(Length);
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
}