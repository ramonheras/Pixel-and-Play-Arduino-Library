#include <Panel.h>

//// public ////

Panel::Panel(uint8_t _ledPin, uint8_t _width, uint8_t _height, bool _style, bool _iniSide) : 
    numLeds(_width*_height), width (_width), height(_height), layoutStyle(_style), iniSide(_iniSide), rotation(0)
{ 
    autoBrightnessMin = 10;
    autoBrightnessMax = 255;
    autoBrightnessMode = false;

    cData = (uint32_t*)malloc(numLeds*sizeof(uint32_t));
    memset(cData, numLeds*sizeof(uint32_t), 0);
    
    strip = new Adafruit_NeoPixel(numLeds, _ledPin, NEO_GRB + NEO_KHZ800); 
}

Panel::~Panel(){
    free(cData);
    delete strip;  
}

bool Panel::begin(){
    strip->begin();
    
    if (!APDS.begin())
      return false;  

    return true;
}

void Panel::setBrightness(int lvl){
    strip->setBrightness(lvl);
}

void Panel::setAutoBrightness(int min, int max, bool enable){
    autoBrightnessMin = min;
    autoBrightnessMax = max;
    autoBrightnessMode = enable;
}

bool Panel::canShow(){
    return strip->canShow();
}

void Panel::show(){
    if(autoBrightnessMode)
          if(APDS.colorAvailable()){
                int r, g, b;
                APDS.readColor(r, g, b);  
                setBrightness((r+g+b)/MAX_APDS_VAL*(autoBrightnessMax-autoBrightnessMin) + autoBrightnessMin);
          }
  
    strip->show();
}

bool Panel::setPixel(uint8_t x, uint8_t y, uint32_t color){
    if( x >= width  || x < 0) return false;
    if( y >= height || y < 0) return false;

    calcRotation(x, y);
    uint8_t pos = XY(x, y);

    cData[pos] = color;
    strip->setPixelColor(pos, color);
    return true;
}
bool Panel::setPixel(uint8_t x, uint8_t y){
    return setPixel(x, y, fillColor);
}

void Panel::setPixel(uint32_t color){
    for(int i=0; i<numLeds; i++)
        strip->setPixelColor(i, color);
}

uint32_t Panel::getPixel(uint8_t x, uint8_t y) const{
    if( x >= width  || x < 0) return -1;
    if( y >= height || y < 0) return -1; 

    calcRotation(x, y);
    return cData[XY(x, y)];
}

void Panel::clear(){
    setPixel(strip->Color(0, 0, 0));
}

void Panel::rotate(uint16_t deg){ //just 90, 180, 270 degrees 
    rotation = deg;
}

void Panel::drawImg(uint32_t *img, uint8_t width, uint8_t height, uint8_t posX, uint8_t posY){
    for(uint8_t x=0; x<width; x++)
        for(uint8_t y=0; y<height; y++)
            setPixel(posX+x, posY+y, *((img + y*width) + x));
}

void Panel::drawImg(Img_t img, uint8_t posX, uint8_t posY){
    for(uint8_t x=0; x<img.width; x++)
        for(uint8_t y=0; y<img.height; y++)
            setPixel(posX+x, posY+y, *((img.mat + y*img.width) + x));
}

void Panel::rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  rect(x0, y0, x1, y1, fillColor);
}

void Panel::rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint32_t color){
    for(uint8_t y = y0; y<=y1; y++)
        for(uint8_t x = x0; x<=x1; x++){
            setPixel(x, y, color);
            delay(100);
        }
  
}

void Panel::testLayout(){
  for(int i=0; i<numLeds; i++){
    strip->setPixelColor(i, strip->Color(random(0, 255), random(0, 255), random(0, 255)));
    show();
    delay(100);
  }//*/
}

/// private /////
uint16_t Panel::XY( uint8_t x, uint8_t y) const{
    uint16_t i;

    if(iniSide == RIGHT)
        x = width-1 - x;
            
    switch(layoutStyle){
        case LINE:
            i = (y * width) + x;
            break;
        case SERPENTINE:
            if( y & 0x01) {
              // Odd rows run backwards
              uint8_t reverseX = (width - 1) - x;
              i = (y * width) + reverseX;
            } else {
              // Even rows run forwards
              i = (y * width) + x;
            }
            break;
    }
      
  return i;
}

void Panel::calcRotation(uint8_t &x, uint8_t &y) const{
    if(rotation == 90){
        uint8_t tx = x;
        x = width-1-y;
        y = tx;
    }
    else if(rotation == 180){
        x = width-1-x;
        y = height-1-y;
    }
    else if(rotation == 270){
        uint8_t tx = x;
        x = y;
        y = height-1-tx;
    }
}