#ifndef Constants_h
#define Constants_h

namespace constants {
    class RocketConstants {
        public:
        static const int DECOUPLER_PIN;
        static const int CLOSE_POS;
        static const int OPEN_POS;
        static const double SEA_LEVEL_PRESSURE;
    };
    class DragConstants {
        public:
        static const double FLIGHT_TIME;
        static const double K;
        static const double mass;
        static const double gravity;
        double getFallTime(double h);
    }
}

#endif