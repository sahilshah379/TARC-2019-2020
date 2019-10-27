#define pin 10

const int refresh_interval = 3333;
const int pulseLower = 750;
const int pulseUpper = 2250;
const int maxDegrees = 145;

int pos = 0;
int pulse = 1500;

void setup() {
    pinMode(pin, OUTPUT);
}

void loop() {
    for (int i = 0; i < 100; i += 1) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(pin, LOW);
        delayMicroseconds(refresh_interval-pulse);
    }
}
int degreesToPulse(int degrees) {
    return map(degrees, 0, 145, pulseLower, pulseUpper);
}
