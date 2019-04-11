# V2-Relay-Plugin
V2Plugin for your esp8266 Relay projects

Providing the best solution for your esp projects...<br>
Want to add voice command feature in your project then this library is for you..!!!<br>

<h1> INTRODUCTION </h1>
Here is the V2 Relay Plugin for those who wants to add a voice command feature, not only voice command who wants to control their
project from an app.... V2 the voice command app helps to send on/off commands directly from the home screen either a voice command or by  using button present on the V2 Widget<br>
So the connectivity of the V2 with your esp smart switch[relay project] can be done in 3 steps..<br>
Let's talk about at the programming end.... in 3 lines of code the library can be step up..

<h1>REQUIREMENTS:</h1><br> 
     Download ArduinoJson: https://github.com/bblanchon/ArduinoJson <br>
     Download V2:The voice command app: https://play.google.com/store/apps/details?id=com.funkupstudio.andri.v2 <br>
<h1>BEFORE UPLOADING:</h1><br>
     Tools > Flash Size > 4M (3M SPIFFS)  <br> 

<h1> HOW TO USE </h1>
1> Download the V2Plugin folder <br>
2> Move it to Documents\Arduino\libraries <br>
3> Download the data folder.. located in the V2PluginExample<br>
4> Move the data folder in your sketch folder<br>

<h1> CODE </h1>
   //Include the plugin <br>
   #include < V2Plugin.h > <br>
   //Create V2 object<br>
   V2 myV2 = V2(relayPin, port);  // relayPin is the GPIO pin and port is for the wifiClient ex: 8085<br>
   //In Start()<br>
   myV2.TryToConnect();<br>
   //In Loop()<br>
   myV2.HandleLoop();<br>
   //To check the conn (inside loop())<br>
   if(myV2.isConn){<br>
      digitalWrite(LED_BUILTIN, HIGH);<br>
   }else{<br>
      digitalWrite(LED_BUILTIN, LOW);<br>
   }<br>

<h1>Other functions </h1>
myV2.GetIP() // returns the IP address of the esp
