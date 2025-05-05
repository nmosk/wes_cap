

#include <SoftwareSerial.h>
#define setPin 4
#define ledPin 7

int IS_OPEN = 0;

// set up time and time function
unsigned long initialMillis = millis();
void calculateTime();

// declare functions
void buttonReact();
void displayValveStatus();
void dataStamp();

// set up Reading from HC12 module
SoftwareSerial HC12(3, 2);         // HC-12 TX Pin, HC-12 RX Pin
byte incomingByte;
String readBuffer = "";
int n;

void setup() {

  Serial.begin(9600);                   // Open serial port to computer

  // print the compile time
  Serial.print("Compile time: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  // start HC12 serial
  HC12.begin(9600);

  // set pins
  pinMode(setPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(setPin, LOW);
  delay(250);
  HC12.write("AT+DEFAULT");
  delay(250);
  digitalWrite(setPin, HIGH);          // HC-12 normal, transparent mode
  Serial.print("\n\n got past setup\n");
}

void loop() {
  delay(100);

  // read data from HC12 module
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }
  delay(100);
  while (Serial.available()) {
    HC12.write(Serial.read());
  }
  delay(50);
  
  // open the data and use it
  if (readBuffer != "") {
    dataStamp();
    if (readBuffer == "B") {
      Serial.println("Sender: TX1 Sends Open Signal");
      buttonReact();
      IS_OPEN = 1;
      digitalWrite(ledPin,HIGH);
    } else;
    if (readBuffer == "C") {
      Serial.println("Sender: TX1 sends Close Signal");
      buttonReact();
      IS_OPEN = 0;
      digitalWrite(ledPin,LOW);
    } else;
    delay(100);
    n++;
  }
  readBuffer = "";
  delay(1000);
  displayValveStatus();

}


void buttonReact(){
    Serial.println("\nbutton was pushed - actuating lights in place of valve.");
    digitalWrite(ledPin,HIGH);
    delay(40);
    digitalWrite(ledPin,LOW);
    delay(20);
    digitalWrite(ledPin,HIGH);
    delay(40);
    digitalWrite(ledPin,LOW);
    delay(20);
    digitalWrite(ledPin,HIGH);
    delay(40);
    digitalWrite(ledPin,LOW);
  
    return 0;
}

// print the status of the peripheral
void displayValveStatus(){
  if (IS_OPEN) {
    Serial.println("Valve would be open.");
  }
  else {
    Serial.println("Valve would be closed.");
  }
  return 0;
}

// function to alert console that data rx, time of reception, and the message
void dataStamp(){
    Serial.println("DATA RECEIVED!!!");
    Serial.println("**************************");
    calculateTime();
    HC12.print(readBuffer);
    Serial.print("Message #");
    Serial.print(n);
    Serial.print(": ");
    Serial.println(readBuffer);
}

// Function to calculate current time
void calculateTime() {
  unsigned long currentMillis = millis();
  unsigned long timeDifference = currentMillis - initialMillis;

  // Convert to hours, minutes, and seconds
  unsigned long seconds = (timeDifference/1000) % 60;
  unsigned long minutes = (timeDifference/1000)/60;
  unsigned long hours = (timeDifference/1000)/3600;

  // Printing
  Serial.print("Reported at compile time (");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.print(") + ");
  Serial.print(hours);
  Serial.print(" hours, ");
  Serial.print(minutes);
  Serial.print(" min, ");
  Serial.print(seconds);
  Serial.println(" sec. Year of our Lord.");

  return 0;
}
