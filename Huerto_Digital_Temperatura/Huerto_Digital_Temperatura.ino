/*

HUERTO DIGITAL

  - TEMPERATURE SENSOR
  - HUMIDITY SENSOR
  - LIGHT SENSOR
  - LCD
  - LEDS

*/

// DHT: temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

// Photoresistor: light sensor
const int photoresistorPin = A0;
int threshold = 500; // lectura analógica (0-1023) por debajo de la cual está oscuro

// Potentiometer
int potentiometer = A1;

// Normal LEDs
int RedLed = 13;
int YellowLed = 12;

// RGB LED
int RedPin = 9;
int GreenPin = 10;
int BluePin = 11;

void setup(){

  Serial.begin(9600); // baud rate

  Serial.println("Medidas para nuestras plantitas!");

  pinMode(RedLed, OUTPUT); // LED rojo
  pinMode(YellowLed, OUTPUT); // LED amarillo

  // RGB LED
  pinMode(RedPin, OUTPUT); // R
  pinMode(GreenPin, OUTPUT); // G
  pinMode(BluePin, OUTPUT); // B

  dht.begin();

  lcd.begin(16, 2); // activamos todos los cuadros del LCD
  lcd.clear(); // lo reseteamos cada vez que iniciemos la máquina

  // Fila 0 (cabeceras), alineadas con los valores de la fila 1:
  // col: 0123456789012345
  //      TEMP  HUM  LUM
  //      23C    45%  78%
  lcd.setCursor(0,0);
  lcd.print("TEMP");
  lcd.setCursor(6,0);
  lcd.print("HUM");
  lcd.setCursor(11,0);
  lcd.print("LUM");
}


void loop(){

  int light = analogRead(photoresistorPin); // 0-1023
  int lum = map(light, 0, 1023, 0, 100);    // 0-100 %

  // Leemos como float: si la lectura falla, la librería devuelve NaN
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // en Celsius

  // Check if any reads failed and try again after a pause.
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0,1);
    lcd.print("Error sensor DHT");
    delay(2000);
    return;
  }

  // Serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum, 0);
  Serial.print(" %  ");
  Serial.print("Temperature: ");
  Serial.print(temp, 0);
  Serial.print(" C ");
  Serial.print(" Luminosity: ");
  Serial.print(lum);
  Serial.println(" %");

  // LCD: escribimos la fila 1 completa (16 caracteres) con anchos fijos,
  // así no quedan restos de valores anteriores en la pantalla.
  // El DHT11 mide en grados enteros, así que mostramos la temperatura sin decimales.
  char line[17];
  snprintf(line, sizeof(line), "%2ldC   %3ld%% %3d%% ", lround(temp), lround(hum), lum);
  lcd.setCursor(0,1);
  lcd.print(line);

  // Conditions temperature
  digitalWrite(YellowLed, temp >= 25 ? HIGH : LOW); // LEDs amarillos a partir de 25 ºC
  digitalWrite(RedLed,    temp >= 28 ? HIGH : LOW); // LEDs rojos a partir de 28 ºC

  if (light < threshold) {        // if it's dark turn the LED on
    cyan();
  }
  else {                          // if it isn't dark turn the LED off
    turnOff();                    // call the turn off function
  }

  delay(2000); // tiempo entre medidas (2s)

}

void red () {

  //set the LED pins to values that make red
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}
void orange () {

  //set the LED pins to values that make orange
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 50);
  analogWrite(BluePin, 0);
}
void yellow () {

  //set the LED pins to values that make yellow
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 0);
}
void green () {

  //set the LED pins to values that make green
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 0);
}
void cyan () {

  //set the LED pins to values that make cyan
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 100);
  analogWrite(BluePin, 100);
}
void blue () {

  //set the LED pins to values that make blue
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 100);
}
void magenta () {

  //set the LED pins to values that make magenta
  analogWrite(RedPin, 100);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 100);
}
void turnOff () {

  //set all three LED pins to 0 or OFF
  analogWrite(RedPin, 0);
  analogWrite(GreenPin, 0);
  analogWrite(BluePin, 0);
}
