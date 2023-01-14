/**
 * CG1111 mBot Project
 * 
 * @file: movement.h
 * @author: 4b-2
 *
 * This library includes functions that allow
 * the mBot to make a left-turn, right-turn and
 * u-turn by adjusting the speeds of the two
 * motors. It also includes the function to keep
 * the mBot moving straight by using the Infrared
 * Sensor.
 *
 */
#define motorDelay 260 // Amount of time required for the mBot to turn 90 degree
#define DST6 690 // IR value at 6 cm (max distance)
#define maxSpeed 255

float dstL = analogRead(0);
float dstR = analogRead(1);

MeDCMotor motorL(M1);
MeDCMotor motorR(M2);

/**
 * This boolean function checks if the mBot is
 * closer to the wall on the right.
 *
 * @return Returns true if the mBot is closer to
 * the wall on the right; returns false otherwise.
 */
bool checkRightCloser()
{
    if (dstR < dstL)
    {
        return true;
    }
    return false;
}

void leftTurn() //To make a left-turn
{
    motorL.run(maxSpeed);
    motorR.run(maxSpeed);
    delay(motorDelay);
    motorL.stop();
    motorR.stop();
}

void rightTurn() //To make a right-turn
{
    motorR.run(-maxSpeed);
    motorL.run(-maxSpeed);
    delay(motorDelay);
    motorR.stop();
    motorL.stop();
}

/**
 * Turn in the anti-clockwise direction if it is
 * closer to the wall on the right. Turn in the
 * clockwise direction otherwise.
 */
void uTurn()
{
    if (checkRightCloser())
    {
        leftTurn();
        leftTurn();
    }
    else
    {
        rightTurn();
        rightTurn();
    }
}

float factor;
int leftSpeed, rightSpeed;

//Course correction
void moveCorrected()
{
    dstL = analogRead(0) * 1.48; //As the sensors weren't equally sensitive, dstL was scaled up 
    dstR = analogRead(1);
    //The distance to either side is capped at DST6
    //to prevent it from turning unneccessarily when
    //there is a missing wall.
    if (dstR > DST6)
    {
        dstR = DST6;
    }
    if (dstL > DST6)
    {
        dstL = DST6;
    }
    //Adjust the speed of both motors by the powered
    //up ration of the distances to both sides.
    factor = pow(dstR / dstL, 10); //pow to increase the correction of the robot
    leftSpeed = int(maxSpeed * factor);
    rightSpeed = int(maxSpeed / factor);
    motorL.run(-leftSpeed);
    motorR.run(rightSpeed);
}

                                                           
