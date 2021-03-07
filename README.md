# Weather Arduino and MySQL (WAM)

Welcome to WAM. This project was put together to insert data directly into a MySQL Database,
without using a php based application to handle the insert of data.

## Parts List:
  - Arduino Uno or Arduino Leonardo
  - Ethernet Shield W5100 for Arduino UNO
  - Wemos D1 Mini Arduino Uno WiFi Shield for ESP8266 ESP-12E
      - Board Manger can be found here: http://arduino.esp8266.com/stable/package_esp8266com_index.json
      - Github Project: https://github.com/esp8266/Arduino


## Software:
  - Arduino IDE (https://www.arduino.cc/en/software)
  - DHT11 Sensor Library: https://github.com/adidax/dht11

## Arduino Libs:

Adafruit Sensors: (https://github.com/adafruit/Adafruit_Sensor.git)
Arduino Ethernet Library: (https://www.arduino.cc/en/Reference/Ethernet)

I prefer this library:
  - DHT11 Sensor Library: https://github.com/adidax/dht11

## Wiring Diagrams:


Ethernet Shield:

![Wiring Diagram Ethernet Shield](https://github.com/cetanhota/wam/blob/main/images/ethernet-shield.png)
 <hr>

 Wemos D1 R1
 
![Wemos D1 R1](https://github.com/cetanhota/wam/blob/main/images/wemos-d1-r1-dht.png)

# MySQL DDL:

CREATE DATABASE `weather` /*!40100 DEFAULT CHARACTER SET latin1 */;

CREATE TABLE dht (
  id int(11) NOT NULL AUTO_INCREMENT,
  tmp int(11) NOT NULL,
  hum int(11) NOT NULL,
  dew int(11) NOT NULL,
  ts timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;
