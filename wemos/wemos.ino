#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
void masterbedroomLightsOn();
void masterbedroomLightsOff();
void kidsbedroomLightsOn();
void kidsbedroomLightsOff();

// Change this before you flash
const char* ssid = "*****";
const char* password = "*****";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *masterbedroom = NULL;
Switch *kidsbedroom = NULL;

void setup()
{
  Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    masterbedroom = new Switch("master bedroom light", 80, masterbedroomLightsOn, masterbedroomLightsOff);
    kidsbedroom = new Switch("kids bedroom light", 81, kidsbedroomLightsOn, kidsbedroomLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*masterbedroom);
    upnpBroadcastResponder.addDevice(*kidsbedroom);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      masterbedroom->serverLoop();
      office->serverLoop();
	 }
}

void masterbedroomLightsOn() {
    Serial.print("Switch 1 turn on ...");
}

void masterbedroomLightsOff() {
    Serial.print("Switch 1 turn off ...");
}

void kidsbedroomLightsOn() {
    Serial.print("Switch 2 turn on ...");
}

void kidsbedroomLightsOff() {
    Serial.print("Switch 2 turn off ...");
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
