#include <Wire.h>
#include <math.h>

#define HMC5883L_ADDRESS 0x1E // HMC5883L I2C address

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize I2C communication
  
  // Set HMC5883L to continuous measurement mode
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x02); // Mode register address
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission(true);
  
  Serial.println("HMC5883L initialized successfully!");
}

void loop() {
  // Read magnetometer data
  int16_t mx, my, mz; // Raw magnetometer values
  
  Wire.beginTransmission(HMC5883L_ADDRESS);
  Wire.write(0x03); // Start reading from register 0x03 (X-axis MSB)
  Wire.endTransmission(false);
  Wire.requestFrom(HMC5883L_ADDRESS, 6, true); // Request 6 bytes (2 bytes per axis)

  mx = Wire.read() << 8 | Wire.read();
  mz = Wire.read() << 8 | Wire.read();
  my = Wire.read() << 8 | Wire.read();

  // Calculate yaw angle (heading)
  float heading = atan2(my, mx); // Use atan2 to get the angle in radians
  if (heading < 0) {
    heading += 2 * PI; // Convert negative angles to positive (0 to 2*PI)
  }
  float headingDegrees = heading * 180 / PI; // Convert radians to degrees

  // Print magnetometer data and yaw angle
  Serial.print("Mag: ");
  Serial.print(mx);
  Serial.print(", ");
  Serial.print(my);
  Serial.print(", ");
  Serial.print(mz);
  Serial.print(" | Yaw Angle: ");
  Serial.println(headingDegrees);
  
  delay(100); // Adjust delay as needed
}
