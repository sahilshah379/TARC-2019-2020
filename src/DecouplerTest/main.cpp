#include <Arduino.h>
#include "Decoupler.h"

rocket::Decoupler decoupler;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    decoupler.init();
}

void loop() {
    
}

int main () {
    setup ();
    while (1) loop();
}