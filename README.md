# Padawan360

## Changes and Version bumping
Since it's release, padawan360 has been a great option for droid builders everywhere. Dan Kraus and before him DanF have been great in providing this code and making it accessible. However over the years standard components and electronics have become cheaper, and changed and there are more and more alternatives. Because of this it has been modified and changed over the years by individual droid builders. Every droid is different and is built with different components and this is my take on the components. I have added several things and swapped out some parts for cheaper ones but the base code remains, and for good reason: it is pretty solid and robust, but the base code doesn't get updated very often.

So this is my versioning of the code and becasuse of the significant changes I figured it's probably time for a version bump. Because a lot of the base cod eis still the same just with lots of additions and customizing I think it is fitting. I may end up generalizing this code and making a PR to the main repo, but over the years I feel that repo and code has gotten a little messy and needs to be cleaned up a little. I do understand that because it is meant to be accessible to as many people as possible there is good reason for this, but pretty much every original sketch hasn't been touched in 10 years except the body arduino mega code so that is the only code here.

This is why I have mainly chosen to keep this repo seperate from the main repo but it is hard to cater to a wide audience when there is so much variation in builds. I will include the hardware I use as that is what this code is written for. 

As a last note I just want to say that one of the great joys of droid building is learning new skills and researching and customizing your droid. Because of this, it is hard to say that this code will work for everyone unless you get the exact components. This is why it is so hard to make genric code that works for everyone. I have modified this readme to reflect my choices, but this doesnt mean these are the only choices or that they are the best choices.

Finally I just wanted to list some of the changes I have made to this code up at the top for new builders to decide if this is the route they want to go

v2.2 Changes:
- Added options to use DY Player or the MP3 Trigger
- Added Maestro support
- Added chatpad support 
- Added hall effect sensor to home the dome
- Added AstroPixels control over serial - Currently a work in progress


Hardware:
- ***Arduino Mega 2560 ADK***
- Microsoft Xbox 360 Controller
- Xbox 360 Chatpad
- Xbox 360 USB Wireless Reciver
- Sabertooth Motor Controller
- Syren Motor Controller
- Sparkfun MP3 Trigger or DY-SV5W sound board

## Donations

Instead of offering to buy me a beer at the next convention, please consider making a donation to [Make-a-Wish](https://wish.org/), your local foodbank, or other local charity that helps kids in need. Drop me a note saying you did and I'll be grateful. We spend quite a bit of money on this hobby and even if you plan on doing some charitable events with your droid in the future, please set aside a few dollars and send a few dollars and put it to an immediate good. if you're embarking on a project like this, you have a few bucks to spare.

The R2/Droid Builders have a long history of raising money for groups like [Make-A-Wish](https://wish.org/).

[Donate to Make-A-Wish](https://wish.org/)

## Contents

- [Padawan360](#padawan360)
- [Intro](#intro)
- [Components](#components)
  - [Arduino Mega (Option 1 and Recommended!)](#arduino-mega--option-1-and-recommended--)
  - [Arduino Uno (Option 2)](#arduino-uno--option-2-)
  - [USB Shield](#usb-shield)
  - [Xbox 360 Wireless USB Receiver](#xbox-360-wireless-usb-receiver)
  - [Xbox 360 Wireless Controller](#xbox-360-wireless-controller)
  - [Sound Board](#sound-board) 
  - [PCA9685](#pca9685-servo-controller)
  - [Sabertooth Motor Controller - Feet](#sabertooth-motor-controller---feet)
  - [Syren Motor Controller - Dome](#syren-motor-controller---dome)
  - [Amp and Speakers](#amp-and-speakers)
  - [Teeces lights](#teeces-lights)
  - [Astropixels](#astropixels)
    - [Optional](#optional)
      - [Slipring](#slipring)
- [Setup](#setup)
  - [Arduino IDE](#arduino-ide)
  - [USB Shield](#usb-shield-1)
  - [Sound](#sound)
  - [Astropixels](#astropixels-1)
  - [Servo Controllers](#servo-controllers)
    - [PCA9685](#pca9685-servo-controller-1)
    - [Pololu Maestro](#pololu-maestro)
  - [Dome](#dome)
    - [Option 1](#option-1)
    - [Option 2 (Best Option)](#option-2--best-option-)
  - [Foot Drive](#foot-drive)
  - [Arduino MEGA](#arduino-uno-mega)
  - [Controller Pairing](#controller-pairing)
  - [Options, Configurations, and Settings](#options--configurations--and-settings)
  - [Video Guide](#video-guide)
- [Controls](#controls)
  - [Controller LED Status](#controller-led-status)
  - [Button Guide](#button-guide)
- [Troubleshooting and FAQs](#troubleshooting-and-faqs)
  - [General Control Issues](#general-control-issues)
  - [Sound](#sound-1)
  - [Dome](#dome-1)
  - [Drives](#drives)
- [Licensing](#licensing)

## Intro

This is a control system for 1:1 scale remote control R2-D2 powered by Arduinos and controlled with an Xbox 360 Controller. It triggers lights, sounds, and controls foot drive and dome drive motors. It also supports astropixels and 

These sketches are heavily based on DanF's Padawan control system that uses Playstation 2 controllers. I found the PS2 controllers to become a bit unreliable and they are increasingly more difficult to come by. I'm also taking advantage of the LEDs around the center Guide button to indicate state of the drive mode (disengaged, engaged w/ speed setting).

The Xbox 360 Controller is over 2.4ghz and uses frequency hopping to avoid interference. At DroidConIII 2014 I was at least over 100ft from the Droid and maintained connection through several walls including two bathrooms with plumbing and tiles. I tested by spinning the dome. When I got out of range, the dome stopped spinning. When I walked a few feet forward, it automatically reconnected and I was able to spin the dome again. Connection status was displayed via LEDs on the controller.

I developed Padawan360 (named with permission from DanF) to use some more easily accessible components with no soldering, wire stripping, etc. It's a bit more plug and play.

A lot of the instructions here are relevant to the original [Padawan PS2](http://astromech.net/droidwiki/index.php?title=PADAWAN) setup instructions. A good chunk of the documentation is reproduced here.

It is strongly recommended that you read this guide completely a few times before plugging things in or trying to run things. You should also review the code to get familiar with some of the options available to adjust.

Note: this does NOT work with an Xbox One controller. Xbox One controllers use different wireless protocols.

[The thread on Astromech for Padawan360 can be found here ](http://astromech.net/forums/showthread.php?t=19263_)

## Contributing

There are quite a few forks either on GitHub, circulating around via email attachments, Google Drive, etc. I'm open to contributions. Please either open an issue, contact me, open a PR, etc. If there's a change you want to see, likely others will benefit as well. The caveat is that the changes need to be generic enough for the average new builder to use and benefit from. If you want to include new and alternate libraries for sound boards, motor drivers, etc, then do so, but wrap them in flags or something so that the compiled sketch is small and effecient still to fit on devices. We already have two Mega and Uno sketches (which arguable could be combined into one), I'd prefer not to add more to manage.

## Components

- ### Arduino Mega ADK (Option 1 and Recommended!)

I run a Mega ADK for the body. It uses the hardware serial pins to connect to the motor controllers. Better performance and memory utilization. The Mega has more memory available too so there's more room to expand and do more if you want. With the Mega ADK's built in USB it makes life much easier than dealing with a USB shield and getting that to work since arduinos can be very picky about the shield used. They are pretty readily available and cheap from Amazon and AliExpress

- ### Arduino Mega (Option 2)

A basic Arduino Mega can also be used with a USB Shield (see below). Technically this can also be used with an Uno but I really do not recommend that. Getting an Arduino Mega ADK is likely going to be the best choice to avoid any headaches. 

- ### USB Shield

  Sourced from [Circuits@Home](https://www.circuitsathome.com/products-page/arduino-shields). They've shuffled their links around. Find it on that link labeled "USB Host Shield 2.0 for Arduino – Assembled". They used to offer it assembled and unassembled but as of this writing, they just have it assembled.

  If Amazon is your thing Sainsmart has a USB Host Shield that has been found to be compatible with Padawan360 (Prime elligble!) SainSmart USB Shield](http://www.amazon.com/SainSmart-Compatible-HOST-Shield-Arduino/dp/B006J4G000/ref=sr_1_1?ie=UTF8&qid=1420994477&sr=8-1&keywords=usb+host+shield+2.0+for+arduino). _NOTE_ I've seen that some SainSmart Shields aren't sending them with the 2x3 headers that connects the shield to the Arduino board together. This connection is absolutely necessary for the Arduino to talk to the shield over serial (ISCP pins). It will NOT work without this connection. Real dumb. The shield is basically useless without the connection. I spent a while a DroidCon trying to help someone it before realizing that was missing and it's absolutely necessary. You can grab 2x3 headers from Sparkfun for 50 cents. The missing headers seem to be a common issue as the Aamazon reviews are rife with that complaint. Check your particular USB Host Shield to see if the headers are there. The production may have been corrected.

Your USB Shield may need voltage pins jumped. See Steve Baudain's YouTube video here walking through it: https://www.youtube.com/watch?v=y9HEeBO3cV0

Because of all thes caveats and issues with sourcing the correct USB shield, I highly recommend just getting an ADK board

- ### Xbox 360 Wireless USB Receiver

  You may be able to source this from Best Buy, GameStop, or something local. It's available on [New Egg](https://www.newegg.com/p/pl?d=microsoft+xbox+360+usb+receiver).

  I personally use a genuine Microsoft one. eBay may be a good option for this nowadays but I haven't looked in a while. 

  Others have bought generic ones from Microcenter that work fine. Some users have gotten cheap ones from Ebay or other non-official/off-brands that did not function. I highly recommend buying 1st party official Microsoft receiver. Your mileage may vary with off-brand components here. If you are having problems pairing, and you've gone through the troubleshooting steps and you are not using an official receiver, it's likely the culprit.

Some people have had luck making some adjustments to get some 3rd Party receivers working. Your mileage may vary so try to obtain first party first. Read up on it here: https://astromech.net/droidwiki/Cheap_XBox_Receivers

I also am planning on working on support for an XBox One controller as they are much more available nowadays but I havent had the time to look into that super in depth yet. I will update this repo once I make progress on that

- ### Xbox 360 Wireless Controller

  [Controller via Amazon](http://www.amazon.com/Xbox-360-Wireless-Controller-Glossy-Black/dp/B003ZSP0WW). I have a nice blue one to match R2 personally ;) Like the USB Receiver, I highly recommend buying a 1st party official Microsoft controller. I know one user bought one cheap on Ebay and it didn't even have a sync button and there was no X on the center Home button. Your mileage may vary with off-brand components here.

  **Note:** I have seen the controller bundled with the USB receiver together. It was in the gaming peripheals department in my local Microcenter. It's marketed for PC gaming. Nice to get it in one package if you can if you don't have an extra 360 controller to spare.

  I also use a chatpad with my controller for more button combination options.

  I also am planning on working on support for an XBox One controller as they are much more available nowadays but I havent had the time to look into that super in depth yet. I will update this repo once I make progress on that


- ### Sound Board
  With this code you have two options:

  A DYSV5W sound board (found on amazon/aliexpress)

  or the typical MP3 Trigger

  [Sourced from SparkFun](https://www.sparkfun.com/products/11029). 
  
  No matter which you go with, be sure to get a microSD card too. Nothing too big, it's just MP3s (1GB is plenty). Some others have actually reported that higher sizes have issues, so try to keep it below 32GB to be sure to avoid any issues. 

- ### PCA9685 Servo Controller
This current sketch removed the PCA9685 support since I switched to a Maestro for ease of creating sequences. If you are interested in using this, there is another branch on this repo with that code.


- ### Sabertooth Motor Controller - Feet

  Depending on your motors you'll want a [Sabertooth 2x32](https://www.dimensionengineering.com/products/sabertooth2x32), [Sabertooth 2x25](https://www.dimensionengineering.com/products/sabertooth2x25) or [2x12](https://www.dimensionengineering.com/products/sabertooth2x12). The 2x32 and the 2x24 seem to be crossing over price point. Might as well get the 2x32 if you're buying brand new. There's some additional bells and whistles in the 32 and can be programmed via Dimension Engineering's software, but some of those usefuls features are handled in the code, like for speed Ramping for example. My styrene droid with Jaycar motors uses 2x12. Most people tend to use 2x25 or 2x32 for scooter motors and NPC motors. Consult with Dimension Engineering to make sure you get the right one you need. Either one will work with the code

  These are still pretty much the gold standard for droid motor controllers and they are very reliable. I may look into using brushless hob motors and controllers in the future, and will update the repo if I end up going that route. 

- ### Syren Motor Controller - Dome

  [Syren 10](https://www.dimensionengineering.com/products/syren10)

- ### Amp and Speakers

  Up to you and how big and loud you want. I have a small speaker and a $20 amp from Amazon. A ground loop isolator might be necessary to protect the MP3 trigger and eliminate buzzing from the speaker.

- ### AstroPixels

  This sketch is updated to support some astropixel commands. It is a low cost alternative to Teeces that uses neopixels. They can be sourced from [we-make-things](https://we-make-things.co.uk/product/astropixels/)

  I recommend these personally but some do not like the look and feel of the led lights. They are also a little dimmer than Teeces, but I find them to be great.

- ### Dome Homing Hall Effect Sensor

  This sketch is updated to include a hall effect sensor so that the dome is able to find home and know where it is relative to this. This gets rid of some of the problems with the dome being off center after a while in automation mode.

  I used a simple A3144 hall effect sensor which is super chear and works great. Just be aware of what sensor you buy as some (like the A3144) are uni-polar which means the magnet does need to be in a specific orientation and if you glue it in wrong the sensor will not work. 

  The sensor is given 5v from either the arduino or power supply, and the signal is wired into pin 4 by default. You can change this if you want in the code. 
  
  I modeled a holder for the sensor which I will try to remember to upload and link here. And then I just glued a magnet to the dome ring for sensing. This may or may not work for you depending on where your done ring sits while the droid is facing forward, but it is not too hard to find alternative places to mount the sensor and magnet. 


- #### Optional

  - ##### Slipring
    Used to pass power up from the body to the dome and also signal for astropixels and the dome Maestro. The slipring allows wires to go from body to dome and allow the dome to spin 360 degrees without tangling the wires.

## Setup

### Arduino IDE

Be sure you have the latest version of the Arduino IDE installed available [here at the Arduino website](https://www.arduino.cc/en/Main/Software) and install the libraries included in this repository from the `libraries` folder.

Installing libraries and using the Arduino IDE is beyond the scope of this documentation. Refer to the Arduino website's documentation for how to install libraries and upload software to your Arduino here https://www.arduino.cc/en/Guide and here https://www.arduino.cc/en/guide/libraries#toc5 . Note, you do not need to include the library references (ex: `#include <Sabertooth.h>`) directly in your sketch as the code to include them is already there. They simply need to be installed so that the Arduino IDE software can find them when compiling the sketch. The libraries are provided as zip files to make for easy installation.

The DYPlayer library has been added to the libraries folder so be sure to install that library as well

### USB Shield

If you decided to use a normal Mega and a USB shield, 

Solder the headers on the USB Shield if you purchased the unassembled version. Fit the shield over the Arduino UNO by lining up the pins and pushing in. It should fit snugly. Plug the Xbox 360 Wireless Receiver USB cable into the USB port. That was easy.

If you're using the Mega, orient the USB ports to line up over each other.

### Sound

#### DYSV5W

Make sure `UseDYPlayer` is `true`. It is true by default, so if you are using this board you should be good. 

Connect the pins from the DY board to the Arduino 

The library is hardcoded to use Serial0 (same as MP3Trigger), if you know what you are doing you can change the serial used, but if not just use the pins below

The busy pin is used to indicate that sound is currently playing. Trying to play a sound when another sound is playing will cause the board to stop playing anything altogether until reset, so we use this to try and prevent that. 

| DY Board Pin    | Arduino Mega|
| --------------- | ----------- |
| IO0/TX          | 0 (RX0)     |
| IO1/RX          | 1 (TX0)     |
| VCC/5v+         | 5v          |
| GND/5v-         | GND         |
| BUSY            | 8           |


Also ensure that the dip switches are set according to the table below. Off is towards 1 2 3, on is towards ON DIP

| Dip Switch | State       |
| ---------- | ----------- |
| 1          | OFF         |
| 2          | OFF         |
| 3          | ON          |


Connect the Micro SD card to your computer and upload the files one by one in the numbered order. If you don't do this, you will not be able to control which sound files are triggered. It's picky about file names and ordering. It's annoying but really, do transfer them over one file at a time. You may also be able to use a Windows program called Drivesort to help out here too. Here's a [helpful video from builder Balders on how to load the sounds with Drivesort](https://youtu.be/UsMI2gW7Q40) 

Now, insert it into the Dy board and hook up either the Ground Loop Isolator / Amp / Speaker using the 3.5mm jack on the board or the speaker pins.



#### MP3 Trigger

Make sure `UseDYPlayer` is `false`. It is true by default, so if you are using an mp3 trigger be sure to change that. 

Connect the following pins from the MP3 Trigger to the Body Arduino

| MP3 Trigger Pin | Arduino Mega|
| --------------- | ----------- |
| RX              | 1 (TX0)     |
| USBVCC          | 5v          |
| GND             | GND         |

**There is a small switch at the bottom of the board labled USB - EXT, make sure that it is pointing to the USB side in order to power it via the Arduino**

In order to load the sounds onto the MP3 Trigger, you will need to grab the sounds files either from [here](https://drive.google.com/drive/folders/19-NO-B2NIjAVUvo6hy9pTMNdYRaIFusJ)

Connect the Micro SD card to your computer and upload the files one by one in the numbered order. If you don't do this, you will not be able to control which sound files are triggered. It's picky about file names and ordering. It's annoying but really, do transfer them over one file at a time. You can also use a Windows program called Drivesort to help out here too. Here's a [helpful video from builder Balders on how to load the sounds with Drivesort](https://youtu.be/UsMI2gW7Q40)

Now, insert it into the MP3 Trigger and hook up either the Ground Loop Isolator / Amp / Speaker using the 3.5mm jack on the board.

For anyone with an older version of the MP3Trigger board, you may need to upgrade your firmware before you can use the Arduino to trigger sounds. If your board has the date 3-1-10 on the back (and possibly others) you have the retired version WIG-09715. You can use the board, but it's likely that you'll need updated firmware.

1. Download and unzip the the firmware file available [here](http://astromech.net/droidwiki/images/8/8f/MP3TRIGR.zip).
2. Copy the resulting hex file to a microSD card and rename it to: “MP3TRIGR.HEX”. It does not need to be the only file on the card – it just needs to have that precise filename.
3. Insert the microSD card into your MP3 Trigger and turn the power on while holding down the center navigation switch. Wait for the Status LED to go solid, then cycle the power.
4. You’re now running the new firmware.

### AstroPixels

  IMPORTANT: In order for this functionality, you MUST have the AstroPixelsPlus firmware installed on your AstroPixels. See [the documentation](https://github.com/reeltwo/AstroPixelsPlus) for more info on this.

  This sketch is updated to support some astropixel commands. These can be customized to be just about anything, but there is a few default ones included in this sketch. If you want to do custom lighting animations and sequences, you can look into the documentation [here](https://github.com/reeltwo/AstroPixelsPlus).

  In order for this to work, astropixels TTLSerial2 should be hooked up to serial3 (Pins 14, 15) on the mega (remember tx goes to rx and vice versa). See the wiring below. Remember it must be hooked up to Serial2 on the AstroPixels.

  The 5v is optional, and as long as you have a common ground between the AstroPixels and the Arduino you shouldn't need to hook it up directly to the Mega.

  | AstroPixels     | Arduino Mega   |
  | --------------- | -----------    |
  | T               | 15 (RX3)       |
  | R               | 14 (TX3)       |
  | V (Optional)    | 5v (Optional)  |
  | G (Optional)    | GND (Optional) |


### Servo Controllers
This script specifically removed support for the PCA9685 servo controller, but I have left that section in here. If you are interesting in using this servo controller, look at the pca9685 branch in this repo

#### PCA9685 Servo Controller
  This sketch is also updated to support the PCA9685 servo controller. It is set up for 2 currently: one for the body servos and one for the dome. I will likely expand this to include 3 of these controllers for full dome servo support (2 each for the HPs, all dome panels, and pie panels, plus any other accessories such as fire extiguisher and dome lifter). There are basic animations included, but they are not the most robust, but works well for basic control for a low cost.

  Wiring for these boards can be seen below. Also be sure to solder the address jumper on the additional boards. 2 boards should be 0x40 and 0x41 (0x40 for the body and 0x41 for the dome). See [this article](https://learn.adafruit.com/16-channel-pwm-servo-driver/chaining-drivers) for more info. 

  | PCA9685-2 Pin   | PCA9685-1 Pin   | Arduino Mega |
  | -----------     | --------------- | -----------  |
  |                 | SCL             | SCL          |
  |                 | SDA             | SDA          |
  | SCL             | SCL             |              |
  | SDA             | SDA             |              |
  |                 | VCC             | 5v           |
  |                 | GND             | GND          |
  | VCC             | VCC             |              |
  | GND             | GND             |              |

  ![PCA_Wiring](https://github.com/scmerrill/padawan360/blob/pca9685/pca9685_wiring.png)

#### Pololu Maestro
  I made the decision to switch to 2 Maestros instead. It made it much easier to program the sequences and really was a major time saver. The Maestros are not cheap compared to the PCA9685, but it is worth it to create complex sequences and I suppose there is a good reason why they are always recommended in droid groups. 

  There are many different tutorials on programming the maestro so I wont go too in depth into that. What I will mention is that the sequences will be different for evey droid. Because the limits and positions will vary from servo to servo I recommend getting familiar with the Pololu software and play with it yourself. It ends up being very fun to program those sequences. You will then need to change the sequence numbers in this sketch. For example:

  ```
  //Open Door B with B
  if (Xbox.getChatpadClick(XBOX_CHATPAD_B, 0)) {
    maestro_body.restartScript(21);
    if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)){
      maestro_body.restartScript(5);
    }
  }
  ```
  Here my sequence for opening door B is index 21, it will likely be different for you. This is why I recommend looking at tutorials for this. It will make your life easier.


### Dome

Review the documentation made available by Dimension Engineering for the Syren Motor Controller.

Connect the pins for the Syren Motor Controller

| Syren10 | Arduino UNO |
| ------- | ----------- |
| S1      | 5           |
| 0v      | GND         |

| Syren10 | Arduino Mega  |
| ------- | ------------- |
| S1      | Serial2 (Tx2) |
| 0v      | GND           |

| Syren10 | Battery  |
| ------- | -------- |
| B+      | Positve  |
| B-      | Negative |

| Syren10 | Dome Motor   |
| ------- | ------------ |
| M1      | 1 (Positve)  |
| M2      | 2 (Negative) |

If you find that the dome spins the opposite direction, flip M1 and M2. When standing behind the droid, moving left on the left analog stick should rotate the dome left.

If using a Syren speed controller for the dome, you will have choose what type of serial connection to use.
Packetized is the best choice for the Syren, but you might need to change the baud rate . This is done by changing the value of int domeBaudeRate at the beginning of the sketch. All supported rates are listed there.
If you can't get packetized to work, (and some people have had problems) you will have 2 options

#### Option 1

Simplified serial- easy to setup, proven to work, BUT a chance that the dome could start spinning if power is lost to the arduino but not the syren. While I haven't seen this happen without physically yanking the power wire or pressing the reset button the risk is there. I would say now that we know of this possibility, do not use this option if people, especially children, will be close enough to the droid to be injured. To use simplified serial, delete the // in front of the line #define SYRENSIMPLE at the beginning of the sketch.

#### Option 2 (Best Option)

Send the syren to Dimension Engineering to be flashed with the Ver. 2 firmware- This allows the syren to be locked in to a set baud rate and eliminates the auto-bauding problem. I have had this done to my own syren and have tested it with several types of arduinos with no problems. All new syrens are being shipped with this new firmware.

The dip switches on the Syren should be set to 1 and 2 off for Packetized, or 2 and 4 off for Simple.

In some cases, we've noticed that the dome may behave eratically after starting up. If this is the case plugging a 10k resister between the S1 and 0V screw terminals. Simply bend the pins and screw them in along with the wires.

Make sure that you use at least a 14 Guage wire between the motor and the Syren 10. Anything hire can cause problems (fire!)

### Foot Drive

Review the documentation made available by Dimension Engineering for the Sabertooth Motor Controller.

Connect the pins for the Syren Motor Controller:

| Sabertooth ((2x32 2x25 or 2x12) | Arduino UNO |
| ------------------------------- | ----------- |
| S1                              | 4           |
| S2                              | 3           |
| 0v                              | GND         |

| Sabertooth ((2x32 2x25 or 2x12) | Arduino Mega  |
| ------------------------------- | ------------- |
| S1                              | Serial1 (Tx1) |
| S2                              | Serial1 (Rx1) |
| 0v                              | GND           |

| Sabertooth (2x32 2x25 or 2x12) | Battery  |
| ------------------------------ | -------- |
| B+                             | Positve  |
| B-                             | Negative |

| Sabertooth ((2x32 2x25 or 2x12) | Foot Motors            |
| ------------------------------- | ---------------------- |
| M1A                             | Right Motor Terminal 1 |
| M1B                             | Right Motor Terminal 2 |
| M2A                             | Left Motor Terminal 1  |
| M2B                             | Left Motor Terminal 2  |

Please use a maximum of 12 Gauge for the wires going to the Sabertooth and motors/power.

Sabertooth 2x32 / 2x25 / 2x12 dip switches should 1 & 2 OFF and all others ON if using a regular SLA battery. If you're using Lithium-Ion batteries set switch #3 off also but consult your Sabertooth controller documentation.

If you're using the 2x32 and you've tinkered with Dimension Engineering's DEScribe Software to tweak settings on your motor controller, under the Serial and USB tab, make sure the Baud Rate is set to 9600. This should be the default, but you should verify.

If you're using 5v to power some components on your I2C device chain, you can use the 5V terminal on the Sabertooth and connect to the positive I2C pin header on the slipring board and 0V on the Sabertooth to GND on the slipring board.

### Arduino MEGA

Install the libraries from the Libraries folder. Upload the corresponding padawan360_maestro sketch from this `padawan360_maestro` folder for your Arduino (UNO or Mega) sketch to the Arduino. There is one for the UNO and one for the Mega.

Review the sketch as there are some configuration options at the top of the sketch with descriptive comments that you may want to adjust to control speed levels, turn speed,

### Controller Pairing

Power the Arduino up. It will also power up the Receiver. Press the big button on the receiver, it will blink. Press the center Guide button the controller to turn the controller too. It will blibk. Press the little sync button located on the top edge of the controller. The controller blink too, then they should sync up and the blinking pattern on the controller will change and swirl. This indicates a connection and that they are paired.

There's also the [Xbox Support Guide](http://support.xbox.com/en-US/xbox-on-other-devices/connections/xbox-360-wireless-gaming-receiver-windows).

### Options, Configurations, and Settings

Review the top of the Arduino Sketch that you will be uploading to your droid. There are a number of options and configurations that you may need to tune for YOUR droid specifically. Provided are defaults and standard that generally work well but you may find that based on your drive system, power system, and personal preferences you may want to adjust them. The top of the sketch documents each of these settings and provides defaults and includes explanations and descriptions of what each of them does and potential changes you may want to change. Review these carefully.

### Video Guide

Builder Steve Baudains has put together a few videos walking through setup of the main components.

- [Video 1](https://www.youtube.com/watch?v=oZw0zzCgoh8)
- [Video 2](https://www.youtube.com/watch?v=wXRDHJttQbc)

## Controls

### Controller LED Status

| LEDs Around Center Guide Button | Description                                   |
| ------------------------------- | --------------------------------------------- |
| 4 Flashing/Blinking in Unison   | Looking for connection to receiver            |
| Rotating/Spinning pattern       | Connected to receiver. Foot motors disengaged |
| Single LED Top Left - Steady    | Foot Motors Engaged. Speed 1                  |
| Single LED Top Right - Steady   | Foot Motors Engaged. Speed 2                  |
| Single LED Bottom Left - Steady | Foot Motors Engaged. Speed 3                  |

### Button Guide

Press guide button to turn on the controller.

Press Start button to engage motors!

Drive stick is now (as of 2019-11-01) on the LEFT STICK and dome control is on the RIGHT STICK.
They have been reversed from what is seen in the below controller guide.
Set `isLeftStickDrive` in the code to false to drive with the RIGHT STICK as seen in the guide.

![Button Guide](https://github.com/dankraus/padawan360/blob/master/xbox360-controller-guide.jpg)

(Button layout image courtesy of LarryJ on Astromech. Thanks!)

## Troubleshooting and FAQs

### Arduino IDE

_**When I try to upload the sketch I get an error like: sabertooth.h: No such file or directory OR somefile: No such file or directory**_
The libraries included in this repository need to be installed. Instructions on how to install Arduino libraries is beyond the scope of this documentation. Refer to the official Arduino documentation here: https://www.arduino.cc/en/guide/libraries#toc5

### General Control Issues

Not triggering sound or getting any movement? Make sure you're paired. The best way is to use the Xbox Wireless library's example sketch.

1. File>Examples>USB Host Shield>Xbox>Xbox Recv
2. Upload that to your Arduino
3. Look at the code and you'll see `Serial.begin`. Take note of that number.
4. Tools>Serial Monitor. Set the baud rate to that number from step 3.
5. Pair your controller.

Press buttons, do you see the button names output? If you don't, you're not paired. If it's paired, you'll see the button names in the serial monitor as you press them.

### Pairing

_**My controller and receiver won't pair. I don't see any output in the serial monitor in the Xbox Recv test sketch**_

- Confirm that you are using an official Xbox 360 Wireless Controller and Xbox 360 Wireless USB Receiver. If you're not, it's likely the culprit. Get yourself an official one and save yourself some trouble. That said, some people have had luck making some adjustments to get some 3rd Party receivers working. Read up on it here: https://astromech.net/droidwiki/Cheap_XBox_Receivers
- Your USB Shield may need voltage pins jumped. See Steve Baudain's YouTube video here walking through it: https://www.youtube.com/watch?v=y9HEeBO3cV0

### Sound

If the MP3 Trigger isn't functioning as expected, or at all, or just behaving a little "odd" - it would be good to make sure you have the most up to date firmware. Version 2.54 is known to work. It can be downloaded [here ](http://robertsonics.com/mp3-trigger-downloads/). The user manual has instruction for installation on the last page (as of this writing) of the instruction manual under "Bootloader". The manual is available from the previous link and also available directly [here](http://robertsonics.com/resources/mp3trigger/MP3TriggerV2UserGuide_2012-02-04.pdf). As of 2020, you're not likely to run into this unless you happened to buy really old inventory or a spare from another builder that had one laying around for years.

Some users had experienced some issues of sounds freezing up and going a bit haywire. It was resolved by using the barrel jack power connector as the Mp3Trigger was browning out. That means that disconnecting the 5v and Ground pins between the Arduino and MP3Trigger and switching flip the switch to EXT. This is indicative of power brown outs or some other electrical issue.

If the wrong sounds are playing for button presses, they are likely added to the SD card in the wrong order. They must be put on the card in the exact order one at a time or by using Drivesort. Reference [this video](https://youtu.be/UsMI2gW7Q40) for how load the sound files up with Drivesort to ensure they're loaded onto the SD Card in proper order.

Loading the sound files must be done with a Windows PC. Unfortunately, MacOS will leave some helper index files to inprove OS features like Spotlight and other assorted MacOS specific things. If anyone has done this on a Mac, please let me know!! Likely this can be done on Linux, but I've yet to do it personally. If you have, please let me know.

### Dome

_**My Dome just randomly spins! My Dome won't spin! My Dome doesn't spin correctly** etc etc.._

Depending on the firmware on the Syren, sometimes you need to change the serial baud rate it communicates at. Change this line in the code: `const int DOMEBAUDRATE = 9600;` For packetized options are: 2400, 9600, 19200 and 38400

_**The dome spins in the wrong direction when I move the dome control stick!**_

Flip the leads to the dome motor. There is no positive and negative. Orient your controls from the perspective of the droid. Stand behind the droid so that pressing
on the stick makes the dome spin left and right on the stick makes the some spin right.

_**The left analog stick is centered but the dome still spins!**_

You need to just adjust the deadzone `const byte DOMEDEADZONERANGE = 20;` Increase this number until you can let the stick go neutral and nothing moves. The sketch has some more info on that above setting that value to adjust for controller stick drift.

### Drives

_**I've wired up my Sabertooth but it doesn't move!**_

Don't forget to press START on the controller to enable the motors. Still not working? Put a multimeter to the terminals for a motor and push forward on the stick (after enabling the motor controls of course by pressing START). The voltage should go from 0v to something near the voltage of the power supplies from your battery. Ex, Speed 1 on a 12v battery may show 7v when stick is pushed forward all the way. Speed 3 may show closer to 12v. If you're not getting any voltage at the terminals, check the baud rate in the code, dip switches, and for correct wiring.

_**When I press left on the drive stick, it turns to the right (or vice versa)! When I push forward on the drive stick it drives backwards (or vice versa)!**_

People often regularly run into this once they power everything up and find that they push forward on the stick and it drives to left, stick to the left drives drives backwards, etc. Don't fret, motors can be wired positive/negative. It doesn't matter. Start flipping the motor connections to the Sabertooth. First flip M1A and M1B. If that doesn't fix it, flip it back and try the other. If it still isn't right, then try flipping both sets. R2 is driven like a tank. Spin the right motor forward to go left, left motor forward to go right. Both motors forward, drive forward. Keep that in mind as you troubleshoot.

Orient your controls from the perspective of the droid. Stand behind the droid so that pressing left on the drive stick makes the droid turn left, right on the stick makes the droid turn right, up on the stick makes the droid drive forward, down on the stick makes the droid drive backwards.

_**The right analog stick is centered but it still drives(turns, drives forward, drives reverse, etc)!**_

You need to just adjust the deadzone `const byte DRIVEDEADZONERANGE = 20;` Increase this number until you can let the stick go neutral and nothing moves. The sketch has some more info on that above setting that value to adjust for controller stick drift.

## Licensing

See the LICENSE file in this repository.
