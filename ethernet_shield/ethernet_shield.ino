/* 2/14/2021
 * Wayne Leutwyler
 */

#include "DHT.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22

#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(000,000,000,000);   // IP of the MySQL *server* here
char user[] = "";                         // MySQL user login username
char password[] = "";                     // MySQL user login password

EthernetClient client;
MySQL_Connection conn((Client *)&client);

//define delay into hours
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac_addr);
  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop() {
  float tf = dht.readTemperature(true);
  float h = dht.readHumidity();
  float dew = (tf-(100-h)/5.0);
  float hif = dht.computeHeatIndex(tf, h);

 //connect to database
 Serial.println("Connecting...");
 if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);

 // asign values for inserting
 char INSERT_DATA[] = "INSERT INTO weather.dht (tmp,hum,dew,hif) VALUES (%s,%s,%s,%s)";
 char query[128];
 char temperature[10];
 char humidity[10];
 char dewpoint[10];
 char heatindex[10];
 int str_temperature = (tf);
 int str_humidity = (h);
 int str_dewpoint = (dew);
 int str_heatindex = (hif);

 //prepare data for inserting
 dtostrf(str_temperature, 4, 2, temperature);
 dtostrf(str_humidity, 4, 2, humidity);
 dtostrf(str_dewpoint, 4, 2, dewpoint);
 dtostrf(str_heatindex, 4, 2, heatindex);

 //send data to mysql
 sprintf(query, INSERT_DATA, temperature, humidity, dewpoint, heatindex);
 MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

 //send data to serial output for debugging remove when code works.
 Serial.println((query));
 Serial.print("Temperature: ");
 Serial.print(tf);
 Serial.println("ºF");
 Serial.print("Humidity: ");
 Serial.print(h);
 Serial.println("%");
 Serial.print("Dew Point: ");
 Serial.print(dew);
 Serial.println("ºF");
 Serial.print("Heat Index: ");
 Serial.print(hif);
 Serial.println("ºF");

 //excute query and delete cursor
 cur_mem->execute(query);
 delete cur_mem;
 Serial.println("Data recorded.");
 }
  else
    Serial.println("Connection failed.");
  //close mysql connection
  conn.close();
  delay (2*HOUR);
}
