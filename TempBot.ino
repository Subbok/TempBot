#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
// #include "Array.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DHTPIN 8
#define DHTTYPE DHT22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    display.clearDisplay();
    display.display();

    dht.begin();
}

void loop()
{
    delay(2000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    float hic = dht.computeHeatIndex(t, h, false);

    displayOled(t, h, hic);
}

void displayOled(float t, float h, float hic)
{
    String t_str = String(t, 1);
    String h_str = String(h, 1);
    String hic_str = String(hic, 1);

    if (t_str.endsWith(".0"))
        t_str.remove(t_str.length() - 2, 2);
    if (h_str.endsWith(".0"))
        h_str.remove(h_str.length() - 2, 2);
    if (hic_str.endsWith(".0"))
        hic_str.remove(hic_str.length() - 2, 2);

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(5, 15);
    display.print("Humidity:");
    display.setCursor(80, 15);
    display.print(h_str);
    display.print("%");
    display.setCursor(5, 30);
    display.print("Temperature:");
    display.setCursor(80, 30);
    display.print(t_str);
    display.print((char)247);
    display.print("C");
    display.setCursor(5, 45);
    display.print("Heat Index:");
    display.setCursor(80, 45);
    display.print(hic_str);
    display.print((char)247);
    display.print("C");
    display.display();
}