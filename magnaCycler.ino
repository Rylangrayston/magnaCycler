
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
 
 
int pauseButtonPin = 6;
int solinoidPulseTime = 100;
int solinoidOnPin = 3;
int solinoidOffPin = 2;
int cammeraShuterPin = 7;
int lastTime = 0;
int ssRelayPin = A0;
int relayStartUpTime = 100;

int powerOffBeforeConnectDelay = 100;
int powerOffBeforeDisconnectDelay = 100;

boolean powerOffBeforeConnect = false;
boolean powerOffBeforeDisconnect = false;


unsigned long cycleCount = 0;

int powerOnTime =  2000;
int powerOffTime = 40;
unsigned long totalCycles = 200000;
int cyclesPerPicture = 1000;

int cycleTime = powerOnTime + powerOffTime + 2 * solinoidPulseTime ;
boolean useScreen = true;
boolean paused = true;



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
   if (powerOffBeforeConnect == true)
  {
    delay(powerOffBeforeConnectDelay);
    digitalWrite(ssRelayPin, HIGH);
  } 
  
  delay(powerOnTime);//////////////////////////////////////////////// amprage is high here
  

  
  if (powerOffBeforeDisconnect == true)
  {
  digitalWrite(ssRelayPin, LOW);
  delay(powerOffBeforeDisconnectDelay);
  }
  
  disconnectBrass();
  delay(powerOffTime);
  digitalWrite(ssRelayPin, LOW);
  
}



void updateJobRunningScreen()
{
 
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
    
  String countMesage = "count: ";
  countMesage += String(cycleCount);
  String totalCyclesMesage = "of:" + String(totalCycles);
  String featureMesage = "POBC:" + String(powerOffBeforeConnect) + " POBD:" + String(powerOffBeforeDisconnect); ///////////////
  String powerOnTimeMesage = "POT:" + String(powerOnTime);
  
  myGLCD.print("Magna Cycler", CENTER , 0);
  myGLCD.print(countMesage, CENTER, 8); 
  myGLCD.print(totalCyclesMesage, CENTER, 16);
  myGLCD.print(powerOnTimeMesage, CENTER, 24);
  myGLCD.print(featureMesage, CENTER , 32);
  if(paused) 
    {
    myGLCD.invertText(true);
    myGLCD.print("PAUSED", CENTER , 40);
    myGLCD.invertText(false);
    }
   else{ myGLCD.print("RUNNING", CENTER , 40);}
   
   myGLCD.update();
    
}
                 


void setup()
{

  myGLCD.InitLCD();
             // initialize serial communication at 9600 bits per second:
                   Serial.begin(9600);
                  
                    pinMode(pauseButtonPin, INPUT_PULLUP);
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

if (useScreen) {
updateJobRunningScreen();
}  

if (digitalRead(pauseButtonPin) == LOW )
{
paused = false;
cycleCount += 1;
cycleContact();
}
else{paused = true;}


}
