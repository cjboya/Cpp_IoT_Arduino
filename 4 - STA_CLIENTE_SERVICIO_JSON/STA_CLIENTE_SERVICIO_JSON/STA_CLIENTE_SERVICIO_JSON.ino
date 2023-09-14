#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

const char ssid[] = "rednueva";
const char password[] = "redclases";

float temp = 0;
int hum = 0;
float pres = 0;


void setup () {
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4,HIGH);
    delay(400);
    digitalWrite(D4,LOW);
    Serial.print(".");
    delay(400);
    //Serial.println(ssid);
  }
  Serial.print("CONECTADO a la Red: ");
  Serial.println(ssid);
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
 
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Buenos%20Aires,AR&units=metric&appid=");//aca hay que agregar el api token  
    int httpCode = http.GET();  
    
    if (httpCode > 0) { 

      String apiclima = http.getString();  
      Serial.println(apiclima);
      JSONVar myObject = JSON.parse(apiclima); 
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
      }
      else{
        if (myObject.hasOwnProperty("main")) {
          temp = (double) myObject["main"]["temp"];
          hum = (int) myObject["main"]["humidity"];
          pres = (double) myObject["main"]["pressure"];
          Serial.print("La temperatura es: ");
          Serial.println(temp);
          Serial.print("La humedad es: ");
          Serial.println(hum);
          Serial.print("La presion es: ");
          Serial.println(pres);
        }  
      }
    }
    http.end();   
  }
  else{
    delay(5000);
    Serial.print("Sin señal de la red :");
    Serial.println(ssid);
  }
  delay(5000);   
}
