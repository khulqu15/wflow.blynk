// #define BLYNK_TEMPLATE_ID           "TMPLb4IkI0OP"
// #define BLYNK_DEVICE_NAME           "Quickstart Device"
// #define BLYNK_AUTH_TOKEN            "W2wgaGsFZ8AQHEXFuL37Xj7rLTsGuEzw"

#define BLYNK_TEMPLATE_ID "TMPL7Kaie5_W"
#define BLYNK_DEVICE_NAME "Flowmeter"
#define BLYNK_AUTH_TOKEN "JO64oQoJbboCslH0kyU9-NMNX7v_iLPE"

#define BLYNK_PRINT Serial

int sensorPin = D6;
volatile long pulse;
unsigned long lastTime;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN D7
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "GH D-Team";
char pass[] = "Jancokcokasulah";

BlynkTimer timer;

void setup()
{
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), increase, RISING);
  lcd.begin();
  dht.begin();
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  float volume = 2.663 * pulse / 1000 * 30;
  if (millis() - lastTime > 2000) {
    pulse = 0;
    lastTime = millis();
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print(F("Water Flow: "));
  Serial.print(volume);
  Serial.print("\n");
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print("\n");
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print("\n");
  delay(250);

  Blynk.virtualWrite(V2, volume);
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0,0);
  lcd.print("W.FLOW:");
  lcd.println(volume);
  lcd.setCursor(0,1);
  lcd.print("T/L:");
  lcd.print(t);
  lcd.print("/");
  lcd.println(h);
  delay(250);

  Blynk.run();
  timer.run();
}

ICACHE_RAM_ATTR void increase() {
  pulse++;
}
