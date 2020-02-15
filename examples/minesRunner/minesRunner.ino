#include <Arduino_APDS9960.h>
#include <Arduino_LSM9DS1.h>
#include <Panel.h>
#include "map.hpp"
#include "ball.hpp"
#include "imageList.hpp"
#include "timeBar.hpp"


#define MAPS_NUM 3
#define LVL_NUM MAPS_NUM + 1
#define LED_PIN 6
#define LIMIT_TIME 20000

int level;
int state;
const Img_t mapList[MAPS_NUM] = {map0, map1, map2};
const Img_t lvlList[LVL_NUM] = {lvl1, lvl2, lvl3, win};
Panel panel(LED_PIN, 16, 16, SERPENTINE, RIGHT, 0);
Ball ball(&panel, int(random(0, 15)), 1);
Map mapp(&panel, &ball);
TimeBar bar(&panel, 0, 0, 16, rgb(255, 255, 0));
int posxAntBall;
int posyAntBall;
int timeout;


void roundPrint(int cont, Img_t list){

     int i = 0;
     while (i < cont) {
        mapp.loadMap(list);
        panel.show();
        delay(100);
        panel.clear();
        panel.show();
        delay(100);
        i++;
     }
}

void startGame(void) {

  level = 0;

  bar.start(LIMIT_TIME);
  roundPrint(3, lvlList[level]);
  panel.show();
  mapp.loadMap(mapList[level]);
  bar.draw();
  ball.mostrar();
  panel.show();
}


void game() {
  
  bar.draw();
  panel.show();
  posxAntBall = ball.x;
  posyAntBall = ball.y;
  if (ball.checkGesture()){
    if (mapp.gameOver()){
      
      state = 4;
      
    } else if (mapp.gameWon()){
      level++;
      if (level < MAPS_NUM){
        ball.x = int(random(0, 15));
        ball.y = 1;
        roundPrint(3, lvlList[level]);
        mapp.loadMap(mapList[level]);
        ball.mostrar();
        bar.start(LIMIT_TIME);
      } else{
        
        state = 5;
      }
        
    } else if (mapp.isOut()){

      state = 4;
    } else {
      panel.setPixel(posxAntBall, posyAntBall, rgb(0, 0, 0));
      ball.mostrar();
    }
  } else {
    ball.mostrar();
  }

  if (bar.isOver()) {

    state = 4;
  }

}

void setup() {

  Serial.begin(9600);
  state = 0;
  panel.setBrightness(BRIGHTNESS);
  if (!panel.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

}

void loop() {


  switch (state) {
    case 0:
      startGame();
      state++;
      break;
    case 1:
      game();
      break;
    case 4: // GAME OVER
      roundPrint(3, mapList[level]);
      state = 0;
      break;
    case 5: // GAME WON
      panel.image(win, 0, 0);
      panel.show();
      delay(2000);
      state = 0;
      break;
  }

}
