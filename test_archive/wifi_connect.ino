
// #include <SoftwareSerial.h>
#include <WiFiS3.h>

#define SECRET_SSID "9-dollars-per-mo.TXT442-216-6325"
#define SECRET_PASS "plainbird962"

// #define SECRET_SSID "Mathnasium Guest"
// #define SECRET_PASS "M@th1234"


#define LED_ON "Water on. (your home cannot possibly burn now)"
#define LED_OFF "Water off."
String led_stat = LED_OFF;

// declare functions
void printWiFiStatus();
int pingTest();

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status

// pingTest vars
String hostName = "www.google.com";
String homeWifi = "192.168.1.42";
int pingResult;


// Access point
int led = LED_BUILTIN;
WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:

  // access point set up
  pinMode(led, OUTPUT);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  delay(4000);

  Serial.println("\nyyyy begin set up yyyy");

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // you can override it with the following:
  WiFi.config(IPAddress(192,168,1,99));

  Serial.println(WiFi.status());

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    
  }

  Serial.println(fv);
  Serial.println("Latest ver:");
  Serial.println(WIFI_FIRMWARE_LATEST_VERSION);

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(8000);
  }

  // // you're connected now, so print out the data:
  // Serial.print("You're connected to the network");

  if (status == WL_CONNECTED){

    Serial.println("Wifi is connected");
  } 

  server.begin();

  printWiFiStatus();

  Serial.println("xxxxx end of set up xxxxx");

}





void loop() {
  // put your main code here, to run repeatedly:

 // Serial.println("Hello");

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

            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>UC SD Fire Suppressor</title>");
            client.println("<h1 style=\"font-size:7vw;\">UC Fire Suppressor ver 1.0</h1>");
            client.println("</head>");
            client.println("</html>");

            // the content of the HTTP response follows the header:
            client.print("<p style=\"font-size:6vw;\">Click <a href=\"/H\">here</a> turn the water on<br></p>");
            client.print("<p style=\"font-size:6vw;\">Click <a href=\"/L\">here</a> turn the water off<br></p>");

            client.print("<p style=\"font-size:6vw;\">Status: </p>");
            client.print("<p style=\"font-size:6vw;\">");
            client.print(led_stat);
            client.print("</p>");




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
          led_stat = LED_ON;
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(led, LOW);                // GET /L turns the LED off
          led_stat = LED_OFF;
        }



      }
    }
    client.stop();
    Serial.println("client disconnected");
  }

 // pingTest();

  showWiFiStat();
  delay(4000);

}


int pingTest(){
 Serial.println("Ping test...");
 Serial.println(hostName);
 
 pingResult = WiFi.ping(homeWifi);

 Serial.print("Ping result: ");
 Serial.println(pingResult);

 Serial.println("End test");

 delay(8000);

 return 0;
}

void showWiFiStat(){

  Serial.print("Wifi status variable: ");
  Serial.println(WiFi.status());

  // FYI
  // 0 = no SSID available, WL_NO_SSID_AVAIL
  // 1 = device is idle, WL_IDLE_STATUS
  // 3 = device connected, WL_CONNECTED
  // 4 = disconnected, WL_DISCONNECTED


}

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
