#include <Arduino_LSM9DS1.h>
#include <SensorFusion.h>

SF fusion;

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
    
    deltat = fusion.deltatUpdate();
//        fusion.MahonyUpdate(gx, gy, gz, ax, ay, az, deltat);
    fusion.MadgwickUpdate(gx, gy, gz, ax, ay, az, mx, my, mz, deltat);
    
    pitch = fusion.getPitch();
    roll = fusion.getRoll();    //you could also use getRollRadians() ecc
    yaw = fusion.getYaw();
    
    Serial.print("Pitch:\t"); Serial.println(pitch);
    Serial.print("Roll:\t"); Serial.println(roll);
    Serial.print("Yaw:\t"); Serial.println(yaw);
    Serial.println();
  }
  delay(5);
}
