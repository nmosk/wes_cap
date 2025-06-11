## 2025 UCSD WES Capstone Project - N. Mosk

### Background: 
This repo contains the code used in the UCSD Fire Defense project developed by Nate Mosk for UC San Diego class WES 207. This is the capstone project for the Masters of Advanced Studies Wireless Embedded Systems program. 

The project goal is to create a fire defense system that allows a sprinkler to be controlled wirelessly. The sprinkler will sit atop a roof and be actuated by a valve connected to a control computer. The control computer can be accessed wirelessly by wifi and also by a wireless remote control.

##### Two files run the system:

| No.  | File name | Where it gets loaded | Purpose |
| ---  | ---       | ---     | --- |
|     1| server_with_remote.ino| Arduino R4 Wifi | Runs the web server, the remote receiver, and actuates the valve relay |
|     2| transmitter_rev1_button_double-click.ino    | Arduino Nano | Runs the remote control signal transmitter | 

The server file (#1) is the main file that runs on the arduino.

The transmitter file (#2) is the file that runs on the arduino nano that powers the remote control.

##### How to use the files:

* For file 1, be sure to select either web mode or AP mode depending on whether you want to access the server via a router bridge or the system's own hotspot.
* If using web mode, you will need to add the SSID and password into the code. You will also need to access your router and set up port forwarding.
* For file 2, nothing should need to be updated, just upload it to the Arduino nano inside the remote.

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
