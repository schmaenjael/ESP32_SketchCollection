/*
 * Author: schmaenjael
 * Language:  C++ using the Arduino Library
 * Github: https://github.com/schmaenjael/ESP32_SketchCollection
 * Date: 21. January 2021
 * Refernce: https://github.com/espressif/arduino-esp32/
 */

const byte red_LED = 33;        // Pin for the LED on the Color-Clicker Module
const int awaitTime = 10000000; // time it waits until it triggeres the alarm
const bool countUp = true;      // sets the count direction --> counting upwards
const bool isEdge = true;       // to set the interrupt type to 'edge'
const bool autoReload = true;   // used to let the alarm reload and generate it periodically
const byte prescaler = 100;     // sets the prescaler to 100 -> it reduces the usually written timer by 100
const byte hw_Timer_Pos = 0;    // used to set the position of the timer to 0. Valid options vary between 0 and 3. if hw_Timer_Pos > 3 --> return NULL;
int flag = 0;                   // the flag variable is used in the method below that is pointed to on timerAttachInterrupt()

/* 
 * This method is used to switch between HIGH and LOW for the light-emitting diode that is connected to port 33
 * If the integer value flag is even the red LED will be set to HIGH but if the modulo calculation by two results in a remainder the
 * light-emitting diode will be set to LOW and thus resulting in not glowing. After every method-call 'flag' will be increased by one to ensure the
 * LED will not be set twice to the same state
 */

void IRAM_ATTR onTimer()
{
  // checks if flag is even or uneven
  if (flag % 2 == 0)
  {
    digitalWrite(red_LED, HIGH); // sets the current voltage state to HIGH (1)
  }
  else
  {
    digitalWrite(red_LED, LOW); // sets the current voltage state to LOW (0)
  }
  flag++; // increments flag by one, is equal to flag += 1;
}

/*
 * The setup method sets the red LED to OUTPUT and initializes the timer with our already defined values. 
 * 
 * --> timerAttachInterrupt selects as a first paramter our *timer and as a second paramter our method onTimer the "&" is required because we want
 *     to point to our defined method. The third paramter got already explained above!
 *                      
 * --> timerAlarmWrite selects as a first paramter our *timer and sets the time it waits and if it should reload and thus be generated periodically.
 * 
 * --> timerAlarmEnable enables the alarm.
 */

void setup()
{
  pinMode(red_LED, OUTPUT);

  hw_timer_t *timer = timerBegin(hw_Timer_Pos, prescaler, countUp); //
  timerAttachInterrupt(timer, &onTimer, isEdge);
  timerAlarmWrite(timer, awaitTime, autoReload);
  timerAlarmEnable(timer);
}

void loop()
{
  // No main loop required to run this sketch
}
