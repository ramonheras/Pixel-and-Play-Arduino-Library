#ifndef PANEL_H
#define PANEL_H

#include <Adafruit_NeoPixel.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>

#define BRIGHTNESS 30
#define DEFAULT_BRIGHTNESS 10
#define MAX_APDS_VAL 1000


enum{
  SERPENTINE,
  LINE
};

enum{
    LEFT,
    RIGHT 
};

typedef struct {
  const uint32_t *mat;
  const uint8_t width, height;  
} Img_t;

 
class Panel{
    private:
      Adafruit_NeoPixel *strip;
      int autoBrightnessMin, autoBrightnessMax;
      bool  layoutStyle, iniSide, autoBrightnessMode;
      uint32_t fillColor;
      uint32_t *cData;



      
      uint16_t XY( uint8_t x, uint8_t y) const;
      void calcRotation(uint8_t &x, uint8_t &y) const;

      
    public:
      uint8_t  width, height;
      int numLeds, rotation;
    
      Panel(uint8_t _ledPin, uint8_t _width, uint8_t _height, bool _style, bool _iniSide);
      ~Panel();
      bool begin();
      void setBrightness(int lvl);
      void setAutoBrightness(int min, int max, bool enable);
      bool canShow();
      void show();
      bool setPixel(uint8_t x, uint8_t y, uint32_t color);
      bool setPixel(uint8_t x, uint8_t y);
      void setPixel(uint32_t color);
      uint32_t getPixel(uint8_t x, uint8_t y) const;
      void clear();
      void rotate(uint16_t deg);
      void testLayout();
      void drawImg(Img_t img, uint8_t posX, uint8_t posY);
      void drawImg(uint32_t *img, uint8_t width, uint8_t height, uint8_t posX, uint8_t posY);
      void rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
      void rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint32_t color);

      static uint32_t color(uint16_t r, uint16_t g, uint16_t b){
          return Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::Color(r, g, b));
      }
      static uint32_t colorHSV(uint16_t hue, uint8_t sat, uint8_t val){
          return Adafruit_NeoPixel::ColorHSV(hue, sat, val);
      }
      static uint32_t colorUncorrected(uint16_t r, uint16_t g, uint16_t b){
          return Adafruit_NeoPixel::Color(r, g, b);
      }
      
};   

#endif
