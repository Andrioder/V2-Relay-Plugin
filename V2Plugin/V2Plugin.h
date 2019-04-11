//funkup studio
//Vinayak Gupta

#ifndef v2_plugin
#define v2_plugin

#if (ARDUINO >= 100)
   #include "Arduino.h"
#else
   #include "WProgram.g"

#endif

class V2 {
  public:
    V2(int pinNumber);
    bool isConn = false;
    void TryToConnect();
    void HandleLoop();
    String GetIP();

  private:
    int connIndex = 1;
    int pn = 4;
    int port = 8085;
    bool start_rcvng = false;
    bool start_info = false;
    int id;
    char ssid[50];
    char password[50];
    bool static_ready = false;
    bool Read();
    bool Save();
    void ExtractInfo(String r);
    void RecveInfo();
    void RecveCommand();
    void StartLANServer();
    void SplitStringAndFillChar(String s, char c[50]);
    void ConnectToWifi();
    void ConnectionStuff();
    void SetStaticIP();
    void SetAsAccesspoint();
    void StartSoftAPServer();
    void ControlRelay(int v);
};

#endif
