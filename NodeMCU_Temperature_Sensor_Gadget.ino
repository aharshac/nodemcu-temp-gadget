/*
  Collaborizm NodeMCU Temperature Sensor Gadget
  * Made for NodeMCU (ESP8266)
  * Configures WiFi authentication using a provisional configuration portal using EasyNTPClient library.
  * Updates a DS1307 RTC by fetching Internet Time from Network Time Protocol
  * Measures temperature using an LM35
  * Displays time and temperature on a 16x2 I2C Lcd
  * Serial debug output at 9600 baud
  
  Created by Harsha Alva, June 29, 2017.
  Released into the public domain.

  Pin Connections:
  + NodeMCU V3 (Chinese clone)
    * D2 = I2C SDA
    * D1 = I2C SCL
    * A0 = Analog input
    * VV = USB voltage output (+5V)
    * G  = Common GND
  + DS1307 (I2C)
    * Vcc = +5V
    * Gnd = Common GND
    * SDA = NodeMCU D2
    * SCL = NodeMCU D1
  + 16x2 Lcd with I2C "Backpack"
    * Vcc = +5V
    * GND = Common GND
    * SDA = NodeMCU D2
    * SCL = NodeMCU D1
  + LM35
    * Refer pinout from https://www.google.co.in/search?q=lm35+pinout
    * Vs = +3.3V
    * Vout = NodeMCU A0
    * GND = Common GND
*/

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <WiFiUdp.h>

#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EasyNTPClient.h>


int tempPin = A0;

LiquidCrystal_I2C Lcd(0x27, 16, 2); // Lcd Backpack
RTC_DS1307 Rtc;

void setup() {
  // Init Serial
  Serial.begin(9600);

  // Init Lcd
  Lcd.begin(16,2);
  Lcd.init();
  Lcd.backlight();

  // Display Project Info
  Serial.println("Collaborizm - Temperature IoT Node");
  Lcd.setCursor(0, 0);
  Lcd.print(F("Collaborizm"));
  Lcd.setCursor(0, 1);      
  Lcd.print(F("Temp IoT Node"));
  delay(2000);

  // Set up WiFi
  WiFiManager wifiManager;
  wifiManager.autoConnect("CZM-Temp-IoT-Node", "collaborizm");

  // Check for RTC
  if (!Rtc.begin()) {
    Serial.println("RTC not found");
    Lcd.clear();
    Lcd.setCursor(0, 0);
    Lcd.println("RTC not found");
    while (1);
  }

  // Display ESP Chip info
  long chipId = ESP.getChipId();
  Serial.print(F("ESP ID = "));
  Serial.println(chipId);
  
  // Get internet time from NTP
  WiFiUDP udp;
  EasyNTPClient ntpClient(udp, "pool.ntp.org", ((5*60*60)+(30*60))); // IST = GMT + 5:30
  unsigned long serverTime = ntpClient.getUnixTime();
  Serial.print("NTP timestamp = "); 
  Serial.println(serverTime);

  Lcd.clear();
  Lcd.setCursor(0, 0);
  if (serverTime > 0) {
    Serial.println("RTC time updated from NTP");
    Lcd.println("RTC time updated from NTP");
    Rtc.adjust(serverTime); // Save NTP time to RTC
  } else {
    Serial.println("Did not receive time from NTP");
    Lcd.println("Did not receive time from NTP");
  }

  delay(1200);
}

// Repeat every 10 seconds
void loop() {
  Lcd.clear();
  printTemp();
  printTime();
  
  delay(10000);
}

void printTemp() {
  float tempC = (analogRead(tempPin) * 330.0) / 1024;   // analogRead(...) * (voltage_across_LM35 * 100) / 1024
  int tempF = (int)round((tempC * 1.8) + 32);

  Serial.print(tempC);
  Serial.print((char) 223); // degree symbol
  Serial.print("C ");
  Serial.print(tempF);
  Serial.print((char) 223); // degree symbol
  Serial.println("F");
  
  Lcd.setCursor(0, 0);
  Lcd.print(tempC);
  Lcd.print((char) 223); // degree symbol
  Lcd.print("C ");
  Lcd.print(tempF);
  Lcd.print((char) 223); // degree symbol
  Lcd.print("F");
}

void printTime() {
  DateTime now = Rtc.now();
  
  String day = String(now.day());
  String month = String(now.month());
  String year = String(now.year() % 2000);
  
  String hour = String(now.hour());
  String minute = String(now.minute());

  // Hours
  if (now.hour() < 10) {
    hour = "0" + hour;
  }
  
  // Minutes
  if (now.minute() < 10) {
    minute = "0" + minute;
  }
  
  Serial.println(day + "/" + month + "/" + year + " " + hour + ":" + minute);
  Lcd.setCursor(0, 1);
  Lcd.print(day);
  Lcd.print("/");
  Lcd.print(month);
  Lcd.print("/");
  Lcd.print(year);
  Lcd.print(" ");
  Lcd.print(hour);
  Lcd.print(":");
  Lcd.print(minute);
}
