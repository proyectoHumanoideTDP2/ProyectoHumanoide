#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
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
  String req,req2,req3;
  
  server.send(200, "text/html", s);   //Send index(html)

  // Each time something is requested from the client with an argument...
  req = server.arg(0);
  Serial.println(req);

  req2 = server.arg("saludar");
  Serial.println(req2);

  req3 = server.argName(0);
  Serial.println(req3);


}
 
//===============================================================
//                  Setup
//===============================================================
void setup(void)
{
 // delay(2000);
  Serial.begin(9600);
  delay(10);
  Serial.println("Go...");  

  WiFi.mode(WIFI_AP);             //Set AP
  WiFi.softAP(ssid,password);
  Serial.print("Access Point bb!!!!! \"");
  
  IPAddress Ip(10, 10, 10, 10);
  IPAddress NMask(255, 255, 255, 0);
  WiFi.softAPConfig(Ip, Ip, NMask);
  
  server.on("/", handleRoot);     //GET / ==> run handleRoot()
  server.begin();                 //Start web server
  Serial.println("Server On");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
}
//===============================================================
//                     Loop
//===============================================================
void loop(void)
{
  server.handleClient();
}
