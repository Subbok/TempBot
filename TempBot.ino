#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DHTPIN 8
#define DHTTYPE DHT22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);
WiFiUDP ntp;
NTPClient timeclient(ntp, "pl.pool.ntp.org");

const char *ssid = "TP-Link_0695";
const char *password = "63156508";

void setup()
{
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    dht.begin();
    WiFi.begin(ssid, password);

    display.clearDisplay();
    display.display();
}

void loop()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    timeclient.update();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);

    if (isnan(h) || isnan(t))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        delay(1000);
        return;
    }

    displayInfo(t, h, hic);
    delay(500);
}

void displayInfo(float t, float h, float hic)
{
    String t_str = String(t, 1);
    String h_str = String(h, 1);
    String hic_str = String(hic, 1);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 2);
    display.println(timeclient.getFormattedTime());
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.print("Temperature: ");
    display.setCursor(90, 20);
    display.print(t_str);
    display.print((char)247);
    display.print("C");
    display.setCursor(10, 35);
    display.print("Humidity: ");
    display.setCursor(90, 35);
    display.print(h_str);
    display.println("%");
    display.setCursor(10, 50);
    display.print("Feels like:");
    display.setCursor(90, 50);
    display.print(hic_str);
    display.print((char)247);
    display.print("C");
    display.display();
}
