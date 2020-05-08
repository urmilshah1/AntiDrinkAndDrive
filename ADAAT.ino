#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h> // This library allows for seial communication on other digital
                            // pins of the Arduino (rather than just using pins 0 and 1)
                            // note: Arduino UART has a 64 byte serial buffer!
                            
#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal.h>   // This library allows for use of LCD.                             

int TIME_UNTIL_WARMUP = 10;
unsigned long time;
int analogPin = 0;
int val = 0;
int pos =0; 
int redLED = 13;
int yellowLED = 6;
int greenLED = 7;
int reset = 12, enable = 11, lcdD4 = 5, lcdD5 = 4, lcdD6 = 3, lcdD7 = 2; 
LiquidCrystal LCDboard(reset, enable, lcdD4, lcdD5, lcdD6, lcdD7); 


Servo myservo1;
Servo myservo2;

 

void setup()   {                
Serial.begin(115200);//sets the baud rate
 myservo1.attach(9);
 myservo2.attach(10);
 pinMode(redLED,OUTPUT);
 pinMode(yellowLED,OUTPUT);
 pinMode(greenLED,OUTPUT);
}
void loop() {  
  
  delay(100);
  val = readAlcohol();
  printTitle();
  printWarming();
  time = millis()/1000;
  
  if(time<=TIME_UNTIL_WARMUP)
  {
    time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    Serial.print(time);
  }else
  {
     printTitle();
     printAlcohol(val);
     printAlcoholLevel(val);  
  }
}
void printTitle()
{
  Serial.println("Breath Analyzer");
       
     LCDboard.print("Breath Analyzer");
     LCDboard.noBlink();
     delay(1000);
     LCDboard.clear();
     digitalWrite(yellowLED, HIGH);
}
void printWarming()
{
  Serial.println("Warming Up");
   LCDboard.blink(); 
     delay(100);
     LCDboard.noBlink();  
     LCDboard.print("Warming Up");
     LCDboard.clear();
}

void printAlcohol(int value)
{
  Serial.println(val);
}
void printAlcoholLevel(int value)
{
 for(pos = 0; pos >= 0; pos -= 1)  // stays at 0 degrees
  myservo1.write(pos);// tell servo to go to position in variable 'pos'
  myservo2.write(pos);
 
  if(value<35)
  {
      Serial.print("You are sober.");
      LCDboard.noBlink();  
      LCDboard.print("You are sober.");
       for(pos = 0; pos <= 120; pos += 1)  // stays at 0 degrees
       myservo1.write(pos);// tell servo to go to position in variable 'pos'
       myservo2.write(pos);
        digitalWrite(greenLED,HIGH);
        digitalWrite(yellowLED, LOW);
        delay(10000);
        digitalWrite(greenLED, LOW);
       LCDboard.clear(); // waits 10sec for the servo to reach the position
  }
  if (value>=200 && value<250)
  {
      Serial.print("You had a drink.");
      delay(20);
      LCDboard.print("You had a drink");
      digitalWrite(redLED,HIGH);
      digitalWrite(yellowLED, LOW);
     delay(10000);
     digitalWrite(redLED,LOW);
  }
  if(value>251)
  {
     Serial.println("You are drunk!");
     LCDboard.noBlink();  
     LCDboard.print("You are drunk.");
     LCDboard.setCursor(0,2);
     delay(1000);
     LCDboard.print("You are not fit to drive");
     delay(1000);
     LCDboard.clear();
     LCDboard.setCursor(0,2);
     LCDboard.print("Book an Uber");
      digitalWrite(redLED,HIGH);
      digitalWrite(yellowLED, LOW);
     delay(10000);
     digitalWrite(redLED,LOW);
     
  }
 }
 
 int readAlcohol()
 {
  int val = 0;
  int val1;
  int val2;
  int val3;  
  val1 = analogRead(analogPin); 
  delay(10);
  val2 = analogRead(analogPin); 
  delay(10);
  val3 = analogRead(analogPin);
  
  val = (val1+val2+val3)/3;
  return val;
 }
