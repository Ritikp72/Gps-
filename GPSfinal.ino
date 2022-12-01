#define BLYNK_TEMPLATE_ID "TMPL5y3fQfVH"
#define BLYNK_DEVICE_NAME "GSP Module"
#define BLYNK_AUTH_TOKEN "195Mpiw4Pgq4DocVs3JXXg3l7iRuux1x"
//------------------------------------------------------------------------------
//--------Global Variables__------------------------------------------------
char auth[] = BLYNK_AUTH_TOKEN;
// WiFi credentials.
char ssid[] = "Wie gehts??";
char pass[] = "idontknow";
char emergency_msg[]="The user is in Emergency, Reach out fast !!";
char normal_msg[]="Running Good !!";
int buttonState = 0;
//------------------------------------------------------------------------------
#define BLYNK_PRINT Serial
//------------------Header Files----------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TinyGPS++.h> 
#include <TimeLib.h>
//------------------------------------------------------------------------------
//GPS Module Settings
//GPS Module RX pin to ESP32 17
//GPS Module TX pin to ESP32 16
#define RXD2 16
#define TXD2 17
const int buttonPin = 2;     // the number of the pushbutton pin
HardwareSerial neogps(2);
TinyGPSPlus gps;
BlynkTimer timer;
void sendgps();
#define INTERVAL 1000L
//------------------------------------------------------------------------------
/************************************************************************************
 *  setup() function
 **********************************************************************************/
void setup()
{
  //-----------------------------------------------------------
  Serial.begin(9600);
  //-----------------------------------------------------------
  Blynk.begin(auth, ssid, pass);
  //Set GPS module baud rate
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(buttonPin, INPUT);
  Serial.println("neogps serial initialize");
  delay(10);
  timer.setInterval(INTERVAL, sendGps);
}

/************************************************************************************
 *  loop() function
 **********************************************************************************/
void loop()
{
  Blynk.run();
  timer.run();
}

void sendGps()
{
  //-----------------------------------------------------------
  while(neogps.available())
  {
    if (gps.encode(neogps.read()))
    {
      break;
    }
  }
  //-----------------------------------------------------------
  if (!gps.location.isValid())
  {
    Serial.println("Failed to get data from GPS Module!");
    return;
  }
  //-----------------------------------------------------------
  //get latitude and longitude
  float latitude = gps.location.lat();
  float longitude = gps.location.lng();
  float speed = gps.speed.kmph();
  //-----------------------------------------------------------
  Serial.print("Latitude:  ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.print("Speed: ");
  Serial.println(speed, 6);
  //-----------------------------------------------------------
  Blynk.virtualWrite(V1, String(latitude, 6));
  Blynk.virtualWrite(V2, String(longitude, 6));
  Blynk.virtualWrite(V0, String(speed));
  //-----------------------------------------------------------
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
      Serial.println("Button Pressed Data sent to blynk");
      Blynk.virtualWrite(V3, emergency_msg);
      }
  if (buttonState == LOW) {
      Blynk.virtualWrite(V3,normal_msg);
      }
}
