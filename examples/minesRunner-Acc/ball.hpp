#ifndef BALL_H
#define BALL_H

#include <Panel.h>


class Ball{
  private:
     Panel *panel;
     float xf, yf;
     unsigned long lastTime;
    
  public:
    int x, y, xant, yant;

    
    Ball(Panel *_panel, int x, int y);
    void calculatePos();
    //bool checkCenter();
    void changePos(int x, int y);
    void mostrar();
   
    
};

    
    
    
 


#endif
