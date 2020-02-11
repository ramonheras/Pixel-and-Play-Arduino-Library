#include "Panel.h"

#include <Arduino_APDS9960.h>
#include <Adafruit_NeoPixel.h>

#define MAT_PIN 6

//////////////--------------------------------------------------------------------|
/// PUBLIC ///                                                                    |
//////////////--------------------------------------------------------------------|

Panel::Panel(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation, neoPixelType stripParams) 
	:  _pin(pin), _numLeds(width*height), _width(width), _height(height), _layoutStyle(style), _iniSide(iniSide), 
		_stripParams(stripParams), _rotation(0), _x_ref(0), _y_ref(0)
{

    rotateMatrix(matrixRotation);
    
    _autoBrightnessMin = 10;
    _autoBrightnessMax = 255;
    _autoBrightnessMode = false;

    _cData = (uint32_t*)malloc(_numLeds*sizeof(uint32_t));
    memset(_cData, _numLeds*sizeof(uint32_t), 0);
    
    _strip = new Adafruit_NeoPixel(_numLeds, _pin, _stripParams); 
}


Panel::Panel(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation)
  : Panel(pin, width, height, style, iniSide, matrixRotation, NEO_GRB + NEO_KHZ800) {}

Panel::Panel(unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, unsigned matrixRotation)
  : Panel(MAT_PIN, width, height, style, iniSide, matrixRotation){}

Panel::~Panel(){
    free(_cData);
    delete _strip;  
}

void Panel::rebuild(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation, neoPixelType stripParams){
    //clear
    free(_cData);
    delete _strip; 

    _width = width;
    _height = height;
    _numLeds = width*height;
  
    _iniSide = iniSide;
    _layoutStyle = style;
    _stripParams = stripParams;
    rotateMatrix(matrixRotation);
   
    _cData = (uint32_t*)malloc(_numLeds*sizeof(uint32_t));
    memset(_cData, _numLeds*sizeof(uint32_t), 0);
    
    _strip = new Adafruit_NeoPixel(_numLeds, _pin, _stripParams); 
}

void Panel::rebuild(unsigned pin, unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation){
    rebuild(pin, width, height, style, iniSide, matrixRotation, _stripParams);
}

void Panel::rebuild(unsigned width, unsigned height, Style_enum style, IniSide_enum iniSide, int matrixRotation){
    rebuild(_pin, width, height, style, iniSide, matrixRotation, _stripParams);
}

void Panel::rebuild(unsigned width, unsigned height){
    rebuild(_pin, width, height, _layoutStyle, _iniSide, _matrixRotation, _stripParams);
}

void Panel::testLayout(){
  for(unsigned i=0; i<_numLeds; ++i){
    _strip->setPixelColor(i, _strip->Color(random(0, 255), random(0, 255), random(0, 255)));
    show();
    delay(100);
  }
}

void Panel::rotateMatrix(int deg){
	deg %= 360;

	if(deg < 0)
  	deg = 360 -deg;

	_matrixRotation = deg;
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

void Panel::fill(uint32_t color){
    _fillColor = color;
}

void Panel::clear(){
    setPixel(_strip->Color(0, 0, 0));
}

void Panel::line(int x0, int y0, int x1, int y1){
    line(x0, y0, x1, y1, _fillColor);
}

void Panel::line(int x0, int y0, int x1, int y1, uint32_t color){ 
    orderPoints(x0, y0, x1, y1);

    // Draw
    if(x0 == x1)
        for (int y = y0; y <= y1; ++y)
            setPixel(x0, y, color);   
    else if(y0 == y1)
        for (int x = x0; x <= x1; ++x)
          setPixel(x, y0, color);
    else {
        int deltaX = abs(x1-x0), deltaY = (y1-y0); 
  
        if(deltaX == deltaY)
          for (int i = 0; i < deltaY; ++i)
            setPixel(x0+i, y0+i, color);
    }
}

void Panel::rect(int x0, int y0, unsigned width, unsigned height){
  rect(x0, y0, width, height, _fillColor);
}

void Panel::rect(int x0, int y0, unsigned width, unsigned height, uint32_t color){
    for(int y = y0; y<(y0+height); ++y)
        for(int x = x0; x<(x0+width); ++x)
            setPixel(x, y, color);        
}

void Panel::image(uint32_t *img, unsigned width, unsigned height, int posX, int posY){
    for(unsigned x=0; x<width; ++x)
        for(unsigned y=0; y<height; ++y)
            setPixel(posX+x, posY+y, *((img + y*width) + x));
}

void Panel::image(Img_t img, int posX, int posY){
    for(unsigned x=0; x<img.width; ++x)
        for(unsigned y=0; y<img.height; ++y)
            setPixel(posX+x, posY+y, *((img.mat + y*img.width) + x));
}

void Panel::pushMatrix(){
  popX = _x_ref;
  popY = _y_ref;
  popRot = _rotation; 
  popMatrixRot = _matrixRotation;
}

void Panel::popMatrix(){
  _x_ref = popX;
  _y_ref = popY;
  _rotation = popRot;
  _matrixRotation = popMatrixRot;
}


void Panel::translate(int x, int y){
  _x_ref += x;
  _y_ref += y;
}

void Panel::rotate(int deg){
	deg %= 360;

	_rotation += deg;

	if(_rotation < 0)
		_rotation = 360 - _rotation;
}





///////////////-------------------------------------------------------------------|
/// PRIVATE ///                                                                   |
///////////////-------------------------------------------------------------------|


inline unsigned Panel::XY(unsigned x, unsigned y) const{
    unsigned i;

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

inline bool Panel::calcTrans(int &x, int &y) const{
    
    //calc dot rotation  
    if (_rotation == 90){
        int tx = x;
    
        x = y;
        y = -tx;
    }
    else if (_rotation == 180){
        x = -x;
        y = -y;
    }
    else if (_rotation == 270){
      int tx = x;
  
      x = -y;
      y = tx;
    }  
    
    //calc translation
    x += _x_ref;
    y += _y_ref;

    //ensure range
    if( x >= _width  || x < 0) return -1;
    if( y >= _height || y < 0) return -1;

    return 0;
}

inline void Panel::orderPoints(int &x0, int &y0, int &x1, int &y1) const{
    //make sure p1 is greater/equal than p0 -> (x1 >= x0 && y1 >= y0) 
    if(x1 < x0 || y1 < y0){
        int tx = x0, ty = y0;
    
        x0 = x1;
        y0 = y1;
    
        x1 = tx;
        y1 = ty;
    }
}