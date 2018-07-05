// 11/6/2018
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
#define MySerial Serial3  // set this to the hardware serial port you wish to use... 
//                           Change to, MySoftSerial, if using uno

String inString = "";      // string to hold input
String inStringpc = "";    // string to hold PC input
int show = 1;    // show all data flag
int incomingByte, BalanceCode, Length, highbyte, lowbyte;
byte Mosfet_control, mosfetnow, BatteryConfigH, BatteryConfigL, bcl, bcln, bch, Checksum, switche;
uint8_t BYTE1, BYTE2, BYTE3, BYTE4, BYTE5, BYTE6, BYTE7, BYTE8, BYTE9, BYTE10;
uint8_t inInts[40], data[9];   // an array to hold incoming data, not seen any longer than 34 bytes, or 9
uint16_t a16bitvar;
float  eresultf; //Cellv1, Cellv2, Cellv3, Cellv4, Cellv5, Cellv6, Cellv7, Cellv8,
float CellMin = 5, CellMax = 0, Cellsum = 0;

//ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss
void setup()
{
  // Serial.begin(9600); // will be sending all data to serial, for later analysis
  Serial.begin(115200); // will be sending all data to serial, for later analysis
  MySerial.begin(9600);  // set the data rate for the MySerial port
}
//======================================================================================
//                 LOOP
//======================================================================================
void loop()
{
  write_request_start();// Found this helps timing issue, by saying hello, hello.
  write_request_end() ; // Or maybe it flushes out any rogue data.
  write_request_start();// Any way it works,
  write_request_end() ; // And accomodates long delays if you want them at the end.
 
  getcommand(); // comes back with ...inStringpc...from IDE serial monitor, set newline option

  // do we want to wait a while to view screen?
  // pause nn     or just disable Autoscroll
  if (inStringpc.substring(0, 5) == ("pause")) {
    String pauses = inStringpc.substring(6, 8);
    int pausel = pauses.toInt();
    delay (pausel * 1000);
  }

  if (inStringpc.equalsIgnoreCase("showoff")) { //do not show all data SHOWOFF
    show = 0;
  }
  if (inStringpc.equalsIgnoreCase("showon")) { //show all data           SHOW
    show = 1;
  }
  if (show == (1)) {

    //-------------------- EPROM READS START ----------------------------------
    // gets  POVP, POVPRelease, PUVP, PUVPRelease, COVP, COVPRelease, CUVP, CUVPRelease, CHGOC, DSGOC
    // from, bms eprom read, 10 in all

    write_request_start(); // the start of read eprom

    // POVP,
    //       HEX  20           E0
    //       DEC  32          224
    //  DD  A5    20  0   FF   E0    77
    // 221  165   32  0  255  224  119
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 32;
    BYTE6 = 224;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float POVP =  eresultf;
    Serial.print(" POVP = ");
    Serial.print(POVP);
    Serial.print("          ");

    // PUVP
    //   22  DE
    //   34  222
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 34;
    BYTE6 = 222;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float PUVP =  eresultf;
    Serial.print(" PUVP = ");
    Serial.print(PUVP);
    Serial.print("          ");

    // COVP
    //   36  220
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 36;
    BYTE6 = 220;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float COVP =  eresultf / 10.0f;
    Serial.print(" COVP = ");
    Serial.print(COVP);
    Serial.print("          ");

    // CUVP
    BYTE3 = 38;
    BYTE6 = 218;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float CUVP =  eresultf / 10.0f;
    Serial.print(" CUVP = ");
    Serial.print(CUVP);
    Serial.println("   ");

    // POVPRelease,
    //   HEX  21      DF
    //   DEC  33     223
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 33;
    BYTE6 = 223;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float POVPRelease =  eresultf;
    Serial.print(" POVPRelease = ");
    Serial.print(POVPRelease);
    Serial.print("   ");

    // PUVPRelease
    //   35  221
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 35;
    BYTE6 = 221;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float PUVPRelease =  eresultf;
    Serial.print(" PUVPRelease = ");
    Serial.print(PUVPRelease);
    Serial.print("   ");

    // COVPRelease
    //   37  219
    // ONLY BYTES 3 AND 6 CHANGE
    BYTE3 = 37;
    BYTE6 = 219;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float COVPRelease =  eresultf / 10.0f;
    Serial.print(" COVPRelease = ");
    Serial.print(COVPRelease);
    Serial.print("   ");

    // CUVPRelease
    BYTE3 = 39;
    BYTE6 = 217;
    call_read_eprom(); // having called this eresultf, will hold the float value
    float CUVPRelease =  eresultf / 10.0f;
    Serial.print(" CUVPRelease = ");
    Serial.print(CUVPRelease);
    Serial.println("   ");

    // CHGOC
    BYTE3 = 40;
    BYTE6 = 216;
    call_read_eprom(); // having called this eresultf, will hold the float value
    uint16_t CHGOC =  eresultf / 1;
    Serial.print(" CHGOC = ");
    Serial.print(CHGOC);
    Serial.print("   ");

    // DSGOC
    BYTE3 = 41;
    BYTE6 = 215;
    call_read_eprom(); // having called this eresultf, will hold the float value
    uint16_t DSGOC =  (eresultf + 5) / 10; // round it up
    Serial.print(" DSGOC = ");
    Serial.print(DSGOC);
    Serial.println("   ");

    //  BatteryConfig, BALANCE ENABLE and CHARGE BALANCE CONTROL
    // long hand as different needs
    BYTE3 = 45;
    BYTE6 = 211;
    flush(); // flush first
    delay (100);
    uint8_t data1[7] = {221, 165, BYTE3, 0, 255, BYTE6, 119};
    MySerial.write(data1, 7);
    get_bms_feedback(); // get the data reply
    // highbyte = (inInts[0]); // bytes 5 and 6, is where the actual data is
    BatteryConfigH = (inInts[0]);
    BatteryConfigL = (inInts[1]);
    /*
        Serial.print("  BatteryConfigH = ");
        Serial.print( BatteryConfigH);
        Serial.print("   ");
        Serial.print("  BatteryConfigL = ");
        Serial.print( BatteryConfigL);
        Serial.print("   ");
        delay (100);
    */
    write_request_end(); // finished eprom reads
  } // end of show on/off
  delay(100);
  // -------------------      EPROM READS END    -----------------------

  //-------------------- EPROM WRITES START ----------------------------------

  // vvvvvvvvvvvvvvvvvvvvvvvvv or move end of show on/off bracket to vvvvvvvvvvvv below
  // do we want to change Balance control??
  // beoff=balance off, beon=balance on
  // cboff=charge balance off, cbon=charge balance on
  // swoff=switch off, swon=switch enabled
  // if changes made with showoff, then some values will not be updated, so turn showon
  // to allow variables to be updated.
  bch = BatteryConfigH; // this relies on showoff not being used right away, to get a real value for
  //                         BatteryconfigH and L
  //                         and if a change is made here, then the temp balance switches are reset
  bcl = BatteryConfigL;
  bcln = bcl;      // temp var to work on, of balance control

  if (inStringpc.equalsIgnoreCase("beoff")) { // Balance off
    bitClear(bcln, 2);                  //  If we are wanting it OFF, need to set the bit 2 low/clear, -4
  }
  if (inStringpc.equalsIgnoreCase("beon")) { // Balance on
    bitSet(bcln, 2);                 //  If we are wanting it ON, need to set the bit 2 high/set, +4
  }
  if (inStringpc.equalsIgnoreCase("cboff")) { // Charge Balance off
    bitClear(bcln, 3);                    //  If we are wanting it OFF, need to set the bit 3 low/clear, -8
  }
  if (inStringpc.equalsIgnoreCase("cbon")) {  // Charge Balance on
    bitSet(bcln, 3);                     //  If we are wanting it ON, need to set the bit 3 high/set, +8
  }
  if (inStringpc.equalsIgnoreCase("swoff")) { // switch off
    bitClear(bcln, 0);
  }
  if (inStringpc.equalsIgnoreCase("swon")) {  // switch on
    bitSet(bcln, 0);
  }
  // Checksum calculate
  // (data + length + command code) checksum, then Complement, then add 1, high bit first, low bit last
  if (bcln != bcl) {  // if they are not equal, we are trying to make a change
    //Serial.print(bcl);
    //Serial.print("  bcls"  );
    //  Serial.print(bcln);
    bcl = bcln;
    Checksum = bcln + 47; // 45+2+(BYTE5+BYTE6)=47+0+bcln
    Checksum = Checksum ^ B11111111; // complement it, by XOR
    Checksum = Checksum + 1;

    write_request_start();

    BYTE5 = bch, BYTE6 = bcln, BYTE8 = Checksum; // Change BYTES 5,6 and 8 with reqd data
    // DD  5A  2D  2 BYTE5 BYTE6 FF  BYTE8 77
    // 221 90  45  2 BYTE5 BYTE6 255 BYTE8 119

    uint8_t data[9] = {221, 90, 45, 2, BYTE5, BYTE6, 255, BYTE8, 119};
    MySerial.write(data, 9);

    // e_write_request_end(); // not needed
    write_request_end(); // finished eprom reads, stick with this one
  }
  //  Show the state of BALANCE control // bits 2 and 3 and 0..switch en.
  Serial.println("");
  Serial.print(F("Balance Enable = ")); // (F) saves memory apparently
  switche = bcl;
  bcl = bcl >> 2; //>> (bitshift right), move bit 2 to bit 0
  bcl = bcl & 1; //& (bitwise and) just want bit 0
  Serial.print(bcl);
  Serial.print(F("  Charge Balance = "));
  bcln = bcln >> 3; //>> (bitshift right), move bit 3 to bit 0
  bcln = bcln & 1; //& (bitwise and) just want bit 0 again
  Serial.print(bcln);
  //  Show the state of SWITCH
  Serial.print(F("  Switch State = "));
  //>> switch already in bit 0
  switche = switche & 1; //& (bitwise and) just want bit 0
  Serial.println(switche);
  Serial.println("");
  Serial.println("   beoff = balance off,    cboff = charge balance off,  swoff = switch off");
  Serial.println("   beon  = balance on,     cbon  = charge balance on,   swon  = switch enable");
  // vvvvvvvvvvvvvvvvvvvvvvvv yes here, comment out the one above, and uncomment out this one.
  // } // end of show on/off
  //-------------------- EPROM WRITES END ----------------------------------

  //CCCCCCCCCCCCCCCCCCCCCCC  CELLS VOLTAGE  CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

  //CELLS VOLTAGE 04
  call_get_cells_v();      // requests cells voltage
  get_bms_feedback();     // returns with up to date, inString= chars, inInts[]= numbers, chksum in last 2 bytes
  //                       Length (length of data string)
  //  got cell voltages, bytes 0 and 1, its 16 bit, high and low
  //  go through and print them
  // Length = Length - 2;
  Serial.println ("");
  // print headings
  for (int i = 2; i < (Length + 1); i = i + 2) {
    Serial.print (" Cell ");
    Serial.print (i / 2);
    Serial.print("  ");
  }

  Serial.print (" CellMax "); // CellMax heading
  Serial.print("  ");

  Serial.print (" CellMin "); // CellMin heading
  Serial.print("  ");

  Serial.print (" Diff "); // diference heading
  Serial.print("  ");

  Serial.print ("  Avg "); // Average heading
  Serial.print("  ");

  // and the values
  Serial.println ("");
  for (int i = 0; i < Length; i = i + 2) {
    highbyte = (inInts[i]);
    lowbyte = (inInts[i + 1]);
    uint16_t Cellnow = two_ints_into16(highbyte, lowbyte);
    float Cellnowf = Cellnow / 1000.0f; // convert to float
    Cellsum = Cellsum + Cellnowf;
    if (Cellnowf > CellMax) {   // get high and low
      CellMax = Cellnowf;
    }
    if (Cellnowf < CellMin) {
      CellMin = Cellnowf;
    }
    Serial.print(" ");
    Serial.print(Cellnowf, 3); // 3 decimal places
    Serial.print("   ");
  }

  Serial.print(" ");
  Serial.print(CellMax, 3); // 3 decimal places
  Serial.print("   ");

  Serial.print("   ");
  Serial.print(CellMin, 3); // 3 decimal places
  Serial.print("   ");

  float Celldiff = CellMax - CellMin; // difference between highest and lowest
  Serial.print("   ");
  Serial.print(Celldiff, 3); // 3 decimal places
  Serial.print("   ");

  Cellsum = Cellsum / (Length / 2); // Average of Cells
  Serial.print(" ");
  Serial.print(Cellsum, 3); // 3 decimal places
  Serial.print("   ");


  //USING BASIC INFO 03 get
  //  CELL BALANCE... info
  call_Basic_info();      // requests basic info.
  get_bms_feedback();   // get that data, used to get BALANCE STATE byte 17 less 4, decimal=byte 13
  //  Serial.print(" BC= ");
  BalanceCode = inInts[13]; //  the 13th byte
  BalanceCode = Bit_Reverse( BalanceCode ) ; // reverse the bits, so they are in same order as cells
  //  Serial.print(BalanceCode, BIN); // works, but, loses leading zeros and get confusing on screen
  print_binary(BalanceCode, 8);// print balance state as binary, cell 1 on the right, cell 8 on left
  //                                    Reversed this. 1 on left, 8 on right
  Serial.print ("  Balancing Cells =1 ");
  Serial.println(" ");

  // PACK VOLTAGE,, bytes 0 and 1, its 16 bit, high and low
  highbyte = (inInts[0]); // bytes 0 and 1
  lowbyte = (inInts[1]);
  uint16_t PackVoltage = two_ints_into16(highbyte, lowbyte);
  float PackVoltagef = PackVoltage / 100.0f; // convert to float and leave at 2 dec places
  Serial.print("Pack Voltage = ");
  Serial.print(PackVoltagef);

  // CURRENT
  highbyte = (inInts[2]); // bytes 2 and 3
  lowbyte = (inInts[3]);
  uint16_t PackCurrent = two_ints_into16(highbyte, lowbyte);
  float PackCurrentf = PackCurrent / 100.0f; // convert to float and leave at 2 dec places
  Serial.print("   Current = ");
  Serial.print(PackCurrentf);

  //REMAINING CAPACITY
  highbyte = (inInts[4]);
  lowbyte = (inInts[5]);
  uint16_t RemainCapacity = two_ints_into16(highbyte, lowbyte);
  float RemainCapacityf = RemainCapacity / 100.0f; // convert to float and leave at 2 dec places
  Serial.print("   Remaining Capacity = ");
  Serial.print(RemainCapacityf);
  Serial.print("Ah");

  //RSOC
  int RSOC = (inInts[19]);
  Serial.print("   RSOC = ");
  Serial.print(RSOC);
  Serial.print("%");

  //Temp probe 1
  highbyte = (inInts[23]);
  lowbyte = (inInts[24]);
  float Temp_probe_1 = two_ints_into16(highbyte, lowbyte);
  float Temp_probe_1f = (Temp_probe_1 - 2731) / 10.00f; // convert to float and leave at 2 dec places
  Serial.println("");
  Serial.print("Temp probe 1 = ");
  Serial.print(Temp_probe_1f);
  Serial.print(" ");

  //Temp probe 2
  highbyte = (inInts[25]);
  lowbyte = (inInts[26]);
  float Temp_probe_2 = two_ints_into16(highbyte, lowbyte);
  float Temp_probe_2f = (Temp_probe_2 - 2731) / 10.00f; // convert to float and leave at 2 dec places
  Serial.print("   Temp probe 2 = ");
  Serial.print(Temp_probe_2f);
  Serial.println(" ");

  //MOSFET FLAG.... still BASIC INFO
  //Serial.println("");
  // Serial.print("   mosfet flag= ");
  // Serial.print(inInts[20]); //  the 20th byte
  Mosfet_control = (inInts[20]);

  //  END          USING BASIC INFO 03
  // ----------------------------------------------------------------------------------------------

  // MMMMMMMMMMMMMMMMMMMMMM    Mosfet_control       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // do we want to change it??,, Has to be done after MOSFET FLAG
  // mdoff=discharge off, mdon=discharge on
  // mcoff=charge off, mcon=charge on
  //            Remember, BMS conditions need to be considered too
  //            Cannot turn on Discharge if batteries are low
  //            Cannot turn on Charge if batteries are already high V.

  mosfetnow = Mosfet_control; // temp var to work on, of mosfet control
  //                               Set required bits, without touching the other bits
  if (inStringpc.equalsIgnoreCase("mdoff")) { //Discharge Mosfet, not case sensitive.
    bitClear(mosfetnow, 1);                  //  If we are wanting it OFF, need to set the bit 1 low/clear
  }
  if (inStringpc.equalsIgnoreCase("mdon")) {
    bitSet(mosfetnow, 1);                  //  If we are wanting it ON, need to set the bit 1 high/set
  }
  if (inStringpc.equalsIgnoreCase("mcoff")) { // Charge Mosfet.
    bitClear(mosfetnow, 0);                  //  If we are wanting it OFF, need to set the bit 0 low/clear
  }
  if (inStringpc.equalsIgnoreCase("mcon")) {
    bitSet(mosfetnow, 0);                  //  If we are wanting it ON, need to set the bit 0 high/set
  }
  // this method particular to mosfet control,  write_request_start(); control_mosfet(); write_request_end
  if (mosfetnow != Mosfet_control) {  // if they are not equal, we are trying to make a change
    Mosfet_control = mosfetnow;       // so mosfet control can equal the new setting we want

    if (Mosfet_control == 0) {        // Change BYTES 6 and 8 with reqd data
      BYTE6 = 3, BYTE8 = 26;
    }
    if (Mosfet_control == 1) {
      BYTE6 = 2, BYTE8 = 27;
    }
    if (Mosfet_control == 2) {
      BYTE6 = 1, BYTE8 = 28;
    }
    if (Mosfet_control == 3) {
      BYTE6 = 0, BYTE8 = 29;
    }
    call_control_mosfet(); // writing in this sequence is how JBDTools write to mosfet control
  }

  // Show the state of MOSFET control
  Serial.println("");
  Serial.print(F("Mosfet Charge = ")); // (F) saves memory apparently
  Mosfet_control = Mosfet_control & 1; //& (bitwise and) just want bit 0
  Serial.print(Mosfet_control);
  Serial.print(F("  Mosfet DisCharge = "));
  mosfetnow = mosfetnow >> 1; //>> (bitshift right) use variabe mosfetnow, move bit 1 to bit 0
  Mosfet_control = mosfetnow & 1; //& (bitwise and) just want bit 0 again
  Serial.println(Mosfet_control);
  Serial.println("");
  Serial.println("   mcooff = charge off,    mdoff = discharge off");
  Serial.println("   mcon   = charge on,     mdon  = discharge on");

  // mmmmmmmmmmmmmmmmmmmmmmmmmm  Mosfet control  end    mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  //
  // BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB TEMP BALANCE CONTROL, ODD/EVEN Short time  BBBBBBBBBBBBBBBBBB
  // having got this working, because of limitations, not much use....
  // And if eprom reads in action, balancing is reset before it even starts!!
  // hence, need to turn showoff, to see working. but honestly not much use.
  /*
     // Because I am now reading the eprom in loop, any change here gets reset,
     // left here for legacy, may come back to it, if I find it might be useful.
     // if you want to see it working, need to have showoff, and rem out the vvvvvvvvvvvv  to vvvvvvvvv bits
    if (inStringpc.equalsIgnoreCase("exit")) {   // EXIT, same as EXIT on JBDTools
     //                                  // BUT, if any mosfet controls are actioned, it effectively runs EXIT
     //                                            JBDTools has same issue.
     write_request_start();
     write_request_end() ;  // same as exit mode codes
    }
    else if (inStringpc.equalsIgnoreCase("close")) { // Close, same as CLOSE on JBDTools
     // Effectively, stops balancing, even if balancing in settings is on
     BYTE6 = 3, BYTE8 = 25; // Change BYTES 6 and 8 with reqd data
     call_change_cells_balance();
    }
    else if (inStringpc.equalsIgnoreCase("bodd")) { //balance ODD cells
     BYTE6 = 1, BYTE8 = 27; // Change BYTES 6 and 8 with reqd data
     call_change_cells_balance();
    }
    else if (inStringpc.equalsIgnoreCase("beven")) { //balance EVEN cells
     BYTE6 = 2, BYTE8 = 26; // Change BYTES 6 and 8 with reqd data
     call_change_cells_balance();
    }
  */
  //  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb TEMP balance control end  bbbbbbbbbbbbbbbbbbbbbbb
  // dont really need this
  // HARDWARE INF 05
  // get hardware info 05, the name of it, not really useful
  //call_Hardware_info(); // requests model number etc
  // get_bms_feedback();   // get that data
  // Serial.print("   " + inString.substring(0, 18));
  //  Serial.print("BMS Name= " + inString.substring(0, 18) + "  "); // no need for now

  // tidy up
   inStringpc.toUpperCase();
  Serial.print(inStringpc); //  prints a copy of commands from serial monitor. Wanted it down here out the way
  inStringpc = "";          //  clear inString from getcommand
  inString = "";
  Cellsum = 0;
  CellMin = 5;
  CellMax = 0;
  Length = 0;
  // new line, what ever happens
  Serial.println("");
  // How often do I want this data? Every 5,,10 seconds??
  //int secs = 10;
  //for (int i = 0; i <= (secs); i ++) { // removed this for better solution at beginning
    delay(10000);
  //  write_request_start();// goto to keep it engaged,, found it doesnt like to be kept waiting too long
  //  write_request_end() ; // 5 seconds without glitch
    // but, dont want to be over run with data!
  //}
}
// eeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnnnnnnnnnnnndddddddddddddddddddddd
//     END
// eeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnnnnnnnnnnnndddddddddddddddddddddd

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
// SUB ROUTINES
// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
//-------------------------------------------------------------------------------
uint8_t call_read_eprom()
{
  flush(); // flush first
  // BYTES 3 and 6 need to be set first
  uint8_t data1[7] = {221, 165, BYTE3, 0, 255, BYTE6, 119};
  MySerial.write(data1, 7);
  get_bms_feedback(); // get the data reply
  highbyte = (inInts[0]); // bytes 5 and 6, is where the actual data is
  lowbyte = (inInts[1]);
  uint16_t eresult = two_ints_into16(highbyte, lowbyte); // TURN THEM INTO ONE LONG INTEGER
  eresultf = eresult / 100.0f; // convert to float

  // Serial.print(" eresultf = ");
  // Serial.print(eresultf);
  // Serial.print("   ");

  //return eresultf;
}
//------------------------------------------------------------------------------------------
//  uint16_t PackCurrent = two_ints_into16(highbyte, lowbyte);
uint16_t two_ints_into16(int highbyte, int lowbyte) // turns two bytes into a single long integer
{
  a16bitvar = (highbyte);
  a16bitvar <<= 8; //Left shift 8 bits,
  a16bitvar = (a16bitvar | lowbyte); //OR operation, merge the two
  return a16bitvar;
}
// ----------------------------------------------------------------------------------------------------
void call_Basic_info()
// total voltage, current, Residual capacity, Balanced state, MOSFET control status
{
  flush(); // flush first

  //  DD  A5 03 00  FF  FD  77
  // 221 165  3  0 255 253 119
  uint8_t data[7] = {221, 165, 3, 0, 255, 253, 119};
  MySerial.write(data, 7);
}
//--------------------------------------------------------------------------
void call_get_cells_v()
{
  flush(); // flush first

  // DD  A5  4 0 FF  FC  77
  // 221 165 4 0 255 252 119
  uint8_t data[7] = {221, 165, 4, 0, 255, 252, 119};
  MySerial.write(data, 7);
}
//--------------------------------------------------------------------------
void call_Hardware_info()
{
  flush(); // flush first

  //  DD  A5 05 00  FF  FB  77
  // 221 165  5  0 255 251 119
  uint8_t data[7] = {221, 165, 5, 0, 255, 251, 119};
  // uint8_t data[7] = {DD, A5, 05, 00, FF, FB, 77};
  MySerial.write(data, 7);
}
//-------------------------------------------------------------------------
void write_request_start()
{
  flush(); // flush first

  //   DD 5A 00  02 56  78  FF 30   77
  uint8_t data[9] = {221, 90, 0, 2, 86, 120, 255, 48, 119};
  MySerial.write(data, 9);
}
//----------------------------------------------------------------------------
void write_request_end()
{
  flush(); // flush first

  //   DD 5A 01  02 00 00   FF  FD 77
  uint8_t data[9] = {221, 90, 1, 2, 0, 0, 255, 253, 119};
  MySerial.write(data, 9);
}
//----------------------------------------------------------------------------
void e_write_request_end()
{
  flush(); // flush first
  delay(50);
  //   DD  5A  1 2 28  28   FF   AD   77
  //  221  90  1 2 40  40  255  173  119

  uint8_t data[9] = {221, 90, 1, 2, 40, 40, 255, 173, 119};
  MySerial.write(data, 9);
}

//-------------------------------------------------------------------------
void call_control_mosfet()  // the sequence required to control the mosfet
{
  write_request_start();
  control_mosfet();
  write_request_end();
}
//---------------------------------------------------------------------------
void control_mosfet()  //5A E1
// a unique sequence for controlling the mosfets, control by BYTE 6 and BYTE 8
// all the other BYTEs are the same.
{
  flush(); // flush first
  //delay(50);
  //   DD 5A  E1 02 00  BYTE6  FF BYTE8  77
  //  221 90 225  2  0  BYTE6 255 BYTE8 119
  uint8_t data[9] = {221, 90, 225, 2, 0, BYTE6, 255, BYTE8, 119};
  MySerial.write(data, 9);
  // Serial.write(data, 9);
}
// -------------------------------------------------------------
void call_change_cells_balance()  //the sequence required to change the balance, odd, even, close, exit
{
  write_request_start();
  change_cells_balance();
}
//----------------------------------------------------------------------
void change_cells_balance()  //5A E2
// a unique sequence for controlling the balance control, to change the balance, odd, even, close, exit
// control by BYTE 6 and BYTE 8
// all the other BYTEs are the same.
{
  flush(); // flush first
  delay(50);
  //   DD 5A E2  2  0  BYTE6  FF BYTE8  77
  //  221 90 226 2  0  BYTE6  255 BYTE8 119
  uint8_t data[9] = {221, 90, 226, 2, 0, BYTE6, 255, BYTE8, 119};
  MySerial.write(data, 9);
}
//--------------------------------------------------------------------------
/*
  void call_eprom_read_function_config()  /
  Serial.print("   Eprom test " + inString.substring(1, 12));// mjb etc bar code
*/
//-------------------------------------------------------------------------
void eprom_read()   //BAR CODE
{
  flush(); // flush first
  //delay(5);
  // SENT CODE depends on WHAT IS REQD???
  //   DD  A5  A2 0  FF 5E  77...BAR CODE
  //  221 165 162 0 255 94 119
  // uint8_t data[7] = {221, 165, 162, 0, 255, 94, 119};
  uint8_t data[7] = {221, 165, 32, 0, 255, 224, 119};
  MySerial.write(data, 7);
}

//-------------------------------------------------------------------------
void eprom_end() // no need at mo
{
  flush(); // flush first
  // delay(5);
  //DD  A5  AA  0 FF  56  77
  //221 165 170 0 255 86  119
  // from eprom read
  uint8_t data[7] = {221, 165, 170, 0, 255, 86, 119};
  MySerial.write(data, 7);
}
//------------------------------------------------------------------------------
void flush()
{ // FLUSH
  delay(100); // give it a mo to settle, seems to miss occasionally without this
  while (MySerial.available() > 0)
  { MySerial.read();
  }
  delay(50); // give it a mo to settle, seems to miss occasionally without this
}
//--------------------------------------------------------------------------
void get_bms_feedback()  // returns with up to date, inString= chars, inInts= numbers, chksum in last 2 bytes
//                          Length
//                          Data only, exclude first 3 bytes
{
  inString = ""; // clear instring for new incoming
  delay(100); // give it a mo to settle, seems to miss occasionally without this
  if (MySerial.available() > 0) {
    {
      for (int i = 0; i < 4; i++)               // just get first 4 bytes
      {
        incomingByte = MySerial.read();
        if (i == 3)
        { // could look at 3rd byte, it's the ok signal
          Length = (incomingByte); // The fourth byte holds the length of data, excluding last 3 bytes checksum etc
          // Serial.print(" inc ");
          //Serial.print(incomingByte);
        }
        if (Length == 0) {
          Length = 1; // in some responses, length=0, dont want that, so, make Length=1
        }
      }
      //  Length = Length + 2; // want to get the checksum too, for writing back, saves calculating it later
      for (int i = 0; i < Length + 2; i++) { // get the checksum in last two bytes, just in case need later
        incomingByte = MySerial.read(); // get the rest of the data, how long it might be.
        inString += (char)incomingByte; // convert the incoming byte to a char and add it to the string
        inInts[i] = incomingByte;       // save incoming byte to array as int
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------

//   if (newData == true) {

//-----------------------------------------------------------------------------------------------------
// gets input from serial monitor, and returns with inStringpc, holding it
void getcommand()
{
  // inStringpc = "";  // clear instringpc for new incoming
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte != '\n') {
      inStringpc += (char)incomingByte;
    }
  }
}

//-----------------------------------------------------------------------------------------------------
void print_binary(int v, int num_places) // prints integer in binary format, nibbles, with leading zeros
// altered a bit, but got from here,  https://phanderson.com/arduino/arduino_display.html
{
  Serial.println("");
  Serial.print("  ");
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
      Serial.print("1        ");
    }
    else
    {
      Serial.print("0        ");
    }
    --num_places;
    if (((num_places % 4) == 0) && (num_places != 0))
    {
      Serial.print("");
    }
  }
}
//-----------------------------------------------------------------------------------------------------
byte Bit_Reverse( byte x )
// http://www.nrtm.org/index.php/2013/07/25/reverse-bits-in-a-byte/
{
  //          01010101  |         10101010
  x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
  //          00110011  |         11001100
  x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
  //          00001111  |         11110000
  x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
  return x;
}
