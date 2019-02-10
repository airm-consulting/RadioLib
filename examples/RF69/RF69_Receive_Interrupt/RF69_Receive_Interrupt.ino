/*
   RadioLib RF69 Receive with Interrupts Example

   This example listens for FSK transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter 
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
*/

// include the library
#include <RadioLib.h>

// RF69 module is in slot A on the shield
RF69 rf = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);
  
  // initialize RF69 with default settings
  Serial.print(F("[RF69] Initializing ... "));
  // carrier frequency:                   434.0 MHz
  // bit rate:                            48.0 kbps
  // Rx bandwidth:                        125.0 kHz
  // frequency deviation:                 50.0 kHz
  // output power:                        13 dBm
  // sync word:                           0x2D01
  int state = rf.begin();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called 
  // when new packet is received
  rf.setDio0Action(setFlag);
  
  // start listening for packets
  Serial.print(F("[RF69] Starting to listen ... "));
  state = rf.startReceive();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // rf.standby()
  // rf.sleep()
  // rf.transmit();
  // rf.receive();
  // rf.scanChannel();
}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
    receivedFlag = false;
    
    // you can read received data as an Arduino String
    String str;
    int state = rf.readData(str);
  
    // you can also read received data as byte array
    /*
      byte byteArr[8];
      int state = lora.receive(byteArr, 8);
    */
    
    if (state == ERR_NONE) {
      // packet was successfully received
      Serial.println("[RF69] Received packet!");
  
      // print data of the packet
      Serial.print("[RF69] Data:\t\t\t");
      Serial.println(str);
    }

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  }

}
