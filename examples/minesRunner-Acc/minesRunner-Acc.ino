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
#define LIMIT_TIME 30000

int level;
int state;
const Img_t mapList[MAPS_NUM] = {map0, map1, map2};
const Img_t lvlList[LVL_NUM] = {lvl1, lvl2, lvl3, win};
Panel *panel;
Map *mapp;
Ball *ball;
TimeBar *bar;
int timeout;

void startGame(void) {

    level = 0;
    // Posicion aleatoria de inicio de la pelota en fila 0
    ball = new Ball(panel, int(random(0, 15)), 1);
    mapp = new Map(panel, ball);
    //mapp->loadMap(mapList[level]);
    bar = new TimeBar(panel, 0, 0, 16, rgb(255, 255, 0));
    bar->start(LIMIT_TIME);
    roundPrint(3, lvlList[level]);
    bar->draw();
    ball->mostrar();
    panel->show();
    mapp->loadMap(mapList[level]);
}

// parpadeo tras pasar de ronda o perder
void roundPrint(int cont, Img_t list){

     int i = 0;
     while (i < cont) {
        mapp->loadMap(list);
        panel->show();
        delay(100);
        panel->clear();
        panel->show();
        delay(100);
        i++;
     }
}

void game() {
  
  ball->calculatePos();
  
  if (mapp->gameOver())
      state = 4;
  else if (mapp->gameWon()){
      level++;
      if (level < MAPS_NUM){
        ball->changePos(int(random(0, 15)), 1);
        roundPrint(3, lvlList[level]);
        mapp->loadMap(mapList[level]);
        bar->start(LIMIT_TIME);     
      } 
      else// juego terminado muestro victoria
        state = 5;
      
    } 
    else if (mapp->isOut())
      state = 4;
    
    else if (bar->isOver()) 
      state = 4;
    
      
    bar->draw();
    ball->mostrar();
    panel->show();   
}

void setup() {

  Serial.begin(9600);
  state = 0;
  panel = new Panel(LED_PIN, 16, 16, SERPENTINE, RIGHT, 0);
  panel->begin();
  panel->setBrightness(BRIGHTNESS);

}

void loop() {


  // maquina de estado del juego
  switch (state) {
    case 0:
      startGame();
      state++;
      break;
    case 1:
      // Ejecuta el juego
      game();
      break;
    case 4: // GAME OVER
      roundPrint(3, mapList[level]);
      state = 0;
      break;
    case 5: // GAME WON
      panel->image(win, 0, 0);
      panel->show();
      delay(2000);
      state = 0;
      break;
  }
}
