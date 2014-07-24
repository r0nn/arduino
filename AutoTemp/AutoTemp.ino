#include <IRremote.h>
#include "SHT1x.h"
#include "U8glib.h"

//unsigned int rawCodes_ac_24[199] = {4500,4400,600,1600,550,550,550,1650,550,1650,550,500,600,500,600,1600,550,550,550,550,550,1650,550,500,600,500,600,1600,550,1650,550,500,600,1650,550,1600,600,500,550,550,550,1650,550,1650,550,1600,600,1600,550,1650,550,500,600,1600,600,1600,600,500,550,550,550,550,550,550,550,500,600,500,600,1600,600,500,550,550,550,550,550,550,550,500,600,500,600,1600,600,500,550,1650,550,1650,550,1600,600,1600,600,1600,550,1650,550,5250,4500,4400,600,1600,600,500,550,1650,550,1650,550,500,600,500,600,1600,600,500,550,550,550,1650,550,500,600,500,600,1600,600,1600,550,550,550,1650,550,1600,600,500,600,500,600,1600,550,1650,550,1600,600,1600,600,1600,550,550,550,1650,550,1600,600,500,600,500,550,550,550,550,550,550,550,500,600,1600,600,500,600,500,550,550,550,550,550,550,550,500,600,1600,600,500,600,1600,550,1650,550,1650,550,1600,600,1600,550,1650,550};
//unsigned int rawCodes_ac_28[199] = {4450,4450,550,1600,600,500,600,1600,600,1600,550,550,550,550,550,1600,600,500,600,500,600,1600,550,550,550,550,550,1650,550,1600,600,500,600,1600,550,1650,550,500,600,500,600,1600,600,1600,550,1650,550,1650,550,1600,600,500,600,1600,550,1650,550,500,600,500,600,500,600,500,600,500,550,1650,550,500,600,500,600,550,550,500,600,500,600,500,550,550,550,550,550,1650,550,1650,550,1600,600,1600,550,1650,550,1650,550,1600,600,5250,4450,4450,550,1650,550,500,600,1650,550,1600,600,500,550,550,550,1650,550,500,600,500,600,1600,600,500,550,550,550,1650,550,1650,550,500,600,1600,600,1600,550,550,550,500,600,1650,550,1600,600,1600,550,1650,550,1650,550,500,600,1600,550,1650,550,550,550,500,600,500,600,500,600,500,600,1600,550,550,550,550,550,500,600,500,600,500,600,500,600,500,600,500,550,1650,550,1650,550,1600,600,1600,550,1650,550,1650,550,1600,600};
//unsigned int rawCodes_ac_29[199] = {4500,4400,600,1600,550,550,550,1650,550,1650,550,500,600,500,600,1600,550,550,550,550,550,1650,550,500,600,500,600,1600,550,1650,550,500,600,1650,550,1600,600,500,550,550,550,1650,550,1650,550,1600,600,1600,550,1650,550,500,600,1600,600,1600,600,500,550,550,550,550,550,550,550,500,550,1650,550,550,550,1650,550,550,550,550,550,550,550,500,600,500,550,550,550,1650,550,550,550,1650,550,1600,600,1600,600,1600,550,1650,550,5250,4500,4400,600,1600,550,550,550,1650,550,1650,550,500,600,500,600,1600,550,550,550,550,550,1650,550,500,600,500,600,1600,550,1650,550,500,600,1650,550,1600,600,500,550,550,550,1650,550,1650,550,1600,600,1600,550,1650,550,500,600,1600,600,1600,600,500,550,550,550,550,550,550,550,500,550,1650,550,550,550,1650,550,550,550,550,550,550,550,500,600,500,550,550,550,1650,550,550,550,1650,550,1600,600,1600,600,1600,550,1650,550};

IRsend irsend;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

#define dataPin 10
#define clockPin 11
SHT1x sht1x(dataPin, clockPin);

#define KEY 12
  
float temp;
float temp_f;
float humidity;

int ac_temp;
unsigned long timer;
boolean key_pressed = false;
boolean oled_sleep = false;

void display()
{ 
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr(0, 12, "Temperature:");
  u8g.setFont(u8g_font_profont22);
  u8g.setPrintPos(0, 30);
  u8g.print(temp);
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr(64, 24, "C");
  u8g.setPrintPos(76, 24);
  u8g.print(temp_f);
  u8g.drawStr(106, 24, "F");
  
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr(0, 44, "Humidity:");
  u8g.setFont(u8g_font_profont22);
  u8g.setPrintPos(0, 62);
  u8g.print(humidity);
  u8g.setFont(u8g_font_profont11);
  u8g.drawStr(64, 54, "%");
}

void controlac()
{
  if (temp > 28.3 && ac_temp != 28)
  {
    irsend.sendMidea(0xb24d9f60807fLL, 48);
    ac_temp = 28;
  }
  else if (temp < 26.4 && ac_temp != 29)
  {
    irsend.sendMidea(0xb24d9f60a05fLL, 48);
    ac_temp = 29;
  }
}

void scankey()
{
  if (digitalRead(KEY) == LOW)
  {
    delay(20);
    if (digitalRead(KEY) == LOW)
    {
      key_pressed = true;
      while (digitalRead(KEY) == LOW);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(KEY,INPUT_PULLUP);
  
  // IR send
  // irsend.sendMidea(0xb24d9f6040bfLL, 48);
  // delay(4000);
  // irsend.sendMidea(0xb24d9f60807fLL, 48);
  // delay(4000);
  // irsend.sendMidea(0xb24d9f60a05fLL, 48);
}

void loop()
{
  if (millis() - timer > 1000UL)
  {
    timer = millis();
    
    // Read values from the sensor
    temp = sht1x.readTemperatureC();
    temp_f = sht1x.readTemperatureF();
    humidity = sht1x.readHumidity();
  
    controlac();
  
    // picture loop
    u8g.firstPage();  
    do {
      display();
    } while( u8g.nextPage() );
  }
  
  scankey();
  if (key_pressed)
  {
    key_pressed = false;
    if (oled_sleep)
    {
      u8g.sleepOff();
      oled_sleep = false;
    }
    else
    {
      u8g.sleepOn();
      oled_sleep = true;
    }
  }
}
