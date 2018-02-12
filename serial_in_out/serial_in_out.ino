#define SEND_MAIL "/notify"

String data;
bool LED_STATUS = false;

void setup() {
  Serial.begin(19200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  char c;
  while(Serial.available()){
    delay(5);
    if (Serial.available() > 0) {
      c = Serial.read();
      data += c ;
    }
  }
  if(data.substring(0,8).equals("SEND TO:")){
    String serial_write = String(SEND_MAIL) + "/" + data.substring(8);
    Serial.write(serial_write.c_str());
    LED_STATUS = !LED_STATUS;
    digitalWrite(LED_BUILTIN, LED_STATUS);
  }
  data = "";
}

