#define STA_CONNECT "STA_CONNECT"
#define ACCESS_POINT "ACCESS_POINT" 

void handleRoot() {
  String html = "<!doctype html><html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=2.0'>"
                "<meta charset='utf-8'><title>WEB управление/api</title></head><body>"
                "<h2>упровление авто поливам</h2>"
                "<p>статус насоса: " + String(actives ? "включен" : "выключен") + "</p>"
                "<p><a href=\"/toggle\">" + String(actives ? "выключить" : "включить") + "</a></p>"
                "<form action=\"/handle_time_nums\" method=\"POST\">"
                "<p><input name=\"times\" type=\"number\" placeholder=\"" + String(time_p) + "\"></p>"
                "<p><input type=\"submit\" value=\"задать время\"></p>"
                "</form>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleToggle() {
   if (actives)
      actives = false;
   else
      actives = true;
      
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "");
}

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void getStatus(){
  server.send(200, "json", "{\"led\":\""+String(actives ? true : false)+"\""+"}");
}

void handleTimeNums() {
  if (server.hasArg("times")) {
    String s = server.arg("times");
    time_p = s.toInt(); // сохраняем как int
    server.send(200, "text/plain", "");
  } else {
    server.send(400, "text/plain", "not found args num");
  }
}

int connect_wifi(const char* sta_ssid, const char* sta_password) {
   Serial.print("Connecting to WiFi");
   WiFi.begin(sta_ssid, sta_password);
   
   while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("."); 
   }
   
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP().toString()); // IP адрес
   Serial.print("WiFi status: ");
   Serial.println(WiFi.status()); 
   return 0;
}

int creat_access_point(const char* ssid, const char* password){
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP); 
    return 0;
}

void rout_server() {
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/getstatus", getStatus);
  server.on("/handle_time_nums", HTTP_POST, handleTimeNums);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started!");

}
