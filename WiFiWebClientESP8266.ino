#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

//Wifi module variables
ESP8266WiFiMulti WiFiMulti;

//Constants, are used for configurations
const char* message = "/notify";
const int buttonPin = 5;

//Variable to manage button state
int ledState = HIGH; 
int buttonState;
int lastButtonState = LOW;

//Debounce Variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void setup() {
    USE_SERIAL.begin(19200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("Casa", "hondutel");

}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only send message if the new button state is HIGH
      if (buttonState == HIGH) {
        sendMail();
      }
      
    }
  }
  lastButtonState = reading;
}

void sendMail(){
  // wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {

      HTTPClient http;

      USE_SERIAL.print("[HTTP] begin...\n");
      // configure traged server and url
      //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
      http.begin("http://192.168.0.137:5000/notify");//HTTP

      USE_SERIAL.print("[HTTP] POST...\n");
      // start connection and send HTTP headers
      int httpCode = http.POST("");
      

      // httpCode will be negative on error
      if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

          // file found at server
          if(httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              USE_SERIAL.println(payload);
          }else{
            USE_SERIAL.print("STATUS: ");
            USE_SERIAL.println(httpCode);
            http.writeToStream(&USE_SERIAL);
          }
      } else {
          USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
  }
}

