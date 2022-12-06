#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

void setup()
{ }

void loop()
{
  
  // run M1 motor with positive speed
  
  
  
  for (int speed = 0; speed <= 255; speed++)
  {
    motors.setM1Speed(speed);
    delay(2);
  }

  for (int speed = 255; speed >= 0; speed--)
  {
    motors.setM1Speed(speed);
    delay(2);
  }
  
  // run M1 motor with negative speed
  
  
  
  for (int speed = 0; speed >= -255; speed--)
  {
    motors.setM1Speed(speed);
    delay(2);
  }
  
  for (int speed = -255; speed <= 0; speed++)
  {
    motors.setM1Speed(speed);
    delay(2);
  }

  // run M2 motor with positive speed
  
  
  
  for (int speed = 0; speed <= 255; speed++)
  {
    motors.setM2Speed(speed);
    delay(2);
  }

  for (int speed = 255; speed >= 0; speed--)
  {
    motors.setM2Speed(speed);
    delay(2);
  }
  
  // run M2 motor with negative speed
  
 
  for (int speed = 0; speed >= -255; speed--)
  {
    motors.setM2Speed(speed);
    delay(2);
  }
  
  for (int speed = -255; speed <= 0; speed++)
  {
    motors.setM2Speed(speed);
    delay(2);
  }

  // run M1 and M2 motor with positive speed

  for (int speed = 0; speed <= 255; speed++)
  {
    motors.setM1Speed(speed);
    motors.setM2Speed(speed);
    delay(2);
  }

  for (int speed = 255; speed >= 0; speed--)
  {
    motors.setM1Speed(speed);
    motors.setM2Speed(speed);
    delay(2);
  }
  
  // run M1 and M2 motor with negative speed
  
  
  
  for (int speed = 0; speed >= -255; speed--)
  {
    motors.setM1Speed(speed);
    motors.setM2Speed(speed);
    delay(2);
  }
  
  for (int speed = -255; speed <= 0; speed++)
  {
    motors.setM1Speed(speed);
    motors.setM2Speed(speed);
    delay(2);
  }
  
  delay(500);

}
