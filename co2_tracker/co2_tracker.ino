#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <K30_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
K30_I2C k30_i2c = K30_I2C(0x68);
int co2 = 0;
int rc  = 1;

//enter details here
String wifiName = "Alexandre's Pixel"; //your wifi name
String wifiPassword = "s059960054"; //your wifi password
String apiKey = "4icsphxnck3za91z5o7jq"; //your secret api key

void setup() {

  lcd.init(); //initialize the lcd
  lcd.init();
  lcd.backlight(); //open the backlight

  Serial.begin(115200);                                  //Serial connection
  WiFi.begin(wifiName, wifiPassword);   //WiFi connection

}

void loop() {

  rc = k30_i2c.readCO2(co2);
  if (rc == 0 && co2 != 0) {
  
    Serial.print(co2);
    Serial.print("ppm ");

    lcd.clear();
    lcd.print(String(co2));
    lcd.print(" ppm");
    lcd.setCursor(0, 1);
    lcd.print("WiFi: ");

    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
      lcd.print("connected");
      DynamicJsonDocument doc(2048);
      doc["value"] = co2;
      doc["apiKey"] = apiKey;
      String json;
      serializeJson(doc, json);
      HTTPClient http;    //Declare object of class HTTPClient
      http.begin("http://mern-co2-tracker.herokuapp.com/api/sensor/update");      //Specify request destination
      http.addHeader("Content-Type", "application/json");  //Specify content-type header
      int httpCode = http.POST(json);   //Send the request
      Serial.print(" HTTP: ");
      Serial.print(httpCode);   //Print HTTP return code
      Serial.println("\n");
      http.end();  //Close connection
    } else {
      lcd.print("connecting");
    }
  } else {
    Serial.print("Failed to read sensor");
    lcd.clear();
    lcd.print("Initializing");
  }
  delay(3000);  //Send a request every 30 seconds
}
