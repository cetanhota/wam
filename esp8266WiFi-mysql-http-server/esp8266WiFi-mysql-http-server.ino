#include <dht11.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>

dht11 DHT11;
#define DHT11_PIN D8
const char* ssid = "";
const char* password = "";

WiFiServer server(80);
WiFiClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  // config static IP
  IPAddress ip(xxx, xxx, x, xxx); // where xx is the desired IP Address
  IPAddress gateway(xxx, xxx, x, x); // set gateway to match your network
  IPAddress subnet(xxx, xxx, xxx, x); // set subnet mask to match your network
  
  WiFi.config(ip, gateway, subnet);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  int chk = DHT11.read(DHT11_PIN);
  mysql_function();
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "\r\n"
               "<!DOCTYPE HTML>"
               "<body bgcolor=#006994 text=black>"
               "</body>"
               "<html>"
               "<center><h1>Environmental Station</h1></center>"
               "<hr>"
               "<center><h2>Temperature: ");
  htmlPage += ((float)DHT11.temperature*1.8+32);
  htmlPage += ("<br>Humidity: ");
  htmlPage += ((float)DHT11.humidity);
  htmlPage += F("</h2></center></html>"
                "\r\n");
  return htmlPage;
}

void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
          break;
        }
      }
    }

    while (client.available()) {
      client.read();
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }
}

void mysql_function() {
  int chk = DHT11.read(DHT11_PIN); 
  MySQL_Connection conn((Client *)&client);
  
  IPAddress server_addr(xxx,xxx,x,x);  // IP of the MySQL *server* here
  char user[] = "weather";              // MySQL user login username
  char dbpassword[] = "weather!";        // MySQL user login password
  
  //connect to database
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, dbpassword)) {
  delay(1000);
  
  // asign values for inserting
  char INSERT_DATA[] = "INSERT INTO weather.temperature (tmp,hum) VALUES (%s,%s)";
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

  //excute query and delete cursor
  cur_mem->execute(query);
  delete cur_mem;
  Serial.println("Data recorded.");
 }
  else
    Serial.println("Connection failed.");
  conn.close();
}
