#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

const char ssid[] = "ACA_TU_RED";
const char password[] = "ACA_TU_PASS";

float temp = 0;
int hum = 0;
float pres = 0;


void setup () {
  lcd.init();                      
  lcd.backlight();
  lcd.print("Iniciando...");
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

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Buenos%20Aires,AR&units=metric&appid=TU_API_TOKEN"); 
    
    int httpCode = http.GET();  //Send the request
    
    if (httpCode > 0) { //Check the returning code

      String apiclima = http.getString();   //Get the request response payload
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
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T:");
          lcd.print(temp);
          lcd.print("*C ");
          lcd.print("H:");
          lcd.print(hum);
          lcd.print("%");
          lcd.setCursor(2,1);
          lcd.print("P: ");
          lcd.print(pres);
          lcd.print("Hpa");
          Serial.print("La temperatura es: ");
          Serial.println(temp);
          Serial.print("La humedad es: ");
          Serial.println(hum);
          Serial.print("La presion es: ");
          Serial.println(pres);
        }  
      }
    }
    http.end();   //Close connection
  }
  else{
    delay(5000);
    Serial.print("Sin señal de la red :");
    Serial.println(ssid);
  }
  delay(5000);
}
