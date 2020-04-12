#include <Servo.h>
#include "LowPower.h"

//
// The sketch emulates time-based finger press with use of a servo
// It uses a potentiometer to adjust the time interval between presses
// The Arduino Pro mini is used and to minimize connections the pot is connected
// to VCC, A3 and A2 (all the pins are close to each other), so A2 serves as GND
// after pin config 
// servo is connected to SERVO_PIN which is recommended to be D0, beause the servo
// is pluged to programming (UART) port to have GND, VCC and PWM pin on it 
// 

#define LOW_POWER

//#define DEBUG

#define SMAX        0   // servo angle (max), [deg]
#define SMIN        20  // servo angle (min), [deg]
#define TMIN        10  // minimum time period between 'finger' clicks, [second]
#define TMAX        20  // maximum time period between 'finger' clicks, [second]
#define SRV_ON_TIME 2   // click time, [second]

#ifdef DEBUG
  #define SERVO_PIN 8
#else
  #define SERVO_PIN 0
#endif

#define LED_PIN     13

Servo myservo;   // create servo object to control a servo

int potpin = 3;  // analog pin used to connect the potentiometer
int val;         // variable to read the value from the analog pin

uint8_t mode = 0;
long now_prev = 0;
uint16_t sec = 0;
bool sleep = false;

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{ 
  mode = 0;
  now_prev = 0;
  sec = 0;
  sleep = true;
  asm volatile ("  jmp 0");  
}  


void setup() {
  myservo.attach(SERVO_PIN);            // attaches the servo on pin 
  val = analogRead(potpin);             // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, TMIN, TMAX);
  myservo.write(SMIN);                  // sets the servo position according to the scaled value
  pinMode(A2, OUTPUT);                  // Ground pin for the pot
  digitalWrite(A2, LOW);
  pinMode(LED_PIN, OUTPUT);             // LED
#ifdef DEBUG
  Serial.begin(115200);
#endif
}

void loop() {
  long now = millis();

#ifdef DEBUG
  Serial.print(val);
  Serial.print("\t");
  Serial.print(sec);
  Serial.print("\t");
  Serial.println(mode);
#endif
  
switch(mode)
{
  case 0: //Move to start position
    sleep = false;
    myservo.write(SMIN);
    if(sec >= SRV_ON_TIME)
    {
      sleep = true;
      mode = 1;
      sec = 0;
    }  
  break;
  case 1: //Waiting
    #ifdef LOW_POWER
      myservo.detach();
    #endif
    if(sec >= val)
    {
      mode = 2;
      sec = 0;
    }  
  break;
  case 2: //Moving to end
    #ifdef LOW_POWER
      myservo.attach(SERVO_PIN); 
    #endif
    myservo.write(SMAX);
    sleep = false;
    if(sec >= SRV_ON_TIME)
    {
      sec = 0;
      mode = 3;
    }
  break;
  case 3: //Reinit
    sleep = true;
    //software_Reset(); //call reset 
    val = map(val, 0, 1023, TMIN, TMAX); 
    mode = 0;
  break;
  }

if(sleep)
{
  sec++;
  digitalWrite(LED_PIN, HIGH);
}
else 
{
  if(now - now_prev > 1000) //second
  {
    digitalWrite(LED_PIN, !digitalRead(13));
    now_prev = now;  
    sec++;
  }
}
      
  delay(20);
#ifdef LOW_POWER
  if(sleep)
  {
    digitalWrite(LED_PIN, LOW);
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }
#endif
}
