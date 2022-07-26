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
    
  /*if (moistVal<=tooDry || moistVal>=tooWet || (value <= tooDark && hour()>=16))
    bad=true;
  else bad=false;*/

  /*if (digitalRead(13)==1)
  bad = true;
  else bad=false;

  if (bad)EasyBuzzer.beep(
            2500,
            500,
            300,
            5,
            2000,
            2
          ); 
  else EasyBuzzer.stopBeep();*/
  /*  
  if (bad){
        digitalWrite(motor1pin1, HIGH); 
        digitalWrite(motor1pin2, LOW); 
        digitalWrite(motor2pin1, HIGH); 
        digitalWrite(motor2pin2, LOW);
        analogWrite(ENA_pin, 100);
        analogWrite(ENB_pin, 100);
        delay(1000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        digitalWrite(motor1pin1, LOW); 
        digitalWrite(motor1pin2, HIGH); 
        digitalWrite(motor2pin1, LOW); 
        digitalWrite(motor2pin2, HIGH);
        analogWrite(ENA_pin, 100);
        analogWrite(ENB_pin, 100);
        delay(2000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        digitalWrite(motor1pin1, HIGH); 
        digitalWrite(motor1pin2, LOW); 
        digitalWrite(motor2pin1, HIGH); 
        digitalWrite(motor2pin2, LOW);
        analogWrite(ENA_pin, 100);
        analogWrite(ENB_pin, 100);
        delay(1000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        if (moistVal<=tooDry){
          EasyBuzzer.beep(3000);
        }
        else if (moistVal>=tooWet){
          EasyBuzzer.beep(2000);
        }
        else if (value<=tooDark && hour()>=16){
          EasyBuzzer.beep(2500);
        }
  }
  else EasyBuzzer.stopBeep();*/
  
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
      /*
      lcd.print(" ________  ");
      lcd.setCursor(0,1);
      lcd.print(" |       |    ");
      */
    }
    else{
      lcd.clear();
      Serial.println("neutral humidity");
      lcd.setCursor(4,0);
      lcd.write(byte(1));
      /*
      lcd.setCursor(0,1);
      lcd.print(" ________  ");
      */
    }
  }
  else {
     if (value <= tooDark && hour()>=16){
      lcd.clear();
      Serial.println("neutral light");
      lcd.setCursor(4,0);
      lcd.write(byte(1));
      /*
      lcd.setCursor(0,1);
      lcd.print(" ________  ");
      */
    }
    else if (value > tooDark){
      lcd.clear();
      Serial.println("Smile");
      lcd.setCursor(4,0);
      lcd.write(byte(0));
      /*
      lcd.setCursor(0,0);
      lcd.print(" |        |  ");
      lcd.setCursor(0,1);
      lcd.print("  ________  ");
      */
      if (!good){
        /*digitalWrite(motor1pin1, HIGH); 
        digitalWrite(motor1pin2, LOW); 
        digitalWrite(motor2pin1, HIGH); 
        digitalWrite(motor2pin2, LOW);
        analogWrite(ENA_pin, speeds);
        analogWrite(ENB_pin, speeds);
        delay(3000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        good = true;
        bad = false;*/
    }

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

  /*if (second()-sec>12 || (second()<sec && second()+60-sec>12)){
    beepBool = true;
    EasyBuzzer.stopBeep();
    Serial.println("stop buzzer");
  }*/
  delay(100);
}

/*void wiggle (){
        digitalWrite(motor1pin1, HIGH); 
        digitalWrite(motor1pin2, LOW); 
        digitalWrite(motor2pin1, HIGH); 
        digitalWrite(motor2pin2, LOW);
        analogWrite(ENA_pin, speeds);
        analogWrite(ENB_pin, speeds);
        delay(1000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        digitalWrite(motor1pin1, LOW); 
        digitalWrite(motor1pin2, HIGH); 
        digitalWrite(motor2pin1, LOW); 
        digitalWrite(motor2pin2, HIGH);
        analogWrite(ENA_pin, speeds);
        analogWrite(ENB_pin, speeds);
        delay(2000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
        digitalWrite(motor1pin1, HIGH); 
        digitalWrite(motor1pin2, LOW); 
        digitalWrite(motor2pin1, HIGH); 
        digitalWrite(motor2pin2, LOW);
        analogWrite(ENA_pin, speeds);
        analogWrite(ENB_pin, speeds);
        delay(1000);
        analogWrite(ENA_pin, 0);
        analogWrite(ENB_pin, 0);
}*/
