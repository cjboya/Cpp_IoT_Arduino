#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char ssid[] = "rednueva";       
const char password[] = "redclases";


void setup () {
  pinMode(D4, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print("Conectando a la red :");
    Serial.println(ssid);
  }
  Serial.print("CONECTADO a la Red: ");
  Serial.println(ssid);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi estado
    HTTPClient http;  //Objeto HTTPClient
    http.begin("http://arduinowebdomotic.000webhostapp.com/LEDstate.txt");  //La ruta del request
    int httpCode = http.GET(); //La devolucion y envio del request
    if (httpCode > 0) { //El codigo de retorno
      String payload = http.getString();   //Get payload
      Serial.println(payload);//Print payload
      int dato = payload.toInt();
      if ( dato == 1) {
        digitalWrite(D4, HIGH);
      }
      else {
        digitalWrite(D4, LOW);
      }
    }
    http.end();   //Close
  }
  delay(2000);    //Request cada 2 segundos
}
