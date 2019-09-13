#include <ESP8266WebServer.h>
#include "index.h"

//AP SSID and Password
const char *ssid = "Bhasky";
const char *password = "123qweasd";
 
ESP8266WebServer server(80); //Web server


//=======================================================================
//    index.html 
//=======================================================================
void handleRoot() {
  String s = MAIN_page; 
  
  server.send(200, "text/html", s);   //Send index(html)

  // Each time something is requested from the client with an argument...

  if ( server.hasArg("angles") || server.hasArg("funcion") ){
    if (server.argName(0) == "angles"){
      Serial.println("1");
    } else if (server.argName(0) == "funcion"){
      Serial.println("2");
    }
    Serial.println(server.arg(0));
  }
  
}
 
//===============================================================
//                  Setup
//===============================================================
void setup(void)
{
  delay(2000);
  Serial.begin(9600);

  WiFi.mode(WIFI_AP);             //Set AP
  WiFi.softAP(ssid,password);
  
  IPAddress Ip(10, 10, 10, 10);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  
  server.on("/", handleRoot);     //GET / ==> run handleRoot()
  server.begin();                 //Start web server
}
//===============================================================
//                     Loop
//===============================================================
void loop(void)
{
  server.handleClient();
}
