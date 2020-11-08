/*
 * 
 *
  Mitch's Weather Box Project!
  - BMP280 (Barometric pressure & Temp Sensor)
    (https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/arduino-test)
  - Anemometer (By jostak): https://www.thingiverse.com/thing:2559929
    (Just requires hall effect sensor and a permenant magnet)
  - 1602A LCD (Using this guide: https://create.arduino.cc/projecthub/najad/interfacing-lcd1602-with-arduino-764ec4)  
 *
 *
  Arduino Hall Effect Sensor Project
    by Arvind Sanjeev
  additions to hall effect sensor project
    by Mitch Miller
 * 
 */
 
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_BMP280.h>

const int rs = 12, en = 11, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Adafruit_BMP280 bmp;

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
  setupBMP();
}
 
void loop()
{
  //Measure RPM
  if (half_revolutions >= 20) { 
    rpm = 30*1000/(millis() - timeold)*half_revolutions;
    timeold = millis();
    half_revolutions = 0;
    printRPMToLCD(rpm);
  }

  printBMPToLCD();
  delay(2000);
}

void setupLCD() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("0RPM");
}

void setupBMP() {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void printBMPToLCD() {
  int alt = bmp.readAltitude(1013.25);
  int temp = bmp.readTemperature();

  //Bottom Line
  lcd.setCursor(0,1);
  lcd.print(temp);
  lcd.print("*C");
  lcd.print(" ");

  lcd.print("Alt:");
  lcd.print(alt);
  lcd.print("m");
}

void printRPMToLCD(int rpm) {
  lcd.setCursor(0,0);
  lcd.print(rpm);
  lcd.print("RPM");
}

// This function is called whenever a magnet/interrupt is detected by the arduino 
void magnet_detect()
{
   half_revolutions++;
   //Serial.println("detect");
}
