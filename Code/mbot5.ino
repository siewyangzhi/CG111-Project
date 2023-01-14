/**
 * CG1111 mBot Project
 *
 * @file: mBot.ino
 * @author: 4b-2
 *
 * This program allows the mBot to finish
 * the entire maze by including the different
 * libraries which enable moving, color sensing
 * and audio to be played.
 */
#include <MeMCore.h>
#include <math.h>
#include "colorSensor.h"
#include "song.h"
#include "movement.h"

//Initialize hardware components
MeLineFollower lineFinder(PORT_2);
MeUltrasonicSensor ultraSensor(PORT_1);
Me4Button btn(PORT_9);

/**
 * Check for black strip using line follower sensors.
 * @return Returns true if the mBot encounters a black strip; returns false
 * otherwise.
 */
bool blackStripCheck()
{   
    int sensor = lineFinder.readSensors();
    if (sensor == 0) // readSensors returns 0 if both sensors read black.
        return true;
    return false;
}

/**
 * Sense a color and carry out an action correspondingly.
 * @param[in] A character representing the color detected.
 */
void action(char color)
{
    switch (color)
    {   
        //If it is red, make a left-turn.
        case 'R':
        {   
            leftTurn();
            break;
        }
        //If it is green, make a right-turn.
        case 'G':
        {   
            rightTurn();
            break;
        }
        //If it is yellow, make a u-turn. 
        case 'Y':
        {   
            uTurn();
            break;
        }
        //If it is purple, make two consecutive left-turns in two grids.
        case 'P':
        {
            //Make the first left-turn immediately after detecting purple.
            leftTurn();
            while (ultraSensor.distanceCm() >= 10) // move till it senses the upcoming wall
            {
                moveCorrected();
            }
            //Make the second left-turn when it reaches 10cm from the wall.
            motorL.stop();
            motorR.stop();
            leftTurn();
            delay(10);
            break;
        }
        //If it is blue, make two consecutive right-turns in two grids.
        case 'B':
        {
            //Make the first right-turn immediately after detecting blue.
            rightTurn();
            while (ultraSensor.distanceCm() >= 10) // move till it senses the upcoming wall
            {
                moveCorrected();
            }
            //Make the second right-turn when it reaches 10cm from the wall.
            motorL.stop();
            motorR.stop();
            rightTurn();
            delay(10);
            break;
        }
        //If it is black, it stops and play a celebratory tone.
        case 'b':
        {
            motorL.stop();
            motorR.stop();
            playMelody(50); //buzzer stuff
            while (1); // Stop the mBot indefinitely
        }
    }
}

void setup()
{
    //Set LED
    currentReading = 0;
    RGBLED.setpin(13);
    RGBLED.setColor(0, 0, 0);
    RGBLED.show();
    while (btn.pressed() != KEY_1) // press button to start mbot
    {
        delay(1);
    }
}
void loop()
{
    moveCorrected();
    if (blackStripCheck()) //If black strip sensed check color above and act accordingly
    {
        motorL.stop();
        motorR.stop();
        action(colorSensor());
    }
}
