#include <Arduino_LSM9DS1.h>
#include <Panel.h>
#include "ball.hpp"


#define BALL_COLOR panel->color(0, 0, 255)
#define COEF 1

//// public ////

// Constructor
Ball::Ball(Panel *_panel, int x, int y){

  panel = _panel;
  this->x = x;
  this->y = y;
  this->xf = x;
  this->yf = y;
  this->lastTime = 0;

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
}


// Procedimiento que muestra la pelota por pantalla
void Ball::mostrar(){

    panel->setPixel(xant, yant , 0);
    panel->setPixel(x, y, BALL_COLOR);
    xant = x;
    yant = y;
}

void Ball::changePos(int x, int y){

    this->x = x;
    this->y = y;
    this->xf = x;
    this->yf = y;
  
}
void Ball::calculatePos(){
     unsigned long actTime = millis();
              
    if((actTime-lastTime) > 100){    
          float accelx, accely, dummy;
          lastTime = actTime;

          if (IMU.accelerationAvailable()){
              float aaccelx, aaccely;            
              IMU.readAcceleration(aaccelx, aaccely, dummy);
              accely = aaccelx*9.8;
              accelx = -aaccely*9.8;   
          }

          xf += accelx*0.1*COEF;
          yf += accely*0.1*COEF;

          x = (int)xf;
          y = (int)yf;

    }
}
