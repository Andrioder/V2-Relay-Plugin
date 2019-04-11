//funkup studio
//Vinayak Gupta

#include "V2Plugin.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "FS.h"

V2::V2(int pinNumber){
  pn = pinNumber;
  pinMode(pn,OUTPUT);
}
WiFiServer server(8085);

bool got_id = false;
bool got_ssid = false;
bool got_pwrd = false;

void V2::ControlRelay(int v){
  digitalWrite(pn,v);
}

String V2::GetIP(){
  if(WiFi.status() == WL_CONNECTED){
    return WiFi.localIP().toString();
  }
  return "Not Conected";

}

bool V2::Read(){
  SPIFFS.begin();
  File data = SPIFFS.open("/data.json","r");
  if(!data){
    return false;
  }
  Serial.println("LOADING");
  std::unique_ptr<char[]> buf(new char[data.size()]);

  data.readBytes(buf.get(),data.size());
  data.close();
  StaticJsonDocument<1024> jsonBuffer;
  deserializeJson(jsonBuffer,buf.get());


  if(!jsonBuffer.isNull()){
    id = jsonBuffer["id"];
    const char* s = jsonBuffer["ssid"];
    const char* p = jsonBuffer["password"];
    strcpy(ssid,s);
	  delay(2000);
    strcpy(password,p);
	  delay(1000);
    return true;
  }else{
    Serial.println("JSON NULL");
  }
}

bool V2::Save(){
  SPIFFS.begin();
  SPIFFS.remove("/data.json");
  File data = SPIFFS.open("/data.json","w");

  if(!data){
    return false;
  }

  Serial.println("SAVING");

  DynamicJsonDocument  doc(1024);
  doc["id"] = id;
  doc["ssid"] = ssid;
  doc["password"] = password;
  serializeJson(doc,data);
  data.close();
  return true;
}
void V2::RecveInfo(){
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("new client");
  while (!client.available()) {}
  String req = client.readStringUntil('\n');
  client.flush();
  Serial.println(req);
  V2::ExtractInfo(req);
  delay(1);
}

void V2::ExtractInfo(String r){
  int row = 1;
  String _id = "";
  String _ssid = "";
  String _pwrd = "";
  for(int v = 0; v < r.length(); v++){
    char d = r.charAt(v);
    if(d == ' '){
      if(row == 3) return;
      row++;
    }else{
      if(row == 1){
        _id += r.charAt(v);
      }
      if(row == 2){
        _ssid += r.charAt(v);
      }
      if(row == 3){
        _pwrd += r.charAt(v);
      }
    }
  }
  Serial.println(_id);
  Serial.println(_ssid);
  Serial.println(_pwrd);

  id = _id.toInt();
  V2::SplitStringAndFillChar(_ssid,ssid);
  delay(2000);
  V2::SplitStringAndFillChar(_pwrd,password);
  delay(2);
  V2::ConnectToWifi();
}

void V2::RecveCommand() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {}

  if(V2::static_ready == true){
    char c = client.read();
    Serial.println(c);
    if(c == '1'){
      V2::ControlRelay(0);
    }else{
      V2::ControlRelay(1);
    }

  }
}

void V2::ConnectionStuff(){
  int threshold = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    threshold ++;
    Serial.print(".");
    if(threshold >= 20){
      Serial.println("\nCONNECTION TIMEOUT");
      delay(5000);
      if(connIndex >= 3){
        connIndex = 0;
        V2::SetAsAccesspoint();
        return;
      }
      Serial.println("Re trying..!!");
      connIndex ++;
      V2::TryToConnect();
      return;
    }
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("\nConnected, IP address: ");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.localIP());
    V2::Save();
    if(static_ready == false){
      V2::SetStaticIP();
    }else{
      V2::StartLANServer();
    }
  }
}

void V2::HandleLoop(){
  if(start_info){
    V2::RecveInfo();
    return;
  }

  if(start_rcvng)
  {
    V2::RecveCommand();
    return;
  }
}

void V2::TryToConnect(){

  if(!V2::Read()){
    Serial.println("LOL");
    V2::SetAsAccesspoint();
    return;
  }
  Serial.print("Connecting");
  V2::ConnectToWifi();
}

void V2::ConnectToWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  Serial.println("Conn SSID: ");
  Serial.println(ssid);
  Serial.println("CONN PASSWORD: ");
  Serial.println(password);
  V2::ConnectionStuff();
}

void V2::SetStaticIP(){
  Serial.println("setting up the static ip");
  IPAddress gateway = WiFi.gatewayIP();
  IPAddress subnet = WiFi.subnetMask();
  Serial.println("My gateway: " + gateway.toString());
  Serial.println("My subnet: " + subnet.toString());

  int i = 26;
  if(id != 0){
    i = id;
  }

  IPAddress ip(gateway[0],gateway[1],gateway[2],i);
  WiFi.config(ip,gateway,subnet);
  WiFi.reconnect();
  Serial.print("\nConnected, IP address: ");
  Serial.println(WiFi.localIP());
  static_ready = true;
  V2::StartLANServer();
}


void V2::StartLANServer()
{
  Serial.println("Starting LAN server..YEAH>>!!!!!");
  start_info = false;
  start_rcvng = true;
  server.begin();
}

void V2::SetAsAccesspoint()
{
  start_rcvng = false;
  WiFi.mode(WIFI_AP_STA);

  if(WiFi.softAP("funkupstudio","vinayak200")){
    Serial.println("Ready AS A HOTSPOT");
    StartSoftAPServer();
  }else
  {
    Serial.println("Not Ready AS A HOTSPOT");
  }
}

void V2::StartSoftAPServer(){
  Serial.println("Starting Hotspot Server");
  start_info = true;
  server.begin();
}


void V2::SplitStringAndFillChar(String s, char c[50]){
  strcpy(c,s.c_str());
}
