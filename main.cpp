
#include <Arduino.h>
#include "RCSwitch.h"

#if 0
// send demo

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  Serial.println("Program start\n");
  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
   mySwitch.setPulseLength(320);

  // Optional set protocol (default is 1, will work for most outlets)
   mySwitch.setProtocol(1);

  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);

}

void loop() {

  /* See Example: TypeA_WithDIPSwitches */
#if 0
  mySwitch.switchOn("11111", "00010");
  delay(1000);
  mySwitch.switchOn("11111", "00010");
  delay(1000);
#endif


  /* Same switch as above, but using decimal code */
#if 1
  mySwitch.send(5393, 24);
  delay(1000);
  mySwitch.send(5396, 24);
  delay(1000);
#endif
  /* Same switch as above, but using binary code */
  mySwitch.send("000000000001010100010001");
  delay(1000);
  mySwitch.send("000000000001010100010100");
  delay(1000);

  /* Same switch as above, but tri-state code */
#if 0
  mySwitch.sendTriState("00000FFF0F0F");
  delay(1000);
  mySwitch.sendTriState("00000FFF0FF0");
  delay(1000);
#endif

  //delay(20000);
}
#endif

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(1);  // Receiver on inerrupt 0 => that is pin #2
  Serial.println("monitoring start gogo song ");
}

void loop() {
  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue(),HEX );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }

    mySwitch.resetAvailable();
  }
}
