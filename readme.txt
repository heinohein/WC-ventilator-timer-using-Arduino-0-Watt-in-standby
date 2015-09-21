Timer for the motor of a WC ventilator using 0 Watt in standby

I wanted to create a timer for the ventilator in the WC, which consumes no electricity
when the timer is inactive. The project is based on using a Arduino Pro mini controling
a double relais "brick".

Priciple details:
The Arduino is fed by a 110-230V AC to 5V Dc transformer. This transformer is switched 
off AC by relais 1, hence 0 Watt standby. 
The motor of the WC ventilator is switched on/off by relais 2. 
When the lightswitch in the WC is switched on, it also provides power to Arduino. 
The Arduino detect by using a small 230V to 7,5 V transformer and some resistors 
(protected by a zener diode) that the lightswitch is switched off. This is the trigger 
that the timer can start counting down. 
The duration of the count down is variable adjustable by a potmeter.

PLEASE BE CAREFULL. 110-230V CAN KILL YOU!!.

Sketch details:
The map Arduino contains the sketch of the timer. All relevant variables are defined with
#define statements, and the sketch contains many comments for easy reading and amendment.
The sketch starts with a 10 seconds waiting. If the light in the WC is switched off during 
this time the motor of the ventilator will not run and the total electronic is switched off
again therefore using 0 Watt. If the motor of the ventilator is start running it will be 
minimal for 1 minute, independant of the input of the variable resistor. (to protect the 
motor).

Led of the Arduino:
off during initial 10 seconds wait
slow blinking during waiting for switching off 220V input (eg light WC)
faster blinking during count down and running of WC motor
very fast blinking after count down. This status is only visible if the 5V DC is not switched
off eg not the 0 Watt construction used.

PCB details:
The map Fritzing-schem-and-PCB contains a fritzing file. Inside this file you will find a
electric schem and the layout of a PCB. The schem and the PCB are also available as JPG 
in the map Wc-timer-pictures.

I designed a PCB based on a perfboard with copper solder rings on the bottom side. 
Only normal sized, not SMD components were used. 
I used for the actual build a two sided 5 x 7 cm perfboard, but did not use the toplayer 
solder rings. The few crossing wires on the topside are made of normal wires, as visible 
on the photo 05 and 06.

I could not find a propper footprint for the 230V to 7,5V AC transformer. So I used in the
schem a small audio transformer, and on the PCB I used via's to connect to the pins of the
actual transformer. This transformer feeds a led, so it is visible on the PCB that the light in
the WC is switched on.

The 230V to 5V DC transformer is switched on to power by plugging it in into a separate outlet.
Be aware that this outlet will only supply power when the WC light is switched on and the sketch
is running. See picture 02. 

The Arduino is connected to the PCB using 2x 12 pins female connector, eg the Arduino is removable.

Pictures:
Photo's of the final test setup before final installation can be found in map 
Wc-timer-pictures. 

Costs:
Total cost of the project ca. 20,45 eur/$. For details see file WC timer-bom2.html

Have fun.
HEINOHEIN