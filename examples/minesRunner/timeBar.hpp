#ifndef TIMEBAR_H
#define TIMEBAR_H

class TimeBar{
  private:
    
    unsigned long startTime, totalTime, endTime;
    uint32_t  barColor;
    uint8_t x, y, barLength;
    Panel *panel;

  public:
    TimeBar(Panel *_panel, uint8_t _x, uint8_t _y, uint8_t _barLength,uint32_t  _barColor) : panel(_panel), x(_x), y(_y), barLength(_barLength), barColor(_barColor){} 
    
    void start(unsigned long _totalTime){
        totalTime = _totalTime;
        startTime = millis();
        endTime = startTime + totalTime; 
    }
    bool isOver(){
        if(millis() >= endTime)
            return true;
        else 
            return false;
    }
    void draw(){
        int len = barLength - (((millis()-startTime)*barLength)/totalTime);
        
        for(int i=0; i<len; i++)
            panel->setPixel(x+i, y, barColor);

        for(int i=len; i<barLength; i++)
            panel->setPixel(x+i, y, Panel::color(0, 0, 0));
    }
};

#endif
