## 2025 UCSD WES Capstone Project - N. Mosk

### Background: 
This repo contains the code used in the UCSD Fire Defense project developed by Nate Mosk for UC San Diego class WES 207. This is the capstone project for the Masters of Advanced Studies Wireless Embedded Systems program. 

The project goal is to create a fire defense system that allows a sprinkler to be controlled wirelessly. The sprinkler will sit atop a roof and be actuated by a valve connected to a control computer. The control computer can be accessed wirelessly by wifi and also by a wireless remote control.

##### File Guide:

| No.  | File name | Where it gets loaded | Purpose |
| ---  | ---       | ---     | --- |
|     1| server_with_remote.ino| Arduino R4 Wifi | Runs the web server, the remote receiver, and actuates the valve relay |
|     2| transmitter_rev1_button_double-click.ino    | Arduino Nano | Runs the remote control signal transmitter | 
|     3| "Fire suppression system 5-30.pptx"    | N/a | Visual Background Info |
|     4| test_archive folder    | N/a | Contains old code used to develop | 

The server file (#1) is the main file that runs on the arduino.

The transmitter file (#2) is the file that runs on the arduino nano that powers the remote control.

##### How to use the files:

* For file 1, be sure to select either web mode or AP mode depending on whether you want to access the server via a router bridge or the system's own hotspot.
* If using web mode, you will need to add the SSID and password into the code. You will also need to access your router and set up port forwarding.
* For file 2, nothing should need to be updated, just upload it to the Arduino nano inside the remote.

##### Hardware details

* Arduino R4 wifi is used as the main server board
* DHT11 is used as a temperature and humidity sensor that would be attached to a roof tile
* HC-12 is a wireless module used to facilitate comms between the remote and the server
* Arduino Nano is used as the controller for the remote
* 12V D batteries are used to power the hardware

##### Outline of server_with_remote.ino
<details>
<summary> server_with_remote.ino </summary>
// File ID Info
<br>// list of libraries added  
<br>// macro definitions of IO pins and variables  
<br>// declaration of functions  
<br>// Set up code  
<br>&nbsp;&nbsp;&nbsp;// Begin serial ops  
<br>&nbsp;&nbsp;&nbsp;	// set digital pins  
<br>&nbsp;&nbsp;&nbsp;	// configure wireless HC12 module  
<br>&nbsp;&nbsp;&nbsp;	// configure the DHT11 sensor module  
<br>&nbsp;&nbsp;&nbsp;	// configure the onboard wifi module  
<br>&nbsp;&nbsp;&nbsp;	// set access mode or web mode  
<br>&nbsp;&nbsp;&nbsp;	// begin the web server  
<br>&nbsp;&nbsp;&nbsp;	// print wifi info to Output  
<br>// Loop Code  
<br>&nbsp;&nbsp;&nbsp;	// read DHT sensor for temp and humidity  
<br>&nbsp;&nbsp;&nbsp;	// Check for client on server  
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;		// Print web app out to client  
<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;		// create links for high /H and low /L for the water valve  
<br>&nbsp;&nbsp;&nbsp;	// read the HC12 sensor for incoming wireless comms  
<br>// Write out functions  
<br>&nbsp;&nbsp;&nbsp;	// define printwifi function  
<br>&nbsp;&nbsp;&nbsp;	// define showWifi status function  
<br>&nbsp;&nbsp;&nbsp;	// define readHC function  
</details>

### Link to Video of System:

[![UC SD Fire Defense System](http://img.youtube.com/vi/_rKZSBtwn8Y/0.jpg)](https://www.youtube.com/watch?v=_rKZSBtwn8Y))

#### How to use DHT11:

To use DHT11 with Arduino, 
1. Download the DHT libary (Sketch>Include Library>Manage Libaries and download DHT sensor library by Adafruit)
2. Include the library in the program `#include <DHT.h>`
3. I created macros for DHTPIN number to track the number of the digital pin that gets data from the sensor, and the DHTTYPE (DHT11)
   `#define DHTTYPE DHT11`
   `#define DHTPIN 6`
4. Use a constructor to declare a dht object `DHT dht(DHTPIN,DHTTYPE);`
5. Then, all you have to do is begin the dht, and collect the data with the following:
  `dht.begin();`
  `float temperature = dht.readTemperature();`
  `float humidity = dht.readHumidity();`
6. Obviously, the sensor will be wired with a wire to VCC (3-5v), ground, and the Digital pin specified earlier (6 in my case).

It really is that simple.

#### How to use HC-12:

To use HC12 with Arduino, 
1. The HC-12 module is compatible with the softwareSerial libary, so include the library in the program `#include <SoftwareSerial.h>`
3. I created macros to track the number of the digital pin that serve as RX and TX, and Set for the HC12, although this is optional
   `#define HC12_RX_PIN 2`
   `#define HC12_TX_PIN 3`
4. Declare a software serial object `SoftwareSerial HC12(3, 2);`
5. Begin the serial console and the HC-12 serial connection , and collect the data with the following:
  `Serial.begin(9600);`
  `HC12.begin(9600);`
6. Just to be safe, configure the HC-12 in default mode. To do this you have to toggle the Set pin low, then change the setting, then toggle the set pin back to high:
  `digitalWrite(setPin, LOW);
  HC12.write("AT+DEFAULT");
  digitalWrite(setPin, HIGH);`
8. Once again, the module will be wired with a wire to VCC (3-5v), ground, and the Digital pins for RX, TX, and Set.
9. Then all you really need is to record incoming data to a buffer `incomingByte = HC12.read();` and that's it.




