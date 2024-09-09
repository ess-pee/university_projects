// Ex07.cpp/.ino
// develop simple utilities for creating web pages

#include "Thing.h"

// there's a thousand ways to do this... below a version that is relatively
// simple to use and to code, based on an array of C strings that are
// concatenated into an Arduino (C++) String (so beware memory fragmentation!)
//
// the replacement_t type definition allows specification of a subset of the
// "boilerplate" strings, so we can e.g. replace only the title, or etc.
//
// note that to see the results you need to load a page at "/7", e.g. 
// http://192.168.4.1/7

const char *boiler[] = { // boilerplate: constants & pattern parts of template
  "<html><head><title>",                                                // 0
  "default title (7)",                                                  // 1
  "</title>\n",                                                         // 2
  "<meta charset='utf-8'>",                                             // 3

  // adjacent strings in C are concatenated:
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
  "<style>body{background:#FFF; color: #000; font-family: sans-serif;", // 4

  "font-size: 150%;}</style>\n",                                        // 5
  "</head><body>\n<h2>",                                                // 6
  "Welcome to Thing (again)!",                                          // 7
  "</h2>\n<p><a href='/'>Home</a>&nbsp;&nbsp;&nbsp;</p>\n",             // 8
  "</body></html>\n\n",                                                 // 9
};

void setup07() {
  setup06(); dln(startupDBG, "\nsetup07...");
  webServer.on("/7", handleSeven);
}

void loop07() {
  if(! (loopIteration++ % 500000 == 0)) // a slice every 0.5m iterations
    return;
  webServer.handleClient(); // serve pending web requests every loop
}

void getHtml( // turn array of strings & set of replacements into a String
  String& html, const char *boiler[], int boilerLen,
  replacement_t repls[], int replsLen
) {
  for(int i = 0, j = 0; i < boilerLen; i++) {
    if(j < replsLen && repls[j].position == i)
      html.concat(repls[j++].replacement);
    else
      html.concat(boiler[i]);
  }
}

void handleSeven() {
  dln(netDBG, "handleSeven: serving page at /7");
  replacement_t repls[] = { // the elements to replace in the boilerplate
    { 1, "a better title" },
    { 7, "Eat more green vegetables!" },
  };
  String toSend = "";
  getHtml(toSend, boiler, ALEN(boiler), repls, ALEN(repls)); // instantiate
  webServer.send(200, "text/html", toSend);
}
