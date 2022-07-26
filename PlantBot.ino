#include <Adafruit_TSL2591.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>
#include <EasyBuzzer.h>
#include <Wire.h>
int show=0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int moistPin = 0;
int moistVal = 0;
int tooDry = 150;
int tooWet = 400;
int tooDark = 50;
int leftLEDPin;
int rightLEDPin;
int motorLPin = 12;
int brakeLPin = 9;
int speedLPin = 3;
int motorRPin = 13;
int brakeRPin = 8;
int speedRPin = 11;
int buzzerPin = 1;  
int lightSensorPin;
bool bad = false;
bool good = true;
bool nightTime = false;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
sensors_event_t event;

void setup() {
  Serial.begin(9600);
  EasyBuzzer.setPin(buzzerPin);
  lcd.setCursor(0, 0);
  lcd.print("    PlantBot    "); 
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Init."); 
  delay(300);
  lcd.setCursor(0, 1);
  lcd.print("Init..."); 
  delay(600);
  lcd.setCursor(0, 1);
  lcd.print("Init........"); 
  delay(700);
  lcd.setCursor(0, 1);
  lcd.print("Init...............");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  pinMode(motorLPin, OUTPUT);
  pinMode(brakeLPin, OUTPUT);
  pinMode(motorRPin, OUTPUT);
  pinMode(brakeRPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(speedLPin, OUTPUT);
  pinMode(speedRPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(lightSensorPin, INPUT);
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);
}

void loop() { 
  analogWrite(leftLEDPin, HIGH);
  analogWrite(rightLEDPin, HIGH);
  EasyBuzzer.update(); 
  lcd.clear();
  moistVal = analogRead(moistPin);
  Serial.println(moistVal);
  int percent = 2.718282 * 2.718282 * (.008985 * moistVal + 0.207762);
  /*Serial.print(percent);
  Serial.println("% Moisture ");*/
  tsl.getEvent(&event);
  float value = event.light;
  Serial.print(value);
  Serial.println(" lux");
  if (!nightTime && value <=tooDark){
    setTime(0,0,0,1,1,2016);
    nightTime = true;
  }
  if (nightTime && value > tooDark)
    nightTime = false;
  if (moistVal <= tooDry || moistVal >= tooWet) {
    if (value>=tooDark){
      lcd.setCursor(0,0);
      lcd.print(" ________  ");
      lcd.setCursor(1,0);
      lcd.print(" |       |    ");
      if (!bad){
        digitalWrite(motorLPin, HIGH); 
        digitalWrite(brakeLPin, LOW); 
        digitalWrite(motorLPin, LOW); 
        digitalWrite(brakeLPin, LOW);
        analogWrite(speedLPin, 122);
        analogWrite(speedRPin, 122);
        delay(250);
        digitalWrite(motorLPin, LOW); 
        digitalWrite(brakeLPin, LOW); 
        digitalWrite(motorLPin, HIGH); 
        digitalWrite(brakeLPin, LOW);
        analogWrite(speedLPin, 122);
        analogWrite(speedRPin, 122);
        delay(500);
        digitalWrite(motorLPin, HIGH); 
        digitalWrite(brakeLPin, LOW); 
        digitalWrite(motorLPin, LOW); 
        digitalWrite(brakeLPin, LOW);
        analogWrite(speedLPin, 122);
        analogWrite(speedRPin, 122);
        delay(250);
        digitalWrite(brakeLPin, HIGH);
        digitalWrite(brakeRPin, HIGH);
        good = false;
        bad = true;
      }
    }
    else{
      lcd.setCursor(1,0);
      lcd.print(" ________  ");
    }
  }
  else {
     if (value <= tooDark){
      lcd.setCursor(1,0);
      lcd.print(" ________  ");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print(" |        |  ");
      lcd.setCursor(1,0);
      lcd.print("  ________  ");
      if (!good){
        digitalWrite(motorLPin, HIGH); 
        digitalWrite(brakeLPin, LOW); 
        digitalWrite(motorLPin, LOW); 
        digitalWrite(brakeLPin, LOW);
        analogWrite(speedLPin, 122);
        analogWrite(speedRPin, 122);
        delay(2000);
        digitalWrite(brakeLPin, HIGH);
        digitalWrite(brakeRPin, HIGH);
        good = true;
        bad = false;
    }

    }
 }
 if (moistVal >= tooWet)
  EasyBuzzer.beep(
            3000,
            500,
            300,
            3,
            3000,
            3
        );
  else if (moistVal <= tooDry)
    EasyBuzzer.beep(
            3000,
            500,
            500,
            5,
            2000,
            5
          );
  else if (value <= tooDark && hour()>=16)
    EasyBuzzer.beep(
            3000,
            500,
            300,
            1,
            2000,
            5
          );
  delay(100);
}
