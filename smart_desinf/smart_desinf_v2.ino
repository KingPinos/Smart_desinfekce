#include <ThingSpeak.h>
#include <Wire.h>
#include <M5StickCPlus.h>
#include <Adafruit_MLX90614.h>
#include <Unit_4RELAY.h>
#include <WiFi.h>

   
// Wi-Fi 
const char* ssid = "_____";     // Nazev Wi-Fi 
const char* password = "_____";  // Heslo Wi-Fi

// ThingSpeak
const char* server = "api.thingspeak.com";
const String apiKey = "_____";  // Write API key

// ThingSpeak Channel
unsigned long myChannelNumber = _____;   
const char * myWriteAPIKey = apiKey.c_str(); 

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
UNIT_4RELAY relay;
WiFiClient client; 

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  mlx.begin();
  WiFi.begin(ssid, password);  // Wi-Fi Connect

  ThingSpeak.begin(client); 
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print(".");
  }

  relay.begin(&Wire, 32, 33);  // PIN 4-relay unit
  relay.Init(1);               // Nastavení relé a LED do synchronního režimu
  relay.relayWrite(3, 0);      // Vypnutí při startu

}

void loop() {
  float tempC = mlx.readObjectTempC(); // Měření teploty MLX90614

  // Displej
  M5.Lcd.fillScreen(BLACK);
  if (tempC > 38.0) {
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), RED);
    relay.relayWrite(3, 0);    // OFF

  } else if (tempC < 32.0) {
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), ORANGE);
    relay.relayWrite(3,0);    // OFF

  } else {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), GREEN);
    relay.relayWrite(3, 1);    // ON
  }

  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("%.2f C", tempC, M5.Lcd.width() / 2, 0);

  delay(2000); // Počkej 2 sekund
}