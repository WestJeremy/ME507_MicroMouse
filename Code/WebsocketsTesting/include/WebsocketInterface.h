/** @file WebsocketInterface.h
 *  This includes functions for websockets communications with a micromouse.
 *
 */

#ifndef WEBSOCKETINTERFACE_H
#define WEBSOCKETINTERFACE_H

#include <Arduino.h>
#include <WiFi.h>                                     // needed to connect to WiFi
#include <WebServer.h>                                // needed to create a simple webserver (make sure tools -> board is set to ESP32, otherwise you will get a "WebServer.h: No such file or directory" error)
#include <WebSocketsServer.h>                         // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>                              // needed for JSON encapsulation (send multiple variables with one string)
#include "Micromouse.h"

/**    functions
 *  
*/
class WebsocketConnection {
public:
    WebsocketConnection(const char* ssid,const char* password,String webpage);
    WebsocketConnection(const char* ssid,const char* password,String webpage,Micromouse* Micromouse);

    void Event(byte num, WStype_t type, uint8_t * payload, size_t length);

    void task();

    void Start();



private:
    const char* ssid_;
    const char* password_;
    String webpage_;
    int serverport=80;
    int socketsport=81;

    int interval = 1000;                                  // send data to the client every 1000ms -> 1s
    unsigned long previousMillis = 0;                     // we use the "millis()" command for time reference and this will output an unsigned long

    WebServer server;                                 // the server uses port 80 (standard port for websites
    WebSocketsServer webSocket; 

    Micromouse* Micromouse_;

};




#endif