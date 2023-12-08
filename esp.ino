#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL61K0_AgJv"
#define BLYNK_TEMPLATE_NAME "Receiver22"
#define BLYNK_AUTH_TOKEN "LJ5Uj4a9Nwob7Oy8XZ25hOH96LZMkRFR"

#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;  // You should get Auth Token in the Blynk App.
char ssid[] = "PHONG TRO";            // Your WiFi credentials.
char pass[] = "phongtro85a";

#define SS 5
#define RST 14
#define DI0 2

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

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Serial.println("LoRa Receiver");
  //LoRa.setTxPower(TX_P);
  //LoRa.setSyncWord(ENCRYPT);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, pass);

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

void loop() {
  // try to parse packet
  int pos1, pos2, pos3, pos4, pos5, pos6, pos7;

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Blynk.run();
    // received a packet
    Serial.print("Received packet:  ");
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData);
    // read packet
    while (LoRa.available()) {
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

    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, pressure);
    Blynk.virtualWrite(V4, altitude);
    Blynk.virtualWrite(V5, dewPoint);
    Blynk.virtualWrite(V6, rainfall);
    Blynk.virtualWrite(V7, lux);
  }
}