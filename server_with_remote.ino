//////////////////////////////////////////////////////////////////////////////////////
//
//    2025 Spring Quarter WES 207
//    Nathaniel Mosk
//    Final Capstone Project - UC Fire Defense System
//
//    About: This is a program which runs on the R4 Arduino Wifi.
//            The program hosts a web server on the arduino, accepts a signal from a remote,
//            and actuates a relay switch. The relay switch is meant to be governing a valve
//            that turns water on so a sprinkler can protect a roof.
//
//    Contact: nmosk@ucsd.edu for questions, AA2-2l6-632S
//
///////////////////////////////////////////////////////////////////////////////////////

// adding libaries
  // WiFiS3.h needed for the built in esp wifi card on the R4 board
  // SoftwareSerial.h is used to allow serial comm to the board
  // Both Wire.h and Adafruit_SHT31.h are in place to support testing the SHT31-D humidity sensor module
  // DHT.h is used for the DHT11 arduino module that senses temp and humidity

#include <WiFiS3.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_SHT31.h>
#include <DHT.h>

// set to either web or AP mode
// When Web mode is set to 1, the server will be accesible thru the internet via port forwarding
// only use web mode when pre-planned to be configured with the router on site
// When AP mode is set to 1, the R4 board creates an access point and wifi hotspot to host the server site.
#define web_mode 0
#define AP_mode 1

// define your wifi password here. OR put into a dif header file for security.
#define SECRET_SSID "9-dollars-per-mo.TXT442-216-6325"
#define SECRET_PASS "plainbird962"


// define digital pins with human readable names
// The HC12 pins and set pins are for the HC12 wireless module that does serial comms
// the ValvePin goes to the relay switch on digital 7
// the DHT pin is the DHT sensor's data pin on digital 6
#define setPin 4
#define valvePin 7
#define HC12_TX_PIN 3
#define HC12_RX_PIN 2
#define DHTPIN 6


// these variables are used to status the water on the system and their control mechanisms
#define WATER_ON "Water on"
#define WATER_OFF "Water off"
String water_stat = WATER_OFF;
int IS_OPEN = 0;
int led = LED_BUILTIN;

// declare functions
void printWiFiStatus();
void showWiFiStat();


// declare sensor
// Define the sensor type (DHT11)
#define DHTTYPE DHT11
// Create a DHT object
DHT dht(DHTPIN, DHTTYPE);


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status
char mySsid[] = "UCSD_Fire_DP";        // your network SSID (name)
char myPass[] = "banana";    // your network password (use for WPA, or use as key for WEP)

// pingTest vars
String hostName = "www.google.com";
String homeWifi = "192.168.1.42";
int pingResult;

// establish server
WiFiServer server(80);

// establish humidity sensor SHT31-D
Adafruit_SHT31 mySensor = Adafruit_SHT31();


// create objects for wireless remote comms
SoftwareSerial HC12(3, 2);  // HC-12 TX Pin, HC-12 RX Pin
byte incomingByte;
String readBuffer = "";
int n;

void setup() {
  // put your setup code here, to run once:

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(4000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("\nyyyy begin set up yyyy");

  // set digital pins
  pinMode(led, OUTPUT); // on board LED - temp
  pinMode(setPin, OUTPUT); // HC12 set pin to take data
  pinMode(valvePin, OUTPUT); // set valve pin to take signal

  // configure HC12
  HC12.begin(9600); // start HC12 serial
  digitalWrite(setPin, LOW);
  delay(250);
  HC12.write("AT+DEFAULT"); // configure the HC12
  delay(250);
  digitalWrite(setPin, HIGH);          // HC-12 normal, transparent mode
  Serial.print("\n\n got past HC12 setup\n");

  dht.begin();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("temp is: ");
  Serial.println(temperature);
  Serial.print("humidity is: ");
  Serial.println(humidity);


  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  
 // WiFi.config(IPAddress(172,20,13,4));


  // print out wifi status - see status codes below
  // while connected to internet, we expect code 3
  // after it begins listening, code 7
  // while we have a client, code 8
  Serial.println(WiFi.status());
      // typedef enum {
    //     WL_NO_SHIELD = 255,
    //     WL_NO_MODULE = WL_NO_SHIELD,
    //     WL_IDLE_STATUS = 0,
    //     WL_NO_SSID_AVAIL,  1
    //     WL_SCAN_COMPLETED, 2
    //     WL_CONNECTED,  3
    //     WL_CONNECT_FAILED, 4
    //     WL_CONNECTION_LOST,  5
    //     WL_DISCONNECTED, 6
    //     WL_AP_LISTENING, 7
    //     WL_AP_CONNECTED, 8
    //     WL_AP_FAILED 9
    // } wl_status_t;


  // check fw version - this is returning 0.4.1 currently which works well
  // the latest may be 0.5.2 but that's regarded as developmental and 
  // procedure to update is not especially well established
  // disregard message to update for now - NJM
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  Serial.println(fv);
  Serial.println("Latest ver:");
  Serial.println(WIFI_FIRMWARE_LATEST_VERSION);


  if(AP_mode){
    status = WiFi.beginAP(mySsid);
    if (status != WL_AP_LISTENING) {

    Serial.println("Creating access point failed");
    while(true);

      // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    delay(50000);
    }
  }

  if(web_mode){

    // set IP to following 4 octets:
    WiFi.config(IPAddress(192,168,1,99));

    // attempt to connect to WiFi network:
    //while(0) {
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network:
      status = WiFi.begin(ssid, pass);
      //status = WiFi.begin(ssid);
      // wait 10 seconds for connection:
      delay(4000);
    }

    // // test connection status and print result:
    if (status == WL_CONNECTED){
      Serial.println("Wifi is connected");
    } 

  }

  // initiate the server process
  server.begin();

  // print out the SSID and local IP
  printWiFiStatus();

  // message to help track end of the set up
  Serial.println("xxxxx end of set up xxxxx");

}


void loop() {
  // put your main code here, to run repeatedly:

  delay(100);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  WiFiClient client = server.available();

  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      delayMicroseconds(10); // jic
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();

          // Below is the WEB APP created to support the Fire Defense System
          // The outline of the site code is:
          //
          // * HTML style settings
          // * Page Title and top Header
          // * Table showing House address of physical system or the Owner
          // * Table showing water status
          // * Tables showing Temp and Humidity of the roof sensor (DHT11)
          // * Buttons allowing user to turn on and off water
          // * Credits line

            client.println("<!DOCTYPE html>");
            
            client.println("<html>"); //font-family:verdana
            client.println("<head>");
            client.print("<style>th{font-size:6vw;}td{font-size:6vw;}");
            client.print("p,h1,h2,h3,h4,h5,h6,th,td,a{font-family:helvetica; color: #182b49;}");
            client.print("table, th, td {border: 1px solid black;border-collapse: collapse;text-align: center;}");
            client.print("th{background-color: #182b49;color: white;}");
            client.print("td{background-color: #C69214;}"); // gold - #C69214 yellow - #FFCD00
            client.print("table{width:80%;}");
            client.print("body{background-color:#ebedef;}");
            client.print("a:link, a:visited {background-color: #00629B; color: white; font-size:6vw; padding: 16px 28px; text-align: center; text-decoration: none; display: inline-block;}");
            client.print("a:active, a:hover {background-color:#FFCD00;}");
            client.println("</style>");
            client.println("</head>");
            client.println("</html>");

            client.println("<title>UC SD Fire Defense</title>");
            client.println("<h1 style=\"font-size:7vw;\">UC Fire Defense System</h1>");

            client.println("<br>");

            client.println("<table>");
            client.println("<tr>");
            client.println("<th>System Address</th>");
            client.println("</tr>");
            client.println("<tr>");
            client.println("<td>12 Pebble St.</td>");
            client.println("</tr>");
            client.println("</table>");
            client.println("<br>");

            client.println("<table>");
            client.println("<tr>");
            client.println("<th>Sprinkler Status</th>");
            client.println("</tr>");
            client.println("<tr>");
            client.print("<td>");
            client.print(water_stat);
            client.println("</td>");
            client.println("</tr>");
            client.println("</table>");
            client.println("<br><br>");

            client.println("<table>");
            client.println("<tr>");
            client.println("<th>Roof Humidity [%]</th>");
            client.println("</tr>");
            client.println("<tr>");
            client.print("<td>");
            client.print(humidity);
            client.println("</td>");
            client.println("</tr>");
            client.println("</table>");
            client.println("<br><br>");

            client.println("<table>");
            client.println("<tr>");
            client.println("<th>Roof Temp [C]</th>");
            client.println("</tr>");
            client.println("<tr>");
            client.print("<td>");
            client.print(temperature);
            client.println("</td>");
            client.println("</tr>");
            client.println("</table>");
            client.println("<br><br>");

            // the content of the HTTP response follows the header:
            client.print("<a href=\"/H\">Turn water on</a><br><br>");
            client.print("<a href=\"/L\">Turn water off</a><br>");
            
            client.print("<p style=\"font-size:4vw;\">v1.0<br>Developed by Nate Mosk for UC San Diego, 2025.</p>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }

                // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(led, HIGH);               // GET /H turns the LED on
          digitalWrite(valvePin, HIGH);
          water_stat = WATER_ON;
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);                // GET /L turns the LED off
          digitalWrite(valvePin, LOW);
          water_stat = WATER_OFF;
        }

        //readHC(); // incase client is connected still want to read...
      } // end of client available
      // here?
    } // end of client connected
    client.stop();
    Serial.println("client disconnected");
  }

 // Read HC is used every loop to check for input signal from the remote control
  delay(100);
  readHC();

  // This part prints the status of the wifi and water to output for the user to read in the arduino serial monitor
  showWiFiStat();
  Serial.println(water_stat);
  delay(3000);

}

////////////////////////////////////////////////////////
//
//    Supporting Functions below
//
/////////////////////////////////////


// showWifiStat very specifically prints out WiFi.Status in a readable format.
// A list of status types is given for convenience

void showWiFiStat(){

  Serial.print("Wifi status variable: ");
  Serial.println(WiFi.status());

  // FYI
  // 0 = no SSID available, WL_NO_SSID_AVAIL
  // 1 = device is idle, WL_IDLE_STATUS
  // 3 = device connected, WL_CONNECTED
  // 4 = disconnected, WL_DISCONNECTED
      // typedef enum {
    //     WL_NO_SHIELD = 255,
    //     WL_NO_MODULE = WL_NO_SHIELD,
    //     WL_IDLE_STATUS = 0,
    //     WL_NO_SSID_AVAIL,  1
    //     WL_SCAN_COMPLETED, 2
    //     WL_CONNECTED,  3
    //     WL_CONNECT_FAILED, 4
    //     WL_CONNECTION_LOST,  5
    //     WL_DISCONNECTED, 6
    //     WL_AP_LISTENING, 7
    //     WL_AP_CONNECTED, 8
    //     WL_AP_FAILED 9
    // } wl_status_t;

}


// printWifiStatus function is used to print the wifi SSID, the IP address of the arduino, and instructions how to access
// No inputs are needed, and no outputs are given

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

// readHC is used to receive data from the HC12/Arduino Nano powered remote control.

void readHC(){
    // read data from HC12 module
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }
  delay(3);
  
  // open the data and use it
  if (readBuffer != "") {
    //dataStamp();
    Serial.println("data received!!!");
    Serial.println(readBuffer);
    if (readBuffer == "B") {
      Serial.println("Sender: TX1 Sends Open Signal");
      IS_OPEN = 1;
      digitalWrite(led,HIGH);
      digitalWrite(valvePin, HIGH);
      water_stat = WATER_ON;
    } else;
    if (readBuffer == "C") {
      Serial.println("Sender: TX1 sends Close Signal");
      IS_OPEN = 0;
      digitalWrite(led,LOW);
      digitalWrite(valvePin, LOW);
      water_stat = WATER_OFF;
    } else;
    delay(3);
    //n++;
  }
  readBuffer = "";

}




