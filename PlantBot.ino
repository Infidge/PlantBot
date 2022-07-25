#include <LiquidCrystal.h>
#include <Wire.h>
int show=0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int moistPin = 0;
int moistVal = 0;
int tooDry = 150;
int tooWet = 400;
int tooDark = 5000;
int greenPin = 13;
int bluePin = 12;
int redPin = 8;
int motorLPin = 12;
int brakeLPin = 9;
int speedLPin = 3;
int motorRPin = 13;
int brakeRPin = 8;
int speedRPin = 11;
bool bad = false;
bool good = true;


void setup() {
  Serial.begin(9600);
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
}

void loop() {  
  lcd.clear();
  moistVal = analogRead(moistPin);
  Serial.println(moistVal);
  int value = analogRead(A0);
  Serial.println("Analog value : ");
  Serial.println(value);
  int percent = 2.718282 * 2.718282 * (.008985 * moistVal + 0.207762);
  Serial.print(percent);
  Serial.println("% Moisture ");
  if (moistVal <= tooDry || moistVal >= tooWet) {
    if (value>=tooDark){
      digitalWrite(redPin, HIGH);
      digitalWrite(bluePin, LOW);
      digitalWrite(greenPin, LOW);
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
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, HIGH);
      digitalWrite(greenPin, LOW);
      lcd.setCursor(1,0);
      lcd.print(" ________  ");
    }
  }
  else {
     if (value>=tooDark){
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, HIGH);
      digitalWrite(greenPin, LOW);
      lcd.setCursor(1,0);
      lcd.print(" ________  ");
    }
    else{
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, LOW);
      digitalWrite(greenPin, HIGH); 
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

  delay(100);

}
