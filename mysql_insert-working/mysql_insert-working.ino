/* 2/14/2021
 * Wayne Leutwyler
 */

#include <dht11.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

dht11 DHT11;
#define DHT11PIN A1

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_addr(000,000,0,00);  // IP of the MySQL *server* here
char user[] = "userID";              // MySQL user login username
char password[] = "passwd";        // MySQL user login password

EthernetClient client;
MySQL_Connection conn((Client *)&client);

//define delay into hours
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac_addr); 
}

void loop() {
 // check that DHT sensor is online
 int chk = DHT11.read(DHT11PIN);

 //connect to database
 Serial.println("Connecting...");
 if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);

 // asign values for inserting
 char INSERT_DATA[] = "INSERT INTO weather.basement (tmp,hum) VALUES (%s,%s)";
 char query[128];
 char temperature[10];
 char humidity[10];
 int str_temperature = (float(DHT11.temperature*1.8+32));
 int str_humidity = (float(DHT11.humidity));
    
 //prepare data for inserting
 dtostrf(str_temperature, 4, 2, temperature);
 dtostrf(str_humidity, 4, 2, humidity);
    
 //send data to mysql
 sprintf(query, INSERT_DATA, temperature, humidity);
 MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

 //send data to serial output for debugging remove when code works.
 Serial.println((query));
 Serial.println(DHT11.temperature*1.8+32);
 Serial.println(DHT11.humidity);

 //excute query and delete cursor
 cur_mem->execute(query);
 delete cur_mem;
 Serial.println("Data recorded.");
 }
  else
    Serial.println("Connection failed.");
  conn.close();
  delay (2*HOUR);
}
