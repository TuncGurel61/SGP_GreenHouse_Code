#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

//Water Sensor Pins and Variables
int resval = 0;  // holds the value
int respin = A0; // sensor pin used

//Solar Detection Variable
int s_detection = 0;

//Connection with Blynk and Internet
char auth[] = "nHhizFxynCVYS9j1K5h3-dul06S4n0h-";
char ssid[] = "TELUS2624";
char pass[] = "g8zxrw299g";

//Temperature and Humidity sensor Type/Pin
#define DHTPIN 2
#define DHTTYPE DHT11
#define red 5
#define green 4
#define blue 13
#define solar 15
#define sun 12

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
 
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Solar Detection
  s_detection=digitalRead(solar);
  if (s_detection == 1){
    digitalWrite(sun, HIGH);
    delay(200);
    digitalWrite(sun, LOW);
    delay(200);
    digitalWrite(sun, HIGH);
    delay(200);
    digitalWrite(sun, LOW);
    delay(200);
    
  } else {
    digitalWrite(sun, LOW);
  }

  //Water Level Data
  resval=analogRead(respin);

  //LED INDICATORS FOR DHT11

  //Red LED
  if (float(t) > 26.00){
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
  //Blue LED
  }else if (float(t) < 20.00){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
  // Green LED
  }else {
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
  }

  //Displaying Code for Blynk App
  Blynk.virtualWrite(V4, resval);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void loop() {
  Blynk.run();
  timer.run();
}
