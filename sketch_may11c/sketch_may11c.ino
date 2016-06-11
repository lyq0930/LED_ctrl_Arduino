#include <Adafruit_NeoPixel.h>

#define PIN 6
#define N 60
#define BRIGHTNESS 60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N, PIN, NEO_RGBW + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // for nice pink
//  for(int i = 0; i < N; i ++){
//    strip.setPixelColor(i, 25,255,25,15);
//  }
//  strip.show();
    
  // for rainbow
  //rainbow(20);
  
  //delay(100);
  fillColor(strip.Color(20, 240, 20, 20));
  strip.setPixelColor(10, 20, 20, 240, 20);
  strip.setPixelColor(15, 240, 20, 0, 20);
  strip.setPixelColor(20, 255, 230, 204, 100);
  strip.setPixelColor(25, 180, 20, 60, 20);
  strip.setPixelColor(30, 80, 180, 20, 20);
  strip.show();
}

void fillColor(uint32_t color){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void rainbow(uint8_t wait){
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }     
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
