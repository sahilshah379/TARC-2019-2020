#include "Decoupler.h"

namespace rocket {
    Decoupler::Decoupler() {
        _pos = constants::RocketConstants::CLOSE_POS;
    }
    Decoupler::Decoupler(double pos) {
        _pos = pos;
    }
    void Decoupler::init() {
        decoupler.attach(constants::RocketConstants::DECOUPLER_PIN);
        decoupler.write(_pos);
    }

    void Decoupler::open() {
        _pos = constants::RocketConstants::OPEN_POS;
        decoupler.write(_pos);
    }
    void Decoupler::close() {
        _pos = constants::RocketConstants::CLOSE_POS;
        decoupler.write(_pos);
    }
    void Decoupler::toPos(double pos) {
        _pos = pos;
        decoupler.write(_pos);
    }
}