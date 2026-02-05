// =======================================================================================
// /////////////////////////Padawan360 Code - Mega v2.2 ////////////////////////////////////
// =======================================================================================
/*
Original code by Dan Kraus
dskraus@gmail.com
Astromech: danomite4047
Project Site: https://github.com/dankraus/padawan360/

Heavily influenced by DanF's Padwan code which was built for Arduino+Wireless PS2
controller leveraging Bill Porter's PS2X Library. I was running into frequent disconnect
issues with 4 different controllers working in various capacities or not at all. I decided
that PS2 Controllers were going to be more difficult to come by every day, so I explored
some existing libraries out there to leverage and came across the USB Host Shield and it's
support for PS3 and Xbox 360 controllers. Bluetooth dongles were inconsistent as well
so I wanted to be able to have something with parts that other builder's could easily track
down and buy parts even at your local big box store.

v2.2 Changes:
- Added options to use DY Player over the MP3 Trigger
- Added Maestro sequences
- Added chatpad support 
- Added hall effect sensor to home the dome
- Added AstroPixels control over serial


Hardware:
***Arduino Mega 2560 ADK***
Microsoft Xbox 360 Controller
Xbox 360 Chatpad
Xbox 360 USB Wireless Reciver
Sabertooth Motor Controller
Syren Motor Controller
Sparkfun MP3 Trigger or DY Player

This sketch supports I2C and calls events on many sound effect actions to control lights and sounds.
It is NOT set up for Dan's method of using the serial packet to transfer data up to the dome
to trigger some light effects.It uses Hardware Serial pins on the Mega to control Sabertooth and Syren

If you are using a Li-Ion battery, switch 3 should be down as well. See the Syren10 documentation for more info

Set Sabertooth 2x25/2x12 Dip Switches 1 and 2 Down, All Others Up
For SyRen Packetized Serial Set Switches 1 and 2 Down, All Others Up (My prefered setup)
For SyRen Simple Serial Set Switchs 2 & 4 Down, All Others Up

*/

// ************************** Options, Configurations, and Settings ***********************************



// SPEED AND TURN SPEEDS
//set these 3 to whatever speeds work for you. 0-stop, 127-full speed.
const byte DRIVESPEED1 = 50;
// Recommend beginner: 50 to 75, experienced: 100 to 127, I like 100.
// These may vary based on your drive system and power system
const byte DRIVESPEED2 = 100;
//Set to 0 if you only want 2 speeds.
const byte DRIVESPEED3 = 127;

// Default drive speed at startup
byte drivespeed = DRIVESPEED1;

// the higher this number the faster the droid will spin in place, lower - easier to control.
// Recommend beginner: 40 to 50, experienced: 50 $ up, I like 70
// This may vary based on your drive system and power system
const byte TURNSPEED = 50;

// Set isLeftStickDrive to true for driving  with the left stick
// Set isLeftStickDrive to false for driving with the right stick (legacy and original configuration)
boolean isLeftStickDrive = true;

// If using a speed controller for the dome, sets the top speed. You'll want to vary it potenitally
// depending on your motor. My Pittman is really fast so I dial this down a ways from top speed.
// Use a number up to 127 for serial
const byte DOMESPEED = 80;

// Ramping- the lower this number the longer R2 will take to speedup or slow down,
// change this by incriments of 1
const byte RAMPING = 5;

// Compensation is for deadband/deadzone checking. There's a little play in the neutral zone
// which gets a reading of a value of something other than 0 when you're not moving the stick.
// It may vary a bit across controllers and how broken in they are, sometimex 360 controllers
// develop a little bit of play in the stick at the center position. You can do this with the
// direct method calls against the Syren/Sabertooth library itself but it's not supported in all
// serial modes so just manage and check it in software here
// use the lowest number with no drift
// DOMEDEADZONERANGE for the left stick, DRIVEDEADZONERANGE for the right stick
const byte DOMEDEADZONERANGE = 20;
const byte DRIVEDEADZONERANGE = 20;

// Set the baude rate for the Sabertooth motor controller (feet)
// 9600 is the default baud rate for Sabertooth packet serial.
// for packetized options are: 2400, 9600, 19200 and 38400. I think you need to pick one that works
// and I think it varies across different firmware versions.
const int SABERTOOTHBAUDRATE = 9600;

// Set the baude rate for the Syren motor controller (dome)
// for packetized options are: 2400, 9600, 19200 and 38400. I think you need to pick one that works
// and I think it varies across different firmware versions.
const int DOMEBAUDRATE = 9600;

// Default sound volume at startup
// 0 = full volume, 255 off
byte vol = 20;


// Automation Delays
// set automateDelay to min and max seconds between sounds
byte automateDelay = random(8, 30);
//How much the dome may turn during automation.
int turnDirection = 35;

// Pin number to pull a relay high/low to trigger my upside down compressed air like R2's extinguisher
#define EXTINGUISHERPIN 3

// Pin to receive the busy signal if the DYPlayer is playing a sound
#define DYBUSYPIN 8

// Enable chatpad with https://github.com/willtoth/USB_Host_Shield_2.0/tree/xbox-chatpad
#define CHATPAD

#include <Sabertooth.h>
#include <MP3Trigger.h>
#include <Wire.h>
#include <XBOXRECV.h>
#include <DYSound.h>
#include <Adafruit_PWMServoDriver.h>


//////////////////////////////////////////////////////////////////////////

// Maestro Config

#include <PololuMaestro.h> // added the Maestro libray
#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(10, 11); //tx pin 11
SoftwareSerial maestroBodySerial(12, 13); // tx on pin 13

MiniMaestro maestro_dome(maestroSerial); //software serial 11
MiniMaestro maestro_body(maestroBodySerial); //software serial 13

//////////////////////////////////////////////////////////////////////////

// Hall Sensor config for Dome Homing

// Center hall sensor - 
const int centerHallSensorPin = 4;  // Pin that the Hall Effect sensor is connected to
int centerHallSensorState;          // Variable to store the state of the sensor


//How many seconds the dome should try to find home (center) before timing out
byte homeDomeTimeout = 8;
unsigned long homingMillis = 0;

// Uncomment below lines of code if using left and right hall sensors as well

//Left hall sensor
// const int leftHallSensorPin = 5;  // Pin that the Hall Effect sensor is connected to
//int leftHallSensorState;          // Variable to store the state of the sensor

//Right hall sensor
//const int rightHallSensorPin = 6;  // Pin that the Hall Effect sensor is connected to
//int rightHallSensorState;          // Variable to store the state of the sensor

///////////////////////////////////////////////////////////////////////////

Sabertooth Sabertooth2x(128, Serial1);
Sabertooth Syren10(128, Serial2);


///////////////////////////////////////////////////////////////////////////


// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

// Set some defaults for start up
// false = drive motors off ( right stick disabled ) at start
boolean isDriveEnabled = false;

// Automated functionality

// Used as a boolean to turn on/off automated functions like periodic random sounds and periodic dome turns
//If using as a static droid and want automation on startup without controller connected, change to true
boolean isInAutomationMode = false;


unsigned long automateMillis = 0;
boolean bodyautomation = false; // used to turn on physical random events with servo animations
// Action number used to randomly choose a sound effect or a dome turn
byte automateAction = 0;

int lastdirection = 0; // used for recalling whether the last direction we turned was left or right (0 is left, 1 is right)
float relativemove = 0; // keep track of how much relative movement we've made from center in automation mode
int movecount = 0; // count how many times we've moved through automation so we can reset dome position occassionally 


int driveThrottle = 0;
int throttleStickValue = 0;
int domeThrottle = 0;
int turnThrottle = 0;

boolean firstLoadOnConnect = false;

AnalogHatEnum throttleAxis;
AnalogHatEnum turnAxis;
AnalogHatEnum domeAxis;
ButtonEnum speedSelectButton;
ButtonEnum hpLightToggleButton;


// this is legacy right now. The rest of the sketch isn't set to send any of this
// data to another arduino like the original Padawan sketch does
// right now just using it to track whether or not the HP light is on so we can
// fire the correct I2C event to turn on/off the HP light.
//struct SEND_DATA_STRUCTURE{
//  //put your variable definitions here for the data you want to send
//  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
//  int hpl; // hp light
//  int dsp; // 0 = random, 1 = alarm, 5 = leia, 11 = alarm2, 100 = no change
//};
//SEND_DATA_STRUCTURE domeData;//give a name to the group of data

boolean isHPOn = false;

//Use DYPlayer instead of MP3Trigger. Change to false to use MP3Trigger instead
boolean UseDYPlayer = true;

int busyPinstate; //State of the DYPlayer busy signal
int soundToPlay = 0; // used to store the result of a random sound to play
int lastSoundPlayed = 0; // used to minimize chances of random sounds playing twice in a row


DYPlayer dyPlayer;

MP3Trigger mp3Trigger;


USB Usb;
XBOXRECV Xbox(&Usb);

// State for Gripper control - true = up, false = down
boolean gripperState = false;

//State for interface control - true = up, false = down
boolean interfaceState = false; 

unsigned long now = millis(); // used for keeping track of delay-free timers

// ************************** Setup ***********************************

void setup() {
  Serial1.begin(SABERTOOTHBAUDRATE);
  Serial2.begin(DOMEBAUDRATE);
  //Astropixels Serial, may need 2400 by default
  //Serial3.begin(9600);

  // Busy pin for DYPlayer
  pinMode(DYBUSYPIN, INPUT);
  
  // Start Maestro serial ports
  maestroSerial.begin(9600);
  maestroBodySerial.begin(9600);

  // Set Maestro positions to default all closed
  maestro_dome.restartScript(0);
  maestro_body.restartScript(0);

  // Hall Effect Sensor setup 
  pinMode(centerHallSensorPin, INPUT);     // Set the center Hall Effect sensor pin as an INPUT
  //pinMode(leftHallSensorPin, INPUT);     // Set the left Hall Effect sensor pin as an INPUT - uncomment if using
  //pinMode(rightHallSensorPin, INPUT);     // Set the right Hall Effect sensor pin as an INPUT - uncomment if using
  

#if defined(SYRENSIMPLE)
  Syren10.motor(0);
#else
  Syren10.autobaud();
#endif

  // Send the autobaud command to the Sabertooth controller(s).
  /* NOTE: *Not all* Sabertooth controllers need this command.
  It doesn't hurt anything, but V2 controllers use an
  EEPROM setting (changeable with the function setBaudRate) to set
  the baud rate instead of detecting with autobaud.
  If you have a 2x12, 2x25 V2, 2x60 or SyRen 50, you can remove
  the autobaud line and save yourself two seconds of startup delay.
  */
  Sabertooth2x.autobaud();
  // The Sabertooth won't act on mixed mode packet serial commands until
  // it has received power levels for BOTH throttle and turning, since it
  // mixes the two together to get diff-drive power levels for both motors.
  Sabertooth2x.drive(0);
  Sabertooth2x.turn(0);


  Sabertooth2x.setTimeout(950);
  Syren10.setTimeout(950);

  pinMode(EXTINGUISHERPIN, OUTPUT);
  digitalWrite(EXTINGUISHERPIN, HIGH);
  if (UseDYPlayer) {
    dyPlayer.setup();
    dyPlayer.init();
    dyPlayer.VolumeMid();
  } else {
    mp3Trigger.setup();
    mp3Trigger.setVolume(vol);
  }

  // Set the dome to center (home)
  findhome();



  if (isLeftStickDrive) {
    throttleAxis = LeftHatY;
    turnAxis = LeftHatX;
    domeAxis = RightHatX;
    speedSelectButton = L3;
    hpLightToggleButton = R3;

  } else {
    throttleAxis = RightHatY;
    turnAxis = RightHatX;
    domeAxis = LeftHatX;
    speedSelectButton = R3;
    hpLightToggleButton = L3;
  }


  // Start I2C Bus. The body is the master.
  Wire.begin();

  //Serial.begin(115200);
  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  while (!Serial)
    ;
  if (Usb.Init() == -1) {
    //Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  //halt
  }
  //Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
}


// ************************** Main Loop ***********************************


void loop() {
  
  now = millis(); // set currentMillis to the current amount of time the sketch has been running
  
  Usb.Task();

  // if we're not connected, return so we don't bother doing anything else.
  // set all movement to 0 so if we lose connection we don't have a runaway droid!
  // a restraining bolt and jawa droid caller won't save us here!
  if (!Xbox.XboxReceiverConnected || !Xbox.Xbox360Connected[0]) {
    Sabertooth2x.drive(0);
    Sabertooth2x.turn(0);
    Syren10.motor(1, 0);
    firstLoadOnConnect = false;
    if (isInAutomationMode == false){
          return;
    }
  }
  // Check if anything is playing, probably introduces race conditions reading once here, but shouldn't be too big of a deal
  busyPinstate = digitalRead(DYBUSYPIN);

  // After the controller connects, Blink all the LEDs so we know drives are disengaged at start
  if (!firstLoadOnConnect) {
    firstLoadOnConnect = true;

    if (UseDYPlayer) {
      if (busyPinstate > 0) {  //nothing playing
        dyPlayer.Play(21);
      }
    } else {
      mp3Trigger.play(21);
    }

    Xbox.setLedMode(ROTATING, 0);
  }
 
  Check_Chatpad();

  if (Xbox.getButtonClick(XBOX, 0)) {
    if (Xbox.getButtonPress(L1, 0) && Xbox.getButtonPress(R1, 0)) {
      Xbox.disconnect(0);
    }
  }

  // enable / disable right stick (droid movement) & play a sound to signal motor state
  if (Xbox.getButtonClick(START, 0)) {
    if (isDriveEnabled) {
      isDriveEnabled = false;
      Xbox.setLedMode(ROTATING, 0);
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(53);
        }
      } else {
        mp3Trigger.play(53);
      }
    } else {
      isDriveEnabled = true;
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(52);
        }
      } else {
        mp3Trigger.play(52);
      }
      // //When the drive is enabled, set our LED accordingly to indicate speed
      if (drivespeed == DRIVESPEED1) {
        Xbox.setLedOn(LED1, 0);
      } else if (drivespeed == DRIVESPEED2 && (DRIVESPEED3 != 0)) {
        Xbox.setLedOn(LED2, 0);
      } else {
        Xbox.setLedOn(LED3, 0);
      }
    }
  }

  //Toggle automation mode with the BACK button
  if (Xbox.getButtonClick(BACK, 0)) {
    if (isInAutomationMode) {
      isInAutomationMode = false;
      automateAction = 0;
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(53);
        }
      } else {
        mp3Trigger.play(53);
      }
    } else {
      isInAutomationMode = true;
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(52);
        }
      } else {
        mp3Trigger.play(52);
      }
    }
  }

/*
// OLD AUTOMATION STUFF, replaced with new logic
  For Dome homing:

  // Read the state of the sensor
  centerHallSensorState = digitalRead(centerHallSensorPin); 

  // Check if the sensor is detecting a magnetic field
  if (centerHallSensorState == LOW) {
    // Magnet is detected here, stop dome rotation
    
  } else {
    // No magnet here, keep rotating 

  }
  
  
  // Plays random sounds or dome movements for automations when in automation mode
  if (isInAutomationMode) {
    unsigned long currentMillis = millis();

    if (currentMillis - automateMillis > (automateDelay * 1000)) {
      automateMillis = millis();
      automateAction = random(1, 5);

      if (automateAction > 1) {
        if (UseDYPlayer) {
          if (busyPinstate > 0) {  //nothing playing
            dyPlayer.Play(random(32, 52));
          }
        } else {
          mp3Trigger.play(random(32, 52));
        }
      }
      if (automateAction < 4) {
#if defined(SYRENSIMPLE)
        Syren10.motor(turnDirection);
#else
        Syren10.motor(1, turnDirection);
#endif

        delay(750);

#if defined(SYRENSIMPLE)
        Syren10.motor(0);
#else
        Syren10.motor(1, 0);
#endif

        if (turnDirection > 0) {
          turnDirection = -35;
        } else {
          turnDirection = 35;
        }
      }

      // sets the mix, max seconds between automation actions - sounds and dome movement
      automateDelay = random(3, 10);
    }
  }
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// New Automation Mode //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/*
AUTOMATION MODE IS USED TO PLAY RANDOM SOUNDS AND SMALL DOME MOVEMENTS WITHOUT ANY REAL INTELLIGENCE
*/

  // Plays random sounds or dome movements for automations when in automation mode
  if (isInAutomationMode) {
    unsigned long currentMillis = millis();
    

    if (currentMillis - automateMillis > (automateDelay * 1000))
    {
      automateMillis = millis();
      automateAction = random(1, 5);

      /* 
      //Remove body automation for now, not using it 
      automateBody = random (1,40);
      
      if ((automateBody > 1) && (bodyautomation == true)){
        if (automateBody == 15)
            {
              maestro.restartScript(1); // run maestro script: animation of gripper 
            }
        if (automateBody == 25)
            {
              maestro.restartScript(2); // run maestro script: animation of interface
            }
        if (automateBody == 35)
            {
              maestro.restartScript(3); // run maestro script: animation of utility arms
            }
        
        
      }
      */


      if (automateAction > 1) {
        soundToPlay = random(32,52); // decide random sound

        if (soundToPlay == lastSoundPlayed)
          {
          soundToPlay = random(32,52); // spin again to reduce chance of repetition 
                if (soundToPlay == lastSoundPlayed)
                {
                soundToPlay = random(32,52); // spin again to reduce chance of repetition 
                }
          }

        lastSoundPlayed = soundToPlay;
        if (UseDYPlayer) {
          if (busyPinstate > 0) {  //nothing playing
              dyPlayer.Play(soundToPlay);
            }
          } else {
            mp3Trigger.play(soundToPlay);
          }
      }

      if (automateAction < 3) {
      
        // keep track of how far we're moving
        randomSeed(now);
        // int turndelay = (random(200,800));
        int whichway = (random(1,100));
      


        // choose next direction randomly
        if (whichway < 50){
          turnDirection = 45; // turn left
          lastdirection = 0;
        }
        else{
          turnDirection = -45; // then turn right
          lastdirection = 1;
        }

        /*
        // choose next direction based on previous direction we moved
        if (lastdirection == 0){ // if we last turned left
          turnDirection = -45; // then turn right
          lastdirection = 1;
        }
        else{
          turnDirection = 45; // else turn left
          lastdirection = 0;
        }
        */


        if (relativemove >= 200){ // we've gone too far left 
          turnDirection = -45; // lets go right
          lastdirection = 1;
        }
        
        if (relativemove <= -200){ // we've gone too far right
          turnDirection = 45; // lets go left
          lastdirection = 0;
        }

        if (turnDirection > 0){ // we decided to go left
          relativemove = (relativemove + 100); // keep track of movement from center
        }
        else if (turnDirection < 0){ // we decided to go right
          relativemove = (relativemove - 100); // keep track of movement from center
        }
      

        movecount++;
        if (movecount > 15) // if we've moved a lot, lets reset the dome position
              {
                if (relativemove >= 0)
                    {
                      lastdirection = 0;  
                    }
                  else if (relativemove < 0)
                    {
                      lastdirection = 1;
                    }
              findhome(); 
              delay(2000);
              }
                  #if defined(SYRENSIMPLE)
                          Syren10.motor(turnDirection);
                  #else
                          Syren10.motor(1, turnDirection);
                  #endif

                  // delay to allow  motors to turn
                  delay(750);


                  // stop motors turning
                  #if defined(SYRENSIMPLE)
                          Syren10.motor(0);
                  #else
                          Syren10.motor(1, 0);
                  #endif

        }

        // sets the mix, max seconds between automation actions - sounds and dome movement
        automateDelay = random(3,12);
      }
    }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////// End Automation Mode //////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////  

  // Volume Control of MP3 Trigger
  // Hold R1 and Press Up/down on D-pad to increase/decrease volume
  if (Xbox.getButtonClick(UP, 0)) {
    // volume up
    if (Xbox.getButtonPress(R1, 0)) {
      if (vol > 0) {
        vol--;
        if (UseDYPlayer) {
          if (busyPinstate > 0) {  //nothing playing
            dyPlayer.VolumeUp();
          }
        } else {
          mp3Trigger.setVolume(vol);
        }
      }
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    //volume down
    if (Xbox.getButtonPress(R1, 0)) {
      if (vol < 255) {
        vol++;
        if (UseDYPlayer) {
          if (busyPinstate > 0) {  //nothing playing
            dyPlayer.VolumeDown();
          }
        } else {
          mp3Trigger.setVolume(vol);
        }
      }
    }
  }
  if (Xbox.getButtonClick(RIGHT, 0)) {
    // Stop Playing on DYPlayer
    if (Xbox.getButtonPress(R1, 0)) {
      dyPlayer.Stop();
    }
  }

  // Logic display brightness.
  // Hold L1 and press up/down on dpad to increase/decrease brightness
  if (Xbox.getButtonClick(UP, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      triggerI2C(10, 24);
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      triggerI2C(10, 25);
    }
  }


  //FIRE EXTINGUISHER
  // When holding L2-UP, extinguisher is spraying. WHen released, stop spraying

  // TODO: ADD SERVO DOOR OPEN FIRST. ONLY ALLOW EXTINGUISHER ONCE IT'S SET TO 'OPENED'
  // THEN CLOSE THE SERVO DOOR
  if (Xbox.getButtonPress(L1, 0)) {
    if (Xbox.getButtonPress(UP, 0)) {
      digitalWrite(EXTINGUISHERPIN, LOW);
    } else {
      digitalWrite(EXTINGUISHERPIN, HIGH);
    }
  }

  // GENERAL SOUND PLAYBACK AND DISPLAY CHANGING

  // Y Button and Y combo buttons
  if (Xbox.getButtonClick(Y, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(8);
        }
      } else {
        mp3Trigger.play(8);
      }
      //logic lights, random
      triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(2);
        }
      } else {
        mp3Trigger.play(2);
      }
      //logic lights, random
      triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(9);
        }
      } else {
        mp3Trigger.play(9);
      }
      //logic lights, random
      triggerI2C(10, 0);
    } else {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(random(13, 17));
        }
      } else {
        mp3Trigger.play(random(13, 17));
      }
      //logic lights, random
      triggerI2C(10, 0);
    }
  }

  // A Button and A combo Buttons
  if (Xbox.getButtonClick(A, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(6);
        }
      } else {
        mp3Trigger.play(6);
      }
      //logic lights
      triggerI2C(10, 6);
      astroPixelsSend("@APLE20005");
      // HPEvent 11 - SystemFailure - I2C
      triggerI2C(25, 11);
      triggerI2C(26, 11);
      triggerI2C(27, 11);
    } else if (Xbox.getButtonPress(L2, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(1);
        }
      } else {
        mp3Trigger.play(1);
      }
      //logic lights, alarm
      triggerI2C(10, 1);
      astroPixelsSend("@APLE10505");
      //  HPEvent 3 - alarm - I2C
      triggerI2C(25, 3);
      triggerI2C(26, 3);
      triggerI2C(27, 3);
    } else if (Xbox.getButtonPress(R1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(11);
        }
      } else {
        mp3Trigger.play(11);
      }
      //logic lights, alarm2Display
      triggerI2C(10, 11);
    } else {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(random(17, 25));
        }
      } else {
        mp3Trigger.play(random(17, 25));
      }
      //logic lights, random
      triggerI2C(10, 0);
    }
  }

  // B Button and B combo Buttons
  if (Xbox.getButtonClick(B, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(7);
        }
      } else {
        mp3Trigger.play(7);
      }
      //logic lights, random
      triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(3);
        }
      } else {
        mp3Trigger.play(3);
      }
      //logic lights, random
      triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(10);
        }
      } else {
        mp3Trigger.play(10);
      }
      //logic lights bargrap
      triggerI2C(10, 10);
      // HPEvent 1 - Disco - I2C
      triggerI2C(25, 10);
      triggerI2C(26, 10);
      triggerI2C(27, 10);
    } else {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(random(32, 52));
        }
      } else {
        mp3Trigger.play(random(32, 52));
      }
      //logic lights, random
      triggerI2C(10, 0);
    }
  }

  // X Button and X combo Buttons
  if (Xbox.getButtonClick(X, 0)) {
    // leia message L1+X
    if (Xbox.getButtonPress(L1, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(5);
        }
      } else {
        mp3Trigger.play(5);
      }
      //logic lights, leia message
      triggerI2C(10, 5);
      astroPixelsSend(":SE08"); //Send leia sequence to astropixels
      // Front HPEvent 1 - HoloMessage - I2C -leia message
      triggerI2C(25, 9);
    } else if (Xbox.getButtonPress(L2, 0)) {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(4);
        }
      } else {
        mp3Trigger.play(4);
      }
      //logic lights
      triggerI2C(10, 4);
    } else if (Xbox.getButtonPress(R1, 0)) {
      mp3Trigger.play(12);
      //logic lights, random
      triggerI2C(10, 0);
    } else {
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(random(25, 32));
        }
      } else {
        mp3Trigger.play(random(25, 32));
      }
      //logic lights, random
      triggerI2C(10, 0);
    }
  }

  // turn hp light on & off with Right Analog Stick Press (R3) for left stick drive mode
  // turn hp light on & off with Left Analog Stick Press (L3) for right stick drive mode
  if (Xbox.getButtonClick(hpLightToggleButton, 0)) {
    // if hp light is on, turn it off
    if (isHPOn) {
      isHPOn = false;
      // turn hp light off
      // Front HPEvent 2 - ledOFF - I2C
      triggerI2C(25, 2);
    } else {
      isHPOn = true;
      // turn hp light on
      // Front HPEvent 4 - whiteOn - I2C
      triggerI2C(25, 1);
    }
  }


  // Change drivespeed if drive is enabled
  // Press Left Analog Stick (L3) for left stick drive mode
  // Press Right Analog Stick (R3) for right stick drive mode
  // Set LEDs for speed - 1 LED, Low. 2 LED - Med. 3 LED High
  if (Xbox.getButtonClick(speedSelectButton, 0) && isDriveEnabled) {
    //if in lowest speed
    if (drivespeed == DRIVESPEED1) {
      //change to medium speed and play sound 3-tone
      drivespeed = DRIVESPEED2;
      Xbox.setLedOn(LED2, 0);
      if (UseDYPlayer) {
        dyPlayer.Play(53);
      } else {
        mp3Trigger.play(53);
      }
      triggerI2C(10, 22);
    } else if (drivespeed == DRIVESPEED2 && (DRIVESPEED3 != 0)) {
      //change to high speed and play sound scream
      drivespeed = DRIVESPEED3;
      Xbox.setLedOn(LED3, 0);
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(1);
        }
      } else {
        mp3Trigger.play(1);
      }
      triggerI2C(10, 23);
    } else {
      //we must be in high speed
      //change to low speed and play sound 2-tone
      drivespeed = DRIVESPEED1;
      Xbox.setLedOn(LED1, 0);
      if (UseDYPlayer) {
        if (busyPinstate > 0) {  //nothing playing
          dyPlayer.Play(52);
        }
      } else {
        mp3Trigger.play(52);
      }
      triggerI2C(10, 21);
    }
  }



  // FOOT DRIVES
  // Xbox 360 analog stick values are signed 16 bit integer value
  // Sabertooth runs at 8 bit signed. -127 to 127 for speed (full speed reverse and  full speed forward)
  // Map the 360 stick values to our min/max current drive speed
  throttleStickValue = (map(Xbox.getAnalogHat(throttleAxis, 0), -32768, 32767, -drivespeed, drivespeed));
  if (throttleStickValue > -DRIVEDEADZONERANGE && throttleStickValue < DRIVEDEADZONERANGE) {
    // stick is in dead zone - don't drive
    driveThrottle = 0;
  } else {
    if (driveThrottle < throttleStickValue) {
      if (throttleStickValue - driveThrottle < (RAMPING + 1)) {
        driveThrottle += RAMPING;
      } else {
        driveThrottle = throttleStickValue;
      }
    } else if (driveThrottle > throttleStickValue) {
      if (driveThrottle - throttleStickValue < (RAMPING + 1)) {
        driveThrottle -= RAMPING;
      } else {
        driveThrottle = throttleStickValue;
      }
    }
  }

  turnThrottle = map(Xbox.getAnalogHat(turnAxis, 0), -32768, 32767, -TURNSPEED, TURNSPEED);

  // DRIVE!
  // right stick (drive)
  if (isDriveEnabled) {
    // Only do deadzone check for turning here. Our Drive throttle speed has some math applied
    // for RAMPING and stuff, so just keep it separate here
    if (turnThrottle > -DRIVEDEADZONERANGE && turnThrottle < DRIVEDEADZONERANGE) {
      // stick is in dead zone - don't turn
      turnThrottle = 0;
    }
    Sabertooth2x.turn(-turnThrottle);
    Sabertooth2x.drive(driveThrottle);
  }

  // DOME DRIVE!
  domeThrottle = (map(Xbox.getAnalogHat(domeAxis, 0), -32768, 32767, DOMESPEED, -DOMESPEED));
  if (domeThrottle > -DOMEDEADZONERANGE && domeThrottle < DOMEDEADZONERANGE) {
    //stick in dead zone - don't spin dome
    domeThrottle = 0;
  }


  Syren10.motor(1, domeThrottle);

}  // END loop()



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// END LOOP ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////





void triggerI2C(byte deviceID, byte eventID) {
  Wire.beginTransmission(deviceID);
  Wire.write(eventID);
  Wire.endTransmission();
}

void astroPixelsSend(byte command) {
  Serial3.println(command);
}

void stopx(){
  Syren10.motor(1,0); // stop motor 
}

void findhome(){
  now = millis();
  unsigned long seekinghome = now;
  centerHallSensorState =  (digitalRead(centerHallSensorPin)); // read the home position sensor  to determine if we need to move

  //Low means we found the magnet
  while (centerHallSensorState !=  LOW){
    now = millis();
    centerHallSensorState =  (digitalRead(centerHallSensorPin)); // read the home position sensor
    if (lastdirection == 1){
        Syren10.motor(1,60); // turn left to find home
    }
    else{
        Syren10.motor(1,-60); // turn right to find home
    }
    

    // If we hit our timeout, stop the dome rotation
    if ((now - seekinghome) >= (homeDomeTimeout * 1000)){
        stopx();
        break;
    }

    relativemove = 0;
    movecount = 0;// reset the count of how many times automation moved the dome
              
  }

  
  stopx();
 
} // end find home routine


/*
Maestro Body Scripts:

0 - All Closed (default)
1 - Both Doors Close
2 - Both Doors Open
3 - Close DataPort
4 - Close Door A
5 - Close Door B
6 - Close util
7 - Dance Doors x3
8 - Flap All x1
9 - Flap All x2
10 - Flap Doors x1
11 - Flap Doors x2
12 - Gripper Close 
13 - Gripper Down
14 - Gripper Open
15 - Gripper Up
16 - Interface Down
17 - Interface Twist
18 - Interface up
19 - Open DataPort
20 - Open Door A
21 - Open Door B
22 - Open util
23 - Util open close
24 - Wave Doors
25 - Wave util
26 -

Maestro Dome Scripts:

0 - Close All 
1 - All Flap
2 - All Open
3 - Dance
4 - Pie CLose
5 - Pie Open
6 - Pie Wave
7 - Wave
8 - Chase - Fast
9 - Chase - Slow
10 - Pie2 Wave
11 - Pie2,3 Dance
12 - Pie Both Wave
13 - 

*/

void Check_Chatpad() {
  /*
  Modifiers:
    MODIFIER_SHIFT = 0,
    MODIFIER_GREENBUTTON = 1,
    MODIFIER_ORANGEBUTTON = 2,
    MODIFIER_MESSENGER = 3
  
  */
 
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D2, 0)) {
      //Theme
      dyPlayer.Play(9); // Star Wars Theme 5m 29s
  }  

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D3, 0)) {
    //Imperial March
    dyPlayer.Play(11); // Imperial March 3m 5s
    // astroPixelsSend("@APLE40500");
  }  
  
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D4, 0)) {
    //Cantina
    dyPlayer.Play(10); // Cantina 2m 50s
  }  

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D9, 0)) {
    // Annoyed
    dyPlayer.Play(8); // Annoyed
  }  

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D0, 0)) {
    //Chatty
    dyPlayer.Play(random(25,29)); // Chatty Random sounds
  }  

  if (Xbox.getChatpadClick(XBOX_CHATPAD_W, 0)) {
    //WolfWhistle
    dyPlayer.Play(4); // Wolf whistle
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_O, 0)) {
    //scream
    dyPlayer.Play(1); // Scream
    maestro_dome.restartScript(1);
    maestro_body.restartScript(9);
  }
/*
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D, 0)) {
      //DOODOO
      dyPlayer.Play(3); // DOODOO
  }
*/   
  if (Xbox.getChatpadClick(XBOX_CHATPAD_L, 0)) {
    dyPlayer.Play(5); // Leia Long 35s
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_X, 0)) {
    //shortcircuit
    dyPlayer.Play(13); // Short Circuit
    maestro_dome.restartScript(7);
    maestro_body.restartScript(7);
  }
  /*

  if (Xbox.getChatpadClick(XBOX_CHATPAD_C, 0)) {
      //Chortle
      dyPlayer.Play(2); // Chortle
  }
  */
  //start Interface Arm animation when pressing I
  if (Xbox.getChatpadClick(XBOX_CHATPAD_I, 0)) {
    // Interface up
    maestro_body.restartScript(18);
    interfaceState = true;
    // Interface Down
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(16);
      interfaceState = false;
    }
    // Green + I will twist interface if it is up
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      if (interfaceState){
        maestro_body.restartScript(17);
      }
    }
      
  }

  //Gripper Controls, G for up shift-G for down, green and orange + G for control
  if (Xbox.getChatpadClick(XBOX_CHATPAD_G, 0)) {
    // Gripper up
    maestro_body.restartScript(15);
    gripperState = true;
    // Gripper Down
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(13);
      gripperState = false;
    }
    // Green + G will Open Gripper if it is up
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      if (gripperState){
        maestro_body.restartScript(14);
      }
    }
    // Orange + G will Close gripper if it is up
    if (Xbox.getChatpadModifierState(MODIFIER_ORANGEBUTTON)){
      if (gripperState){
        maestro_body.restartScript(12);
      }
    }
  }

//Use "SPACE" to stop music and return to random lights
  if (Xbox.getChatpadClick(XBOX_CHATPAD_SPACE, 0)) {
      //Stop music tracks
      dyPlayer.Stop();
  }

  //Maestro Commands
  // TODO: Finish converting to Maestro commands

  //start movement UTIL to upper or lower when pressing U
  if (Xbox.getChatpadClick(XBOX_CHATPAD_U, 0)) {
    //ChatPad U - Open util arms
    maestro_body.restartScript(22);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(6);
    }
    // Flap Util
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_body.restartScript(23);
    }
    // Wave util
    if (Xbox.getChatpadModifierState(MODIFIER_ORANGEBUTTON)){
      maestro_body.restartScript(25);
    }
  }

  //Open Dataport with D, shut with shift D
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D, 0)) {
    maestro_body.restartScript(19);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(3);
    }
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_dome.restartScript(11);
    }
  }

  //Open Door A with A
  if (Xbox.getChatpadClick(XBOX_CHATPAD_A, 0)) {
    maestro_body.restartScript(20);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(4);
    }
  }

  //Open Door B with B
  if (Xbox.getChatpadClick(XBOX_CHATPAD_B, 0)) {
    maestro_body.restartScript(21);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(5);
    }
  }

  //Open all Dome with C, close all Dome with shift c
  if (Xbox.getChatpadClick(XBOX_CHATPAD_C, 0)) {
    maestro_dome.restartScript(2);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_dome.restartScript(0);
    }
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_dome.restartScript(1);
    }
  }

  //Open all Dome pies (1,2,3,4) with P, shut with shift P
  //Open Pies
  if (Xbox.getChatpadClick(XBOX_CHATPAD_P, 0)) {
    maestro_dome.restartScript(5);
    //Close Pies
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_dome.restartScript(4);
    }
    // Green + p = pie wave
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_dome.restartScript(6);
    }
    // orange + p = All wave
    if (Xbox.getChatpadModifierState(MODIFIER_ORANGEBUTTON)){
      maestro_dome.restartScript(7);
    }
  }
  // Wave Hi with Pie 2 or 2 and 3 or dance both with H
  if (Xbox.getChatpadClick(XBOX_CHATPAD_H, 0)) {
    // Pie 2 wave
    maestro_dome.restartScript(10);
    // Both wave
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_dome.restartScript(12);
    }
    // Both Dance
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_dome.restartScript(11);
    }
  }

  // Flap Doors or panels with F
  if (Xbox.getChatpadClick(XBOX_CHATPAD_F, 0)) {
    // Flap All x2
    maestro_body.restartScript(9);
    // Flap Doors x2
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(11);
    }
    // Flap Pies
    if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON)){
      maestro_dome.restartScript(1);
    }
  }

  // Chase with Y
  if (Xbox.getChatpadClick(XBOX_CHATPAD_Y, 0)) {
    // Fast Chase
    maestro_dome.restartScript(8);
    // Slow Chase
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_dome.restartScript(9);
    }
  }
  // Manually home the dome with J
  if (Xbox.getChatpadClick(XBOX_CHATPAD_J, 0)) {
    findhome();
  }
 
}
