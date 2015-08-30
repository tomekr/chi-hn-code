#include "LPD8806.h"
#include "SPI.h"

int nLEDs = 160;
int dataPin  = 2;
int clockPin = 3;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

void setup() {
  // Start up the LED strip
  strip.begin();
  Serial.begin(9600);
  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop() {
  int data[6][2] = {{ 500, 1 }, { 1500, 2 }, { 2500, 3 }, { 3500, 4 }, { 4500, 5 }, { 5500, 6 }};
  colorChunk(10, data);
}

void colorChunk(uint8_t chunk, int array[6][2]){

  for(int count = 0; count < 6; count++){
    colorChooser(chunk, array[count][0]);
  }

}//end colorChunk

void colorChooser(uint8_t chunk, uint32_t frequency){
  int i;
  uint32_t c;

  for(float fade = 5; fade < 127; fade++){
    Serial.print(fade);
    Serial.print("\n");
    if(frequency < 1000){
      i = 0;
      c = strip.Color(fade, 0, 0);
    }
    else if(frequency > 1000 && frequency < 2000){
      i = 1;
      c = strip.Color(0, fade, 0);
    }
    else if(frequency > 2000 && frequency < 3000){
      i = 2;
      c = strip.Color(0, 0, fade);
    }
    else if(frequency > 3000 && frequency < 4000){
      i = 3;
      c = strip.Color(fade, 20/127 * fade, 0);
    }
    else if(frequency > 4000 && frequency < 5000){
      i = 4;
      c = strip.Color(fade, fade, fade);
    }
    else{
      i = 5;
      c = strip.Color(fade, 0, fade);
    }

    int startSpot = i * chunk;
    chunkChooser(startSpot, c, chunk);

    strip.show();
//    delay(0.0);
  }//end fade loop
}//end color chooser

void chunkChooser(int startSpot, uint32_t color, int chunk){
  for(int j = startSpot; j < startSpot + chunk; j++){
    strip.setPixelColor(j, color);
  }//end setPixelColor
}//chunkchooser

