#include <avr/sleep.h>
#include <avr/power.h>
#include <SoftwareSerial.h>

int bluetoothTx = 10;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 11;  // RX-I pin of bluetooth mate, Arduino D3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int pin2 = 2;


/***************************************************
 *  Name:        pin2Interrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt  
 *
 ***************************************************/
void pin2Interrupt(void)
{
  /* This will bring us back from sleep. */
  
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();
  
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the Arduino.           
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  while(!Serial){
  }
  bluetooth.begin(9600);  // The Bluetooth Mate defaults to 115200bps
  
  /* Setup the pin direction. */
  pinMode(pin2, INPUT);
  
  Serial.println("Initialisation complete.");
}



/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
int seconds=0;
void loop()
{
  delay(1000);
  seconds++;
  while(bluetooth.available())  // If the bluetooth sent any characters
  {
    //Serial.println("Was erhalten");
    // Send any characters the bluetooth prints to the serial monitor
    Serial.print((char)bluetooth.read());  
  }
  Serial.println("");  
  Serial.print("Awake for ");
  Serial.print(seconds, DEC);
  Serial.println(" second");
  
  if(seconds == 3)
  {
    Serial.println("Entering sleep");
    delay(200);
    seconds = 0;
    enterSleep();
  }
}
