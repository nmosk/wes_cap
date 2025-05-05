#include <SoftwareSerial.h>
#define setPin 4
#define buttonPin 5

SoftwareSerial HC12(3, 2);         // HC-12 TX Pin, HC-12 RX Pin
byte incomingByte;
String readBuffer = "";
int p = 0;
int val = 0;
int on = 0; 
int IS_PRESSED = 0;
int SEND_CLOSE = 0;

// buffer counters
int pset = 0;
int pbuff = 0;
int tap = 0;
int tapSet = 0;

// click controls
int CLICK_ONE = 0;
int CLICK_TWO = 0;
int PAUSE = 0;

void setup() {
  Serial.begin(9600);                   // Open serial port to computer
  HC12.begin(9600);                     // Open serial port to HC12
  Serial.print("\nGot here.\n");
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, LOW);
  delay(250);
  HC12.write("AT+DEFAULT");
  Serial.print("\n2vGot here.\n");
  delay(250);
  digitalWrite(setPin, HIGH);           // HC-12 normal, transparent mode
  pinMode(buttonPin, INPUT);
  Serial.print("\n 3 Got here.\n");
}


void loop() {
  
  delay(20); // shorter than 10 does not register double click... longer than 20 and you wait longer for command to proceed
  buttonCheck();
  clickCount();
  // debugging click counting
      // Serial.print("tap is ");
      // Serial.print(tap);
      // Serial.print("\tCLICK_ONE is ");
      // Serial.print(CLICK_ONE);
      // Serial.print("\tPAUSE is ");
      // Serial.print(PAUSE);
      // Serial.print("\tCLICK_TWO is ");
      // Serial.println(CLICK_TWO);

  p++;
  pbuff = p - pset;
  // buffer less than 18 cycles and may be too quick, over 25 is slow to reset.
  if (pbuff > 18){
    if (CLICK_TWO){
      sendClose();
      delay(800);
    } 
    else if (CLICK_ONE){
      sendOpen();
      delay(800);
    };
    tap = 0;
    CLICK_ONE = 0;
    PAUSE = 0;
    CLICK_TWO = 0;
  } else;


}

void buttonCheck(){
  val = digitalRead(buttonPin);
  //Serial.println("Button value is :");
  //Serial.println(val);
  if (val == 0) {
    tap++;
    pset = p;}
  return tap;
}

void clickCount(){

  if (tap > 0) {
    CLICK_ONE = 1;
    } else;
  if ((tap > 0) && (val == 1)) {
    PAUSE = 1;
    tapSet = tap;
  } else;
  if (CLICK_ONE && PAUSE && (tap > tapSet)){
    CLICK_TWO = 1;
  } else;
  return 0;

}

void sendOpen(){
    Serial.println("Sending Open Signal...");
    Serial.println("Sending Open Signal...");
    HC12.write("B");
    Serial.println("Signal Sent..!");
}

void sendClose(){
    Serial.println("Sending Close Signal...");
    Serial.println("Sending Close Signal...");
    HC12.write("C");
    Serial.println("Signal Sent..!");
}

