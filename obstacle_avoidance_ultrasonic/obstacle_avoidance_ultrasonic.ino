/*********************************************
 * Author: Bhaskar Tallamraju
 * Date: 11 Oct 2020
 * Description: Obstacle avoidance robot
 * License : Mozilla Public License 2.0 
 *********************************************/

 
#include <AFMotor.h>  //AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install 
#include <NewPing.h>  //NewPing Library https://github.com/eliteio/Arduino_New_Ping.git
#include <Servo.h>    //Servo Library https://github.com/arduino-libraries/Servo.git 

/*****************************  WIRING *******************************************
 *    1. ECHO PIN and TRIG PIN from Ultrasonic sensor to A4 and A5 respectively
 *    2. right front motor to M1, right rear motor to M2
 *    3. left front motor to M3 and left rear motor to M4
 *    4. servo motor to Servo 1
 **********************************************************************************/
     
// CONSTANTS     
#define ECHO_PIN            A4   // Pin A4 on the Motor Drive Shield connected to the ultrasonic sensor
#define TRIG_PIN            A5   // Pin A5 on the Motor Drive Shield connected to the ultrasonic sensor
#define MAX_DISTANCE        100 
#define MAX_SPEED           150  // sets the maximum speed of DC motors
#define MAX_SPEED_OFFSET    20

#define TURN_RIGHT          50
#define TURN_LEFT           130

// INIT 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
AF_DCMotor rightFrontMotor(1, MOTOR12_64KHZ); 
AF_DCMotor rightRearMotor(2, MOTOR12_64KHZ);
AF_DCMotor leftFrontMotor(3, MOTOR34_64KHZ);
AF_DCMotor leftRearMotor(4, MOTOR34_64KHZ);

// GLOBAL Variables
Servo myservo;   
boolean setForward=false;
int distance = 100;
int speedSet = 0;
int checkLeftRight = 0;

// SETUP the environment 
void setup() 
{
    checkLeftRight = 0;
    myservo.attach(10);  
    myservo.write(90); //115); 
    delay(2000);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    //Serial.begin(115200);
}

// Continuous loop, Arduino keeps running in this, till we turn off
void loop() 
{
    int rightDistance = 0;
    int leftDistance = 0;
    delay(40);

    // initialized distance to 100, however during operation
    // if distance is less than 25 centimeters, it will check 
    // right and left and move accordingly
    if (distance <= 25)
    {
        // first stop the motors
        stopMoving();
        delay(100);

        // move backwards
        moveBackward();
        delay(200);
        stopMoving();
        delay(200);

        // Now calculate distance on the right and left
        rightDistance = look(TURN_RIGHT); // move sensor to right using servo
        delay(200);
        leftDistance = look(TURN_LEFT);   // move sensor to left using servo
        delay(200);

        //Serial.print(checkLeftRight);
        // This is a counter, if we have checked left and right twice, turn the vehicle around
        if (checkLeftRight >= 2)  
        {
          turnAround();
          stopMoving();
          checkLeftRight = 0;
        }
        else
        {    
          // if right has more clearance than left, turn right
          if (rightDistance >= leftDistance)
          {
              turnRight();
              stopMoving();
              checkLeftRight += 1;
          }
          // if left has more clearance than right, turn left 
          else if (leftDistance > rightDistance)
          {
              turnLeft();
              stopMoving();
              checkLeftRight += 1;
          }
        }
    } 
    // ofcourse, keep moving forward otherwise
    else
    {
        moveForward();
        checkLeftRight = 0;  // set the counter of checks to zero
    }
    distance = readPing();
}

// common function, pass the degree of rotation and it returns the distance in cm
int look(int turnDegree)
{
    // write turn degree to the servo, to move the ultrasonic sensor mounted on it
    myservo.write(turnDegree); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(90); 
    return distance;
}

// called from look(), will return ping (from ultrasonic sensor) value in cm
int readPing() 
{ 
    delay(100);
    // read in centimeter
    int cm = sonar.ping_cm();
    if(cm==0)
    {
        cm = 250;
    }
    return cm;
}

// stop all motors - BRAKE
void stopMoving() 
{
    rightFrontMotor.run(RELEASE); 
    rightRearMotor.run(RELEASE);
    leftFrontMotor.run(RELEASE);
    leftRearMotor.run(RELEASE);
} 

// move the motors forward
void moveForward() 
{
    if (!setForward)
    {
        setForward=true;
        rightFrontMotor.run(FORWARD);      
        rightRearMotor.run(FORWARD);
        leftFrontMotor.run(FORWARD); 
        leftRearMotor.run(FORWARD);     

        // slowly bring the speed up to avoid draininig the batteries
        for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
        {
            rightFrontMotor.setSpeed(speedSet);
            rightRearMotor.setSpeed(speedSet);
            leftFrontMotor.setSpeed(speedSet);
            leftRearMotor.setSpeed(speedSet);
            delay(5);
        }
    }
}

// move motors in the reverse direction
void moveBackward() 
{
    setForward=false;
    rightFrontMotor.run(BACKWARD);      
    rightRearMotor.run(BACKWARD);
    leftFrontMotor.run(BACKWARD);
    leftRearMotor.run(BACKWARD);  

    // slowly bring the speed up to avoid draininig the batteries
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) 
    {
        rightFrontMotor.setSpeed(speedSet);
        rightRearMotor.setSpeed(speedSet);
        leftFrontMotor.setSpeed(speedSet);
        leftRearMotor.setSpeed(speedSet);
        delay(5);
    }
}  

// turn the vehicle left
void turnLeft() 
{
    rightFrontMotor.run(FORWARD);
    rightRearMotor.run(FORWARD);
    leftFrontMotor.run(BACKWARD);
    leftRearMotor.run(BACKWARD);     
    delay(800);
    rightFrontMotor.run(FORWARD);      
    rightRearMotor.run(FORWARD);
    leftFrontMotor.run(FORWARD);
    leftRearMotor.run(FORWARD);      
} 

// turn the vehicle right
void turnRight() 
{
    rightFrontMotor.run(BACKWARD);     
    rightRearMotor.run(BACKWARD);  
    leftFrontMotor.run(FORWARD);
    leftRearMotor.run(FORWARD);   
    delay(800);
    rightFrontMotor.run(FORWARD);     
    rightRearMotor.run(FORWARD);
    leftFrontMotor.run(FORWARD);
    leftRearMotor.run(FORWARD);
}

// turn the vehicle around - moving right forward for long enough will turn 
// the vehicle around. I calculated about 1700 delay turns it around exactly
void turnAround() {

    rightFrontMotor.run(FORWARD);   // turn M1 forward
    rightRearMotor.run(FORWARD);    // turn M2 forward
    leftFrontMotor.run(BACKWARD);   // turn M3 backward
    leftRearMotor.run(BACKWARD);    // turn M4 backward

    leftFrontMotor.setSpeed(speedSet+MAX_SPEED_OFFSET);    
    leftRearMotor.setSpeed(speedSet+MAX_SPEED_OFFSET);
    delay(1700); // run motors this way for 1700

    rightFrontMotor.run(FORWARD);      // set both motors back to forward
    rightRearMotor.run(FORWARD);
    leftFrontMotor.run(FORWARD);
    leftRearMotor.run(FORWARD);    
} 

/* end of file */
