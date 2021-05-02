#include <SoftwareSerial.h> 
#include <WiFi.h>
#include <WiFiUdp.h>

#define analogWrite ledcWrite
int incomingByte = 0;  

int speed = 1023;//0 to 1023

int ENA = 16;//d0
int IN1 = 5;//d1
int IN2 = 4;//d2

int ENB = 14;//d5
int IN3 = 0;//d3
int IN4 = 2;//d4

//wifi stuff
const char* ssid     = "sasagad"; // wifi network name
const char* password = "Moustafa312000"; // wifi network password

WiFiUDP Udp;
unsigned int localUdpPort = 1998;
char incomingPacket[255];
     
void setup(){
Serial.begin(115200);
delay(10);
Serial.println("Motor test!");

// We start by connecting to a WiFi network
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
Serial.println("WiFi connected"); 
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
Serial.println("Starting UDP");
Udp.begin(localUdpPort);  

pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
}
void forward(){ 
  digitalWrite(ENA,HIGH); //ENAble L298n A channel
  digitalWrite(ENB,HIGH); //ENAble L298n B channel
  digitalWrite(IN1,HIGH); //set IN1 hight level
  digitalWrite(IN2,LOW);  //set IN2 low level
  digitalWrite(IN3,LOW);  //set IN3 low level
  digitalWrite(IN4,HIGH); //set IN4 hight level
  Serial.println("Forward");//send message to serial monitor
}

void backward(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Back");
}

void right(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  Serial.println("Left");
}

void left(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Right");
}
void stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}



void ListenPacketRoutine()
{
  //listen for packets
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    int len = Udp.read(incomingPacket, 255);
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (incomingPacket[0] == 'f'){
        forward();
    } else if (incomingPacket[0] == 'b'){
        backward();
    } else if (incomingPacket[0] == 'l'){
        left();
    } else if (incomingPacket[0] == 'r'){
        right();
    } else if (incomingPacket[0] == 's'){
        stop();
    }
  }
}



void ListenKeyboardRoutine(){

 if (Serial.available() > 0) {
    incomingByte = Serial.read();
    }
  
 switch(incomingByte)
  {
     case 's': 
      { stop();
       Serial.println("Stop\n"); 
       incomingByte='*';}
      
     break;
     
     case 'f':
     {  forward(); 
       
       Serial.println("Forward\n");
       incomingByte='*';}
     break;
    
      case 'b':  
    {   backward();
       
       Serial.println("Backward\n");
       incomingByte='*';}
     break;
     
     case 'r':
     {  
       right(); 
       Serial.println("Rotate Right\n");
       incomingByte='*';}
     break;

       case 'l':
      { 
       left();    
       Serial.println("Rotate Left\n");
       incomingByte='*';}
     break;       
  }
}

void loop()
{
    ListenPacketRoutine();
    ListenKeyboardRoutine();
}
