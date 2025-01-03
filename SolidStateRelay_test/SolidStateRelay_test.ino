/*
 * This is code to control 2 channel Solid State Relay (SSR) module using Arduino
 * Written by Ahmad Shamshiri for Robojax.com 
 * on Feb 24, 2019 at 15:11 in Ajax, Ontario, Canada

  Watch instruction video for this code: https://youtu.be/_giGPBS_WAo
  
 * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Robojax 2ch SSR control 20190224
int ch1Pin = 8;// define pin 2 for chan 1 of SSR
int ch2Pin = 9;// define pin 3 for chan 2 of SSR

int triggerType = HIGH;// type LOW if low trigger and HIGH if high trigger SSR is used

int ssrON, ssrOFF;// used for two different SSR trigger type. Do not change

void setup() {
    // Robojax 2ch SSR control 20190224
    pinMode(ch1Pin, OUTPUT);//define ch1Pin as output
    pinMode(ch2Pin, OUTPUT);//define ch2Pin as output
    
    if(triggerType == HIGH)
    {
      ssrON = HIGH;
      ssrOFF = LOW;
    }else{
      ssrON = LOW;
      ssrOFF = HIGH; 
    }       
  // Robojax 2ch SSR control 20190224
   digitalWrite(ch1Pin,ssrOFF);// set initial state of SSR 1: OFF
   digitalWrite(ch2Pin,ssrOFF);// set initial state of SSR 2: OFF      
    
}

void loop() {
    // Robojax 2ch SSR control 20190224
    digitalWrite(ch1Pin,ssrON);// turn relay SSR1 ON
    digitalWrite(ch2Pin,ssrOFF);// turn relay SSR2 OFF    
    delay(3000);// waite for 3 seconds
    
    digitalWrite(ch1Pin,ssrOFF);// turn relay SSR1 OFF
    digitalWrite(ch2Pin,ssrON);// turn relay SSR2 ON    
    delay(3000);// waite for 3 seconds    


     
    // Robojax 2ch SSR control 20190224
   delay(1);
   

}// loop
