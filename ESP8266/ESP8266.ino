#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define SSID "GTMssj"
#define PASSWD "12345679"
#define AP_SSID "WiFidufckie"
#define AP_PASSWD "12345679"

WiFiUDP Udp;
unsigned int localUdpPort = 4444;
char inPacket[256];
char rePacket[256];
int packetSize;
int len;
uint8_t modrcv;
//WiFi

int MODE;

#define S_ADDR 0x26
#define ANS_SIZE 8
uint8_t sendI2C;
uint8_t rcvI2C;
//I2C

// MODES:
// 0 10000000  INIT
// 1 10000001  str
// 2 10000010  KPRESS
// 3 10000011  OPEN
// 4 10000100  KREL
// 5 10000101  KSTROK

// 6 10000110  ...


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASSWD);
  WiFi.begin(SSID, PASSWD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP:");
  Serial.print(WiFi.localIP());
  Serial.print(", ");
  Serial.print(WiFi.softAPIP());
  Udp.begin(localUdpPort);
  Serial.print(" Port:");
  Serial.print(localUdpPort);
  Serial.println(" WiFi初始化完成");
  Wire.begin();
  Serial.println("I2C初始化完成");
  flash(4, 25, 500);
}

void loop() {
  packetSize = Udp.parsePacket();
  if (packetSize){
    handle_UDP();
    handle_I2C();
  }
}

void handle_I2C(){
  //发送模式字
  String cmd = "";
  switch(MODE){
    case 1: //str
      for (int i = 0; i < len; i ++){
        send_uint8(inPacket[i]);
      }
      break;
    case 2: //kpress
      for (int i = 0; i < len; i ++){
        cmd += inPacket[i];
      }

      if (cmd == "up"){
        send_uint8(0b00001000);
      }
      else if (cmd == "down"){
        send_uint8(0b00001001);
      }
      else if (cmd == "left"){
        send_uint8(0b00001010);
      }
      else if (cmd == "right"){
        send_uint8(0b00001011);
      }
      else if (cmd == "win"){
        send_uint8(0b00001100);
      }
      else if (cmd == "enter"){
        send_uint8(0b00001101);
      }
      else if (cmd == "bdel"){
        send_uint8(0b00001110);
      }
      else if (cmd == "del"){
        send_uint8(0b00001111);
      }
      else{
        send_uint8(inPacket[0]);
      }
      break;

    case 3: //OPEN
      for (int i = 0; i < len; i ++){
        cmd += inPacket[i];
      }
      
      if (cmd == "winr"){
        send_uint8(0b00000000);
      }
      if (cmd == "cmd"){
        send_uint8(0b00000001);
      }
      if (cmd == "powershell"){
        send_uint8(0b00000010);
      }



      if (cmd == "exit"){
        send_uint8(0b00000111);
      }
      break;
  }
}

void handle_UDP(){
  len = Udp.read(inPacket, 255);
  if (len > 0){
    inPacket[len] = '\0';
  }
  
  modrcv = inPacket[0];
  for (int i = 0; i < len; i ++){
    inPacket[i] = inPacket[i + 1];
  }
  len -= 1;
  inPacket[len] = '\0';
  
  switch(modrcv){
    case 0b11111001:
      MODE = 1; //str
      break;
    case 0b11111010:
      MODE = 2; //KPRESS
      break;
    case 0b11111011:
      MODE = 3; //OPEN
      break;
    // case 0b11111100:
    //   MODE = 4; //KREL
    //   break;
    // case 0b11111101:
    //   MODE = 5; //INIT
    //   break;
    default:
      MODE = -1;
      break;
  }
  Serial.print(" MOD:");
  Serial.println(MODE);

  // for (int i = 0; i < len; i ++){
  //   rePacket[i] = inPacket[i];
  // }

  // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  // Udp.write(rePacket);
  // Udp.endPacket();
  // Serial.print("UDP收到:");
  // Serial.print(inPacket);
  // Serial.print(" 回复:");
  // Serial.print(rePacket);
  // for (int i = 0; i < len; i ++){
  //   rePacket[i] = '\0';
  // }
}

void send_uint8(uint8 info){
  Wire.beginTransmission(S_ADDR);
  Wire.write(info);
  Wire.endTransmission();
}

void flash(int t, int delta, int looptime){
  for(int i=0; i < t; i++){ 
    digitalWrite(LED_BUILTIN, LOW);
    delay(delta);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delta);
  }
  delay(looptime);
}