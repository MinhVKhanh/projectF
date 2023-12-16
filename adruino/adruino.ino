#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>       // adds BH1750 library file 
#include <DHT.h>
 
//#define SS 10
//#define RST 9
//#define DI0 2
 
//#define TX_P 17
//#define ENCRYPT 0x78
 
#define BAND 433E6
#define rain_sensor A0
const int DHTPIN = 3;
const int DHTTYPE = DHT11;
// #define BMP280_ADDRESS  0x76
 
#define SEALEVELPRESSURE_HPA (1013.25)

DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP280 bmp;
 
BH1750 lightMeter;
 
String LoRaMessage = "";
char device_id[12] = "MyDevice123";
 
 
void setup()
{
  delay(2000);
  Serial.begin(115200); 
  Wire.begin();
  lightMeter.begin();
  dht.begin(); 
  pinMode (rain_sensor, INPUT);

  while (!Serial);
 
  Serial.println(F("LoRa Sender"));
 
  //LoRa.setPins(SS, RST, DI0);
  //LoRa.setTxPower(TX_P);
  //LoRa.setSyncWord(ENCRYPT); 
  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Starting test bmp280");
  if (!bmp.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  delay(500);
  Serial.println("Finish checking");
}
 
void loop()
{
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  
  // float humidity = bmp.readHumidity();
  float humidity = dht.readHumidity();
 
  double dewPoint = dewPointFast(temperature, humidity);
 
  int rainfall = map(analogRead(rain_sensor), 780, 0, 0, 100);
  if (rainfall >= 100)
  {
    rainfall = 100;
  }
  if (rainfall <= 0)
  {
    rainfall = 0;
  }
 
  float lux = lightMeter.readLightLevel();
 
  Serial.print(F("Device ID: "));
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
 
  Serial.print(F("Dew point = "));
  Serial.print(dewPoint);
  Serial.println(F(" *C"));
 
  Serial.print(F("Rainfall = "));
  Serial.print(rainfall);
  Serial.println(F("%"));
 
  Serial.print(F("Light = "));
  Serial.print(lux);
  Serial.println(F(" lx"));
 
 
  Serial.println();
//  + "#" + String(altitude) + "@" + String(humidity) + "$" + String(dewPoint)
  LoRaMessage = String(device_id) + "/" + String(temperature) + "&" + String(pressure)
                + "#" + String(altitude) + "@" + String(humidity) + "$" + String(dewPoint)
                + "^" + String(rainfall) + "!" + String(lux);
 
  // send packet
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  delay(10000);
}
 
 
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}