#include <WiFi.h>
#include <ESP32Ping.h>
 
#define SECRET_SSID ""   // your network SSID (name)
#define SECRET_PASS ""  // your network password (use for WPA, or use as key for WEP)

void setup() {
  Serial.begin(115200);
 
  WiFi.begin(SECRET_SSID, SECRET_PASS);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
 
  bool success = Ping.ping("www.google.com", 3);
 
  if(!success){
    Serial.println("Ping failed");
    return;
  }
 
  Serial.println("Ping succesful.");
}
 
void loop() { }