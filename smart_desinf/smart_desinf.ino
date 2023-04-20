#include <ThingSpeak.h>

#include <Wire.h>
#include <M5StickCPlus.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>

// Wi-Fi 
const char* ssid = "LF";     // Nazev Wi-Fi 
const char* password = "hesloksiti";  // Heslo Wi-Fi

// ThingSpeak
const char* server = "api.thingspeak.com";
const String apiKey = "7RUVV7G5K5XMMRYD";  // Write API key

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 

WiFiClient client; 

// ThingSpeak Channel
unsigned long myChannelNumber = 2106319;   
const char * myWriteAPIKey = apiKey.c_str();         

void setup() {
  M5.begin();         
  M5.Lcd.setRotation(3);  
  mlx.begin();        
  WiFi.begin(ssid, password);  // Wi-Fi Connect

  ThingSpeak.begin(client); 
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  float tempC = mlx.readObjectTempC();  // MLX90614 Read

  // Displej
  M5.Lcd.fillScreen(BLACK);
  if (tempC > 38.0) {
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), RED);
  } else if (tempC < 34.0) {
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), ORANGE);
  } else {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), M5.Lcd.height(), GREEN);
  }
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("%.2f C", tempC, M5.Lcd.width() / 2, 0);

  // ThingSpeak Send
  ThingSpeak.setField(1, tempC);
  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
  Serial.print("HTTP CODE: ");
  Serial.println(httpCode);

  delay(10000);  
}