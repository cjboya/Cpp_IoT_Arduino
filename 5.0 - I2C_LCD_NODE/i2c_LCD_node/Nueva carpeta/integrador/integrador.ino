#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

// set columnas y filas
int lcdColumns = 16;
int lcdRows = 2;

//dht
DHT dht(DHTPIN, DHTTYPE);
//Armar el objeto lcd
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


void setup() {
  Serial.begin(9600);
  dht.begin();// initialize the sensor
  lcd.backlight();// turn on lcd backlight
  lcd.begin();// initialize lcd
}

void loop() {
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0); // set the cursor on the first row and column
  int h = dht.readHumidity();
  lcd.print("Humidity=");
  lcd.print(dht.readHumidity());//print the humidity
  lcd.print("%");
  lcd.setCursor(0, 1); //set the cursor on the second row and first column
  int t = dht.readTemperature();
  lcd.print("Temp=");
  lcd.print(dht.readTemperature());//print the temperature
  lcd.print("*C");
  Serial.print("Temp:");
  Serial.println(t);
  Serial.print("Hum:");
  Serial.print(h);
}
