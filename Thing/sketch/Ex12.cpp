// Ex12.cpp/.ino
// send your project choice priority list to our cloud server

#include "Thing.h"

void setup12() {
  setup11(); // let's include the wifi provisioning stuff
  dln(startupDBG, "\nsetup12...");
  dbg(netDBG, "wifi connected; IP address: "); dln(netDBG, WiFi.localIP());
  delay(2000); // let the connection settle...

  // a URI for the request: the basics
  String url = "/com3505-2023?email=fictional@sheffield.ac.uk&mac=";
  url += MAC_ADDRESS;

  // my choice of project from: 1 air quality monitor;
  // 2 campus panic button; 3 RoboThing; 4 WaterElf: sustainable food tech;
  // 5 Peer-to-Peer voting system; 6 other
  url += "&1st=4&2nd=5&3rd=1";

  // reuse the Ex08 code... this is moved into setup for this exercise to
  // reduce the number of requests that will hit the server simultaneously.
  // this means that you may need to a) configure wifi access then b) reset
  // the ESP to trigger it to connect to the network chosen in a)...
  cloudConnect(); // connect to the cloud server
  cloudGet(url);  // send the request to the server
  while(cloudAvailable()) dbg(netDBG, cloudRead()); // print the reply
  cloudStop();    // shut the connection
} // setup12()

void loop12() {
  dnsServer.processNextRequest(); // handle DNS  requests
  webServer.handleClient();       // handle HTTP requests
} // loop12()
