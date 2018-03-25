Reads this smart BMS, 
https://www.aliexpress.com/item/8S-60A-Li-ion-LiPo-BMS-PCM-PCB-battery-protection-board-for-8-Packs-18650-Battery/32844170262.html,
using an Arduino Mega256, but there are options within code to us an arduino UNO, with SoftwareSerial.
I am still experimenting with it. Registers found with trial and error.
Mosfet control is now available, using the serial monitor.
con= Charge Mosfet ON.
coff= Charge Mosfet OFF.
don= DisCharge Mosfet ON.
doff= DisCharge Mosfet OFF.
Temperatures of the probes now displayed, 2 of them
balance of the cells is shown, a bit cryptically at mo.
Balance control, same as JBDTools
bodd= balance odd cells
beven= balance even cells
Close, effecively stops balancing, but only as long as no Mosfet controls are actioned, otherwise it effectively runs EXIT, same issue appears on JBDTools.
exit= exit


I was provided this link from the supplier of my BMS
https://drive.google.com/file/d/0B3UXptx89r4NZ3VLTHlVS1ZGTTQ/view
So, it's an Arduino UNO
Cable from the BMS, with balance socket facing you, on the left hand side, the communication connector, is from bottom to top.
Gnd, Rx, Tx, 5v.
connect these to the arduino, Gnd to Gnd, 5v to 5v, if you use SoftwareSerial, Rx to pin 11, Tx to pin 10, but can be changed.
That way I can see the output from the Serial.print commands, and later using something like coolterm, save this data and analyse it.
This all to be used in my other project, so I wont control using serial monitor myself, I will be using other data to make the swith overs as and when required.
This program allows you to test the smart bms.



Having taken the time to format this page github, wraps it all together...why???
How do I stop that, to make this more legible??
