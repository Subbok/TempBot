# 1 "D:\\Arduino\\TempBot\\TempBot.ino"
# 2 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 3 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 4 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 5 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 6 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 7 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 8 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 9 "D:\\Arduino\\TempBot\\TempBot.ino" 2
# 17 "D:\\Arduino\\TempBot\\TempBot.ino"
Adafruit_SSD1306 display(128, 64, &Wire, -1);
DHT dht(8, DHT22);
WiFiUDP ntp;
NTPClient timeclient(ntp, "pl.pool.ntp.org");

const char *ssid = "TP-Link_0695";
const char *password = "63156508";

void setup()
{
    Serial.begin(9600);

    if (!display.begin(0x02 /*|< Gen. display voltage from 3.3V*/, 0x3C))
    {
        Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("SSD1306 allocation failed")))));
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
        Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("Failed to read from DHT sensor!")))));
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
    display.setTextColor(1 /*|< Draw 'on' pixels*/ /*|< Draw 'on' pixels*/);
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
