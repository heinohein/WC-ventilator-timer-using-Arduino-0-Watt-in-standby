/*
WC timer switches on and off the motor of the ventilator in a controlled way. 
Because the powersource of the Arduino (230V to 5V DC transformer) is switched on 
by the lightswitch in the WC, this system is using 0 Watt when standby.

This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

written by hfmv V1.0 18-9-2015
*/
#define VERSION "V1.0"

#define LED_PIN 13                  // internal led

// input from transmiter
#define INPUT_PIN A0                // potmeter for setting variable time
#define INPUT_PIN_TXT "A0"
#define RELAIS_PIN1 A1
#define RELAIS_PIN2 A2
#define PIN_230V A3

#define ON HIGH                     // some relais have low level trigger input, in that case reverse the values
#define OFF LOW

#define START_DELAY 10000           // delay 10 seconds before doing anything

unsigned long DELAY_LED=2000;       // delay for blinking led in milli seconds
#define BLINK_FASTER_FACTOR 4       // at program end blink led x times faster

#define DELAY_RELAIS_PIN 60000      // 60 seconds = minimum time the motor is switched on 

bool start_delay = true;
unsigned long time;
unsigned long timeout;
unsigned long timeoutrelais;
bool swrelais=false;
bool swtest=true;                   

int status_led;                     // used for blinking the led
unsigned long timeled;

int timer_input;                    // input from the potentiometer
unsigned long time_factor=420;      // multiplyer to get the time in seconds from the potentiometer
//unsigned long time_factor=50;       // for tests
int i;

// ************************************************************************
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  };
  pinMode(LED_PIN, OUTPUT);
  pinMode (INPUT_PIN, INPUT);
  pinMode(RELAIS_PIN1, OUTPUT);
  pinMode(RELAIS_PIN2, OUTPUT);
  pinMode (PIN_230V, INPUT);

  Serial.print("Start WC timer ");
  Serial.print(VERSION);
  Serial.print(", delay set by pin ");
  Serial.println (INPUT_PIN_TXT);

  time = millis();                  // get the current time
  
  digitalWrite(LED_PIN, LOW);       // initially turn the led off
  timeled=time;                     // used for blinking led
  status_led = 0;
  
  digitalWrite(RELAIS_PIN1, OFF); // turn 2x RELAIS off
  digitalWrite(RELAIS_PIN2, OFF);

  // read value external variable resistor (potentiometer)
  timer_input=analogRead(INPUT_PIN);
  if (swtest){
    Serial.print("timer_input ");
    Serial.print (timer_input);
    Serial.print (" = ");
    Serial.print (timer_input*time_factor/1000);
    Serial.println(" seconds");
  } 
  
  delay(START_DELAY);                // first wait 10 seconds before action. if power switched off in between, no motor has been running

  digitalWrite(RELAIS_PIN2, ON);   // first relais 2 to giver power back to relais 1 and to switch on motor
  delay(200);                        // give relais 2 time to settle
  digitalWrite(RELAIS_PIN1, ON);   // 220->5VDC now via relais 2 
  swrelais=true;
  
  if (swtest){
    Serial.println("wait till 230V off ");
  }

  // read input pin check if 230V "input" is still switched on
  bool timeout_230V=false;
  int value_pin_230V=0;
  int value_pin_230V_saved=0;
  while (!(timeout_230V)){
    i=analogRead(PIN_230V);
    value_pin_230V=i;
    if (value_pin_230V<100){            // suppress noise 
      value_pin_230V=0;
    }

    if (value_pin_230V==0){
      if (value_pin_230V_saved==0){
        time = millis();
        Serial.print ("waiting start at ");
        Serial.print (millis());
        Serial.print (" at ");
        Serial.println (i);
        value_pin_230V_saved=1;           // i found a 0 Volt value
      } else {
        if (millis()-time>2000){
          timeout_230V=true;             //stop waiting after 2 second 0 Volt
          Serial.print ("waiting stopped at ");
          Serial.println (millis());
        }
      }
    } else {
      value_pin_230V_saved=0;          // no continues 0 Volt found, start search again
    }
  }

   if (swtest){
    Serial.println("230V is off ");
  }
  DELAY_LED=DELAY_LED/BLINK_FASTER_FACTOR;  // change speed blinking led
  time = millis();                  // get the current time
  
} // END SETUP

void loop() {

  if (swrelais) {  
    timeout=time+timer_input*time_factor;
    timeoutrelais=time+DELAY_RELAIS_PIN;

    if (swtest) {
      Serial.print("now ");
      Serial.print(millis());
      Serial.print(" timeout ");
      Serial.print(timeout);
      Serial.print(" timeoutrelais ");
      Serial.print(timeoutrelais);
      Serial.print(" time ");
      Serial.println(time);
    }
  
    if (millis()>(timeout)){        //timed out
        if (millis()>(timeoutrelais)){ //switch relais after a minimum time
          if (swrelais) {
            Serial.println("Relais OFF");
            delay(200);             // wait finishing serial print
            DELAY_LED=DELAY_LED/BLINK_FASTER_FACTOR;  // change speed blinking led
          }
          digitalWrite(RELAIS_PIN1, OFF); // turn 2x RELAIS off
          digitalWrite(RELAIS_PIN2, OFF);
          swrelais=false;
        }
    }
  }
  if (status_led == 1){             // make LED blink
    if (millis()>(timeled+DELAY_LED)){
      digitalWrite(LED_PIN, LOW);   // turn the LED off
      status_led=0;
      timeled=millis();
    }
  } else {
    if (millis()>(timeled+DELAY_LED)){
      digitalWrite(LED_PIN, HIGH);  // turn the LED on 
      status_led=1;
      timeled=millis();
    }
  }
} // END LOOP
