// Ex08.cpp/.ino
// become a web client and send your email to our cloud server over HTTPS

#include "Thing.h"

// globals and utilities for connecting to COM3505 cloud server
WiFiClientSecure com3505Client; // the TLS web client
const char *com3505Addr   = "SERVER IP ADDRESS"; // server IP (see blackboard)
const int   com3505Port   = 9194;                // server port
const char* wifiSsid      = "CHANGE_ME";         // wifi AP for ESP to connect
const char* wifiPassword  = "CHANGE_ME_TOO";     // wifi PSK
const char* myEmail       = "AND_CHANGE_ME_AS_WELL@sheffield.ac.uk";// yr email

// x509 certificate for the server
const char* cert = \
  "-----BEGIN CERTIFICATE-----\n" \
  "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" \
  "PQ0ptJ0=" \
  "-----END CERTIFICATE-----\n";


void setup08() {
  setup07(); dln(startupDBG, "\nsetup08...");

  // a URI for the request
  String url;
  url = "/com3505-2023?email=";
  url += myEmail;
  url += "&mac=";
  url += MAC_ADDRESS;

  // conect to the uni network
  // (if your device is already provisioned you can use "WiFi.begin();" here)
  dbg(netDBG, "connecting to wifi...");
//  WiFi.begin();
  WiFi.begin(wifiSsid, wifiPassword);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    dbg(netDBG, ".");
  }

  dbg(netDBG, "wifi connected; IP address: ");
  dln(netDBG, WiFi.localIP());

  // now we'll try and talk to the "cloud" server; this is
  // in setup instead of loop so we don't send too much data to the server...
  //
  // first connect to the cloud server or reset after pause
  if(! cloudConnect()) {
    Serial.println("** OOOPS! no connection to server; restarting in 10 **");
    delay(10000);
    ESP.restart();
  }
    
  // send the request to the server
  cloudGet(url);

  // read all the lines of the reply from server and print to serial
  while(cloudAvailable()) dbg(netDBG, cloudRead());

  // job's a goodun
  dln(netDBG, "closing connection...");
  cloudStop();
} // setup08()

void loop08() {
  // serve pending web requests
  webServer.handleClient();
} // loop08()

// use WiFiClient class to create TCP connection to cloud
bool cloudConnect() {
  dbg(netDBG, "\nconnecting to ");
  dbg(netDBG, com3505Addr);
  dbg(netDBG, ":");
  dln(netDBG, com3505Port);

  // configure the TLS client for connection; ordinarily we would do
  // "setCACert(...)" but the server is using a self-signed certificate so the
  // CA chain doesn't exist; instead we just give the client certificate and
  // do "setInsecure" - the connection should still be encrypted, but more
  // vulnerable to a MitM attack
  com3505Client.setCertificate(cert); // client verification only
  // setInsecure doesn't exist in some of the Arduino core versions that we
  // may be using, so this is commented here; try uncommenting, and see the
  // notes: https://iot.unphone.net/#patch-the-core-for-ex08
  com3505Client.setInsecure(); // tell mbedTLS to ignore the CA chain

  // open the connection, or punt
  if(com3505Client.connect(com3505Addr, com3505Port)) {
    dln(netDBG, "connected to com3505 server; doing GET");
  } else {
    dbg(netDBG, com3505Addr);
    dln(netDBG, " - no com3505 server");
    return false;
  }
  return true;
} // cloudConnect()

// do a GET request on com3505Client
void cloudGet(String url) {
  dbg(netDBG, "requesting URL: ");
  dln(netDBG, url);
  com3505Client.print(
    String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + com3505Addr + "\r\n" + "Connection: close\r\n\r\n"
  );
  unsigned long timeout = millis();
  while(com3505Client.available() == 0) {
    if(millis() - timeout > 5000) {
      dln(netDBG, ">>> client timeout !");
      cloudStop();
      return;
    }
  }
} // cloudGet()

// close cloud connection
void cloudStop() { com3505Client.stop(); }

// read a line from the cloud client after a request
String cloudRead() { return com3505Client.readStringUntil('\r'); }

// is there more to read from the response?
bool cloudAvailable() { return com3505Client.available(); }
