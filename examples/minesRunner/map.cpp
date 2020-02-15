#include "map.hpp"
#include <Panel.h>

#define WINNER_COLOR panel->color(52, 229, 0)
#define BOMB_COLOR panel->color(255, 0, 0)

//// public ////

Map::Map(Panel *_panel, Ball *_ball){

    panel = _panel;
    ball = _ball;
}


void Map::loadMap(Img_t field){

    panel->image(field, 0, 0);
}

bool Map::gameOver(){

  if (panel->getPixel(ball->x, ball->y) == BOMB_COLOR){
  
      return true;
  } else{

    return false;
  }
}

bool Map::gameWon(){

    if (panel->getPixel(ball->x, ball->y) == WINNER_COLOR){
       
        return true;
     } else{

       return false;
     }
}

bool Map::isOut(){

    if ((ball->x >= panel->getWidth()) || (ball->x < 0)){

      return true;
      
    } else if ((ball->y >= panel->getHeight()) || (ball->y < 1)){

      return true;
    }

    return false;
}
