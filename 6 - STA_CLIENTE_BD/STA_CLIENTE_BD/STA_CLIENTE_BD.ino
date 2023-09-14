/*
   Subir temperatura y humedad a una BD
   MySQL
   Usamos un servidor online Gratuito

*/


#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

int humidity;
int temperature;
int ah = 0;
int at = 0;
                      
IPAddress server_addr(37, 59, 55, 185); // IP del server MySQL


//red
char ssid[] = "rednueva";
char pass[] = "redclases";

//acceso bd
char user[] = "av..";              // MySQL user
char password[] = "ll...";          // MySQL password
char bdname[] = "av...";

//como seria la consulta 
String INSERT_SQL = "INSERT INTO clima (temperatura,humedad) VALUES (00,00)";


WiFiClient client;

MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

DHTesp dht;

void setup()
{
  Serial.begin(9600);
  dht.setup(D2, DHTesp::DHT11);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a la Red");
  pinMode(D4, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(300);
    digitalWrite(D4, LOW);
    Serial.print(".");
    delay(300);
  }
  Serial.println("Conectado a WIFI");
  delay(1000);

  if (conn.connect(server_addr, 3306, user, password,bdname)) {
    Serial.println("Conectado a base de datos");
  }
  else {
    Serial.println("FALLO");
  }

  cursor = new MySQL_Cursor(&conn);

}

void loop()
{
  delay(2000);
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  if (humidity != ah || temperature != at) {
    ah = humidity;
    at = temperature;
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print("\t Humedad: ");
    Serial.println(humidity);
    Serial.println( "INSERT INTO clima (temperatura,humedad) VALUES (" + String(temperature) + "," + String(humidity) + ")");
    INSERT_SQL = "INSERT INTO clima (temperatura,humedad) VALUES (" + String(temperature) + "," + String(humidity) + ")";
    if (conn.connected())cursor->execute(INSERT_SQL.c_str());
    delay(10000);
  }
}
