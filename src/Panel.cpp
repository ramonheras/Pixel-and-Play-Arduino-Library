#include "Panel.h"

#include <Arduino_APDS9960.h>

#define MAT_PIN 6

//// public ////


Panel::Panel(unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation){
	Panel(MAT_PIN, width, height, style, iniSide, matrixRotation);
}

Panel::Panel(int pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation){
	Panel(pin, width, height, style, iniSide, matrixRotation, NEO_GRB + NEO_KHZ800);
}

Panel::Panel(int pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation, neoPixelType stripParams) 
	:  _pin(pin), _numLeds(width*height), _width(width), _height(height), _layoutStyle(style), _iniSide(iniSide), 
		_matrixRotation(matrixRotation), _stripParams(stripParams)
{

    _autoBrightnessMin = 10;
    _autoBrightnessMax = 255;
    _autoBrightnessMode = false;

    _cData = (uint32_t*)malloc(_numLeds*sizeof(uint32_t));
    memset(_cData, _numLeds*sizeof(uint32_t), 0);
    
    _strip = new Adafruit_NeoPixel(_numLeds, _pin, _stripParams); 
}

Panel::~Panel(){
    free(_cData);
    delete _strip;  
}

bool Panel::begin(){
    _strip->begin();
    
    if (!APDS.begin())
      return false;  

    return true;
}

void Panel::setBrightness(int lvl){
    _strip->setBrightness(lvl);
}

void Panel::setAutoBrightness(int _min, int _max, bool enable){
    _autoBrightnessMin = _min;
    _autoBrightnessMax = _max;
    _autoBrightnessMode = enable;
}

bool Panel::canShow(){
    return _strip->canShow();
}

void Panel::show(){
    if(_autoBrightnessMode)
          if(APDS.colorAvailable()){
                int r, g, b;
                APDS.readColor(r, g, b);  
                setBrightness((r+g+b)/MAX_APDS_VAL*(_autoBrightnessMax-_autoBrightnessMin) + _autoBrightnessMin);
          }
  
    _strip->show();
}

bool Panel::setPixel(int x, int y, uint32_t color){
    if(calcTrans(x, y))
      return -1;
    
    unsigned pos = XY(x, y);

    _cData[pos] = color;
    _strip->setPixelColor(pos, color);
    return true;
}
bool Panel::setPixel(int x, int y){
    return setPixel(x, y, _fillColor);
}

void Panel::setPixel(uint32_t color){
    for(int i=0; i<_numLeds; i++)
        _strip->setPixelColor(i, color);
}

uint32_t Panel::getPixel(int x, int y) const{ 

    if(calcTrans(x, y))
      return -1;
      
    return _cData[XY(x, y)];
}

void Panel::clear(){
    setPixel(_strip->Color(0, 0, 0));
}

void Panel::rotate(int deg){
	deg %= 360;

	if(deg < 0)
		deg = 360 -deg;

	_rotation = deg;
}

void Panel::rotateMatrix(int deg){
	deg %= 360;

	if(deg < 0)
  	deg = 360 -deg;

	_matrixRotation = deg;
}

void Panel::drawImg(uint32_t *img, unsigned width, unsigned height, int posX, int posY){
    for(unsigned x=0; x<width; x++)
        for(unsigned y=0; y<height; y++)
            setPixel(posX+x, posY+y, *((img + y*width) + x));
}

void Panel::drawImg(Img_t img, int posX, int posY){
    for(unsigned x=0; x<img.width; x++)
        for(unsigned y=0; y<img.height; y++)
            setPixel(posX+x, posY+y, *((img.mat + y*img.width) + x));
}

void Panel::rect(int x0, int y0, int x1, int y1){
  rect(x0, y0, x1, y1, _fillColor);
}

void Panel::rect(int x0, int y0, int x1, int y1, uint32_t color){
    for(int y = y0; y<=y1; y++)
        for(int x = x0; x<=x1; x++)
            setPixel(x, y, color);        
}

void Panel::testLayout(){
  for(unsigned i=0; i<_numLeds; i++){
    _strip->setPixelColor(i, _strip->Color(random(0, 255), random(0, 255), random(0, 255)));
    show();
    delay(100);
  }//*/
}

/// private /////
unsigned Panel::XY(unsigned x, unsigned y) const{
    unsigned i;

    if(_iniSide == RIGHT)
        x = _width-1 - x;
            
    switch(_layoutStyle){
        case LINE:
            i = (y * _width) + x;
            break;
        case SERPENTINE:
            if( y & 0x01) {
              // Odd rows run backwards
              unsigned revX = (_width - 1) - x;
              i = (y * _width) + revX;
            } else {
              // Even rows run forwards
              i = (y * _width) + x;
            }
            break;
    }
      
  return i;
}

bool Panel::calcTrans(int &x, int &y) const{
    
    //calc translation <todo>
    //calc dot rotation <todo>    


    //ensure range
    if( x >= _width  || x < 0) return -1;
    if( y >= _height || y < 0) return -1;
  

    //calc matrix rotation
    if(_matrixRotation == 90){
        unsigned tx = x;
        x = _width-1-y;
        y = tx;
    }
    else if(_matrixRotation == 180){
        x = _width-1-x;
        y = _height-1-y;
    }
    else if(_matrixRotation == 270){
        unsigned tx = x;
        x = y;
        y = _height-1-tx;
    }

    return 0;
}
