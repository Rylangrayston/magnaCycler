
/*


 This program requires a Nokia 5110 LCD 

 It is assumed that the LCD module is connected to
 the following pins using a levelshifter to get the
 correct voltage to the module.
      SCK  - Pin 8
      MOSI - Pin 9
      DC   - Pin 10
      RST  - Pin 12
      CS   - Pin 11
 */
 
 
 
int solinoidPulseTime = 100;
int solinoidOnPin = 3;
int solinoidOffPin = 2;
int cammeraShuterPin = 7;
int lastTime = 0;
int ssRelayPin = 6;
int relayStartUpTime = 100;

int powerOffBeforeConnectDelay = 100;
int powerOffBeforeDisconnectDelay = 100;

boolean powerOffBeforeConnect = false;
boolean powerOffBeforeDisconnect = false;


float cycleCount = 0;

int powerOnTime =  500;
int powerOffTime = 40;
long totalCycles = 200000;
int cyclesPerPicture = 1000;

int cycleTime = powerOnTime + powerOffTime + 2 * solinoidPulseTime ;





//  Screen stuff:                     
#include <LCD5110_Graph.h>
//             sck, Mosi, dc, rst, cs
LCD5110 myGLCD(8,   9,  10, 12,   11);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];

void connectBrass()
{
  digitalWrite(solinoidOnPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(solinoidPulseTime);               // wait for a second
  digitalWrite(solinoidOnPin, LOW);    // turn the LED off by making the voltage LOW
 }


void disconnectBrass()
{
  digitalWrite(solinoidOffPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(solinoidPulseTime);               // wait for a second
  digitalWrite(solinoidOffPin, LOW);    // turn the LED off by making the voltage LOW
  }

void cycleContact()
{
  if (powerOffBeforeConnect == false)
  {
    digitalWrite(ssRelayPin, HIGH);
    delay(relayStartUpTime);
  }
  
  if (powerOffBeforeConnect == true)
  {
    digitalWrite(ssRelayPin, LOW);
  }
  
  connectBrass();
  delay(powerOnTime);
  
  if (powerOffBeforeConnect == true)
  {
    delay(powerOffBeforeConnectDelay);
    digitalWrite(ssRelayPin, HIGH);
  }
  
  if (powerOffBeforeDisconnect == true)
  {
  digitalWrite(ssRelayPin, LOW);
  delay(powerOffBeforeDisconnectDelay);
  }
  
  disconnectBrass();
  delay(powerOffTime);
  
}



void doScreenStuff()
{
 
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.print("Will Toppings", CENTER, 0);
  
  //myGLCD.invertText(true);
  myGLCD.print(" Magna Cycler ", CENTER, 8);
  //myGLCD.invertText(false);
  myGLCD.print("Cycles", CENTER, 16);
  myGLCD.print(" Fuck Ya ", CENTER, 40);
  myGLCD.update();

  myGLCD.setFont(MediumNumbers);
  
  
  //for (int s=20; s>=0; s--)
  //for (int s=20; s>=0; s--)
 // {
    //myGLCD.printNumI(s, CENTER, 24, 2, '0');
    
    
    myGLCD.printNumI(          cycleCount    , CENTER, 24, 2, '0');
    myGLCD.update();
    //delay(1000);
 // }
  
    //myGLCD.enableSleep();
    //myGLCD.clrScr();
    //myGLCD.setFont(SmallFont);
    //myGLCD.print("Awake again!", CENTER, 0);
    //myGLCD.print("Text has been", CENTER, 16);
    //myGLCD.print("changed while", CENTER, 24);
    //myGLCD.print("in Sleep Mode.", CENTER, 32);
    //delay(5000);
    //myGLCD.disableSleep();
    //delay(5000);
    
     
}
  
                    


void setup()
{
  myGLCD.InitLCD();
                   // initialize serial communication at 9600 bits per second:
                   Serial.begin(9600);
                  
                   //pinMode(pushButtonPin, INPUT_PULLUP);
                    pinMode(solinoidOnPin, OUTPUT); 
                    pinMode(solinoidOffPin, OUTPUT);
                    pinMode(cammeraShuterPin, OUTPUT);
                    pinMode(ssRelayPin, OUTPUT);
                    
                    
                    

if (powerOffBeforeConnect == true) {cycleTime += powerOffBeforeConnect;} 
if (powerOffBeforeConnect == false) {cycleTime += relayStartUpTime;}
if (powerOffBeforeDisconnect == true) {cycleTime += powerOffBeforeDisconnect;} 


}



void loop()
{
cycleCount += 1;

doScreenStuff();  
    
cycleContact();



}
