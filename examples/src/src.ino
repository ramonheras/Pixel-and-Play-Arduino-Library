//this file is used as test bench 

#include "Panel.h"


#define W 16
#define H 16
#define L 16

uint32_t r = Panel::color(100, 0, 0);
uint32_t g = Panel::color(0, 100, 0);
uint32_t b = Panel::color(0, 0, 100);

void demo1();
void demo2();

Panel p(L, L, SERPENTINE, RIGHT, 0);

void setup(){
  Serial.begin(9600);
  p.begin();
  p.clear();
}


void loop(){

  //Serial.println("hola");
  demo1();

  
}

void demo1(){
  p.setPixel(0, 0, r);
    delay(500); p.show();

  p.pushMatrix();     // store current system
  p.translate(W/2, H/2);   // set a new origin
  p.setPixel(0, 0, r);    // draw a dot at the origin
    delay(500); p.show();

  p.translate(1, 1);     // another translate
  p.setPixel(0, 0, g);    // draw a dot at the origin
    delay(500); p.show();

  p.setPixel(W/3, H/3, g);  // draw 
    delay(200); p.show();
  p.rotate(90);
  p.setPixel(W/3, H/3, g);  
    delay(200); p.show();
  p.rotate(90);
  p.setPixel(W/3, H/3, g);  
    delay(200); p.show();
  p.rotate(90);
  p.setPixel(W/3, H/3, g);  
    delay(500); p.show();

  p.popMatrix();      //reacall last system
  p.setPixel(0, 0, g);  
    delay(500); p.show();
}


void demo2(){
  int T = L/2;
  
  p.fill(r);
  p.pushMatrix();
  p.translate(T, T);
  p.line(-T, 0, T,  0);
  p.line( 0, T, 0, -T);
  p.popMatrix();
  p.line(0, 0, L, L);
  p.translate(T, T);
  p.rotate(90);
  p.line(-T, -T, T, T);
  p.popMatrix();
  p.show();
  delay(500);
  p.clear();

}
