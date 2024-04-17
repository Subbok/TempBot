# 1 "D:\\Arduino\\TempBot\\TempBot.ino"
# 2 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 3 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 4 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 5 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 6 "D:\\Arduino\\TempBot\\TempBot.ino" 2
// #include "Array.h"
# 15 "D:\\Arduino\\TempBot\\TempBot.ino"
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht(8, DHT22);

void setup()
{
    Serial.begin(9600);

    if (!display.begin(0x02 /*|< Gen. display voltage from 3.3V*/, 0x3C))
    {
        Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("SSD1306 allocation failed")))));
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
        Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Failed to read from DHT sensor!")))));
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
    display.setTextColor(1 /*|< Draw 'on' pixels*/ /*|< Draw 'on' pixels*/);
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
