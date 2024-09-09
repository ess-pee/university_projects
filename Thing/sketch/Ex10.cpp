// Ex10.cpp/.ino
//
// OTA update from version.bin; run `python -m http.server 8000`
// to serve, and store the current revision number in "version.txt"
//
// when you bump the revision to higher than the current running number (in
// firmwareVersion) you should see an OTA update, and following restart you
// should be on the latest version

#include "Thing.h"

// what version of the firmware are we? (used to calculate need for updates)
// see firmwareVersion in sketch.ino

// IP address and port number: CHANGE THE IP ADDRESS!
#define FIRMWARE_SERVER_IP_ADDR "10.0.0.49"
#define FIRMWARE_SERVER_PORT    "8000"

// setup ////////////////////////////////////////////////////////////////////
void setup10() {
  Serial.begin(115200); // initialise the serial line
  getMAC(MAC_ADDRESS);  // store the MAC address as a chip identifier
  dln(startupDBG, "\nsetup10..."); // debug printout
  Serial.printf("running firmware is at version %d\n", firmwareVersion);

  // get on the network
  WiFi.begin(); // register MAC first! and add SSID/PSK details if needed
  uint16_t connectionTries = 0;
  Serial.print("trying to connect to Wifi...");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if(connectionTries++ % 75 == 0) Serial.println("");
    delay(250);
  }
  delay(500); // let things settle for half a second
  Serial.println("connected :)");

  // materials for doing an HTTPS GET on github from the BinFiles/ dir
  HTTPClient http;
  int respCode;
  int highestAvailableVersion = -1;

  // read the version file from the cloud
  respCode = doCloudGet(&http, "version.txt");
  if(respCode > 0) // check response code (-ve on failure)
    highestAvailableVersion = atoi(http.getString().c_str());
  else
    Serial.printf("couldn't get version! rtn code: %d\n", respCode);
  http.end(); // free resources

  // do we know the latest version, and does the firmware need updating?
  if(respCode < 0) {
    return;
  } else if(firmwareVersion >= highestAvailableVersion) {
    Serial.printf("firmware is up to date\n");
    return;
  }

  // do a firmware update
  Serial.printf(
    "upgrading firmware from version %d to version %d\n",
    firmwareVersion, highestAvailableVersion
  );

  // do a GET for the .bin, e.g. "23.bin" when "version.txt" contains 23
  String binName = String(highestAvailableVersion);
  binName += ".bin";
  respCode = doCloudGet(&http, binName);
  int updateLength = http.getSize();

  // possible improvement: if size is improbably big or small, refuse
  if(respCode > 0 && respCode != 404) { // check response code (-ve on failure)
    Serial.printf(".bin code/size: %d; %d\n\n", respCode, updateLength);
  } else {
    Serial.printf("failed to get .bin! return code is: %d\n", respCode);
    http.end(); // free resources
    return;
  }

/*
  // debug code, checks how much we can download of the bin file, then aborts
  if(Update.begin(updateLength)) {
    Serial.printf("starting OTA may take 2-5 mins to complete...\n");
  }
  WiFiClient s = http.getStream();
  int bytesRead = 0;
  while(s.available() > 0) {
    if(s.read() == -1)
      break;
    bytesRead++;
  }
  Serial.printf("read %d bytes from stream\n", bytesRead);
  Update.abort();
  Serial.printf("OTA aborted\n");
*/

  // write the new version of the firmware to flash
  WiFiClient stream = http.getStream();
  Update.onProgress(handleOTAProgress); // print out progress
  if(Update.begin(updateLength)) {
    Serial.printf("starting OTA may take a minute or two...\n");
    Update.writeStream(stream);
    if(Update.end()) {
      Serial.printf("update done, now finishing...\n");
      Serial.flush();
      if(Update.isFinished()) {
        Serial.printf("update successfully finished; rebooting...\n\n");
        ESP.restart();
      } else {
        Serial.printf("update didn't finish correctly :(\n");
        Serial.flush();
      }
    } else {
      Serial.printf("an update error occurred, #: %d\n" + Update.getError());
      Serial.flush();
    }
  } else {
    Serial.printf("not enough space to start OTA update :(\n");
    Serial.flush();
  }
  stream.flush();
}

// loop /////////////////////////////////////////////////////////////////////
void loop10() {
  int sliceSize = 500000;
  loopIteration++;
  if(loopIteration % sliceSize == 0) // a slice every sliceSize iterations
    dln(otaDBG, "OTA loop");

  webServer.handleClient(); // serve pending web requests every loop, as Ex09
}

// helper for downloading from cloud firmware server; for experimental
// purposes just use a hard-coded IP address and port (defined above)
int doCloudGet(HTTPClient *http, String fileName) {
  // build up URL from components; for example:
  // http://192.168.4.2:8000/Thing.bin
  String url =
    String("http://") + FIRMWARE_SERVER_IP_ADDR + ":" +
    FIRMWARE_SERVER_PORT + "/" + fileName;
  Serial.printf("getting %s\n", url.c_str());

  // make GET request and return the response code
  http->begin(url);
  http->addHeader("User-Agent", "ESP32");
  return http->GET();
}

// callback handler for tracking OTA progress ///////////////////////////////
void handleOTAProgress(size_t done, size_t total) {
  float progress = (float) done / (float) total;
  // dbf(otaDBG, "OTA written %d of %d, progress = %f\n", done, total, progress);

  int barWidth = 70;
  Serial.printf("[");
  int pos = barWidth * progress;
  for(int i = 0; i < barWidth; ++i) {
    if(i < pos)
      Serial.printf("=");
    else if(i == pos)
      Serial.printf(">");
    else
      Serial.printf(" ");
  }
  Serial.printf(
    "] %d %%%c", int(progress * 100.0), (progress == 1.0) ? '\n' : '\r'
  );
  // Serial.flush();
}
