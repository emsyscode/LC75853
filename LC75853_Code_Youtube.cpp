/**********************************************************************/
/*This code is not clean and OFFCOURSE will be possible do it better! */ 
/*this is only a group of functions to be used as independent code,  */ 
/*and by this way will be possible to any person can do the changes   */ 
/*of code and see what happen.                                         */
/*The code don't depend of any external library or extenal functions  */
/*complicated.                                                         */
/*I'm let the values to sent as binnary, this allow swap bit by bit */
/* to is possible test segment by segment without convert to HEX    */
/**********************************************************************/
/* This code show how is possible work with the driver LC75853 Sanyo */
/*Begining of Auto generated code by Atmel studio */
#define VFD_in 7  // This is the pin number 7 on Arduino UNO
#define VFD_clk 8 // This is the pin number 8 on Arduino UNO
#define VFD_ce 9 // This is the pin number 9 on Arduino UNO
//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.
//*************************************************//
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_ce, OUTPUT);
 
  pinMode(13, OUTPUT);
  Serial.begin(9600); // only to debug
}
void send_char(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  // the validation of data happen when clk go from LOW to HIGH.
  digitalWrite(VFD_ce, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  digitalWrite(VFD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(VFD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(VFD_ce, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  }
}
/*********************************************************************/
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(VFD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
}
/*********************************************************************/
void send_char_8bit_stb(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
 int i = -1;
  data=a;
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
   i++;
   digitalWrite(VFD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(VFD_clk,HIGH);// 
    delayMicroseconds(1);
    if (i==7){
    Serial.println(i);
    digitalWrite(VFD_ce, HIGH);
    delayMicroseconds(1);
    }  
  }
}
/*******************************************************************/
void allON_LC75853(){
unsigned char group=0;
//send total of 56 bits (7Bytes), the 4 last 14 bits belongs to controls
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 164 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8      -9:16// 
          send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24    -25:32// 
          send_char_without(0B11111111);  
          send_char_without(0B00000011); // The firts 2 bits belongs to the group of D1 to D126  
              switch (group){ //-120:127//Last 3 bits is "DD" data direction, and is used
                case 0: send_char_without(0B00000000); break;
                case 1: send_char_without(0B01000000); break;
                case 2: send_char_without(0B10000000); break;
              }
          
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
/**************************************************************************/
void allOFF_LC75853(){
unsigned char group=0;
//send total of 64 bits, the 4 last bits belongs to set of DR, SC, BU, etc;
//The p0, p1, p2 & p3 at 0, means all pins from s1 to s12 will belongs to segments, other's settings tell will px is a port general purpose!
      for(int i=0; i<3;i++){   // Dx until 600 bits
            group=i;
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          
          send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8      -9:16// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000000);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
              switch (group){ //-120:127//Last 3 bits is "DD" data direction, and is used
                case 0: send_char_without(0B00000000); break;
                case 1: send_char_without(0B01000000); break;
                case 2: send_char_without(0B10000000); break;
              }
          
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
      }
}
/*******************************************************************/
void LC75853(void){
//
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B11111111); //   1:8      -9:16// 
          send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24    -25:32// 
          send_char_without(0B11110111);  
          send_char_without(0B00000011); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B00000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8      -9:16// 
          send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24    -25:32// 
          send_char_without(0B11111111);  
          send_char_without(0B00000011); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B01000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B11111111);  send_char_without(0B11111111); //   1:8      -9:16// 
          send_char_without(0B11111111);  send_char_without(0B11111111); //  17:24    -25:32// 
          send_char_without(0B11111111);  
          send_char_without(0B00000011); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B10000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1); 
}
/*******************************************************************/
void LC75853_numbers_0123(void){
//
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B01010000); //   1:8      -9:16// 
          send_char_without(0B10001100);  send_char_without(0B01010101); //  17:24    -25:32// 
          send_char_without(0B00000001);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B00000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B00010110); //   1:8      -9:16// 
          send_char_without(0B10001100);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000110);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B01000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8      -9:16// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000000);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B10000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1); 
}
/*******************************************************************/
void LC75853_numbers_4567(void){
//
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B01100000); //   1:8      -9:16// 
          send_char_without(0B00001100);  send_char_without(0B01011001); //  17:24    -25:32// 
          send_char_without(0B00000001);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B00000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B11000110); //   1:8      -9:16// 
          send_char_without(0B00000100);  send_char_without(0B00000011); //  17:24    -25:32// 
          send_char_without(0B00001100);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B01000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8      -9:16// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000000);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B10000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1); 
}
/*******************************************************************/
void LC75853_numbers_8900(void){
//
digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B01110000); //   1:8      -9:16// 
          send_char_without(0B11101100);  send_char_without(0B01011001); //  17:24    -25:32// 
          send_char_without(0B00000000);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B00000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B11010110); //   1:8      -9:16// 
          send_char_without(0B10001100);  send_char_without(0B00000011); //  17:24    -25:32// 
          send_char_without(0B00001110);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B01000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1);
//
      digitalWrite(VFD_ce, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000010); //(0x42) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // On the 75878 the message have first 16*8 bits more 8 times to performe 128 bits(last byte is control: 0BXXX00000)
          send_char_without(0B00000000);  send_char_without(0B00000000); //   1:8      -9:16// 
          send_char_without(0B00000000);  send_char_without(0B00000000); //  17:24    -25:32// 
          send_char_without(0B00000000);  
          send_char_without(0B00000000); // The firts 2 bits belongs to the group of D1 to D126  
          send_char_without(0B10000000);
      // to mark the 5 groups of 120 bits, 000, 001, 010, 011, 100.
      delayMicroseconds(1);
      digitalWrite(VFD_ce, LOW); // 
      delayMicroseconds(1); 
}
/*******************************************************************/
void loop() {
 //
 // Last 14(2Bytes but 2 bits is Data) bits:D41,D42,[ 0,0,0,0, S0, S1,K0,K1,P0,01,SC,DR, 0,0 ]
 allOFF_LC75853(); // All off
 delay(2000);
 allON_LC75853(); // All on
 delay(2000);
 //LC75853();  // all bits ON to check position of segments
 LC75853_numbers_0123();
 delay(1000);
 LC75853_numbers_4567();
 delay(1000);
 LC75853_numbers_8900();
 delay(1000);
}
