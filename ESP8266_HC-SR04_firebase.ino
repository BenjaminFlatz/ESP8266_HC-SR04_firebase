/*#include <WiFiClientSecureAxTLS.h>
using namespace axTLS;
WiFiClientSecure client;

#include <ESP8266HTTPClient.h>
*/

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

#define FIREBASE_HOST "colorrize-a1a0f.firebaseio.com"
#define FIRMWARE_URL "https://firebasestorage.googleapis.com/v0/b/colorrize-a1a0f.appspot.com/o/mqtt.ino.bin?alt=media&token=b2ec949f-46d8-4051-8ac4-3a06bb159008"
#define FIREBASE_AUTH "mv5r81n16mLY6nxPKlSAHMc8xBXZ2HS6HVCwVkkt"

#define echo 4
#define trig 5

WiFiServer server(80);
String header;

// Assign output variables to GPIO pins


FirebaseData firebaseData;
FirebaseJson json;
String path = "/HC-SR04";

const char* SSID = "lumina";
const char* PSK = "funzel123";

unsigned long lastTime = 0;


void setup() {
 
  pinMode(trig, OUTPUT);                // Trigger Pin als Ausgang definieren
  pinMode(echo, INPUT);  
  // put your setup code here, to run once:

  
  Serial.begin(9600);
  WiFiManager wifiManager;
  
  wifiManager.autoConnect("AutoConnectAP");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
  
  server.begin();

  /*
  WiFi.begin(SSID, PSK);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

int get_distance()
{
  long distance=0;
  long time=0;
  int i = 0;
  for(i=0; i<10; i++){
    digitalWrite(trig, LOW);
    delayMicroseconds(3);
    noInterrupts();
    digitalWrite(trig, HIGH); //Trigger Impuls 10 us
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    time = pulseIn(echo, HIGH); // Echo-Zeit messen
    interrupts();
    distance = distance+((time/2)/ 29.1); // Zeit halbieren
    
  }
  distance = distance/10;
  return(distance);
}

void firebase_setInt(int distance){
  
  //Firebase.setInt("/HC-SR04/roman/distance", distance); 
  if (Firebase.setInt(firebaseData, path + "/roman/distance", distance))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
    
}
void loop() {
  //handle_wifi_setup();
  firebase_setInt(get_distance());
  //delay(10);

}
