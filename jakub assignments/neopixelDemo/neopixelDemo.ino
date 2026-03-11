#include <Adafruit_NeoPixel.h>

const int neopixelsPin = 2;
const int totalPixels = 4;

Adafruit_NeoPixel pixels(totalPixels, neopixelsPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();
}

void loop() {
  for (int i = 0; i < 255; i++){
    pixels.setPixelColor(0, pixels.Color(i, 255 - i, 0));
    pixels.setPixelColor(1, pixels.Color(i, 255 - i, 0));
    pixels.setPixelColor(2, pixels.Color(i, 255 - i, 0));
    pixels.setPixelColor(3, pixels.Color(i, 255 - i, 0));
    pixels.show();
    delay(10);
  }

  for (int i = 0; i < 255; i++){
    pixels.setPixelColor(0, pixels.Color(255 - i, 0, i));
    pixels.setPixelColor(1, pixels.Color(255 - i, 0, i));
    pixels.setPixelColor(2, pixels.Color(255 - i, 0, i));
    pixels.setPixelColor(3, pixels.Color(255 - i, 0, i));
    pixels.show();
    delay(10);
  }

  for (int i = 0; i < 255; i++){
    pixels.setPixelColor(0, pixels.Color(0, i, 255 - i));
    pixels.setPixelColor(1, pixels.Color(0, i, 255 - i));
    pixels.setPixelColor(2, pixels.Color(0, i, 255 - i));
    pixels.setPixelColor(3, pixels.Color(0, i, 255 - i));
    pixels.show();
    delay(10); 
  }
}
