/*

HUERTO DIGITAL

   - TEMPERATURE SENSOR 
   - LIGHT SENSOR
   - LCD
  
  More information on the temperature sensor is available in the datasheet:
  http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Temp/TMP35_36_37.pdf

*/


#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

const int temperaturePin = 0;

int buzzerPin = 9;

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000; 

const int sensorPin = 1; 
int lightLevel, high = 0, low = 1023;

void setup()
{
  /*
  SECCIÓN DE ACTIVACIÓN 
  */
  Serial.begin(9600); // baud rate
  
  pinMode(10, OUTPUT); // LEDs amarillos
 // pinMode(11, OUTPUT); // LEDs rojos
  
 // pinMode(9, OUTPUT); // LED azul para la luminosidad
   
  pinMode(9, OUTPUT); // Buzzer 
  
  lcd.begin(16, 2); // activamos todos los cuadros del LCD
  lcd.clear(); // lo reseteamos cada vez que iniciemos la máquina
  lcd.print("Temp");
  lcd.setCursor(5,1);
  lcd.print("C");
  // lcd.setCursor(14,1);
  // lcd.print("F");
}


void loop()
{ 
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin); // 0-1023 
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;
  
  lightLevel = analogRead(sensorPin);
  manualTune();
 // analogWrite(10, 255-lightLevel);
  
  Serial.print("voltage: "); // esto es para imprimir por el monitor serial
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.print(degreesF);
  Serial.print(" lum(%): ");
  Serial.println(lightLevel*100/255);
  
  // 5 caracteres con 1 decimal (" 23.5" o "-10.5"), justo antes de la "C" (columna 5)
  char tempStr[6];
  dtostrf(degreesC, 5, 1, tempStr);
  lcd.setCursor(0,1);
  lcd.print(tempStr);
  delay(1000);
  

  if(degreesC < 25){
    digitalWrite(10, LOW);
    analogWrite(9, 0);
  }
  else { // a partir de 25 ºC: pitido + parpadeo una vez por medida
    analogWrite(9, 20);
    digitalWrite(10, HIGH);
    delay(50);
    analogWrite(9, 0);
    digitalWrite(10, LOW);
    delay(50);
    analogWrite(9, 20);
    digitalWrite(10, HIGH);
    delay(100);
    analogWrite(9, 0);
    digitalWrite(10, LOW);
    delay(50);
    analogWrite(9, 20);
    digitalWrite(10, HIGH);
    delay(100);
    analogWrite(9, 0);
    digitalWrite(10, LOW);
  }
  
}
  
  
  
  

  
   
  
  



// Arduino regula automáticamente el rango de luz 
void manualTune(){
  lightLevel = map(lightLevel, 300, 800, 0, 255);
  lightLevel = constrain(lightLevel, 0, 255);
}


// This equation converts the 0 to 1023 value that analogRead()
// returns, into a 0.0 to 5.0 value that is the true voltage
// being read at that pin.
float getVoltage(int pin){ 
  return (analogRead(pin) * 0.004882814);
}
