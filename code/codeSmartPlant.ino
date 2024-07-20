#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
SoftwareSerial btSerial(2, 3); // RX, TX
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Configure l'adresse I2C de l'écran
unsigned long lgUpdateTime;
const int sensorPin = A0;
const int tempPin = A1; 

void setup() 
{
  lcd.init(); // Initialiser l'écran LCD
  lcd.backlight(); // Allumer le rétroéclairage
  Serial.begin(9600);
  btSerial.begin(9600);
  Serial.println("Communication Bluetooth initialisée");

  if (!bmp.begin()){
    Serial.println("pas de valeur valide trouver pour le bmp");
    while (1) {}
  }
}

int convertirEnPourcentage(int valeur) {
  int minVal = 435; // Valeur d'humidité maximale (100% d'humidité)
  int maxVal = 755; // Valeur d'humidité minimale (0% d'humidité)

  // Pour que la valeur est toujours entre minVal et maxVal
  valeur = max(minVal, min(valeur, maxVal));
  Serial.print("valeur : ");
  Serial.println(valeur);

  int pourcentage = ( (valeur - minVal ) * 100 ) / (maxVal - minVal);
  pourcentage = 100 - pourcentage;
  return pourcentage;
}

void loop()
{
  int sensorValue = analogRead(sensorPin);
  int humi = convertirEnPourcentage(sensorValue);
  int temp = bmp.readTemperature();

  // SERIAL PRINT
  Serial.print("Humi: ");
  Serial.println(sensorValue);
  Serial.print("temp: ");
  Serial.print(temp);
  Serial.println("°C");

  // LCD PRINT
  lcd.clear(); // Nettoyer l'écran avant d'afficher la nouvelle valeur
  lcd.setCursor(0, 0); // Positionner le curseur au début de la première ligne
  lcd.print("  Humidite:"); 
  lcd.print(humi); // Afficher la valeur d'humidité
  lcd.print("%");

  lcd.setCursor(0, 1); 
  lcd.print("Temperature:");
  lcd.print(temp); 
  lcd.print(((char)223)); // symbole degré en ASCII
  lcd.print("C");

  // BLUETOOTH PRINT
  btSerial.print(sensorValue);
  btSerial.print(";"); 
  btSerial.print(temp); 

  delay(3000);
  Serial.println();  //This tells App "End of Data"
                                        //= Ascii LineFeed Char Num 10
           
      
}