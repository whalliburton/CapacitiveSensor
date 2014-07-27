
// AD7747 Capacitive Sensor
// ad7747 datasheet:http://www.analog.com/static/imported-files/data_sheets/AD7747.pdf
// Open Source Ecology

#include <Wire.h> // i2c
#include "ad7747.h"

#define PROJECT "CAP"
#define VERSION "0.4"
#define DEBUGGING

#define CAP_ADDRESS 0x48

void write_register(byte number, byte value){
#ifdef DEBUGGING
  Serial.print("write: ");
  if (number >= 0 && number <= R_LAST_REGISTER) {
    Serial.print(register_names[number]);
  } else {
    Serial.print((byte)number,HEX);
  }
  Serial.print(" ");
  Serial.println(value,HEX);
#endif
  Wire.beginTransmission(CAP_ADDRESS);
  Wire.write(number);
  Wire.write(value);
  Wire.endTransmission(0);
}

void wait_for_data(int count){
  for(int i=0;i<10;i++){
    if (Wire.available() == count){
      return;
    }
    delay(100);
  }
  Serial.print("Failure for data to appear.");
}

int read_register(byte number){
  Wire.beginTransmission(CAP_ADDRESS);
  Wire.write(number);
  Wire.endTransmission(0);
  Wire.requestFrom(CAP_ADDRESS,1);
  wait_for_data(1);
  return Wire.read();
}

void print_all_registers(){
  for(int number=R_STATUS; number<=R_VOLT_GAIN_L; number++) {
    Serial.print(register_names[number]);
    Serial.print(" ");
    Serial.println(read_register(number),HEX);
  }
}

void setup(){
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Starting " + (String)PROJECT);
  Serial.println("Version " + (String)VERSION);
  write_register(R_RESET,0x00);
  delay(5);
  write_register(R_CAP_SETUP,0xA0);
  write_register(R_EXC_SETUP,0x0E);
  write_register(R_CONFIGURATION,0x21);
  print_all_registers();
  Serial.println("Setup complete.");
}

unsigned int read_sensor(){
  Wire.beginTransmission(CAP_ADDRESS);
  Wire.write(R_CAP_DATA_H);
  Wire.endTransmission(0);
  Wire.requestFrom(CAP_ADDRESS,3);
  wait_for_data(3);
  return (Wire.read() << 16 | Wire.read() << 8 | Wire.read());
}

void loop(){
  if (read_register(R_STATUS) == 2){
    Serial.println(read_sensor());
  }
  delay(1000);
}






