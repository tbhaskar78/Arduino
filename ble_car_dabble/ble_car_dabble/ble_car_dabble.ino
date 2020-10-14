#include <AFMotor.h>
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

//initial motors pin
AF_DCMotor rightFrontMotor(1, MOTOR12_64KHZ); 
AF_DCMotor rightRearMotor(2, MOTOR12_64KHZ);
AF_DCMotor leftFrontMotor(3, MOTOR34_64KHZ);
AF_DCMotor leftRearMotor(4, MOTOR34_64KHZ);

#define RX 0
#define TX 1

//SoftwareSerial mySerial(RX, TX);

void setup() 
{       
    Dabble.begin(9600, RX, TX);
    Serial.begin(115200);  //Set the baud rate to your Bluetooth module.
}

void loop()
{
    char command;
    Dabble.processInput(); 
    if (GamePad.isUpPressed())
    {
        Serial.print("UP");
        Serial.print("\n Going Forward \n");
        forward();
    }

    if (GamePad.isDownPressed())
    {
        Serial.print("DOWN");
        Serial.print("\n Going back \n");
        back();
    }

    if (GamePad.isLeftPressed())
    {
        Serial.print("Left");
        Serial.print("\n Going left \n");
        left();
    }

    if (GamePad.isRightPressed())
    {
        Serial.print("Right");
        Serial.print("\n Going right \n");
        right();
    }
    if (GamePad.isCirclePressed())
    {
        Serial.print("STOP");
        Serial.print("\n Going to stop \n");
        Stop();
    }


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
}
