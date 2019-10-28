#define pin 10

const int refresh_interval = 3333;
const int pulse_lower = 750;
const int pulse_upper = 2250;
const int max_degrees = 145;
const int close_pulse = 1400;
const int open_pulse = 1000;

int pos = 0;

void setup() {
  pinMode(pin, OUTPUT);
}

void loop() {
    for (int i = 0; i < 120; i += 1) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(close_pulse);
        digitalWrite(pin, LOW);
        delayMicroseconds(refresh_interval-close_pulse );
    }
    delay(1000);
    for (int i = 0; i < 120; i += 1) {
        digitalWrite(pin, HIGH);
        delayMicroseconds(open_pulse);
        digitalWrite(pin, LOW);
        delayMicroseconds(refresh_interval-open_pulse);
    }
    delay(1000);
}
int degreesToPulse(int degrees) {
    return map(degrees, 0, max_degrees, pulse_lower, pulse_upper);
}
