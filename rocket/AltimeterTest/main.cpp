#include <Arduino.h>
#include "src/rocket/Altimeter.h"

rocket::Altimeter altimeter;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Serial.println("Test");

    // if (!altimeter.init()) {
    //     Serial.println("Failure to initialization altimeter");
    //     while(1);
    // }
}

void loop() {
    double altitude = altimeter.altitude();
    Serial.println(altitude);
}

int main () {
    setup ();
    // while (1) loop();
}