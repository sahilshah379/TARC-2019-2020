#include <Arduino.h>
#include <string.h>
#include <time.h>
#include "Altimeter.h"
#include "Decoupler.h"
rocket::Altimeter altimeter;
rocket::Decoupler d;
bool isDescent = false;
clock_t t;
void setup() {
    if (!altimeter.init()) {
        while(1);
    }
    d.init();
    t = clock();
}

void loop() { //checks if reached/surpassed max altitude
    double altitude = altimeter.altitude();
    if(!isDescent){
        if (altitude < altimeter.getlastAltitude()){
            isDescent = true;
        }
        altimeter.setLastAltitude(altitude);
    }
    else{
        double dt = clock() - t;
    }
}
int main () {
    setup ();
    while (true) loop();
}