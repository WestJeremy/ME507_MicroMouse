
#include "WebsocketInterface.h"

WebsocketConnection::WebsocketConnection(const char *ssid, const char *password, String webpage, Micromouse* Micromouse)
    : ssid_(ssid), password_(password), webpage_(webpage), webSocket(socketsport),Micromouse_(Micromouse) 
{

    // The JSON library uses static memory, so this will need to be allocated:
    // -> in the video I used global variables for "doc_tx" and "doc_rx", however, I now changed this in the code to local variables instead "doc" -> Arduino documentation recomends to use local containers instead of global to prevent data corruption
    // We want to periodically send values to the clients, so we need to define an "interval" and remember the last time we sent data to the client (with "previousMillis")

    // Initialization of webserver and websocket
    WebServer server(serverport);                               // the server uses port 80 (standard port for websites
    WebSocketsServer webSocket = WebSocketsServer(socketsport); // the websocket uses port 81 (standard port for websockets
}

WebsocketConnection::WebsocketConnection(const char *ssid, const char *password, String webpage)
    : ssid_(ssid), password_(password), webpage_(webpage), webSocket(socketsport) 
{

    // The JSON library uses static memory, so this will need to be allocated:
    // -> in the video I used global variables for "doc_tx" and "doc_rx", however, I now changed this in the code to local variables instead "doc" -> Arduino documentation recomends to use local containers instead of global to prevent data corruption
    // We want to periodically send values to the clients, so we need to define an "interval" and remember the last time we sent data to the client (with "previousMillis")

    // Initialization of webserver and websocket
    WebServer server(serverport);                               // the server uses port 80 (standard port for websites
    WebSocketsServer webSocket = WebSocketsServer(socketsport); // the websocket uses port 81 (standard port for websockets
}
void WebsocketConnection::Event(byte num, WStype_t type, uint8_t *payload, size_t length)
{ // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
    switch (type)
    {                         // switch on the type of information sent
    case WStype_DISCONNECTED: // if a client is disconnected, then type == WStype_DISCONNECTED
        Serial.println("Client " + String(num) + " disconnected");
        break;
    case WStype_CONNECTED: // if a client is connected, then type == WStype_CONNECTED
        Serial.println("Client " + String(num) + " connected");
        // optionally you can add code here what to do when connected
        break;
    case WStype_TEXT: // if a client has sent data, then type == WStype_TEXT
        // try to decipher the JSON string received
        StaticJsonDocument<200> doc; // create a JSON container
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        else
        {
            // JSON string was received correctly, so information can be retrieved:
            const char *g_brand = doc["brand"];
            const char *g_type = doc["type"];
            const int g_year = doc["year"];
            const char *g_color = doc["color"];
            Serial.println("Received guitar info from user: " + String(num));
            Serial.println("Brand: " + String(g_brand));
            Serial.println("Type: " + String(g_type));
            Serial.println("Year: " + String(g_year));
            Serial.println("Color: " + String(g_color));
        }
        Serial.println("");
        break;
    }
}

void WebsocketConnection::Start()
{
    WiFi.begin(ssid_, password_);                                                  // start WiFi interface
    Serial.println("Establishing connection to WiFi with SSID: " + String(ssid_)); // print SSID to the serial interface for debugging

    while (WiFi.status() != WL_CONNECTED)
    { // wait until WiFi is connected
        delay(1000);
        Serial.print(".");
    }
    Serial.print("Connected to network with IP address: ");
    Serial.println(WiFi.localIP()); // show IP address that the ESP32 has received from router

    server.on("/", [this]()
              { server.send(200, "text/html", webpage_); });

    server.begin(); // start server

    webSocket.begin(); // start websocket
    webSocket.onEvent(std::bind(&WebsocketConnection::Event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void WebsocketConnection::task()
{
    for (;;)
    {
        server.handleClient(); // Needed for the webserver to handle all clients
        webSocket.loop();      // Update function for the webSockets

        unsigned long now = millis(); // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
        if ((unsigned long)(now - previousMillis) > interval)
        { // check if "interval" ms has passed since last time the clients were updated

            String jsonString = "";                   // create a JSON string for sending data to the client
            StaticJsonDocument<200> doc;              // create a JSON container
            JsonObject object = doc.to<JsonObject>(); // create a JSON Object
            object["rand1"] = random(100);            // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
            object["rand2"] = random(100);
            serializeJson(doc, jsonString);     // convert JSON object to string
            Serial.println(jsonString);         // print JSON string to console for debug purposes (you can comment this out)
            webSocket.broadcastTXT(jsonString); // send JSON string to clients

            previousMillis = now; // reset previousMillis
        }
        vTaskDelay(10);
    }
}
