#include "LPD8806.h"
#include "SPI.h"

int nLEDs = 4;
int clockPin = 2;
int dataPin3 = 3;
int dataPin4 = 4;
int dataPin5 = 5;
int dataPin6 = 6;
int dataPin7 = 7;
int dataPin8 = 8;

//String msg = "";
String msg = "";         // a string to hold incoming data
String input_val = "1&1|1&1|1&1|1&1|1&1|1&1$";
boolean stringComplete = false;  // whether the string is complete

LPD8806 strip1 = LPD8806(nLEDs, dataPin3, clockPin);
LPD8806 strip2 = LPD8806(nLEDs, dataPin4, clockPin);
LPD8806 strip3 = LPD8806(nLEDs, dataPin5, clockPin);
LPD8806 strip4 = LPD8806(nLEDs, dataPin6, clockPin);
LPD8806 strip5 = LPD8806(nLEDs, dataPin7, clockPin);
LPD8806 strip6 = LPD8806(nLEDs, dataPin8, clockPin);

void setup() {
  Serial.begin(9600);
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
  strip6.begin();
  
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
  strip6.show();
}

void loop() {
  Serial.println("Calling serialEvent");
  serialEvent();
  Serial.println("Done serialEvent");
  if (stringComplete) {
    input_val = msg;
    Serial.println(input_val);
    // clear the string:
    msg = "";
    stringComplete = false;
  }
  int data[6][2];
  // "881&11|920&22|960&33|990&44|1020&55|1071&66$"
  parseMessage(input_val, data);
  allChunks(10, data);
}

void allChunks(uint8_t chunk, int array[6][2]){
  for(int count = 0; count < 6; count++){
    colorChooser(chunk, array[count][0]);
  }
}//end colorChunk

void colorChooser(uint8_t chunk, int frequency){
  int i;
  uint32_t color;

  for(float fade = 5; fade < 127; fade++){
    for(int j = 0; j < 4; j++){
      if(frequency < 913){
        color = strip1.Color(fade, 0, 0);
        strip1.setPixelColor(j, color);
        strip1.show();
      }
      else if(frequency > 913 && frequency < 945){
        color = strip2.Color(0, fade, 0);
        strip2.setPixelColor(j, color);
        strip2.show();
      }
      else if(frequency > 945 && frequency < 977){
        color = strip3.Color(0, 0, fade);
        strip3.setPixelColor(j, color);
        strip3.show();
      }
      else if(frequency > 977 && frequency < 1009){
        color = strip4.Color(0, fade, fade);
        strip4.setPixelColor(j, color);
        strip4.show();
      }
      else if(frequency > 1009 && frequency < 1041){
        color = strip5.Color(fade, fade, fade);
        strip5.setPixelColor(j, color);
        strip5.show();
      }
      else{
        color = strip6.Color(fade, 0, fade);
        strip6.setPixelColor(j, color);
        strip6.show();
      }
    }//j for loop
//    delay(20);
  }//end fade loop
  delay(20);
}//end color chooser



void parseMessage(String msg, int result[6][2]) {
  int pipeDelimeter = msg.indexOf('|');
  int packetDelimeter = msg.indexOf('&');
  
  //String frequency = msg.substring(0, packetDelimeter);

  int index = 0;
  int previousPipeDelimeter = 0;
  int previousPacketDelimeter = 0;
  int frequency;
  int power;
  String debug = "";
  
  
  while (msg.indexOf('&', previousPacketDelimeter) != -1) {
    int packetDelimeter = msg.indexOf('&',previousPacketDelimeter);
    int pipeDelimeter = msg.indexOf('|', previousPipeDelimeter);
    
    debug = msg.substring(previousPipeDelimeter, packetDelimeter);
    //Serial.println(debug);
    
    frequency = debug.toInt();
    //Serial.println(frequency);
    
    debug = msg.substring(packetDelimeter+1, pipeDelimeter);
    //Serial.println(debug);

    power = debug.toInt();
    //Serial.println(power);

    result[index][0] = frequency;
    result[index][1] = power;

    previousPipeDelimeter = pipeDelimeter+1;
    previousPacketDelimeter = packetDelimeter+1;
    //Serial.println(index);
    index = index + 1;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '$') {
      stringComplete = true;
    }
    // add it to the inputString:
    msg += inChar;
  }
}
