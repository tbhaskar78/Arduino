#include <SoftwareSerial.h>

#define RX 2
#define TX 3
int LED_PIN=13;
int GREEN=8;
int RED=9;

SoftwareSerial mySerial(RX, TX);

void setup()
{
    mySerial.begin(9600);
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    String msg = "";
    if (mySerial.available())
    {
        msg=mySerial.readString();
        Serial.println("\nDATA RECEIVED:\n");
        Serial.print(msg);
        
        
        if((msg.indexOf("Green on") > -1) || (msg.indexOf("Green On") > -1))
        {
            digitalWrite(GREEN, HIGH);
            Serial.println("\nGREEN LED On\n");
        }
        else if((msg.indexOf("Green off") > -1) || (msg.indexOf("Green Off") > -1))
        {
            digitalWrite(GREEN, LOW);
            Serial.println("\nGREEN LED Off\n");
        }
        else if((msg.indexOf("Read on") > -1) || (msg.indexOf("Read On") > -1))
        {
            digitalWrite(RED, HIGH);
            Serial.println("\nGREEN LED On\n");
        }
        else if((msg.indexOf("Read off") > -1) || (msg.indexOf("Read Off") > -1))
        {
            digitalWrite(RED, LOW);
            Serial.println("\nGREEN LED Off\n");
        }
        else
        {
            if((msg.indexOf("on") > -1) || (msg.indexOf("On") > -1))
            {
                digitalWrite(LED_PIN, 1);
                digitalWrite(GREEN, HIGH);
                digitalWrite(RED, HIGH);
                Serial.println("\nLED On\n");
            }
            if((msg.indexOf("off") > -1) || (msg.indexOf("Off") > -1))
            {
                digitalWrite(LED_PIN, 0);
                digitalWrite(GREEN, LOW);
                digitalWrite(RED, LOW);
                Serial.println("\nLED Off\n");
            }
        }
    }
}
