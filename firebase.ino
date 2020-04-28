
	#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "casacuidatrc.firebaseio.com"
#define FIREBASE_AUTH "obDc5Ym3c38zSEvmVTjD21LI8H7ZrD6YhAVjXbR8"

const char* ssid = "MM";
const char* password = "nopassword";

const int trigPin = D0;
const int echoPin = D1;
const int bm1a=D2;
const int bm1b=D3;
const int bm2a=D4;
const int bm2b=D5;
const int wm=D6;
const int dm1a=D7;
const int dm1b=D8;
const int dm2a=D9;
const int dm2b=D10;
long duration;
int distance;
String VoiceInput,VoiceOutput, Rain;



void off(){
  digitalWrite(trigPin,LOW);
  digitalWrite(echoPin,LOW);
  digitalWrite(bm1a,LOW);
  digitalWrite(bm1b,LOW);
  digitalWrite(bm2a,LOW);
  digitalWrite(bm2b,LOW);
  digitalWrite(wm,LOW);
  digitalWrite(dm1a,LOW);
  digitalWrite(dm1b,LOW);
  digitalWrite(dm2a,LOW);
  digitalWrite(dm2b,LOW);
  Firebase.setString("CasaCuida/VoiceOutput", "Sleepmode");
}


void trash(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  if(distance<=2){
    Firebase.setString("CasaCuida/VoiceOutput", "TrashFull");
    digitalWrite(dm1b,HIGH);
  }
  else{
    digitalWrite(dm1b,LOW);
  }
}

void front(){
  digitalWrite(bm1a,HIGH);
  digitalWrite(bm1b,LOW);
  digitalWrite(bm2a,HIGH);
  digitalWrite(bm2b,LOW);
  Firebase.setString("CasaCuida/VoiceOutput", "Movingfront");
  Serial.println("Front");
}

void back(){
  digitalWrite(bm1a,LOW);
  digitalWrite(bm1b,HIGH);
  digitalWrite(bm2a,LOW);
  digitalWrite(bm2b,HIGH);
  Firebase.setString("CasaCuida/VoiceOutput", "Movingback");
  Serial.println("Back");
}

void right(){
  digitalWrite(bm1a,HIGH);
  digitalWrite(bm1b,LOW);
  digitalWrite(bm2a,LOW);
  digitalWrite(bm2b,HIGH);
  Firebase.setString("CasaCuida/VoiceOutput", "Movingright");
  Serial.println("Right");
  delay(1000);
}

void left(){
  digitalWrite(bm1a,LOW);
  digitalWrite(bm1b,HIGH);
  digitalWrite(bm2a,HIGH);
  digitalWrite(bm2b,LOW);
  Firebase.setString("CasaCuida/VoiceOutput", "Movingleft");
  Serial.println("Left");
  delay(1000);
}

void rest(){
  digitalWrite(bm1a,LOW);
  digitalWrite(bm1b,LOW);
  digitalWrite(bm2a,LOW);
  digitalWrite(bm2b,LOW);
  Serial.println("Rest");
  Firebase.setString("CasaCuida/VoiceOutput", "Atrest");
}

void revolve(){
  digitalWrite(bm1a,HIGH);
  digitalWrite(bm1b,LOW);
  digitalWrite(bm2a,LOW);
  digitalWrite(bm2b,HIGH);
  Firebase.setString("CasaCuida/VoiceOutput", "Youarerecognised");
  delay(2000);
}

void TdoorOpen(){
  digitalWrite(dm1a,HIGH);
  digitalWrite(dm1b,LOW);
  Serial.println("Trash Door Open");
  Firebase.setString("CasaCuida/VoiceOutput", "Door.open");
  delay(3000);
}
void TdoorClose(){
  digitalWrite(dm1a,LOW);
  digitalWrite(dm1b,HIGH);
  Serial.println("Trash Door Close");
  Firebase.setString("CasaCuida/VoiceOutput", "Doorclose");
  delay(3000);
}
void UdoorOpen(){
  digitalWrite(dm2a,HIGH);
  digitalWrite(dm2b,LOW);
  Serial.println("Umbrella Door Open");
  Firebase.setString("CasaCuida/VoiceOutput", "Door.open");
  delay(3000);
}
void UdoorClose(){
  digitalWrite(dm2a,LOW);
  digitalWrite(dm2b,HIGH);
  Serial.println("Umbrella Door Close");
  Firebase.setString("CasaCuida/VoiceOutput", "Doorclose");
  delay(3000);
}

void water(){
  digitalWrite(wm,HIGH);
  delay(1000);
  digitalWrite(wm,LOW);
  Serial.println("Water Ready");
  Firebase.setString("CasaCuida/VoiceOutput", "Grab.bottle");
}




void setup() {
  Serial.begin(115200);
  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(bm1a,OUTPUT);
  pinMode(bm1b,OUTPUT);
  pinMode(bm2a,OUTPUT);
  pinMode(bm2b,OUTPUT);
  pinMode(wm,OUTPUT);
  pinMode(dm1a,OUTPUT);
  pinMode(dm2a,OUTPUT);
  pinMode(dm1b,OUTPUT);
  pinMode(dm2b,OUTPUT);
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // get value
  VoiceInput = Firebase.getString("CasaCuida/VoiceInput");
  VoiceOutput = Firebase.getString("CasaCuida/VoiceOutput");
  Rain = Firebase.getString("CasaCuida/VoiceOutput");
  Serial.print("VoiceInput: ");
  Serial.println(VoiceInput);
  
  
  if(VoiceInput=="\"water bottle\""){
    water();
    trash();
  }

  if(VoiceInput=="\"shutdown\"" or VoiceInput=="\"sleep\""){
    off();
  }
  
  if(VoiceInput=="\"stop\""or VoiceInput=="\"rest\""){
    rest();
  }
  else if(VoiceInput=="\"forward\"" or VoiceInput=="\"move front\""){
    front();
  }
  else if(VoiceInput=="\"backword\"" or VoiceInput=="\"go back\""){
    back();
  }
  else if(VoiceInput=="\"right\"" or VoiceInput=="\"take right\""){
    right();
  }
  else if(VoiceInput=="\"left\"" or VoiceInput=="\"take right\""){
    left();
  }


  if(Rain=="\"Raining\"" or VoiceInput=="\"umbrella\""){
    UdoorOpen();
    delay(2000);
    UdoorClose();
  }


  if(VoiceOutput=="m.m"or VoiceOutput=="aneela" 
  or VoiceOutput=="shvejen" or VoiceOutput=="nitish" or VoiceOutput=="charan" 
  or VoiceOutput=="rishitha" or VoiceOutput=="sireesha" 
  or VoiceOutput=="ramya" or VoiceOutput=="manasa" or VoiceOutput=="priya"){
    revolve();
  }

  if(VoiceInput=="\"clear input\""){
    Firebase.setString("CasaCuida/VoiceInput", "");
  }
  
  } 
