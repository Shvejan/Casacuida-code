#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "casacuidatrc.firebaseio.com"
#define FIREBASE_AUTH "obDc5Ym3c38zSEvmVTjD21LI8H7ZrD6YhAVjXbR8"
#define ssid "MM"
#define password "nopassword"

int nRainIn = A0;
int nRainDigitalIn = D1;
int nRainVal;
boolean bIsRaining = false;
String strRaining, Rain;

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(500);
    ESP.restart();
  }
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(nRainDigitalIn,INPUT);  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  nRainVal = analogRead(nRainIn);
  bIsRaining = !(digitalRead(nRainDigitalIn));
  Rain = Firebase.getString("CasaCuida/Rain");
  
  if(bIsRaining){
    strRaining = "YES";
    Firebase.setString("CasaCuida/Rain", "Raining");
  }
  else{
    strRaining = "NO";
    if(Rain=="Raining"or Rain==""){
      Firebase.setString("CasaCuida/Rain", "Rainstopped");
    }
    
  }
  
  Serial.print("Raining?: ");
  Serial.print(strRaining);  
  Serial.print("\t Moisture Level: ");
  Serial.println(1024-nRainVal);
  
  delay(200);

}
