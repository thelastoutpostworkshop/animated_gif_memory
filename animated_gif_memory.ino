// Animated GIF with Round Display
//
// ESP32 40MHz SPI single frame rendering performance
// Note: no DMA performance gain on smaller images or transparent pixel GIFs
//
#define USE_DMA       // ESP32 ~1.25x single frame rendering performance boost for badgers.h
                      // Note: Do not use SPI DMA if reading GIF images from SPI SD card on same bus as TFT
#define NORMAL_SPEED  // Comment out for rame rate for render speed test
#include <SPI.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
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

// Uncomment the image to display
//
#define GIF_IMAGE hyperspace
// #define GIF_IMAGE nostromo
// #define GIF_IMAGE darthvader
// #define GIF_IMAGE hud_1
// #define GIF_IMAGE hud_2
// #define GIF_IMAGE hud_5
// #define GIF_IMAGE hud_6
// #define GIF_IMAGE hud_7
// #define GIF_IMAGE x_wing

TFT_eSPI tft = TFT_eSPI();
void setup() {
  Serial.begin(115200);
  tft.begin();
#ifdef USE_DMA
  tft.initDMA();
#endif
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
}
#ifdef NORMAL_SPEED // Render at rate that is GIF controlled
void loop()
{
  // Put your main code here, to run repeatedly:
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    tft.startWrite(); // The TFT chip slect is locked low
    while (gif.playFrame(true, NULL))
    {
      yield();
    }
    gif.close();
    tft.endWrite(); // Release TFT chip select for other SPI devices
  }
}
#else // Test maximum rendering speed
void loop()
{
  long lTime = micros();
  int iFrames = 0;
  if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
  {
    tft.startWrite(); // For DMA the TFT chip slect is locked low
    while (gif.playFrame(false, NULL))
    {
      // Each loop renders one frame
      iFrames++;
      yield();
    }
    gif.close();
    tft.endWrite(); // Release TFT chip select for other SPI devices
    lTime = micros() - lTime;
    Serial.print(iFrames / (lTime / 1000000.0));
    Serial.println(" fps");
  }
}
#endif