/** 
 * @file WebsocketInterface.h
 * @brief This file includes functions for Websockets communications with a Micromouse.
 *  @author Jeremy West
 *  @date   2023-Dec-10
 */

#ifndef WEBSOCKETINTERFACE_H
#define WEBSOCKETINTERFACE_H

#include <Arduino.h>
#include <WiFi.h>           // needed to connect to WiFi
#include <WebServer.h>      // needed to create a simple webserver
#include <WebSocketsServer.h> // needed for instant communication between client and server through Websockets
#include <ArduinoJson.h>    // needed for JSON encapsulation (send multiple variables with one string)
#include "Micromouse.h"     // needed to upload all Mouse class variables to the websever 

/** @class WebsocketConnection
 *  @brief Class for managing Websockets communications with a Micromouse.
 */
class WebsocketConnection {
public:
    /**
     * @brief Constructor for WebsocketConnection class without Micromouse parameter.
     * 
     * @param ssid SSID for WiFi connection.
     * @param password WiFi password.
     * @param webpage HTML webpage content as a String.
     */
    WebsocketConnection(const char* ssid, const char* password, String webpage);

    /**
     * @brief Constructor for WebsocketConnection class with Micromouse parameter.
     * 
     * @param ssid SSID for WiFi connection.
     * @param password WiFi password.
     * @param webpage HTML webpage content as a String.
     * @param Micromouse Pointer to the Micromouse object.
     */
    WebsocketConnection(const char* ssid, const char* password, String webpage, Micromouse* Micromouse);

    /**
     * @brief Event handler for WebSockets.
     * 
     * @param num Client ID.
     * @param type Type of WebSocket event.
     * @param payload Payload data.
     * @param length Length of payload.
     */
    void Event(byte num, WStype_t type, uint8_t * payload, size_t length);

    /**
     * @brief Task function for handling clients and updating data.
     */
    void task();

    /**
     * @brief Start the webserver and websockets.
     */
    void Start();

private:
    const char* ssid_;         ///< SSID for WiFi connection.
    const char* password_;     ///< WiFi password.
    String webpage_;           ///< HTML webpage content.
    int serverport = 80;        ///< Port for the webserver.
    int socketsport = 81;       ///< Port for the websockets.
    int interval = 1000;        ///< Interval for updating data (in milliseconds).
    unsigned long previousMillis = 0; ///< Previous millis for updating data.

    WebServer server;           ///< WebServer instance.
    WebSocketsServer webSocket; ///< WebSocketsServer instance.
    Micromouse* Micromouse_;    ///< Pointer to the Micromouse object.
};

#endif
