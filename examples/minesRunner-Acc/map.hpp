#ifndef MAP_H
#define MAP_H

#include "ball.hpp"
#include <Panel.h>


class Map{
  private:
    Panel *panel;
    Ball  *ball;


  public:

    
    Map(Panel *_panel, Ball *_ball);
    void loadMap(Img_t field);
    bool gameOver();
    bool gameWon();
    bool isOut();
    
    
  
};

#endif
