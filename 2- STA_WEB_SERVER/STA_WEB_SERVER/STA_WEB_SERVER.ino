/*
  STA NodeMCU WEB SERVER - Conexión en modo Station con un punto de acceso (router).
  Crea una conexión del NodeMCU en modo Station con un punto de acceso que permite
  encender y apagar un LED conectado a la salida D4 del módulo NodeMCU. Y leer temperatura y humedad con un DHT11
*/

#include <ESP8266WiFi.h> //Incluye la biblioteca ESP8266WiFi
#include "DHTesp.h" //Incluye la biblioteca DHT

DHTesp dht;
const char ssid[] = "rednueva";        //Indicamos el nombre de la red WiFi (SSID) a la que queremos conectarnos.
const char password[] = "redclases";    //Indicamos la contraseña de de red WiFi

WiFiServer server(80); // Creamos el server y definimos el puerto de comunicaciones


IPAddress ip(192,168,0,11);  //Creamos un objeto IP, la ip donde queremos el server   
IPAddress gateway(192,168,0,1); // IP del Router
IPAddress subnet(255,255,255,0);   

void setup() {
  Serial.begin(9600);
  pinMode(D4, OUTPUT); //Inicializamos el D4 como salida
  digitalWrite(D4, LOW); //Dejamos inicialmente el D4 en LOW
  dht.setup(D2, DHTesp::DHT11); // DHT11 sensor en el GPIO 12
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin(ssid, password); //Inicializamos  la conexión del NodeMCU con la red WiFi
  WiFi.config(ip, gateway, subnet); //Fijamos la ip con la cual debe aparecer en la red
  delay(50);
  Serial.println("\nConectando a la red:");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) { // Verifica el estado de la conexión del NodeMCU cada 0.3s hasta que conecta
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada");       // Indica que el NodeMCU conectado con la red WiFi
  server.begin();                         // Inicia el NodeMCU en modo Station
  Serial.println("Servidor inicializado");
  Serial.print("\nUtiliza esta ip para conectarte: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Comprueba si el cliente ha conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Espera hasta que el cliente envía alguna petición
  Serial.println("nuevo cliente");
  while (!client.available()) {
    delay(10);
  }
  // Lee la petición
  String peticion = client.readStringUntil('\r');
  Serial.println(peticion);
  client.flush();
  // Comprueba la petición
  if (peticion.indexOf('/LED=ON') != -1) {
    digitalWrite(D4, HIGH);
    delay(1000);
  }
  if (peticion.indexOf('/LED=OFF') != -1) {
    digitalWrite(D4, LOW);
    delay(1000);
  }
  //Enciende o apaga el LED en función de la petición
  // Envía la página HTML de respuesta al cliente
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
  
  float hum = dht.getHumidity();
  float temp = dht.getTemperature();
  
  client.print("<H3> Temperatura: ");
  client.print(temp);
  client.print("°C Humedad: ");
  client.print(hum);
  client.println("% </H3>");
  client.println("</html>"); 
  delay(10);
  Serial.println("Petición finalizada");
  delay(1000);
}
