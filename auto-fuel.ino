#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <EEPROM.h>

#include "Ticker.h"
#include "unit.h"

#define WIFI_MODE STA_CONNECT // настройка WiFi; STA_CONNECT - подключение к сужествующей WiFi сети, ACCESS_POINT - запуск собственой точки доступа

LCD_1602_RUS lcd(0x27, 16, 2, 2); //задаем адрес экрана 0x27, 16 символов, 2 строки
ESP8266WebServer server(9000); //сервер на пору 9000

volatile unsigned long seconds = 0, interval = 0;
unsigned short time_p = 0, interval_time = 0;
int timer_address = 0, time_p_address = 2;
bool actives = false;
int temp;

#include "wifi.h"

void onTick() { if (actives) seconds++; } // подсчет секунд
void intervalTimer() { interval++; }


Ticker ticker(onTick, 1000);
Ticker ticker_interval(intervalTimer, 1000);

void setup() {
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D1, OUTPUT);
  
  Serial.begin(115200);
  Wire.begin(D6, D7);
  EEPROM.begin(512);  // Инициализация EEPROM с размером 512 байт

  Serial.println("Loading...");
  lcd.init(); // Инициализируем экран включаем подсветку
  lcd.backlight();
  lcd.clear(); // Очистка дисплея
  lcd.setCursor(0, 0); // Установка курсора в левый верхний

  ticker.start();
  ticker_interval.start();
  
  lcd.setCursor(0, 1);
  time_p = read_number_in_eerom(time_p_address);
  interval_time = read_number_in_eerom(timer_address);

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
  ticker_interval.update();
  server.handleClient();
    
  if (!digitalRead(D4)){
        time_p++;
        write_number_to_eerom(time_p_address, time_p);
        delay(25);
      }
      
  if (!digitalRead(D5)){
        if (time_p > 0){ 
          time_p--;
          write_number_to_eerom(time_p_address, time_p);
          delay(25);
        }
   }

  if (!digitalRead(D2)){
     actives = !actives;
     seconds = 0;
     delay(35);
   }
  
  if (actives) {
    if (seconds >= time_p * 60){
      actives = false;
      seconds = 0;
    }
    digitalWrite(D1, true);
    lcd.setCursor(12, 0);
    lcd.print("вкл ");
  } else {
    digitalWrite(D1, false);
    lcd.setCursor(12, 0);
    lcd.print("выкл");
  }

  if (interval >= interval_time * 60) {
    actives = true;
    interval = 0;
    seconds = 0;
  }

   
   delay(10);
   if (temp != time_p + interval_time) {
      lcd.setCursor(0, 0);
      lcd.print("            ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
   }
   lcd.setCursor(0, 0);
   lcd.print("время: " + String(time_p));
   //lcd.setCursor(0, 1);
   //lcd.print("интервал: " + String(interval_time));
   temp = time_p + interval_time;
 }
