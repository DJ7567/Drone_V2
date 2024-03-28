#include <Wire.h>

#define MPU6050_ADDRESS 0x68                                       // MPU6050 I2C address


void setup() 
{
  Serial.begin(115200);
  Wire.begin(); 
  Wire.setClock(400000);                                            // Initialize I2C communication
  
  // Wake up MPU6050
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x6B);                                                 // Power Management Register
  Wire.write(0);                                                    // Wake up MPU6050 (set to 0)
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU6050_ADDRESS);                                      
  Wire.write(0x1B);                                                 //We want to write to the GYRO_CONFIG register (1B hex)
  Wire.write(0x08);                                                 //Set the register bits as 00001000 (500dps full scale)
  Wire.endTransmission(true);                                                    

  Wire.beginTransmission(MPU6050_ADDRESS);                           
  Wire.write(0x1C);                                                  //We want to write to the ACCEL_CONFIG register (1A hex)
  Wire.write(0x10);                                                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);                                        

  Wire.beginTransmission(MPU6050_ADDRESS);                                      
  Wire.write(0x1A);                                                  //We want to write to the CONFIG register (1A hex)
  Wire.write(0x00);                                                  //Set the register bits as 00000000 (Set Digital Low Pass Filter to ~260Hz)
  Wire.endTransmission();        
                                                                                            
  Serial.println("MPU6050 initialized successfully!");

}

void loop() {
  // Read accelerometer data
  int16_t ax, ay, az; // Raw accelerometer values
  int16_t gx, gy, gz; // Raw gyroscope values
  int16_t temp;       // Raw temperature value

  // Read accelerometer and gyroscope data
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(0x3B); // Start reading from register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 14, true); // Request 14 bytes (accelerometer, gyroscope, and temperature data)

  ax   = Wire.read() << 8 | Wire.read();
  ay   = Wire.read() << 8 | Wire.read();
  az   = Wire.read() << 8 | Wire.read();
  temp = Wire.read() << 8 | Wire.read();
  gx   = Wire.read() << 8 | Wire.read();
  gy   = Wire.read() << 8 | Wire.read();
  gz   = Wire.read() << 8 | Wire.read();


  uint32_t Angle_X ,Angle_Y ,Angle_Z;

  for(int i =0 ; i<2000 ;i++)
  {
    Angle_X += gx;
    Angle_Y += gy;
    Angle_Z += gz;
  }

  Angle_X /= 2000;
  Angle_Y /= 2000;
  Angle_Z /= 2000;

  
  // Convert raw temperature value to degrees Celsius
  float temperature = (temp / 340.0) + 36.53;





  // Print accelerometer, gyroscope, and temperature data

  Serial.print("Accel: ");
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.print(az);
  
  Serial.print(" | Gyro: ");
  Serial.print(Angle_X);
  Serial.print(", ");
  Serial.print(Angle_y);
  Serial.print(", ");
  Serial.print(Angle_Z);

  Serial.print(" | Temp: ");
  Serial.println(temperature);

  delay(100); // Adjust delay as needed
}
