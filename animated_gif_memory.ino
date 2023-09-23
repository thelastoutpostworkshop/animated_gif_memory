// Animated GIF with Round Display
//
// ESP32 40MHz SPI single frame rendering performance

#include <SPI.h>
#include <TFT_eSPI.h>     // Install this library with the Arduino IDE Library Manager
                          // Don't forget to configure the driver for the display!
                          
#include <AnimatedGIF.h>  // Install this library with the Arduino IDE Library Manager
AnimatedGIF gif;

// Examples images
#include "images/hyperspace.h"
#include "images/nostromo.h"
#include "images/hud_1.h"
#include "images/hud_2.h"
#include "images/hud_5.h"
#include "images/hud_6.h"
#include "images/hud_7.h"
#include "images/darthvader.h"
#include "images/x_wing.h"
#include "images/colortest.h"

// Uncomment the image to display
// You could run out of program memory if you load more than one image
// You can also increase the program memory by changing the partition of the ESP32
// See this tutorial to increase program memory: https://youtu.be/NTbiKssK8_8
//
// #define GIF_IMAGE colortest
// #define GIF_IMAGE hyperspace
// #define GIF_IMAGE nostromo
// #define GIF_IMAGE darthvader
// #define GIF_IMAGE hud_1
// #define GIF_IMAGE hud_2
// #define GIF_IMAGE hud_5
// #define GIF_IMAGE hud_6
// #define GIF_IMAGE hud_7
#define GIF_IMAGE x_wing

TFT_eSPI tft = TFT_eSPI();
void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(2);     // Adjust Rotation of your screen (0-3)
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
}

void loop()
{
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    tft.startWrite();
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
    tft.endWrite(); 
  }
}