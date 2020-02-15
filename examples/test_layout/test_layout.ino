#include <Panel.h>

#define LED_PIN 6
#define WIDTH   16
#define HEIGHT  16

Panel panel(LED_PIN, WIDTH, HEIGHT, SERPENTINE, RIGHT, 0); // Dont mind SERPENTINE and RIGHT

void setup() {
    panel.begin();
    panel.setBrightness(30);
}

void loop() {
    panel.testLayout();
}
