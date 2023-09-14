/*
 Conectividad wifi: NodeMCU como Access Point 
 Comportamiento: Servidor Web
 
 Crea un servidor Web en modo Access Point que permite encender y apagar un LED conectado a la salida D4 NodeMCU.
 Y leer la temperatura y humedad
 */
 
#include <ESP8266WiFi.h> //Incluye la librería ESP8266WiFi
#include "DHTesp.h" //Incluye la librería DHT

const char ssid[] = "nodemcu"; //Definimos la SSDI de nuestro servidor WiFi -nombre de red- 
const char password[] = "12345678"; //Definimos la contraseña de nuestro servidor 


WiFiServer server(80); //Creamos el server y definimos el puerto de comunicaciones, puerto 80 web.

DHTesp dht; //Creamos el objeto DHT

void setup() {
  Serial.begin(9600);
  pinMode(D4, OUTPUT); //Inicializamos el pin D4 como salida
  digitalWrite(D4, LOW); //Inicializamos el D4 en LOW
  dht.setup(D2, DHTesp::DHT11); //  DHT11 sensor en GPIO 12 (Ver pinOUT)
  server.begin(); //Inicializamos el servidor
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password); //Nombre de Red visible, con clave.
  Serial.println();
  Serial.print("Direccion IP Access Point - por defecto: ");//Imprime la dirección IP en el monitor serial
  Serial.println(WiFi.softAPIP()); 
}


void loop() 
{
  // Crea un cliente y con el server revisa si hay alguno
  WiFiClient client = server.available();  
  if (!client) { //Si no hay cliente dispara un return y vuelve al principio del loop
    return;
  }
  Serial.println("nuevo cliente");
  while(!client.available()){ // Espera hasta que el cliente envía alguna petición
    delay(10);
  }
  String peticion = client.readStringUntil('\r');
  Serial.println(peticion);
  client.flush();
  // Comprueba la petición .indexOf
  if (peticion.indexOf('/LED=ON') != -1) {
    digitalWrite(D4, HIGH);
    delay(1000);
  } 
  if (peticion.indexOf('/LED=OFF') != -1){
    digitalWrite(D4, LOW);
    delay(1000);
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<meta charset='UTF-8'>");
  client.println("<html>");
  client.println("<head><title>Node AP SERVER </title></head>");
  client.print("<h1>El LED está ahora: ");                 
  if(digitalRead(D4) == HIGH) {
    client.print("ENCENDIDO</h1>");  
  } else {
    client.print("APAGADO</h1>");
  }
  client.println("<br>");
  
  client.println("<button type='button' onClick=location.href='/LED=ON'> ENCENDER </button>");
  client.println("<button type='button' onClick=location.href='/LED=OFF'> APAGAR </button><br><br>");
  
  int hum = dht.getHumidity();
  int temp = dht.getTemperature();
  
  client.print("<h3> Temperatura: ");
  client.print(temp);
  client.print("°C Humedad: ");
  client.print(hum);
  client.println("% </h3>");
  client.println("</html>"); 
  delay(10);
  Serial.println("Petición finalizada");
  delay(1000);
}
