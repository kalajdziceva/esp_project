#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <CurtainControl.h>
#include <iostream>


const char* ssid     = "RalphHome";
const char* password = "Ralph123";

CurtainControl curtain(4,5); 
//serial.available serial.read compare value of the byte 

AsyncWebServer server(80);



void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();


// Set GPIO 5 as output
pinMode(5, OUTPUT);

//Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println();
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  pinMode(BUILTIN_LED,OUTPUT);
  digitalWrite(BUILTIN_LED,HIGH);

  //handler za led

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/LED", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;

    data = json.as<JsonObject>();

    String response;
    // serializeJson(data, response);
    //request->send(200, "application/json", response);
    Serial.println(response);

    String value = data["command"];
    if (value) {
      Serial.println(value);

      if (value == "ON") {
          digitalWrite(BUILTIN_LED, LOW);
      } else if (value = "OFF") {
          digitalWrite(BUILTIN_LED, HIGH);
      } else {

      }
    }

  });

  //handler za down i up

  AsyncCallbackJsonWebHandler *handler1 = new AsyncCallbackJsonWebHandler("/GPIO", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;

    data = json.as<JsonObject>();

    String response;
    // serializeJson(data, response);
      //request->send(200, "application/json", response);
      Serial.println(response);

    String value = data["command"];
    if (value) {
      Serial.println(value);

      if (value == "DOWN") {
          curtain.move_down1();
          //Serial.println(digitalRead(4));
      } else if (value = "UP") {
        curtain.move_up1();
          //digitalWrite(5, HIGH);
          //Serial.println(digitalRead(5));
      } else {

      }

    }
  });

    //forma
  
  AsyncCallbackJsonWebHandler *handler2 = new AsyncCallbackJsonWebHandler("/MOVETO", [](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data;

    data = json.as<JsonObject>();

    String response;
    serializeJson(data, response);
      //request->send(200, "application/json", response);
     Serial.println(response);

    String value = data["position"];
     // Serial.println(value);

    curtain.move_to(value.toInt()); 

  });
  server.addHandler(handler);
  server.addHandler(handler1);
  server.addHandler(handler2);

/*
  server.on("/LED", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<100> data;
    if (request->hasParam("command"))
    {
      data["command"] = request->getParam("command")->value();
      String value = data["command"];
        Serial.print(value);
    }
    else {
      data["command"] = "No message parameter";
    }
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);

      //request->send(200, "application/json", "{\"LEDSTATUS\":\"Welcome\"}");
    });
*/
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();

  Serial.println("HTTP server started");

Serial.println( "Type commands: [up - u, down - d, stop - s, info - i]"); 

}
    
   // int  pos=0;
    
int set_pos() {
  
    int pos;
    Serial.println("unesi poziciju");
    pos=Serial.read();  Serial.println("pozicija");Serial.println(pos);
  

    // 0 - 18000

    return pos;
}
char cmd;
void loop() {
    curtain.loop1();

cmd=Serial.read();

        switch (cmd) {
            case 'u':
                curtain.move_up1();
                break;
            case 'd':
                curtain.move_down1();
                break;
            case 's':
                curtain.disable();
                break;
            case 'e':
                curtain.enable();
                break;
            case 'i':
                curtain.trigger_write();
            break;
            /*case 'm':
              { curtain.move_to(set_pos());}
           break;*/

        }


}