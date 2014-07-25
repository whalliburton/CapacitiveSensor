	

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

//Define the address pointers of the chip & 
#define CHIPADDRESS 0x48
#define RESET 0xBF
#define CAPSETUP 0x07
#define EXECSETUP 0x09
#define CONFIGREG 0x0A
#define CAPOFFSET 0x0B
#define CAPDATA1  0x01

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

//in any i2c interaction, first the host sends the address of the slave with which it wishes to correspond. Normally, the address of the AD7747 is 0x90 and 0x91 for read/write. 
//in arduino, we specify read/write in the wire.write or wire.recieve commands so the address gets bit shifted to the left and the last bit is dropped. The address becomes 0x48 for both read
//and write. After the first command is sent we send a register address from which to recieve data.    
   
   void sendData(byte regAddress, byte data){
     Wire.beginTransmission(CHIPADDRESS);
     Wire.write(regAddress);
     Wire.write(data);
     Wire.endTransmission();
     delay(5);
   }

//For    
   void readData(byte regAddress, int count){
     Wire.beginTransmission(CHIPADDRESS);
     Wire.write(regAddress);
     Wire.endTransmission();
     Wire.requestFrom(CHIPADDRESS, count);
     Wire.endTransmission();
     delay(5);
   }
   
   void setup(){
//sets up i2c for operation
     Wire.begin();                   
// we will monitor this via serial cable
     Serial.begin(9600);             

      sendData(RESET, 0x00);          
 //next we will access the Capacitance Setup Register and load the suggested values
      sendData(CAPSETUP, 0xA0);      
 //This is a sigma-delta converter which uses excitation as an integral part of its operation. The excitation register is initialized in this transmission using the datasheet recommended value.
      sendData(EXECSETUP, 0x0E);    
 //Configuration Register
      sendData(CONFIGREG, 0x21);     
 //Capacitance reading offset register.
      sendData(CAPOFFSET,0x80);   
//Make sure we got through the setup OK
      Serial.println("Setup Complete");        

//Print All the Values We just Sent     
      Serial.println(" ");
      
      Serial.print("CapSetup:");
      readData(CAPSETUP,1);    
      Serial.println(Wire.read(),HEX);
      Serial.print("ExecSetup:");
      readData(EXECSETUP,1);
       Serial.println(Wire.read(),HEX);
      
      Serial.print("ConfigReg:");
      readData(CONFIGREG,1);
  Serial.println(Wire.read(),HEX);
      Serial.print("CapOffset:");
      readData(CAPOFFSET,1);
  Serial.println(Wire.read(),HEX);
}
   
   
   void loop(){
        delay(1000);
        //readyReceive();
        //Serial.print("Status Register: ");
        //Serial.println(readycap,BIN);
        //if(readycap==0x02){
          //requestCapData();
          //addData();
          //scaleData();
        //}
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
  
   
        
         


