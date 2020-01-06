#ifndef Altimeter_h
#define Altimeter_h

#include <Arduino.h>
#include "../Constants.h"
#include "../../lib/Arduino_LPS22HB/src/Arduino_LPS22HB.h"
#include "../../lib/Arduino_HTS221/src/Arduino_HTS221.h"

namespace rocket {
    class Altimeter {
        private:
        double _p0;
        double _lastAltitude;
        
        public:
        Altimeter();
        Altimeter(double p0);

        bool init();
        double altitude();
        double velocity();
        double lastAltitude();
        void zero(double p0);
    };
}

#endif