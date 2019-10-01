#include <Arduino_LSM9DS1.h>

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -12.6 // Declination (degrees) in Boulder, CO.

float pitch, roll, yaw;
float deltat;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float ax, ay, az, gx, gy, gz, mx, my, mz;
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    IMU.readMagneticField(mx, my, mz);

    float roll = atan2(ay, az);
    float pitch = atan2(-ax, sqrt(ay * ay + az * az));

    float heading;
    if (my == 0) {
      heading = (mx < 0) ? PI : 0;
    } else {
      heading = atan2(mx, my);
    }  
    heading -= DECLINATION * PI / 180;
    
    if (heading > PI) heading -= (2 * PI);
    else if (heading < -PI) heading += (2 * PI);
    
    heading *= 180.0 / PI;
    pitch *= 180.0 / PI;
    roll  *= 180.0 / PI;
    
    Serial.print("Pitch, Roll: ");
    Serial.print(pitch, 2);
    Serial.print(", ");
    Serial.println(roll, 2);
    Serial.print("Heading: "); Serial.println(heading, 2);
  }
  delay(5);
}
