#include "Altimeter.h"

namespace rocket {
    Altimeter::Altimeter() {
        _p0 = constants::RocketConstants::SEA_LEVEL_PRESSURE;
    }
    Altimeter::Altimeter(double p0) {
        _p0 = p0;
    }

    bool Altimeter::init() {
        if (BARO.begin() && HTS.begin()) {
            _lastAltitude=altitude();
            return true;
        }
        return false;
    }

    double Altimeter::altitude() {
        float temperature = HTS.readTemperature(); // temperature (celsius)
        float pressure = BARO.readPressure() * 10; // pressure (hPa)
        double altitude = ((pow((_p0/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065;
        return altitude;
    }

    double Altimeter::velocity() {
        double velocity = 0; //1000*(altitude-prevAlt)/((millis()-prevTime));
        return velocity;
    }

    void Altimeter::setLastAltitude(double lastAlt) {
        _lastAltitude = lastAlt;
    }
    double Altimeter::getLastAltitude() {
        return _lastAltitude;
    }

    void Altimeter::zero(double p0) {
        _p0 = p0;
    }
}