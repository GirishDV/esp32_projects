
#include "ThingSpeak.h"
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 27      // digital of your ESP32 connected to DHT11
#define DHTTYPE DHT11 // exact model of temperature sensor DHT 11 for the general library
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Aibot";                            // your network SSID (name) 
char pass[] = "2a2a21c10366584";                  // your network password
int keyIndex = 0;                                 // your network key Index number (needed only for WEP)
unsigned long myChannelNumber = 1003424;
const char * myWriteAPIKey = "2QT5VXM9TTA03AMU";

WiFiClient  client;

void setup() {
  Serial.begin(9600);    //Initialize serial
  
    WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);          // Initialize ThingSpeak
}     

void loop() {

  /*------------------------------Connect or reconnect to WiFi-----------------------*/
 {
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);        // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
 }
  /* -----------------------------DHT11----------------------------*/
  delay(10000);
   // Let the library get the Temperature that DHT11 probe is measuring.
   float t = dht.readTemperature(); 
   if (isnan(t))
       t=-1; // if so, check the connection of your DHT11 sensor... something is disconnected ;-)
   float h = dht.readHumidity();
   if (isnan(h))
       h=-1; // if so, check the connection of your DHT11 sensor... something is disconnected ;-)
   // Show values, just for debuging
   Serial.println(""); Serial.print("Temperature: "); Serial.println(t); Serial.print("Humidity: "); Serial.println(h);

  /*----------------------------- Write to ThingSpeak--------------------------*/
 {
  ThingSpeak.writeField(myChannelNumber,1, t, myWriteAPIKey);
  int x = ThingSpeak.writeField(myChannelNumber,2, h, myWriteAPIKey);
  
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(15000); // Wait 15 seconds to update the channel again
 }
}
