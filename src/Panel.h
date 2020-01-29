#ifndef PANEL_H
#define PANEL_H

#include <Adafruit_NeoPixel.h>
//#include <Arduino_LSM9DS1.h>
//#include <Arduino_APDS9960.h>

#define BRIGHTNESS 30
#define DEFAULT_BRIGHTNESS 10
#define MAX_APDS_VAL 1000


typedef enum Style_enum{
  SERPENTINE,
  LINE
};

typedef enum IniSide_enum{
    LEFT,
    RIGHT 
};

typedef struct {
  const uint32_t *mat;
  const uint8_t width, height;  
} Img_t;

 
class Panel{
    private:
      //params
      neoPixelType _stripParams;
      unsigned  _pin;
      unsigned  _width, _height, _numLeds;
      Style_enum _layoutStyle;
      IniSide_enum _iniSide;
      
      // transformations
      int _x_ref, _y_ref; 
      unsigned _matrixRotation, _rotation;
      int popX, popY, popRot, popMatrixRot;
      
      //containers
      Adafruit_NeoPixel *_strip;
      uint32_t *_cData;

      int _autoBrightnessMin, _autoBrightnessMax;
      bool _autoBrightnessMode;
      
      uint32_t _fillColor;

      
      unsigned XY( unsigned x, unsigned y) const;
      bool calcTrans(int &x, int &y) const;

      
    public:
    
      Panel(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation, neoPixelType stripParams);   
      Panel(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation); 
      Panel(unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation);     
      
      ~Panel();

      void rebuild(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation, neoPixelType stripParams);
      void rebuild(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation);
      void rebuild(unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation);
      void rebuild(unsigned width, unsigned height);

      bool begin();
      void setBrightness(int lvl);
      void setAutoBrightness(int _min, int _max, bool enable);
      bool canShow();
      void show();

      bool setPixel(int x, int y, uint32_t color);
      bool setPixel(int x, int y);
      void setPixel(uint32_t color);
      void fill(uint32_t color){
        _fillColor = color;
      }

      uint32_t getPixel(int x, int y) const;
      void clear();
      
      void pushMatrix();
      void popMatrix();
      void translate(int x, int y);
      void rotate(int deg);
      void rotateMatrix(int deg);

      void testLayout();
      
      void drawImg(Img_t img, int posX, int posY);
      void drawImg(uint32_t *img, unsigned width, unsigned height, int posX, int posY);
      void line(int x0, int y0, int x1, int y1);
      void line(int x0, int y0, int x1, int y1, uint32_t color);
      void rect(int x0, int y0, int x1, int y1);
      void rect(int x0, int y0, int x1, int y1, uint32_t color);

      static uint32_t color(uint8_t r, uint8_t g, uint8_t b){
          return Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::Color(r, g, b));
      }
      static uint32_t colorHSV(uint16_t hue, uint8_t sat, uint8_t val){
          return Adafruit_NeoPixel::ColorHSV(hue, sat, val);
      }
      static uint32_t colorUncorrected(uint8_t r, uint8_t g, uint8_t b){
          return Adafruit_NeoPixel::Color(r, g, b);
      }

      //getters <todo>

      
};   

#endif
