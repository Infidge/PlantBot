#include <Adafruit_TSL2591.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>
#include <EasyBuzzer.h>
#include <Wire.h>
int show=0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int moistVal = 0;
int tooDry = 150;
int tooWet = 800;
int tooDark = 50;
int motor1pin1 = 1;
int motor1pin2 = 8;
int motor2pin1 = 12;
int motor2pin2 = 13;
int ENA_pin = 9;
int ENB_pin = 10;
int buzzerPin = 11;  
int speeds = 75;
bool bad = false;
bool good = true;
bool nightTime = false;
bool beepBool = false;
int sec;
byte smiley[8] = {
 B00000,
 B10001,
 B00000,
 B00000,
 B10001,
 B01110,
 B00000,
};
byte sad[8] = {
 B00000,
 B10001,
 B00000,
 B00000,
 B01110,
 B10001,
 B00000,
};

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
sensors_event_t event;

void setup() {
  lcd.createChar(0,smiley);
  lcd.createChar(1,sad);
  Serial.begin(9600);
  lcd.begin(16,1);
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
  pinMode(A0,INPUT);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(ENA_pin, OUTPUT);
  pinMode(ENB_pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  //pinMode(13,OUTPUT);
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
}

void loop() { 
  EasyBuzzer.update(); 
  moistVal = analogRead(A0);
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

  if ((moistVal <= tooDry) || (moistVal >= tooWet)) {
    
      if (!bad){
        //wiggle();
        good = false;
        bad = true;
      }
    if (value<=tooDark && hour()>=16){
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.write(byte(1));
    }
    else{
      lcd.clear();
      Serial.println("neutral humidity");
      lcd.setCursor(4,0);
      lcd.write(byte(1));
    }
  }
  else {
     if (value <= tooDark && hour()>=16){
      lcd.clear();
      Serial.println("neutral light");
      lcd.setCursor(4,0);
      lcd.write(byte(1));
    }
    else if (value > tooDark){
      lcd.clear();
      Serial.println("Smile");
      lcd.setCursor(4,0);
      lcd.write(byte(0));
    }
 }
 if (!beepBool && (moistVal>=tooWet || moistVal<=tooDry || (value <=tooDark && hour()>=16)) && (second()-sec>12 || (second()<sec && second()+60-sec>12))){
    beepBool = true;
    EasyBuzzer.stopBeep();
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
  }
  delay(100);
}
