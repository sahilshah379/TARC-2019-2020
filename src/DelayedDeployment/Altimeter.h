#ifndef Altimeter_h
#define Altimeter_h

#include <Arduino.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include "Constants.h"

namespace rocket {
    class Altimeter {
        private:
        double _p0;
        double _lastAltitude;
        
        public:
        Altimeter();
        Altimeter(double p0);
        void setLastAltitude(double lastAlt);
        double getLastAltitude();

        bool init();
        double altitude();
        double velocity();
        double lastAltitude();
        void zero(double p0);
    };
}

#endif