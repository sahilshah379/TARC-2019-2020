#ifndef Decoupler_h
#define Decoupler_h

#include <Arduino.h>
#include <Servo.h>
#include "Constants.h"

namespace rocket {
    class Decoupler {
        private:
        double _pos;
        Servo decoupler;
        
        public:
        Decoupler();
        Decoupler(double pos);

        void init();
        void open();
        void close();
        void toPos(double pos);
    };
}

#endif