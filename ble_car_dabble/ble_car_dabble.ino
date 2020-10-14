#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

#define RX                   0   // DIGITAL PIN 0  - RX to TX
#define TX                   1   // DIGITAL PIN 1  - TX to RX
#define IR_PIN              A2
#define ECHO_PIN            A4   // Pin A4 on the Motor Drive Shield connected to the ultrasonic sensor
#define TRIG_PIN            A5   // Pin A5 on the Motor Drive Shield connected to the ultrasonic sensor
#define MAX_DISTANCE        100 
#define TURN_RIGHT          50
#define TURN_LEFT           130
#define STRAIGHT            90
#define LED_PIN             13
#define FWD                 1
#define BCK                 2
#define LFT                 3
#define RGT                 4
#define NON                 0

//INIT
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
AF_DCMotor rightFrontMotor(1, MOTOR12_64KHZ); 
AF_DCMotor rightRearMotor(2, MOTOR12_64KHZ);
AF_DCMotor leftFrontMotor(3, MOTOR34_64KHZ);
AF_DCMotor leftRearMotor(4, MOTOR34_64KHZ);
Servo myservo;   

int distance = 100;
int IR_Sensor = 1000;
int setDirection = NON;

void setup() 
{       
    Dabble.begin(9600, RX, TX);
    Serial.begin(115200);  //Set the baud rate to your Bluetooth module.
    pinMode(IR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    myservo.attach(10);  
    myservo.write(TURN_RIGHT);
    delay(500);
    myservo.write(STRAIGHT);
    delay(500);
    myservo.write(TURN_LEFT);
    delay(500);
    myservo.write(STRAIGHT);
    setDirection = NON;
}

void loop()
{
LOOP_BEGIN:
    if(setDirection == FWD)
    {
        distance = readPing();
        if (distance < 25)
        {
            Serial.print("1. Distance is less than 25 cm, stopping \n");
            Stop();
            distance = 100;
            setDirection = NON;
            goto LOOP_BEGIN;
        }
    }
    else if (setDirection == BCK)
    {
        IR_Sensor = analogRead(IR_PIN);
        Serial.println(IR_Sensor);
        if (IR_Sensor < 100)
        {
            Serial.print("\2. IR Sensor is LOW, stopping \n");
            Stop();
            IR_Sensor = 1000;
            setDirection = NON;
            goto LOOP_BEGIN;
        }
    }
    Dabble.processInput();

    if (GamePad.isUpPressed())
    {
        setDirection = FWD;
        distance = readPing();
        if(distance < 25 )
        {
            Serial.print("2. Distance is less than 25 cm, stopping \n");
            Stop();
            distance = 100;
            setDirection = NON;
            goto LOOP_BEGIN;
        }
        Serial.print("UP");
        Serial.print("\n Going Forward \n");
        forward();
    }

    if (GamePad.isDownPressed())
    {
        setDirection = BCK;
        IR_Sensor = analogRead(IR_PIN);
        Serial.println(IR_Sensor);
        if (IR_Sensor < 100)
        {
            Serial.print("\n2. IR Sensor is LOW, stopping \n");
            Stop();
            IR_Sensor = 1000;
            setDirection = NON;
            goto LOOP_BEGIN;
        }

        Serial.print("DOWN");
        Serial.print("\n Going back \n");
        back();
    }

    if (GamePad.isLeftPressed())
    {
        setDirection = NON;
        Serial.print("Left");
        Serial.print("\n Going left \n");
        left();
    }

    if (GamePad.isRightPressed())
    {
        setDirection = NON;
        Serial.print("Right");
        Serial.print("\n Going right \n");
        right();
    }
    if (GamePad.isCirclePressed())
    {
        setDirection = NON; 
        Serial.print("STOP");
        Serial.print("\n Going to stop \n");
        Stop();
    }


}

void flashLED(int numberOfTimes)
{
  int i = 0;
  for (i = 0; i < numberOfTimes; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  digitalWrite(LED_PIN, LOW);
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

void forward()
{
    rightFrontMotor.setSpeed(255); //Define maximum velocity
    rightFrontMotor.run(FORWARD); //rotate the motor clockwise
    rightRearMotor.setSpeed(255); //Define maximum velocity
    rightRearMotor.run(FORWARD); //rotate the motor clockwise
    leftFrontMotor.setSpeed(255);//Define maximum velocity
    leftFrontMotor.run(FORWARD); //rotate the motor clockwise
    leftRearMotor.setSpeed(255);//Define maximum velocity
    leftRearMotor.run(FORWARD); //rotate the motor clockwise
}

void back()
{
    rightFrontMotor.setSpeed(255); //Define maximum velocity
    rightFrontMotor.run(BACKWARD); //rotate the motor anti-clockwise
    rightRearMotor.setSpeed(255); //Define maximum velocity
    rightRearMotor.run(BACKWARD); //rotate the motor anti-clockwise
    leftFrontMotor.setSpeed(255); //Define maximum velocity
    leftFrontMotor.run(BACKWARD); //rotate the motor anti-clockwise
    leftRearMotor.setSpeed(255); //Define maximum velocity
    leftRearMotor.run(BACKWARD); //rotate the motor anti-clockwise
}

void left()
{
    rightFrontMotor.setSpeed(255); //Define maximum velocity
    rightFrontMotor.run(FORWARD); //rotate the motor clockwise
    rightRearMotor.setSpeed(255); //Define maximum velocity
    rightRearMotor.run(FORWARD); //rotate the motor clockwise
    leftFrontMotor.setSpeed(255); //Define maximum velocity
    leftFrontMotor.run(BACKWARD); //rotate the motor anti-clockwise
    leftRearMotor.setSpeed(255); //Define maximum velocity
    leftRearMotor.run(BACKWARD); //rotate the motor anti-clockwise
}

void right()
{
    rightFrontMotor.setSpeed(255); //Define maximum velocity
    rightFrontMotor.run(BACKWARD); //rotate the motor anti-clockwise
    rightRearMotor.setSpeed(255); //Define maximum velocity
    rightRearMotor.run(BACKWARD); //rotate the motor anti-clockwise
    leftFrontMotor.setSpeed(255); //Define maximum velocity
    leftFrontMotor.run(FORWARD);  //rotate the motor clockwise
    leftRearMotor.setSpeed(255); //Define maximum velocity
    leftRearMotor.run(FORWARD);  //rotate the motor clockwise
} 

void Stop()
{
    rightFrontMotor.setSpeed(0); //Define minimum velocity
    rightFrontMotor.run(RELEASE); //stop the motor when release the button
    rightRearMotor.setSpeed(0); //Define minimum velocity
    rightRearMotor.run(RELEASE); //rotate the motor clockwise
    leftFrontMotor.setSpeed(0); //Define minimum velocity
    leftFrontMotor.run(RELEASE); //stop the motor when release the button
    leftRearMotor.setSpeed(0); //Define minimum velocity
    leftRearMotor.run(RELEASE); //stop the motor when release the button
    setDirection = NON;
}
