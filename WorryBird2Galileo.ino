#include <Servo.h> 

Servo servoTwist;  // create servo object to control a servo 
Servo servoTilt;  // create servo object to control a servo 
Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo 

int leftPin = 0;  // analog pin used to connect the left sensor
int potPin = 2;  // analog pin used to connect the potentiometer
int rightPin = 1;  // analog pin used to connect the right sensor
int buttonPin = 4;  // analog pin used to connect the button
int color = 0;

boolean gotTweet = false;


//bird #1
/*
int twistMax = 162;  //Right
int twistMin = 2;    //Left
int twistCenter = 82;

int tiltMax = 135;    //left ear down
int tiltMin = 77;     //right ear down
int tiltCenter = 103;

int lWingMax = 160;  // Down!
int lWingMin = 55;   // Up!

int rWingMax = 150; //UP
int rWingMin = 35;  //DOWN
*/


//bird #2

int twistMax = 175;  //Right
int twistMin = 30;    //Left
int twistCenter = 100;

int tiltMax = 161;    //left ear down
int tiltMin = 84;     //right ear down
int tiltCenter = 128;

int lWingMax = 140;  // Down!
int lWingMin = 35;   // Up!

int rWingMax = 150; //UP
int rWingMin = 45;  //DOWN  

////////////////////////////////

int valL=0;  // variable to read the value from the analog pin
int valLOld=0;  //to hold last sensor value
int valR=0;
int valROld=0;
int valPot=0;
int valPotOld=0;
int tweetButton = 0;


int mood = 500;

unsigned long previousMillis = 0;
const int interval = 25;  // for timing the servo
unsigned long tiltTimerOld = 0;



///////////////////
int twist = 100;//82; //
int tilt = 128; //103; //
int lWing = 125; //155; //
int rWing = 55; //45; //

int twistTarget = 100; //82; //
int tiltTarget = 128; //103; //
int lTarget = 125; //155; //
int rTarget = 55; //45; //


int wingSpeed = 1;   ///////for servo damping
int twistSpeed = 4;
int tiltSpeed = 4;




// 
unsigned long timer = 0;
unsigned long timerOld = 0;
unsigned long tiltTimer = 0;
long timeLimit = 0;  ///  this will be the number in milliseconds for the gesture movement interval- will change with each gesture
unsigned long tempTimeLimit = 0; /// tempTimeLimit will be the incrementer.
int gesturePos = 0; ////////for counting through the position array.
int state = 0;  //will use this to select gestures from a list
long moodmap = 0;

int motionPosSelect = 0;
int selectRandom = 0;

int redLED = 10;  //pins for LEDs
int blueLED = 11;



void setup() 
{ 
  //////////////////////////////////
  //Serial.begin(9600);
  ///////////////////////////////////
  
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  servoTwist.attach(3);  
  servoTilt.attach(5);   
  servoL.attach(6);   
  servoR.attach(9);  

} 

void loop() {
  
unsigned long currentMillis = millis();

if (gotTweet == true){
 tweetGesture();
 
}

  
  
  valL = analogRead(leftPin);          // reads the value of the potentiometer (value between 0 and 1023) 
  valL = map(valL, 0, 1023, 0, 100);     // scale it to make it useful later. 
  valR = analogRead(rightPin);          // reads the value of the potentiometer (value between 0 and 1023) 
  valR = map(valR, 0, 1023, 0, 100); 
  valPot = analogRead(potPin);          // reads the value of the potentiometer (value between 0 and 1023) 
  tweetButton = analogRead(buttonPin);
  
  mood = map(valPot, 0, 1023, 10, 10000);
  color = map(valPot, 0, 1023, 0, 255);//// to send to LEDS
  
  if (tweetButton > 1000){
   gotTweet = true; 
  }

  timer = (currentMillis - timerOld);
  tiltTimer = (currentMillis - tiltTimerOld);
  
  if (timer > 10000){
     timerOld = (currentMillis - 1000);
     tiltTimer = (currentMillis - 1000);
    
  }


//////////////////////////control the lights

analogWrite(redLED, (color));
analogWrite(blueLED, (255-color));

  /////////////////reset timer sensors read big move  - adjust values until it feels right.
  if ((valL - valLOld) > 20){
   timerOld = currentMillis;
   tiltTimerOld = currentMillis;
  }

  if ((valL - valLOld) < -20){
   timerOld = currentMillis;
   tiltTimerOld = currentMillis;
  }

  if ((valR - valROld) > 20){
    timerOld = currentMillis;
    tiltTimerOld = currentMillis;
  }

  if ((valR - valROld) < -20){
    timerOld = currentMillis;
    tiltTimerOld = currentMillis;
  }


  valLOld = valL;
  valROld = valR;
  valPotOld = valPot;


  if (timer < 5000){  ////  at start and if sensor sees big change, look left or right - toward sensor that changed?  THIS NEEDS TO BE FIGURED OUT
     if (valR - valL > 10){  //// if right sensor is significantly greater
       twistTarget = 150;  ///  135; // look right
     } 
       
     else if (valL - valR > 10){  //// right sensor is significantly greater
     twistTarget = 65; //45;  //
     }
       
   
   else{
     twistTarget = 100; //82; //
   }
          if (valR > 50){lTarget = 45;}//(65);}       //////////wings respond to sensors too.
       else if (valR > 35){lTarget = 75;}//(85);}
       else if (valR > 25){lTarget = 105;}//(115);}
       else {lTarget = 145;}//(155);}
       
         if (valL > 50){rTarget = (140);}  ////////////////////revisit for bird 2!!!!!!!!!!
       else if (valL > 35){rTarget = (100);}
       else if (valL > 25){rTarget = (65);}
       else {rTarget = (45);}
  }
  
     moodmap = (120 * mood);

  if (timer >=5000){
    if (state == 0){
    motionPosSelect = 0 + (random() %800);
   // Serial.println(motionPosSelect);
      if ((motionPosSelect == 0)||(motionPosSelect == 1)||(motionPosSelect == 2)||(motionPosSelect == 3)){
        tiltTarget = 128;  //103
        twistTarget = twistMin + (random() % (twistMax - twistMin)); 
        tiltTimerOld = currentMillis;
      
      //  posY = (random(80,160));
      }
      if ((motionPosSelect == 4)||(motionPosSelect == 5)||(motionPosSelect == 6)||(motionPosSelect == 7)||(motionPosSelect == 8)||(motionPosSelect == 9)||(motionPosSelect == 10)){
        if (tiltTimer > 150){
        tiltTarget = tiltMin + (random() % (tiltMax - tiltMin)); 
        }
      //  posY = (random(80,160));
      }
      
      if(motionPosSelect == 20){
          state = 1 + (random() % 4); ////if you roll a 20, then pick a state 1-4  This will start gesture 1, 2, 3 or 4. 
      }
    }

    if(state > 0){  /////if a state is selected above, 1, 2 or 3, do gesture 1, 2, or 3.
      switch(state){
      case 1:
        gesture1();
        break;

      case 2:
        gesture2();
        break;

      case 3:
        gesture3();
        break;
        
      case 4:
        gesture4();
        break;
      }
    }
  }
  if(currentMillis - previousMillis > interval) {      // Issue command only if desired position changes
previousMillis = currentMillis;
updateMotors();
  }
}

////////////////////////////////////////external functions//////////////////////

///////////////////////////////////////////////gesture1 prog/////////////////////////////

//bird # 1 gesture 1
/*
int gestureTwist1[] = {82, 82,82};
int gestureTilt1[] = {103, 103,103};
int gestureLWing1[] =  { 145, 55, 145};
int gestureRWing1[] =  { 45, 150, 45};
const int gesture1Count = 2; ///////////how many numbers are in the array -1?
*/

//bird #2 gesture 1
int gestureTwist1[] = {100, 100,100};
int gestureTilt1[] = {128, 128,128};
int gestureLWing1[] =  { 135, 35, 135};
int gestureRWing1[] =  { 45, 150, 45};
const int gesture1Count = 2; ///////////how many numbers are in the array -1?

/////////////////////////////////////////////////////////////////////////////////////////


void gesture1()///////////////////////////////////////////////GESTURE1/////////////////////
{
  timeLimit = 2500;  ///number of milliseconds between movement in array.  i.e. how quickly it goes through the list.
  if(millis() > tempTimeLimit){  ////if time has passed
    tempTimeLimit = (millis() + timeLimit); /// increment the time limit to repeat
    gesturePos++;  ///increment the gesture position counter each time the time limit passed
        ////////////////////////////////set the movement targets to the current position in the array/////
    twistTarget = gestureTwist1[gesturePos];
    tiltTarget = gestureTilt1[gesturePos];
    lTarget = gestureLWing1[gesturePos];
    rTarget = gestureRWing1[gesturePos];

  }
  if(gesturePos >= gesture1Count){  //////once the end of the gesture is reached, reset
    gestureReset();
 //   Serial.println('gesture 1');
  }
}
/////////////////////////////////////////////////END OF GESTURE1//////////

//bird #1 gesture 2
/*
int gestureTwist2[] = {82, 82, 82, 82, 100, 110};
int gestureTilt2[] = {103, 115, 103, 103, 103, 103};
int gestureLWing2[] =  { 150, 55, 55, 85, 55, 150};
int gestureRWing2[] =  { 40, 150, 150, 120, 150, 45};
const int gesture2Count = 5; ///////////how many numbers are in the array -1?
*/
///bird #2 gesture 2
int gestureTwist2[] = {100, 100, 100,100 , 130, 130};
int gestureTilt2[] = {128, 100, 128,128, 128, 128};
int gestureLWing2[] =  { 135,35, 35, 65, 35, 135};
int gestureRWing2[] =  { 45,150, 150, 120, 150, 45};
const int gesture2Count = 5; ///////////how many numbers are in the array -1?

/////////////////////////////////////////////////////////////////////////////////////////


void gesture2()///////////////////////////////////////////////GESTURE2/////////////////////
{
  timeLimit = 500;  ///number of milliseconds between movement in array.  i.e. how quickly it goes through the list.
  wingSpeed = 3;
  if(millis() > tempTimeLimit){  ////if time has passed
    tempTimeLimit = (millis() + timeLimit); /// increment the time limit to repeat
    gesturePos++;  ///increment the gesture position counter each time the time limit passed
        ////////////////////////////////set the movement targets to the current position in the array/////
    twistTarget = gestureTwist2[gesturePos];
    tiltTarget = gestureTilt2[gesturePos];
    lTarget = gestureLWing2[gesturePos];
    rTarget = gestureRWing2[gesturePos];

  }
  if(gesturePos >= gesture2Count){  //////once the end of the gesture is reached, reset
    gestureReset();
    wingSpeed = 1;
   // Serial.println('gesture 2');
  }
}
/////// bird #1  gesture 3
/*
int gestureTwist3[] = {82, 82, 82, 82, 100, 100};
int gestureTilt3[] = {103, 90, 103,103, 103, 103};
int gestureLWing3[] =  { 150,65, 65, 95, 60, 150};
int gestureRWing3[] =  { 45, 45, 45, 45, 45, 45};
const int gesture3Count = 5; ///////////how many numbers are in the array -1?
*/

/// bird #2 gesture 3
int gestureTwist3[] = {100, 100, 100,100 , 130, 130};
int gestureTilt3[] = {128, 100, 128,128, 128, 128};
int gestureLWing3[] =  { 130,35, 35, 85, 35, 135};
int gestureRWing3[] =  { 45,45, 45, 45, 45, 45};
const int gesture3Count = 5; ///////////how many numbers are in the array -1?


void gesture3()///////////////////////////////////////////////GESTURE2/////////////////////
{
  timeLimit = 500;  ///number of milliseconds between movement in array.  i.e. how quickly it goes through the list.
  wingSpeed = 3;
  if(millis() > tempTimeLimit){  ////if time has passed
    tempTimeLimit = (millis() + timeLimit); /// increment the time limit to repeat
    gesturePos++;  ///increment the gesture position counter each time the time limit passed
        ////////////////////////////////set the movement targets to the current position in the array/////
    twistTarget = gestureTwist3[gesturePos];
    tiltTarget = gestureTilt3[gesturePos];
    lTarget = gestureLWing3[gesturePos];
    rTarget = gestureRWing3[gesturePos];

  }
  if(gesturePos >= gesture3Count){  //////once the end of the gesture is reached, reset
    gestureReset();
    wingSpeed = 1;
  //  Serial.println('gesture 3');
  }
}
//bird #1 gesture 4
/*
int gestureTwist4[] = {82, 82, 82, 82, 82, 82};
int gestureTilt4[] = {103, 85, 103, 103, 103, 103};
int gestureLWing4[] =  { 155,155, 155, 155, 155, 155};
int gestureRWing4[] =  { 150,120, 145, 120, 150, 45};
const int gesture4Count = 5;
*/
//bird #2 gesture 4  
int gestureTwist4[] = {100, 100, 100,100 , 130, 130};
int gestureTilt4[] = {128, 100, 128,128, 128, 128};
int gestureLWing4[] =  { 135,135, 135, 135, 135, 135};
int gestureRWing4[] =  { 150,120, 145, 120, 150, 50};
const int gesture4Count = 5;

  
  void gesture4()///////////////////////////////////////////////GESTURE2/////////////////////
{
  timeLimit = 500;  ///number of milliseconds between movement in array.  i.e. how quickly it goes through the list.
  wingSpeed = 3;
  if(millis() > tempTimeLimit){  ////if time has passed
    tempTimeLimit = (millis() + timeLimit); /// increment the time limit to repeat
    gesturePos++;  ///increment the gesture position counter each time the time limit passed
        ////////////////////////////////set the movement targets to the current position in the array/////
    twistTarget = gestureTwist4[gesturePos];
    tiltTarget = gestureTilt4[gesturePos];
    lTarget = gestureLWing4[gesturePos];
    rTarget = gestureRWing4[gesturePos];

  }
  if(gesturePos >= gesture4Count){  //////once the end of the gesture is reached, reset
    gestureReset();
    wingSpeed = 1;
 //   Serial.println('gesture 4');
  }
}

//bird#1 tweet gesture
/*
int gestureTwistT[] = {150, 10, 150, 10, 150, 10, 82};
int gestureTiltT[] = {103, 103, 103, 103, 103, 103, 103};
int gestureLWingT[] =  { 65, 85, 65, 95, 65, 85, 155};
int gestureRWingT[] =  { 150,120, 145, 120, 150, 130, 45};
const int gestureTCount = 6;
*/

//bird # 2 tweet gesture 
int gestureTwistT[] = {160, 40, 160, 40, 160, 40, 100};
int gestureTiltT[] = {128, 128, 128,128, 128, 128, 128};
int gestureLWingT[] =  { 35,75, 45, 85, 35, 75, 135};
int gestureRWingT[] =  { 150,120, 145, 120, 150, 130, 50};
const int gestureTCount = 6;

void tweetGesture(){
  state = 10;
  timeLimit = 500;  ///number of milliseconds between movement in array.  i.e. how quickly it goes through the list.
  wingSpeed = 3;
  if(millis() > tempTimeLimit){  ////if time has passed
    tempTimeLimit = (millis() + timeLimit); /// increment the time limit to repeat
    gesturePos++;  ///increment the gesture position counter each time the time limit passed
        ////////////////////////////////set the movement targets to the current position in the array/////
    twistTarget = gestureTwistT[gesturePos];
    tiltTarget = gestureTiltT[gesturePos];
    lTarget = gestureLWingT[gesturePos];
    rTarget = gestureRWingT[gesturePos];

  }
  if(gesturePos >= gestureTCount){  //////once the end of the gesture is reached, reset
    gestureReset();
    wingSpeed = 1;
    gotTweet = false;
    //Serial.println('tweet gesture');
  }
}


void gestureReset(){
  gesturePos = 0;  ////////////////////set the gesture position back to zero 
  state = 0; //////////////////////////set the state back to 0 - (roll the dice again)
}





void updateMotors(){

   // twistTarget = map(twistTarget, 0, 180, twistMin, twistMax); /////////////////adjust values for servo max & min

  //tiltTarget = map (tiltTarget, 0, 180, tiltMin, tiltMax);
 // lTarget = map(lTarget, 180, 0, lWingMin, lWingMax);
//  rTarget = map(rTarget, 180, 0, rWingMin, rWingMax);
 
 
 //increment motor position from current position to target by speed.
  if (twistTarget > twist){
    twist = (twist + twistSpeed);
  }
    if (twistTarget < twist){
    twist = (twist - twistSpeed);
  }
  
    if (tiltTarget > tilt){
    tilt = (tilt + tiltSpeed);
  }
    if (tiltTarget < tilt){
    tilt = (tilt - tiltSpeed);
  }
  
      if (lTarget > lWing){
    lWing = (lWing + wingSpeed);
  }
    if (lTarget < lWing){
    lWing = (lWing - wingSpeed);
  }
  
  
        if (rTarget > rWing){
    rWing = (rWing + wingSpeed);
  }
    if (rTarget < rWing){
    rWing = (rWing - wingSpeed);
  }

  
  servoTwist.write(twist);
  servoTilt.write(tilt);
  servoL.write(lWing);
  servoR.write(rWing);
}





