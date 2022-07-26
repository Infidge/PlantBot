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
int buzzerPin = 9;  
int lightSensorPin;
bool bad = false;
bool good = true;
bool nightTime = false;
bool beepBool = false;
int sec;
int mins;
int hours;
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
  Serial.print(percent);
  Serial.println("% Moisture ");
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
 if (!beepBool && (moistVal>=tooWet || moistVal<=tooDry || value <=tooDark)){
    beepBool = true;
 }
 if (moistVal >=tooWet && beepBool){
  EasyBuzzer.beep(
            2500,
            500,
            300,
            3,
            3000,
            2
        );
  beepBool = false;
  sec = second();
  mins = minute();
  hours = hour();
 }
  else if (moistVal <= tooDry && beepBool){
    EasyBuzzer.beep(
            2500,
            500,
            300,
            5,
            2000,
            2
          );
    beepBool = false;
    sec = second();
    mins = minute();
    hours = hour();
}
  else if (value <= tooDark && hour()>=16 && beepBool){
    EasyBuzzer.beep(
            2500,
            500,
            300,
            1,
            2000,
            5
          );
    beepBool = false;
    sec = second();
    mins = minute();
    hours = hour();
  }

  if (second()-sec>12 || (second()<sec && second()+60-sec>12)){
    beepBool = true;
  }
  
  delay(10);
}
