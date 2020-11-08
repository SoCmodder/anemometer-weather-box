/*
 * 
 *
  Mitch's Weather Box Project!
 *
 *
  Arduino Hall Effect Sensor Project
    by Arvind Sanjeev
  additions to hall effect sensor project
    by Mitch Miller
 * 
 */
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile byte half_revolutions;
unsigned int rpm;
unsigned long timeold;
 
void setup()
{
  Serial.begin(115200);
  //Initialize the intterrupt pin (Arduino digital pin 2)
  attachInterrupt(0, magnet_detect, RISING);
  half_revolutions = 0;
  rpm = 0;
  timeold = 0;

  setupLCD();
}
 
void loop()
{
  //Measure RPM
  if (half_revolutions >= 20) { 
    rpm = 30*1000/(millis() - timeold)*half_revolutions;
    timeold = millis();
    half_revolutions = 0;
    //Serial.println(rpm,DEC);
    //printToLCD(rpm);
  }
}

void setupLCD() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("0 RPM");
  lcd.setCursor(0,1);
  lcd.print("Weather Box"); 
}

void printToLCD(int rpm) {
  lcd.setCursor(0,0);
  lcd.print(rpm);
  lcd.print(" RPM");
  lcd.setCursor(0,1);
  lcd.print("Weather Box");  
}

// This function is called whenever a magnet/interrupt is detected by the arduino 
void magnet_detect()
{
   half_revolutions++;
   //Serial.println("detect");
}
