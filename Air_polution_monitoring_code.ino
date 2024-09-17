#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Airquality";   // your network SSID (name) 
const char* password = "123456789";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 2595762;
const char * myWriteAPIKey = "R8AJGKIH44AVOE0B";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

void Connect_wifi();
int Buzzer=15;
char ch;
int CH4_val;
int PM_val;
int CO_val;

void setup() 
{
  pinMode(Buzzer,OUTPUT);
  digitalWrite(Buzzer,LOW);
  Serial.begin(9600);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Air Pollution");
  lcd.setCursor(0, 1);
  lcd.print("  Monitoring..");
  delay(1000);
  Connect_wifi();
   Serial.print("Air Pollution Monitoring...");
}
void Connect_wifi()
{
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() 
{
  
 CH4_Read();
 PM_Read();
 CO_Read();

 lcd.clear();
 lcd.print("CH4:"+String(CH4_val));
 lcd.setCursor(8,0);
 lcd.print("PM:"+String(PM_val));
 lcd.setCursor(0,1);
 lcd.print("CO:"+String(CO_val));
 
    ThingSpeak.setField(1, CH4_val);
    ThingSpeak.setField(2, PM_val);
    ThingSpeak.setField(3, CO_val);
   
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
      Connect_wifi();
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
//  }
} 

void CH4_Read() 
{
    CH4_val=analogRead(36);
    Serial.println("CH4:"+String( CH4_val));
    delay(1000);
    if(CH4_val>240)
    {
      Serial.println("CH4 is More");
      lcd.clear();
      lcd.print("CH4 is More");
      digitalWrite(Buzzer,HIGH);
      delay(1000);
    }
    if(CH4_val<220)
    {
      digitalWrite(Buzzer,LOW);
      delay(1000);
    }
}
void PM_Read() 
{
    PM_val=analogRead(39);
    Serial.println("PM2.5:"+String( PM_val));
    delay(1000);
    if(PM_val>2000)
    {
      Serial.println("PM is More");
      lcd.clear();
      lcd.print("PM is More");
      digitalWrite(Buzzer,HIGH);
      delay(1000);
    }
    if(PM_val<1800)
    {
      digitalWrite(Buzzer,LOW);
      delay(1000);
    }
}
void CO_Read() 
{
    CO_val=analogRead(34);
  //  CO_val=4095-CO_val;
    Serial.println("CO:"+String( CO_val));
    delay(1000);
    if(CO_val>2000)
    {
      Serial.println("CO is More");
      lcd.clear();
      lcd.print("CO is More");
      digitalWrite(Buzzer,HIGH);
      delay(1000);
    }
    if(CO_val<1800)
    {
      digitalWrite(Buzzer,LOW);
      delay(1000);
    }
}
