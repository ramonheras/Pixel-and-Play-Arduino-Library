#include "ball.hpp"
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>
#include <Panel.h>


#define BALL_COLOR panel->color(0, 0, 255)


//// public ////

Ball::Ball(Panel *_panel, int x, int y){

  panel = _panel;
  this->x = x;
  this->y = y;
}


void Ball::mostrar(){

    panel->setPixel(x, y, BALL_COLOR);
}

bool Ball::checkGesture(){

  bool advance = false;
  
  if (APDS.gestureAvailable()){
    
    int gesture = APDS.readGesture();
    advance = true;

    switch (gesture) {
      case GESTURE_UP:
        this->y = this->y - 1;
        break;

      case GESTURE_DOWN:
        this->y = this->y + 1;
        break;

      case GESTURE_LEFT:
        this->x = this->x - 1;
        break;

      case GESTURE_RIGHT:
        this->x = this->x + 1;
        break;

      default:
        // ignore
        break;
    }
  }

  return advance;
}
