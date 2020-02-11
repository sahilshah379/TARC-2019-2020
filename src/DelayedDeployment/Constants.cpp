#include "Constants.h"
#include <math.h> 

namespace constants {
    const int RocketConstants::DECOUPLER_PIN = 9;
    const int RocketConstants::CLOSE_POS = 0.15;
    const int RocketConstants::OPEN_POS = 0.85;
    const double RocketConstants::SEA_LEVEL_PRESSURE = 1019; // https://weather.us/observations/pressure-qnh.html
    const double DragConstants::FLIGHT_TIME = 41; // https://rocketcontest.org/wp-content/uploads/2020-Illustrated-TARC-Handbook.pdf (40-43 seconds)
    const double DragConstants::K = 0.40222533776;
    const double DragConstants::mass = 574.83/1000; // g to kg
    const double DragConstants::gravity = 9.78; // m/s^2
    double getFallTime(double h){
        double time = (sqrt(mass/(gravity*K))*acosh(exp((h*K)/mass)));
    }
}