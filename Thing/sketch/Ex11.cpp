// Ex11.cpp/.ino
// captive portal

#include "Thing.h"

DNSServer dnsServer;
const byte DNS_PORT = 53;

void setup11() {
  setup09();             // UI for choosing and joining wifi networks
  dln(startupDBG, "\nsetup11...");                 // say hi
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // DNS * to our IP
  dln(startupDBG, WiFi.localIP());
}

void loop11() {
  dnsServer.processNextRequest(); // handle DNS  requests
  webServer.handleClient();       // handle HTTP requests
}
