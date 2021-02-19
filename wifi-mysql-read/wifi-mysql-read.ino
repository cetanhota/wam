/* 2/14/2021
 * Wayne Leutwyler
 */

#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

const char* ssid = "";
const char* password = "";

const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;

IPAddress server_addr(0,0,0,0);  // IP of the MySQL *server* here
char user[] = "";              // MySQL user login username
char dbpassword[] = "";        // MySQL user login password

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cur = MySQL_Cursor(&conn);

char query[] = "SELECT tmp,hum from weather.basement order by ts";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);


// config static IP
  IPAddress ip(192, 168, 1, 15); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
  WiFi.config(ip, gateway, subnet);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

    if (conn.connect(server_addr, 3306, user, dbpassword)) {
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  //row_values *row = NULL;
  //long row_count = 0;

  delay(1000);
  
  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(query);

// Fetch the columns and print them
  column_names *cols = cur_mem->get_columns();
  for (int f = 0; f < cols->num_fields; f++) {
    Serial.print(cols->fields[f]->name);
    if (f < cols->num_fields-1) {
      Serial.print(',');
    }
  }
  Serial.println();
  // Read the rows and print them
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      for (int f = 0; f < cols->num_fields; f++) {
        Serial.print(row->values[f]);
        if (f < cols->num_fields-1) {
          Serial.print(',');
        }
      }
      Serial.println();
    }
  } while (row != NULL);
  // Deleting the cursor also frees up memory used
  delete cur_mem;


    // Now we close the cursor to free any memory
  cur.close();
  delay (.25*HOUR);

}
