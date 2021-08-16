#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include "arduino_secrets.h"
#include <GUVA-S12SD.h>

#define DHTTYPE DHT11 // DHT 11
#define DHTPIN 2

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;      // the WiFi radio's status

WiFiClient client;

IPAddress server_addr(0,0,0,0);    // IP of the MySQL *server* here
char user[] = "user";                // MySQL user login username
char dbpassword[] = "passwd";         // MySQL user login password
MySQL_Connection conn((Client *)&client);

//define delay into hours
  const unsigned long SECOND = 1000;
  const unsigned long HOUR = 3600*SECOND;

GUVAS12SD uv(A0);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);

  dht.begin();

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(SECOND*10);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
}

void loop() {

  float mV = uv.read();
  float uv_index = uv.index(mV);
  float tf = dht.readTemperature(true);
  float h = dht.readHumidity();
  float dew = (tf-(100-h)/5.0);
  float hif = dht.computeHeatIndex(tf, h);
  float p = bmp.readPressure();
  float pi = (p/3386);

if (conn.connect(server_addr, 3306, user, dbpassword)) {
    delay(1000);
}
else
{
    Serial.println("Connection failed.");
}

// asign values for inserting
 char INSERT_DATA[] = "INSERT INTO weather.percona (tmp,hum,dew,hif,prs,uv) VALUES (%s,%s,%s,%s,%s,%s)";
 char query[128];
 char temperature[10];
 char humidity[10];
 char dewpoint[10];
 char heatindex[10];
 char pressure[10];
 char uvindex[10];

 float str_temperature = (tf);
 float str_humidity = (h);
 float str_dewpoint = (dew);
 float str_heatindex = (hif);
 float str_pressure = (pi);
 float str_uvindex = (uv_index);

//prepare data for inserting
 dtostrf(str_temperature, 4, 2, temperature);
 dtostrf(str_humidity, 4, 2, humidity);
 dtostrf(str_dewpoint, 4, 2, dewpoint);
 dtostrf(str_heatindex, 4, 2, heatindex);
 dtostrf(str_pressure, 4, 2, pressure);
 dtostrf(str_uvindex, 4, 2, uvindex);

//send data to mysql
 sprintf(query, INSERT_DATA, temperature, humidity, dewpoint, heatindex, pressure, uvindex);
 MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  Serial.println((query));
  Serial.println();
  Serial.print("Temperature: "); Serial.print(tf); Serial.println("ºF");
  Serial.print("Humidity: "); Serial.print(h); Serial.println("%");
  Serial.print("DewPoint: "); Serial.print(dew); Serial.println("ºF");
  Serial.print("Heat Index: "); Serial.print(hif); Serial.println("ºF");
  Serial.print("Pressure = "); Serial.print(pi); Serial.println(" inHg");
  Serial.print("UV Index = "); Serial.println(uv_index);
  Serial.println();

//excute query and delete cursor close connection
  cur_mem->execute(query);
  delete cur_mem;
  Serial.println("Data recorded.");
  conn.close();
  delay (SECOND*300);
}
