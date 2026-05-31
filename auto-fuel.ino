#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>

#include "lib/Ticker.h"

#define WIFI_MODE ACCESS_POINT

LCD_1602_RUS lcd(0x27, 16, 2, 2); //задаем адрес экрана 0x27, 16 символов, 2 строки
ESP8266WebServer server(80); //сервер на пору 80

volatile unsigned long seconds = 0;
unsigned short time_p = 0;
bool actives = false;

#include "wifi.h"

void onTick() { // подсчет сикунд
  if (actives) 
    seconds++; 
}

Ticker ticker(onTick, 1000);

void setup() {
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D1, OUTPUT);
  
  Serial.begin(115200);
  
  Wire.begin(D6, D7);
  lcd.init(); // Инициализируем экран включаем подсветку
  lcd.backlight();
  lcd.clear(); // Очистка дисплея
  lcd.setCursor(0, 0); // Установка курсора в левый верхний

  ticker.start();
  lcd.setCursor(0, 1);

  int status_c = 1;

  if (WIFI_MODE == STA_CONNECT) {
    lcd.print("подкл. к WiFi...");
    IPAddress staticIP(192, 168, 0, 150);
    status_c = connect_wifi("TP-Link_6", "112398111");

  } else if (WIFI_MODE == ACCESS_POINT) {
    lcd.print("запуск wifi...");
    IPAddress staticIP(192, 168, 0, 150);
    status_c = creat_access_point("авто_полив", "00000000");
  }
  
  lcd.clear();
  lcd.setCursor(0, 1);
  
  lcd.print("запуск сервера");
  rout_server();

  lcd.clear();
  lcd.setCursor(0, 1);
  if (status_c == 0) {
    lcd.print("успешно !");
  }
  delay(1000);
  lcd.clear();
}

void loop() {
  ticker.update();
  server.handleClient();
  
  lcd.setCursor(0, 0);
  lcd.print("время: " + String(time_p));
  
  if (!digitalRead(D4)){
        time_p++;
        delay(25);
      }
      
  if (!digitalRead(D5)){
        if (time_p > 0){ 
          time_p--;
          delay(25);
        }
   }

  if (!digitalRead(D2)){
     if (actives)
         actives = false;
     else
        actives = true;
     seconds = 0;
     delay(35);
   }

  if (actives) {
    if (seconds >= time_p * 60){
      actives = false;
      seconds = 0;
    }
    digitalWrite(D1, true);
    lcd.setCursor(0, 1);
    lcd.print("включено ");
  } else {
    digitalWrite(D1, false);
    lcd.setCursor(0, 1);
    lcd.print("выключено");
  }

      
   delay(10);
   lcd.setCursor(0, 0);
   lcd.print("               ");
 }
