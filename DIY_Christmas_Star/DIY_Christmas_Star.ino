/* code for max 7219 from maxim, 
reduced and optimised for using more than one 7219 in a row,
______________________________________

 Code History:
 --------------

The original code was written for the Wiring board by:
 * Nicholas Zambetti and Dave Mellis /Interaction Design Institute Ivrea /Dec 2004
 * http://www.potemkin.org/uploads/Wiring/MAX7219.txt

First modification by:
 * Marcus Hannerstig/  K3, malmÃ¶ hÃ¶gskola /2006
 * http://www.xlab.se | http://arduino.berlios.de

This version is by:
 * tomek ness /FH-Potsdam / Feb 2007
 * http://design.fh-potsdam.de/ 

 * @acknowledgements: eric f. 

-----------------------------------

General notes: 

-if you are only using one max7219, then use the function maxSingle to control
 the little guy ---maxSingle(register (1-8), collum (0-255))

-if you are using more than one max7219, and they all should work the same, 
then use the function maxAll ---maxAll(register (1-8), collum (0-255))

-if you are using more than one max7219 and just want to change something
at one little guy, then use the function maxOne
---maxOne(Max you want to control (1== the first one), register (1-8), 
column (0-255))

During initiation, be sure to send every part to every max7219 and then
upload it.
For example, if you have five max7219's, you have to send the scanLimit 5 times
before you load it-- otherwise not every max7219 will get the data. the
function maxInUse keeps track of this, just tell it how many max7219 you are
using.
*/

int dataIn = 2;
int load = 3;
int clock = 4;

int maxInUse = 4;    //change this variable to set how many MAX7219's you'll use

int e = 0;           // just a variable

int n=0;
// define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a single max7219

  digitalWrite(load, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(load, LOW);       // and load da stuff
  digitalWrite(load,HIGH); 
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin     
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for addressing different MAX7219's, 
//while having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin     

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da stuff
  digitalWrite(load,HIGH); 
}


void setup () {

  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);

  digitalWrite(13, HIGH);  

//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}  

void loop () {
maxAll(1,8);          // ----@---
maxAll(2,74);         // -@--@-@-
maxAll(3,44);         // --@-@@--
maxAll(4,248);        // @@@@@---
maxAll(5,31);         // ---@@@@@
maxAll(6,52);         // --@@-@--
maxAll(7,82);         // -@-@--@-
maxAll(8,16);         // ---@----
delay(100);
maxAll(1,72);         // -@--@---
maxAll(2,41);         // --@-@--@
maxAll(3,26);         // ---@@-@-
maxAll(4,252);        // @@@@@@--
maxAll(5,63);         // --@@@@@@
maxAll(6,88);         // -@-@@---
maxAll(7,148);        // @--@-@--
maxAll(8,18);         // ---@--@-
delay(100);
maxAll(1,68);         // -@---@--
maxAll(2,37);         // --@--@-@
maxAll(3,218);        // @@-@@-@-
maxAll(4,60);         // --@@@@--
maxAll(5,60);         // --@@@@--
maxAll(6,91);         // -@-@@-@@
maxAll(7,164);        // @-@--@--
maxAll(8,34);         // --@---@-
delay(100);
maxAll(1,36);         // --@--@--
maxAll(2,20);         // ---@-@--
maxAll(3,217);        // @@-@@--@
maxAll(4,62);         // --@@@@@-
maxAll(5,124);        // -@@@@@--
maxAll(6,155);        // @--@@-@@
maxAll(7,40);         // --@-@---
maxAll(8,36);         // --@--@--
delay(100);

}
