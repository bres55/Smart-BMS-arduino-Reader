// 27/3/2018
// Using Arduino Mega256
// https://github.com/bres55/Smart-BMS-arduino-Reader/blob/master/README.md
/* If using UNO
     RX is digital pin 10 (connect to TX of other device)
     TX is digital pin 11 (connect to RX of other device)
     Not all pins on the Mega and Mega 2560 support change interrupts,
     so only the following can be used for RX:
     10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
    // https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
    #include <SoftwareSerial.h>                   //  un rem
    SoftwareSerial MySoftSerial(10, 11); // RX, TX  //  un rem
    #define MySerial MySoftSerial                    //  change MySerial
*/

#include <ModbusMaster.h> // https://github.com/4-20ma/ModbusMaster
#define MySerial Serial1  // set this to the hardware serial port you wish to use... 
//                           Change to, MySoftSerial, if using uno

//char inString = "";      // string to hold input
String inString = "";      // string to hold input
String inStringpc = "";    // string to hold PC input
int incomingByte, BalanceCode, Length;
// int length;
int inInts[40];           // an array to hold incoming data, not seen any longer than 34 bytes
float CellOverVP, CellOverVPR, CellUnderVP, CellUnderVPR, PackOverVP, PackOverVPR, PackUnderVP, PackUnderVPR;
float CHGOCmA, DSGOCmA, PackVoltage, PackCurrent, RemainCapacity;
int CHGOT, CHGUT, DSGOT, DSGUT, CHGOTR, CHGUTR, DSGOTR, DSGUTR;
float un1, un2, un3, un4, un5, un6, un7, un8, TempProbe1, TempProbe2, un12;
float Cellv1, Cellv2, Cellv3, Cellv4, Cellv5, Cellv6, Cellv7, Cellv8;
uint8_t result;
byte Mosfet_control, mosfetnow;


// initialise ModbusMaster object
ModbusMaster BMS_modbus; //old node

// What we would do pre and post transmission. nothing
void preTransmission() {}
void postTransmission() {}

// this is to check if we can write since rs485 is half duplex ??? not sure what this is about??
bool rs485DataReceived = true;


//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
void setup()
{
  Serial.begin(9600); // will be sending all data to serial, for later analysis

  // set the data rate for the MySerial port
  MySerial.begin(9600);

  //  Initialize Modbus communication slave ID 1, over MySerial
  BMS_modbus.begin(1, MySerial);

  // let --BMS_modbus-- know what we want for pre and post transmission,
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
    //  /* leave these for mo
    // settings
    CellOverVP = BMS_modbus.getResponseBuffer(0x00) / 1000.0f;
    Serial.print(CellOverVP);
    Serial.print(", ");

    CellOverVPR = BMS_modbus.getResponseBuffer(0x01) / 1000.f;
    Serial.print(CellOverVPR);
    Serial.print(", ");

    CellUnderVP = BMS_modbus.getResponseBuffer(0x02) / 1000.0f;
    Serial.print(CellUnderVP);
    Serial.print(", ");

    CellUnderVPR = BMS_modbus.getResponseBuffer(0x03) / 1000.0f;
    Serial.print(CellUnderVPR);
    Serial.print(", ");

    PackOverVP = BMS_modbus.getResponseBuffer(0x04) / 100.0f;
    Serial.print(PackOverVP);
    Serial.print(", ");

    PackOverVPR = BMS_modbus.getResponseBuffer(0x05) / 100.0f;
    Serial.print(PackOverVPR);
    Serial.print(", ");

    PackUnderVP = BMS_modbus.getResponseBuffer(0x06) / 100.0f;
    Serial.print(PackUnderVP);
    Serial.print(", ");

    PackUnderVPR = BMS_modbus.getResponseBuffer(0x07) / 100.0f;
    Serial.print(PackUnderVPR);
    Serial.print(", ");

    CHGOCmA = BMS_modbus.getResponseBuffer(0x10) / 100.0f;
    Serial.print(CHGOCmA);
    Serial.print(", ");

    DSGOCmA = BMS_modbus.getResponseBuffer(0x11) / 100.0f;
    Serial.print(DSGOCmA);
    Serial.print(", ");

    PackVoltage = BMS_modbus.getResponseBuffer(0x12) / 100.0f;
    Serial.print(PackVoltage);
    Serial.print(", ");

    PackCurrent = BMS_modbus.getResponseBuffer(0x13) / 100.0f;
    Serial.print(PackCurrent);
    Serial.print(", ");

    RemainCapacity = BMS_modbus.getResponseBuffer(0x14) / 100.0f;
    Serial.print(RemainCapacity);
    Serial.print(",");

    // temperatures
    // Using absolute temperature transmission, 2731+ (actual temperature *10)
    Serial.print(" Temps ");
    CHGOT = BMS_modbus.getResponseBuffer(0x08);
    CHGOT = (CHGOT - 2731) / 10;
    Serial.print(CHGOT);
    Serial.print(", ");

    CHGUT = BMS_modbus.getResponseBuffer(0x09);
    CHGUT = (CHGUT - 2731) / 10;
    Serial.print(CHGUT);
    Serial.print(", ");

    DSGOT = BMS_modbus.getResponseBuffer(0x0a);
    DSGOT = (DSGOT - 2731) / 10;
    Serial.print(DSGOT);
    Serial.print(", ");

    DSGUT = BMS_modbus.getResponseBuffer(0x0b);
    DSGUT = (DSGUT - 2731) / 10;
    Serial.print(DSGUT);
    Serial.print(", ");

    CHGOTR = BMS_modbus.getResponseBuffer(0x0c);
    CHGOTR = (CHGOTR - 2731) / 10;
    Serial.print(CHGOTR);
    Serial.print(", ");

    CHGUTR = BMS_modbus.getResponseBuffer(0x0d);
    CHGUTR = (CHGUTR - 2731) / 10;
    Serial.print(CHGUTR);
    Serial.print(", ");

    DSGOTR = BMS_modbus.getResponseBuffer(0x0e);
    DSGOTR = (DSGOTR - 2731) / 10;
    Serial.print(DSGOTR);
    Serial.print(", ");

    DSGUTR = BMS_modbus.getResponseBuffer(0x0f);
    DSGUTR = (DSGUTR - 2731) / 10;
    Serial.print(DSGUTR);
    Serial.println(", ");
    //   */

    // Cell voltages... for a 8S

    Cellv1 = BMS_modbus.getResponseBuffer(0x15) / 1000.f;
    Serial.print(" Cells 1 to 8  ");
    Serial.print(Cellv1, 3);
    Serial.print(", ");

    Cellv2 = BMS_modbus.getResponseBuffer(0x016) / 1000.f;
    Serial.print(Cellv2, 3);
    Serial.print(", ");

    Cellv3 = BMS_modbus.getResponseBuffer(0x17) / 1000.0f;
    Serial.print(Cellv3, 3);
    Serial.print(", ");

    Cellv4 = BMS_modbus.getResponseBuffer(0x18) / 1000.0f;
    Serial.print(Cellv4, 3);
    Serial.print(", ");

    Cellv5 = BMS_modbus.getResponseBuffer(0x19) / 1000.0f;
    Serial.print(Cellv5, 3);
    Serial.print(", ");

    Cellv6 = BMS_modbus.getResponseBuffer(0x1a) / 1000.0f;
    Serial.print(Cellv6, 3);
    Serial.print(", ");

    Cellv7 = BMS_modbus.getResponseBuffer(0x1b) / 1000.0f;
    Serial.print(Cellv7, 3);
    Serial.print(", ");

    Cellv8 = BMS_modbus.getResponseBuffer(0x1c) / 1000.0f;
    Serial.print(Cellv8, 3);
    Serial.print(", ");

    // unknown at moment, need to explore,
    // suspect these are used for an S16 system, since its the same sw on all the various units.
    un1 = BMS_modbus.getResponseBuffer(0x1d) / 100.0f;
    Serial.print(" Unknown ");
    Serial.print(un1);
    Serial.print(", ");

    un2 = BMS_modbus.getResponseBuffer(0x1e) / 100.0f;
    Serial.print(un2);
    Serial.print(", ");

    un3 = BMS_modbus.getResponseBuffer(0x1f) / 100.0f;
    Serial.print(un3);
    Serial.print(", ");

    un4 = BMS_modbus.getResponseBuffer(0x20) / 100.0f;
    Serial.print(un4);
    Serial.print(", ");

    un5 = BMS_modbus.getResponseBuffer(0x21) / 100.0f;
    Serial.print(un5);
    Serial.print(", ");

    un6 = BMS_modbus.getResponseBuffer(0x22) / 100.0f;
    Serial.print(un6);
    Serial.print(", ");

    un7 = BMS_modbus.getResponseBuffer(0x23) / 100.0f;
    Serial.print(un7);
    Serial.print(", ");

    un8 = BMS_modbus.getResponseBuffer(0x24) / 100.0f;
    Serial.print(un8);
    Serial.println(", ");
    //  *************************   TEMPERATURES    ***************************
    TempProbe1 = BMS_modbus.getResponseBuffer(0x25);
    TempProbe1 = (TempProbe1 - 2731) / 10;
    Serial.print("TP_P1=");
    Serial.print(TempProbe1);
    Serial.print(", ");

    TempProbe2 = BMS_modbus.getResponseBuffer(0x26);
    TempProbe2 = (TempProbe2 - 2731) / 10;
    Serial.print("TP_P2=");
    Serial.print(TempProbe2);
    Serial.print(", ");

    // MMMMMMMMMMMMM    MOSFET CONTROL  MMMMMMMMMMMMMMMMMMMMMMMM
    Mosfet_control = BMS_modbus.getResponseBuffer(0x27); // MOSFET control,, can read it ok.
    // Are we going to change it?

    // result = BMS_modbus.writeSingleRegister(0x0027, 1);  // didn't work
    // Will have to use other method to write, to mosfet control.

    un12 = BMS_modbus.getResponseBuffer(0x28) / 100.0f; //(0x0b)
    Serial.print(un12);
    Serial.print(", ");
  }
  else
  {
    rs485DataReceived = false;
    Serial.println("Error on BMS_modbus.readHoldingRegisters(0x0000, 28); ");
    // Serial.print("00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,00.00,");
  }

  // ----------------------------------------------------------------------------------------------
  //Mosfet_control
  // TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  // do we want to change it??
  getcommand(); // comes back with ...inStringpc...from IDE serial monitor, set newline option
  mosfetnow = Mosfet_control; // temp var to work on, of mosfet control

  if (inStringpc.equalsIgnoreCase("doff")) { //Discharge Mosfet, not case sensitive.
    bitClear(mosfetnow, 1);                  //  If we are wanting it off, need to set the bit 1 low/clear
  }
  if (inStringpc.equalsIgnoreCase("don")) {
    bitSet(mosfetnow, 1);                  //  If we are wanting it on, need to set the bit 1 high/set
  }
  if (inStringpc.equalsIgnoreCase("coff")) { // Charge Mosfet.
    bitClear(mosfetnow, 0);                  //  If we are wanting it off, need to set the bit 0 low/clear
  }
  if (inStringpc.equalsIgnoreCase("con")) {
    bitSet(mosfetnow, 0);                  //  If we are wanting it on, need to set the bit 0 high/set
  }
  // could do more permutations eg
  //    if (inStringpc.equalsIgnoreCase("doffcoff")) {
  //      bitClear(mosfetnow, 1)
  //      bitClear(mosfetnow, 0)
  //    }
  // but I'll see how this goes.

  if (mosfetnow != Mosfet_control) {  // if they are not equal, we are trying to make a change
    Mosfet_control = mosfetnow;       // so mosfet control can equal the new setting we want
    if (Mosfet_control == 0) {        // depending on it's vale, we call the routine
      call_control_off_off(); //  221 90 225  2  0  3 255 26 119...11
    }
    if (Mosfet_control == 1) {
      call_control_off_on(); //  221 90 225  2  0  2 255 27 119...10
    }
    if (Mosfet_control == 2) {
      call_control_on_off(); //  221 90 225  2  0  1 255 28 119...01
    }
    if (Mosfet_control == 3) {
      call_control_on_on(); //  221 90 225  2  0  0 255 29 119...00
    }

    else {
      // so mosfet control= mosfetnow, so no change
    }
  }
  // write to Mosfet control
  //                                                  Mosfet_control
  //                      Discharge   Charge  Signal  Register    Control is Active low, but register is Active high
  //  call_control_on_on();    //ON   ON      00      11
  //  call_control_on_off();   //ON   OFF     01      10          Remember, BMS conditions need to be considered too
  //  call_control_off_on();   //OFF  ON      10      01          Cannot turn on Discharge if batteries are low
  //  call_control_off_off();  //OFF  OFF     11      00          Cannot turn on Charge if batteries are already high v

  delay(500); // give it a mo to settle, otherwise modbus reads seems to get tangled
  // Show the state of MOSFET control
  Serial.println("");
  Serial.print(F("  Mosfet Ch=")); // (F) saves memeory apparently
  Mosfet_control = Mosfet_control & 1;
  Serial.print(Mosfet_control);
  Serial.print(F("  Mosfet Dis="));
  mosfetnow = mosfetnow >> 1;
  Mosfet_control = mosfetnow & 1;
  Serial.print(Mosfet_control);
  Serial.print(",           >>> ");
  // ttttttttttttttttttttttttttt   Mosfet control  end     ttttttttttttttttttttttttttttttttttttttttttt
  //  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb BALANCE CONTROL    bbbbbbbbbbbbbbb

  if (inStringpc.equalsIgnoreCase("beven")) { //balance EVEN cells
    call_Open_Even_Cells();                  //
  }
  if (inStringpc.equalsIgnoreCase("bodd")) { //balance ODD cells
    call_Open_Odd_Cells();                  //
  }
  if (inStringpc.equalsIgnoreCase("close")) { // Close, same as CLOSE on JBDTools
    call_Close_All();                         // Effectively, stops balancing, even if balancing in settings is on
  }                                           // to go back to normal run exit, doesnt last long, 5 mins?
  // not sure what triggers an exit
  if (inStringpc.equalsIgnoreCase("exit"))    // EXIT, same as EXIT on JBDTools
    call_Exit_Mode();                         // BUT, if any mosfet controls are actioned, it effectively runs EXIT
  // JBDTools has same issue.
  //  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb balance control end    bbbbbbbbbbbbbbb

  // get hardware info
  call_Hardware_info(); // requests model number etc
  get_bms_feedback();   // get that data
  Serial.print("   ");
  Serial.print(inString); // and print it out


  // get basic information
  call_Basic_info();      // requests basic info cells etc, but I dont use.
  get_bms_feedback();   // get that data, used to get BALANCE STATE byte 17-4 decimal
  Serial.print(" BC= ");
  BalanceCode = inInts[13];
  print_binary(BalanceCode, 8);// print balance state as binary, cell 1 on the right, cell 8 on left
  inString = "";


  // tidy up
  Serial.print(inStringpc); // prints a copy of commands from serial monitor. Wanted it down here out the way
  inStringpc = ""; //clear inString from getcommand
  // new line, what ever happens
  Serial.println("");
  Serial.println("");
  // How often do I want this data? Every 5,,10 seconds??
  delay(4000);
  // dont want to be over run with data!
}
// eeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnnnnnnnnnnnndddddddddddddddddddddd
//     END
// eeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnnnnnnnnnnnndddddddddddddddddddddd

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// SUB ROUTINES
// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// -------------------------------------------------------------
void call_control_on_on()
{
  control_line_l();
  //get_bms_feedback_7();
  control_on_on();        // could just change byes 6 and 8, for all four of these and just one sub routine.
  // get_bms_feedback_7();
  control_line_3();
  //get_bms_feedback_7();
}
// -------------------------------------------------------------
void call_control_on_off()
{
  control_line_l();
  //get_bms_feedback_7();
  control_on_off();
  // get_bms_feedback_7();
  control_line_3();
  //get_bms_feedback_7();
}
// -------------------------------------------------------------
void call_control_off_on()
{
  control_line_l();
  //get_bms_feedback_7();
  control_off_on();
  // get_bms_feedback_7();
  control_line_3();
  //get_bms_feedback_7();
}
//----------------------------------------------------------------------
void call_control_off_off()
{
  control_line_l();
  // get_bms_feedback_7();
  control_off_off();
  // get_bms_feedback_7();
   control_line_3();
  // get_bms_feedback_7();
}
//----------------------------------------------------------------------
void call_Open_Odd_Cells()
{
  control_line_l();
  // get_bms_feedback_7();
  Open_Odd_Cells();
  // get_bms_feedback_7();
}
//----------------------------------------------------------------------
void call_Open_Even_Cells()
{
  control_line_l();
  // get_bms_feedback_7();
  Open_Even_Cells();
  // get_bms_feedback_7();
}
//----------------------------------------------------------------------
void call_Close_All()
{
  control_line_l();
  // get_bms_feedback_7();
  Close_All();
  // get_bms_feedback_7();
}
//----------------------------------------------------------------------
void call_Exit_Mode()
{
  control_line_l();
  // get_bms_feedback_7();
  Exit_Mode();
  // get_bms_feedback_7();
}
//-------------------------------------------------------------------------------
void call_Hardware_info()
{
  flush(); // flush first
  delay(50);
  //  DD  A5 05 00  FF  FB  77
  // 221 165  5  0 255 251 119
  uint8_t data[7] = {221, 165, 5, 0, 255, 251, 119};
  MySerial.write(data, 7);
}
//-------------------------------------------------------------------------
void call_Basic_info()
{
  flush(); // flush first
  delay(50);
  //  DD  A5 03 00  FF  FD  77
  // 221 165  3  0 255 253 119
  uint8_t data[7] = {221, 165, 3, 0, 255, 253, 119};
  MySerial.write(data, 7);
}
//-------------------------------------------------------------------------
void control_line_l()
{
  flush(); // flush first
  delay(50);
  //   DD 5A 00  02 56  78  FF 30   77
  uint8_t data[9] = {221, 90, 0, 2, 86, 120, 255, 48, 119};
  MySerial.write(data, 9);
}
//----------------------------------------------------------------------------
void control_line_3()
{
  flush(); // flush first
  delay(50);
  //   DD 5A 01  02 00 00   FF  FD 77
  uint8_t data[9] = {221, 90, 1, 2, 0, 0, 255, 253, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void control_on_on()
{
  flush(); // flush first
  delay(50);
  //   DD 5A  E1 02 00 00 FF  1D  77
  uint8_t data[9] = {221, 90, 225, 2, 0, 0, 255, 29, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void control_on_off()
{
  flush(); // flush first
  delay(50);
  //   DD  5A  E1  2 0 1 FF  1C  77
  uint8_t data[9] = {221, 90, 225, 2, 0, 1, 255, 28, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void control_off_on()
{
  flush(); // flush first
  delay(50);
  //   DD  5A  E1  2 0 2 FF  1B  77
  uint8_t data[9] = {221, 90, 225, 2, 0, 2, 255, 27, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void control_off_off()
{
  flush(); // flush first
  delay(50);
  //   DD 5A  E1 02 00 03  FF 1A  77
  //  221 90 225  2  0  3 255 26 119
  uint8_t data[9] = {221, 90, 225, 2, 0, 3, 255, 26, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void Open_Odd_Cells()     // balancing
{
  flush(); // flush first
  // delay(500);
  //    DD 5A E2 02 00 01 FF 1B 77
  uint8_t data[9] = {221, 90, 226, 2, 0, 1, 255, 27, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void Open_Even_Cells()    // balancing
{
  flush(); // flush first
  // delay(500);
  //   DD 5A  E2 02 00 02  FF 1A  77
  uint8_t data[9] = {221, 90, 226, 2, 0, 2, 255, 26, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void Close_All()      // balancing
{
  flush(); // flush first
  // delay(500);

  //   DD 5A  E2 02 00 03  FF 19  77
  uint8_t data[9] = {221, 90, 226, 2, 0, 3, 255, 25, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
void Exit_Mode()      // balancing
{
  flush(); // flush first
  // delay(500);
  //   DD 5A  01 02 00 00  FF  FD  77
  uint8_t data[9] = {221, 90, 1, 2, 0, 0, 255, 253, 119};
  MySerial.write(data, 9);
}
//------------------------------------------------------------------------------
void flush()
{ // FLUSH
  delay(100); // give it a mo to settle, seems to miss occasionally without this
  while (MySerial.available() > 0)
  { MySerial.read();
  }
}
//--------------------------------------------------------------------------
void get_bms_feedback()  // returns with up to date, inString= chars, inInts= numbers, Length
// can handle all now,
{
  inString = ""; // clear instring for new incoming
  delay(100); // give it a mo to settle, seems to miss occasionally without this
  if (MySerial.available() > 0) {
    {
      for (int i = 0; i < 4; i++)               // just get first 4 bytes
      {
        incomingByte = MySerial.read();
        if (i == 3)
        {                           // could look at 3rd byte, it's the ok signal
          Length = (incomingByte); // The fourth byte holds the length of data, excluding last 3 bytes checksum etc
        }
        if (Length == 0) {Length=1;} // in some responses, length=0, dont want that, so, make Length=1
      }
      for (int i = 0; i < Length; i++) {
        incomingByte = MySerial.read(); // get the rest of the data, how long it might be.
        inString += (char)incomingByte; // convert the incoming byte to a char and add it to the string
        inInts[i] = incomingByte;       // save incoming byte to array
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------

//   if (newData == true) {

//-----------------------------------------------------------------------------------------------------
// gets input from serial monitor, and returns with inStringpc, holding it
void getcommand() {
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte != '\n') {
      inStringpc += (char)incomingByte;
    }
  }
}
//-----------------------------------------------------------------------------------------------------
void print_binary(int v, int num_places)
// https://phanderson.com/arduino/arduino_display.html
{
  int mask = 0, n;
  for (n = 1; n <= num_places; n++)
  {
    mask = (mask << 1) | 0x0001;
  }
  v = v & mask;  // truncate v to specified number of places
  while (num_places)
  {
    if (v & (0x0001 << num_places - 1))
    {
      Serial.print("1");
    }
    else
    {
      Serial.print("0");
    }
    --num_places;
    if (((num_places % 4) == 0) && (num_places != 0))
    {
      Serial.print("_");
    }
  }
}
//-----------------------------------------------------------------------------------------------------
