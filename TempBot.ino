#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 8
#define DHTTYPE DHT22 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

sensors_event_t event;
sensors_event_t humidityEvent;
sensor_t sensor;

String temp_text = "Temp:";
String hum_text = "Hum:";

String text = "23.7%";

String temp = String(event.temperature, 2);
String hum = String(humidityEvent.relative_humidity);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

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

    dht.temperature().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
}

void loop()
{
    delay(delayMS);

    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
        return;
    }

    dht.humidity().getEvent(&humidityEvent);
    if (isnan(humidityEvent.relative_humidity))
    {
        Serial.println(F("Error reading humidity!"));
        return;
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(64 - temp_text.length() * 2, 0);
    display.print(F("Temp: "));
    display.setCursor(64 - 12, 10);
    display.print(event.temperature);
    display.print((char)247);
    display.println("C");

    display.setCursor(64 - hum_text.length() * 2, 20);
    display.print(F("Hum: "));
    display.setCursor(64 - 5, 30);
    display.print(event.relative_humidity);
    display.println(F("%"));

    display.display();

    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    Serial.print(F("Humidity: "));
    Serial.print(humidityEvent.relative_humidity);
    Serial.println(F("%"));
    Serial.print(text.length());
}
