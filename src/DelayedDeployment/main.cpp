#include <Arduino.h>
#include <string.h>
#include "Altimeter.h"
#include "Decoupler.h"
rocket::Altimeter altimeter;
rocket::Decoupler d;

void setup() {
    if (!altimeter.init()) {
        Serial.println("Failed initializing the altimeter");
        while(1);
    }
    d.init();
}

void loop() {
    double altitude = altimeter.altitude();
    Serial.println(altitude);
}

int main () {
    setup ();
    while (1) loop();
}