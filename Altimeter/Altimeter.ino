#include <Arduino_LPS22HB.h> // pressure
#include <Arduino_HTS221.h> // humidity & temperature

const seaLevelPressure = 1013; // https://weather.us/observations/air-pressure-station.html

void setup() {
    if (!BARO.begin()) {
        Serial.println("Failed to initialize pressure sensor!");
        while (1);
    }
    if (!HTS.begin()) {
        Serial.println("Failed to initialize humidity temperature sensor!");
        while (1);
    }
}

void loop() {
    float temperature = HTS.readTemperature(); // temperature (celsius)
    float pressure = BARO.readPressure() * 10; // pressure (hPa)
    float altitude = ((pow((seaLevelPressure/pressure),(1/5.257))-1)*(temperature+273.15))/0.0065
    print(altitude)
}
