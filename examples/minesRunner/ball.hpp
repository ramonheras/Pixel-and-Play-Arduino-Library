#ifndef BALL_H
#define BALL_H

#include <Panel.h>


class Ball{
  private:
     Panel *panel;
     

    
  public:
    int x, y;
    
    Ball(Panel *_panel, int x, int y);
    void mostrar();
    bool checkGesture();
   
    
};

    
    
    
 


#endif
