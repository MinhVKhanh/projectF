#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
// #include <WiFi.h>
#include <Esp.h>
 
String apiKey = "GKME9T2K2MI0JTD2"; // Enter your Write API key from ThingSpeak
const char* ssid = "PHONG TRO";
const char* password = "phongtro85a";
const char* server = "api.thingspeak.com";
 
// #define SS 5
// #define RST 14
// #define DI0 2

 #define SS 15
 #define RST 16
 #define DI0 4
 
//#define TX_P 17
#define BAND 433E6
//#define ENCRYPT 0x78
 
String device_id;
String temperature;
String pressure;
String altitude;
String humidity;
String dewPoint;
String rainfall;
String lux;
 
WiFiClient client;
 
//void ICACHE_RAM_ATTR ISRoutine ();
 
void setup()
{
//  ESP.wdtDisable();
  delay(2000);  
  Serial.begin(115200);  
//  while (!Serial){ESP.wdtFeed();};
  Serial.println("LoRa Receiver");
  Serial.println("TEST1");
  
//  ESP.wdtEnable(2000);
  //LoRa.setTxPower(TX_P);
  //LoRa.setSyncWord(ENCRYPT);
 
   LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1){ESP.wdtFeed();};
  };
  Serial.println("TEST2");
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  Serial.println("TEST3");
 
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
 
}
 
void loop()
{
  // try to parse packet
//  ESP.wdtFeed(); // service the WDT here
//  Serial.println("TEST4");
  int pos1, pos2, pos3, pos4, pos5, pos6, pos7;
 
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // received a packet
    Serial.print("Received packet:  ");
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData);
    // read packet
    while (LoRa.available())
    {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
 
    pos1 = LoRaData.indexOf('/');
    pos2 = LoRaData.indexOf('&');
    pos3 = LoRaData.indexOf('#');
    pos4 = LoRaData.indexOf('@');
    pos5 = LoRaData.indexOf('$');
    pos6 = LoRaData.indexOf('^');
    pos7 = LoRaData.indexOf('!');
 
    device_id = LoRaData.substring(0, pos1);
    temperature = LoRaData.substring(pos1 + 1, pos2);
    pressure = LoRaData.substring(pos2 + 1, pos3);
    altitude = LoRaData.substring(pos3 + 1, pos4);
    humidity = LoRaData.substring(pos4 + 1, pos5);
    dewPoint = LoRaData.substring(pos5 + 1, pos6);
    rainfall = LoRaData.substring(pos6 + 1, pos7);
    lux = LoRaData.substring(pos7 + 1, LoRaData.length());
 
    Serial.print(F("Device ID = "));
    Serial.println(device_id);
 
    Serial.print(F("Temperature = "));
    Serial.print(temperature);
    Serial.println(F("*C"));
 
    Serial.print(F("Pressure = "));
    Serial.print(pressure);
    Serial.println(F("hPa"));
 
    Serial.print(F("Approx. Altitude = "));
    Serial.print(altitude);
    Serial.println(F("m"));
 
    Serial.print(F("Humidity = "));
    Serial.print(humidity);
    Serial.println(F("%"));
 
    Serial.print("Dew point = ");
    Serial.print(dewPoint);
    Serial.println(" *C");
 
    Serial.print(F("Rainfall = "));
    Serial.print(rainfall);
    Serial.println(F("%"));
 
    Serial.print(F("Light = "));
    Serial.print(lux);
    Serial.println(F(" lx"));
 
    Serial.println();
 
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
    {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(temperature);
      postStr += "&field2=";
      postStr += String(pressure);
      postStr += "&field3=";
      postStr += String(altitude);
      postStr += "&field4=";
      postStr += String(humidity);
      postStr += "&field5=";
      postStr += String(dewPoint);
      postStr += "&field6=";
      postStr += String(rainfall);
      postStr += "&field7=";
      postStr += String(lux);
      postStr += "r\n";
 
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
 
      Serial.println("Data Send to Thingspeak");
      delay(500);
    }
    client.stop();
    Serial.println("Waiting...");
 
  }
}
