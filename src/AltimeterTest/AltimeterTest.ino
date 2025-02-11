#include <Arduino.h>
#include <string.h>
#include "Altimeter.h"

rocket::Altimeter altimeter;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // Serial.println("Test");

    if (!altimeter.init()) {
        Serial.println("Failed initializing the altimeter");
        while(1);
    }
}

void loop() {
    double altitude = altimeter.altitude();
    Serial.println(altitude);
}

// int main () {
//     setup ();
//     while (1) loop();
// }