# Collaborizm NodeMCU Temperature Sensor Gadget


## Highlights
* Made for NodeMCU (ESP8266)
* Configures WiFi authentication using a provisional configuration portal using EasyNTP library.
* Updates a DS1307 RTC by fetching Internet Time from Network Time Protocol
* Measures temperature using an LM35
* Displays time and temperature on a 16x2 I2C Lcd
* Serial debug output at 9600 baud

## Pin Connections
See breadboard prototype image.
+ **NodeMCU V3** - Chinese clone   
    * D2 = I2C SDA
    * D1 = I2C SCL
    * A0 = Analog input
    * VV = USB voltage output (+5V)
    * G  = Common GND
+ **DS1307** (I2C)    
    * Vcc = +5V
    * Gnd = Common GND
    * SDA = NodeMCU D2
    * SCL = NodeMCU D1
+ **16x2 Lcd with I2C "Backpack"**    
    * Vcc = +5V
    * GND = Common GND
    * SDA = NodeMCU D2
    * SCL = NodeMCU D1
+ **LM35 Temperature Sensor**   
    * Vs = +3.3V
    * Vout = NodeMCU A0
    * GND = Common GND