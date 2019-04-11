#include <V2Plugin.h>

V2 myV2 = V2(4);

void setup() {
  Serial.begin(115200);
  myV2.TryToConnect();
}

void loop() {
  myV2.HandleLoop();
  if(myV2.isConn){
    digitalWrite(LED_BUILTIN,HIGH);
  }else{
    digitalWrite(LED_BUILTIN,LOW);
  }
} 
