// 6/3/2018
// Using Arduino UNO

/*
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
*/
// https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
#include <SoftwareSerial.h>

SoftwareSerial MySoftSerial(10, 11); // RX, TX

#include <ModbusMaster.h> // https://github.com/4-20ma/ModbusMaster

float CellOverVP, CellOverVPR, CellUnderVP, CellUnderVPR, PackOverVP, PackOverVPR, PackUnderVP, PackUnderVPR;
float CHGOCmA, DSGOCmA, PackVoltage, PackCurrent, RemainCapacity;
float CHGOT, CHGUT, DSGOT, DSGUT, CHGOTR, CHGUTR, DSGOTR, DSGUTR;
float un1, un2, un3, un4, un5, un6, un7, un8, un9, un10, un11, un12;
float Cellv1, Cellv2, Cellv3, Cellv4, Cellv5, Cellv6, Cellv7, Cellv8;
uint8_t result;

// initialise ModbusMaster object

ModbusMaster BMS_modbus; //old node

// What we would do pre and post transmission. nothing
// tracer requires no handshaking, suspect this is why I have had to put in delays.. letting CC settle
void preTransmission() {}
void postTransmission() {}

// this is to check if we can write since rs485 is half duplex ??? not sure what this is about??
bool rs485DataReceived = true;
//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
void setup()
{
  Serial.begin(9600); // will be sending all data to serial, for later analysis

  // set the data rate for the SoftwareSerial port
  MySoftSerial.begin(9600);

  //  Initialize Modbus communication slave ID 1, over MySoftSerial
  BMS_modbus.begin(1, MySoftSerial);

  // let --BMS_modbus-- know what we want for pre and post transmission, we call above void,
  // which was--do nothing.
  BMS_modbus.preTransmission(preTransmission);
  BMS_modbus.postTransmission(postTransmission);
}
//======================================================================================
//                 LOOP
//======================================================================================
void loop()
{
  // get settings
  // void AddressRegistry_0x0000()
  // Modbus function 0x03 Read Holding Registers.
  result = BMS_modbus.readHoldingRegisters(0x0000, 0x028);
  if (result == BMS_modbus.ku8MBSuccess)
  {
    CellOverVP = BMS_modbus.getResponseBuffer(0x00) / 1000.0f;
    Serial.print(",");
    Serial.print(CellOverVP);
    Serial.print(",");

    CellOverVPR = BMS_modbus.getResponseBuffer(0x01) / 1000.f;
    Serial.print(CellOverVPR);
    Serial.print(",");

    CellUnderVP = BMS_modbus.getResponseBuffer(0x02) / 1000.0f;
    Serial.print(CellUnderVP);
    Serial.print(",");

    CellUnderVPR = BMS_modbus.getResponseBuffer(0x03) / 1000.0f;
    Serial.print(CellUnderVPR);
    Serial.print(",");

    PackOverVP = BMS_modbus.getResponseBuffer(0x04) / 100.0f;
    Serial.print(PackOverVP);
    Serial.print(",");

    PackOverVPR = BMS_modbus.getResponseBuffer(0x05) / 100.0f;
    Serial.print(PackOverVPR);
    Serial.print(",");

    PackUnderVP = BMS_modbus.getResponseBuffer(0x06) / 100.0f;
    Serial.print(PackUnderVP);
    Serial.print(",");

    PackUnderVPR = BMS_modbus.getResponseBuffer(0x07) / 100.0f;
    Serial.print(PackUnderVPR);
    Serial.print(",");

    CHGOCmA = BMS_modbus.getResponseBuffer(0x10) / 100.0f;
    Serial.print(CHGOCmA);
    Serial.print(",");

    DSGOCmA = BMS_modbus.getResponseBuffer(0x11) / 100.0f;
    Serial.print(DSGOCmA);
    Serial.print(",");

    PackVoltage = BMS_modbus.getResponseBuffer(0x12) / 100.0f;
    Serial.print(PackVoltage);
    Serial.print(",");

    PackCurrent = BMS_modbus.getResponseBuffer(0x13) / 100.0f;
    Serial.print(PackCurrent);
    Serial.print(",");

    RemainCapacity = BMS_modbus.getResponseBuffer(0x14) / 100.0f;
    Serial.print(RemainCapacity);
    Serial.print(",,,");

    // temperatures

    CHGOT = BMS_modbus.getResponseBuffer(0x08) / 100.0f;
    Serial.print(CHGOT);
    Serial.print(",");

    CHGUT = BMS_modbus.getResponseBuffer(0x09) / 100.0f;
    Serial.print(CHGUT);
    Serial.print(",");

    DSGOT = BMS_modbus.getResponseBuffer(0x0a) / 100.0f;
    Serial.print(DSGOT);
    Serial.print(",");

    DSGUT = BMS_modbus.getResponseBuffer(0x0b) / 100.0f;
    Serial.print(DSGUT);
    Serial.print(",");

    CHGOTR = BMS_modbus.getResponseBuffer(0x0c) / 100.0f;
    Serial.print(CHGOTR);
    Serial.print(",");

    CHGUTR = BMS_modbus.getResponseBuffer(0x0d) / 100.0f;
    Serial.print(CHGUTR);
    Serial.print(",");

    DSGOTR = BMS_modbus.getResponseBuffer(0x0e) / 100.0f;
    Serial.print(DSGOTR);
    Serial.print(",");

    DSGUTR = BMS_modbus.getResponseBuffer(0x0f) / 100.0f;
    Serial.print(DSGUTR);
    Serial.print(",,,");
    
    Cellv1 = BMS_modbus.getResponseBuffer(0x15) / 1000.0f;
    Serial.print(Cellv1);
    Serial.print(",");

    Cellv2 = BMS_modbus.getResponseBuffer(0x016) / 1000.0f;
    Serial.print(Cellv2);
    Serial.print(",");

    Cellv3 = BMS_modbus.getResponseBuffer(0x17) / 1000.0f;
    Serial.print(Cellv3);
    Serial.print(",");

    Cellv4 = BMS_modbus.getResponseBuffer(0x18) / 1000.0f;
    Serial.print(Cellv4);
    Serial.print(",");

    Cellv5 = BMS_modbus.getResponseBuffer(0x19) / 1000.0f;
    Serial.print(Cellv5);
    Serial.print(",");

    Cellv6 = BMS_modbus.getResponseBuffer(0x1a) / 1000.0f;
    Serial.print(Cellv6);
    Serial.print(",");

    Cellv7 = BMS_modbus.getResponseBuffer(0x1b) / 1000.0f;
    Serial.print(Cellv7);
    Serial.print(",");

    Cellv8 = BMS_modbus.getResponseBuffer(0x1c) / 1000.0f;
    Serial.print(Cellv8);
    Serial.print(",");
    
    un1 = BMS_modbus.getResponseBuffer(0x1d) / 100.0f;
    Serial.print(un1);
    Serial.print(",");

    un2 = BMS_modbus.getResponseBuffer(0x1e) / 100.0f;
    Serial.print(un2);
    Serial.print(",");

    un3 = BMS_modbus.getResponseBuffer(0x1f) / 100.0f;
    Serial.print(un3);
    Serial.print(",");

    un4 = BMS_modbus.getResponseBuffer(0x20) / 100.0f;
    Serial.print(un4);
    Serial.print(",");

    un5 = BMS_modbus.getResponseBuffer(0x21) / 100.0f;
    Serial.print(un5);
    Serial.print(",");

    un6 = BMS_modbus.getResponseBuffer(0x22) / 100.0f;
    Serial.print(un6);
    Serial.print(",");

    un7 = BMS_modbus.getResponseBuffer(0x23) / 100.0f;
    Serial.print(un7);
    Serial.print(",");

    un8 = BMS_modbus.getResponseBuffer(0x24) / 100.0f;
    Serial.print(un8);
    Serial.print(",");

    un9 = BMS_modbus.getResponseBuffer(0x25) / 100.0f;
    Serial.print(un9);
    Serial.print(",");

    un10 = BMS_modbus.getResponseBuffer(0x26) / 100.0f;
    Serial.print(un10);
    Serial.print(",");
    
result = BMS_modbus.writeSingleRegister(0x0027, 0);

    un11 = BMS_modbus.getResponseBuffer(0x27) / 100.0f;
    Serial.print(un11);
    Serial.print(",");

    un12 = BMS_modbus.getResponseBuffer(0x28) / 100.0f; //(0x0b)
    Serial.print(un12);
    Serial.print(",");
  }
  else
  {
    rs485DataReceived = false;
    Serial.println("Error on BMS_modbus.readHoldingRegisters(0x0000, 20); ");
    // Serial.print("00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,");
  }
  // ----------------------------------------------------------------------------------------------

  /*
    // try mosfet control
    // off off
    MySoftSerial.print("dd 5a 00 02 56 78 ff 30 77");
    delay(500);
    MySoftSerial.print("dd 5a e1 02 00 00 ff 1d 77");
    delay(500);
    MySoftSerial.print("dd 5a 01 02 00 00 ff fd 77");
  */
  // rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  // new line, what ever happens
  Serial.println("");
  // How often do I want this data? Every 5,,10 seconds??
  delay(5000);
  // dont want to be over run with data!
}
// eeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnnnnnnnnnnnndddddddddddddddddddddd

