	

    //----------------------------------------------------------------------------------------
    //|     AD7747 Capacitive Sensor                                                         |
    //|  inspired by code by  MiG found at:http://forum.arduino.cc/index.php/topic,11882.0.html|                                           |
    //|                                                                                      |
    //|  Author: Aidan Williamson                  |
    //|  Written: 7/25/2014                                                                   |
    //|  ad7747 datasheet:http://www.analog.com/static/imported-files/data_sheets/AD7747.pdf |
    //|                                                                                      |
    //|                                                                                      |
    //|                                                                                      |
    //|                                                                                      |
    //-----------------------------------------------------------------------------------------
  
 
 //include the library for i2c communication   
#include <Wire.h>                 

//PIN MAPPINGS
byte readycap=0;
//We will move our 24bit capacitance data into capData from the three byte registers hi, mid, and low
byte hi=0;
byte mid=0;
byte lo=0;
//The Raw Data will be combined into one long
unsigned long capData=0;
//The raw data will be scaled by a number determined in our testing.
float scaledData=0;
    
   void setup(){
//sets up i2c for operation
     Wire.begin();                   
// we will monitor this via serial cable
     Serial.begin(9600);             
//in any i2c interaction, first the host sends the address of the slave with which it wishes to correspond. Normally, the address of the AD7747 is 0x90 and 0x91 for read/write. 
//in arduino, we specify read/write in the wire.write or wire.recieve commands so the address gets bit shifted to the left and the last bit is dropped. The address becomes 0x48 for both read
//and write. 
//After the first command is sent we send a register address from which to recieve data. First we will reset the device by sending the reset address 0xBF and then writing nothing to it
      Wire.beginTransmission(0x48);
      Wire.write(0xBF);
      Wire.write(0x00);
      Wire.endTransmission();          
 //delay for good measure
      delay(5);     
 //next we will access the Capacitance Setup Register and load the suggested values
      Wire.beginTransmission(0x48);  
      Wire.write(0x07);              
      Wire.write(0xA0);             
      Wire.endTransmission();        
      delay(5);                      
 //This is a sigma-delta converter which uses excitation as an integral part of its operation. The excitation register is initialized in this transmission using the datasheet recommended value.
      Wire.beginTransmission(0x48);   
      Wire.write(0x09);            
      Wire.write(0x0E);              
      Wire.endTransmission();
      delay(5);      
 //Configuration Register
      Wire.beginTransmission(0x48);
      Wire.write(0x0A);              
      Wire.write(0x21);              
      Wire.endTransmission();      
 //Capacitance reading offset register.
      Wire.beginTransmission(0x48);
      Wire.write(0x0B);              
      Wire.write(0x00);               
      Wire.endTransmission();      
//Make sure we got through the setup OK
      Serial.println("Setup Complete");        
//Print All the Values We just Sent     
      Serial.println(" ");
      Serial.print("CapSetup:");
      Wire.beginTransmission(0x48);  
      Wire.write(0x07);  
      Wire.endTransmission();      
      Wire.requestFrom(0x48,1);
      Serial.println(Wire.read(),HEX);                   
      delay(5);     
      Serial.print("ExecSetup:");
      Wire.beginTransmission(0x48);
      Wire.write(0x09);
      Wire.endTransmission();
      Wire.requestFrom(0x48,1);
      Serial.println(Wire.read(),HEX);                   
      delay(5);  
      Serial.print("ConfigReg:");
      Wire.beginTransmission(0x48);
      Wire.write(0x0A);
      Wire.endTransmission();
      Wire.requestFrom(0x48,1);
      Serial.println(Wire.read(),HEX);                   
      delay(5);
      Serial.print("CapOffset:");
      Wire.beginTransmission(0x48);
      Wire.write(0x0B);
      Wire.endTransmission();
      Wire.requestFrom(0x48,1);
      Serial.println(Wire.read(),HEX);                   
      delay(5);
        
}
   
   
   void loop(){
        delay(1000);
        readyReceive();
        //Serial.print("Status Register: ");
        //Serial.println(readycap,BIN);
        if(readycap==0x02){
          requestCapData();
          addData();
          //scaleData();
        }
        delay(500);
        Serial.println();
    }

    void readyReceive(){
      Wire.beginTransmission(0x48);               //talking to chip
      Wire.write(byte(0x00));                    //status register address
      Wire.endTransmission();
      Wire.requestFrom(0x48,1);                  //request status register data
      readycap=(Wire.read());
      return;
    }
    
   void requestCapData(){
 //Point to the first of the three cap data regisiters
      Wire.beginTransmission(0x48); //arduino asks for data from ad7747
      Wire.write(0x01);             //set address point to capacitive DAC register 1
      Wire.endTransmission();       //pointer is set so now we can read the data
//Request 3 bytes of data
      Wire.requestFrom(0x48,3);     //reads data from cap DAC registers 1-3
      if(Wire.available() != 0x03){
         Serial.println("Corrupted Data");
        return;
         }  
      Serial.println("Data Incoming");
      return;
   }
   void addData()
   {
     hi=Wire.read();
     mid=Wire.read();
     lo=Wire.read();
     Serial.println("hi  mid  lo");
     Serial.print(hi,HEX);Serial.print("  ");Serial.print(mid,HEX);Serial.print("  ");Serial.println(lo,HEX);
     capData=(hi<<16)+(mid<<8)+(lo);
     Serial.print("Raw Data:   ");
     Serial.println(capData,DEC);
     return;
   }
   
  /* void scaleData()
   {
     const float multiplier = (float)(0x800000*8.192);
     Serial.print("multiplier:"); Serial.println(multiplier);
     scaledData=((float)capData/(multiplier));
     Serial.print("Scaled Data:");
     Serial.print(capData,DEC);
     Serial.println();
     return;
   }
  */
  
   
        
         


