# Weather Arduino and MySQL (WAM)

Welcome to WAM. This project was put together to insert data directly into a MySQL Database,
without using a php based application to handle the insert of data.

WAM is a great way to help introduce you to several different technologies.

 1. Arduino Sketches.
 2. Arduino Circuit setup.
 3. MySQL

 These technologies will help you become more comfortable with technologies above. Note: I will
 not detail how to install MySQL. There are many sites for this a few are below:

 - [Install MySQL on Raspberry Pi from Source Code.](https://www.percona.com/community-blog/2019/08/01/how-to-build-a-percona-server-stack-on-a-raspberry-pi-3/)
 - [Setup a Raspberry Pi MYSQL Database](https://pimylifeup.com/raspberry-pi-mysql/)
 - [How To Install MySQL on Ubuntu 20.04](https://www.digitalocean.com/community/tutorials/how-to-install-mysql-on-ubuntu-20-04)

 <h1>Coming Soon:</h1>

 - Wifi with no database connections.

 - Ethernet with no database connections.

## Parts List:
  - Arduino Uno or Arduino Leonardo
  - Ethernet Shield W5100 for Arduino UNO
  - [DHT11 Sensor](https://www.amazon.com/KeeYees-Temperature-Humidity-Single-Bus-Raspberry/dp/B07V5MTQJG/ref=sr_1_4?crid=WGA2J1ZTPJB1&dchild=1&keywords=dht11+sensor&qid=1617229571&s=industrial&sprefix=DHT11%2Cindustrial%2C166&sr=1-4)
  - Wemos D1 Mini Arduino Uno WiFi Shield for ESP8266 ESP-12E
      - [Board Manger can be found here:](http://arduino.esp8266.com/stable/package_esp8266com_index.json)
      - [Github Project:](https://github.com/esp8266/Arduino)
  - [Breadboard](https://www.amazon.com/Pcs-MCIGICM-Points-Solderless-Breadboard/dp/B07PCJP9DY/ref=sr_1_2?crid=15JNDDF7ZVFJT&dchild=1&keywords=breadboard&qid=1617229330&s=industrial&sprefix=breadbboards%2Caps%2C163&sr=1-2)
  - [Jumper wires](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78/ref=sr_1_4?crid=2EXHES7CLPLS5&dchild=1&keywords=jumper+wires&qid=1617229476&s=industrial&sprefix=ju%2Cindustrial%2C176&sr=1-4)


## Software:
  - [Arduino IDE](https://www.arduino.cc/en/software)
  - [DHT11 Sensor Library:](https://github.com/adidax/dht11)

## Arduino Libs:

[Adafruit Sensors:](https://github.com/adafruit/Adafruit_Sensor.git)
[Arduino Ethernet Library:](https://www.arduino.cc/en/Reference/Ethernet)
[MySQL Connector:](https://github.com/ChuckBell/MySQL_Connector_Arduino)

## Wiring Diagrams:


Ethernet Shield:

![Wiring Diagram Ethernet Shield](https://github.com/cetanhota/wam/blob/main/images/ethernet-shield.png)
 <hr>

 Wemos D1 R1

![Wemos D1 R1](https://github.com/cetanhota/wam/blob/main/images/wemos-d1-r1-dht.png)

# MySQL DDL:

CREATE DATABASE `weather` /*!40100 DEFAULT CHARACTER SET latin1 */;

CREATE TABLE `dht` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tmp` int(11) NOT NULL,
  `hum` int(11) NOT NULL,
  `dew` int(11) NOT NULL DEFAULT '0',
  `hif` int(45) NOT NULL DEFAULT '0',
  `ts` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=64 DEFAULT CHARSET=latin1;
