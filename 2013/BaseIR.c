#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          IRThing,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    tip,                  tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

/**
* Method finds the amount of turn needed to
* line up with the IR Sensor
*
* @return position - the amount of degrees the
* IR has turned
* NOTE - DEPRECATED
*/
int findPos()
{
	nMotorEncoder[IRThing] = 0;
	nMotorEncoderTarget[IRThing] = 180;
	motor[IRThing] = 10;
	while(nMotorRunState[IRThing] != runStateIdle)
	{
		if(SensorValue[IR] != 5){}
		else
		{
			//int position = nMotorEncoder[IRThing];
		}
	}
	int position = 0;
	return position;
}

/**
* Method finds the IR beacon and sets up for the drop() method
*
*	@return iterations - the number of times NXT has checked
* if IR beacon is in front
*/
int lineUp()
{
	// Using the light sensor to find a line
	//motor[motorD] = 50;
	//motor[motorE] = 50;
	//untilLight(50 , S3);
	//motor[motorD] = 0;
	//motor[motorE] = 0;
 	// Not using this because we
	// found a way around this by lining up
  // parallel manually

	int searchZone = 4; // the zone that we are looking for
	int iterations = 0; // set the number of times checked to 0
	while(SensorValue(IR) != searchZone) // 6 for other side
	{
		if(SensorValue(IR) < searchZone) // if IR sensor is less than the searchZone ...
		{
			motor[motorD] = -50;
			motor[motorE] = -50;
			wait1Msec(100);
			//then move the robot backwards
			iterations++; // increment the times searched
		}

		else if(SensorValue(IR) > searchZone) // if IR sensor is more than the searchZone ...
		{
			motor[motorD] = 50;
			motor[motorE] = 50;
			wait1Msec(100);
			//then move the robot forwards
			iterations++; // increment the times searched
		}

		else {break;}
	}

	// FREEZE!
	motor[motorD] = 0;
	motor[motorE] = 0;

	// this block moves the IR sensor straight
	nMotorEncoder[IRThing] = 0;
	nMotorEncoderTarget[IRThing] = 90;
	motor[IRThing] = -10;
	while(nMotorRunState[IRThing] != runStateIdle){}
	motor[IRThing] = 0;

	// keep turning until we are perpendicular to the basket
	while(SensorValue(IR) != searchZone) //for other side 6
	{
		motor[motorD] = -50;
		motor[motorE] = 50;
	}

	// Move forwards to basket
	motor[motorD] = 50;
	motor[motorE] = 50;
	wait1Msec(500);

	// STOP!
	motor[motorD] = 0;
	motor[motorE] = 0;
	wait1Msec(50);

	return iterations;
}

/**
*	Method drops the block into the basket
* Requires that robot is in front of basket
* SCORES 40 POINTS FOR BLOCK IN IR BASKET
*/
void drop()
{
	wait1Msec(100);
	motor[motorF] = 20; // dump that block!
	wait1Msec(1000);

	motor[motorF] = 0; // pause for clean drop
	wait1Msec(2000);

	motor[motorF] = -20; // retract back
	wait1Msec(900);

	motor[motorF] = 0; // pause for clean transition
	wait1Msec(1000);
}

/**
*	Method gets the robot from the basket to the ramp
*	Requires that the block has been dropped
*	SCORES 20 POINTS ON THE RAMP
*
* @param iterations - the number of times NXT has checked
* which basket the IR is under
*/
void ramp(int iterations)
{
	motor[motorD] = -50; // run backwards for iterations * 100 milliseconds
	motor[motorE] = -50; // because each iteration is 100 milliseconds long
	wait1Msec(100 * (iterations - 3));// iterations normally 2

	motor[motorD] = -50; // turns toward the ramp
	motor[motorE] = 50;
	wait1Msec(1000);

	motor[motorD] = 50; // drives toward center of ramp
	motor[motorE] = 50;
	wait1Msec(3000);

	motor[motorD] = 50; // squares upto ramp
	motor[motorE] = -50;
	wait1Msec(1100);

	motor[motorD] = 100; // drive up onto ramp
	motor[motorE] = 100; // for a score!
	wait1Msec(2100);
}

task main()
{
	waitForStart(); // wait for the FCS to initiate autonomous

	int timesRun = lineUp();

	// Drops the block into the basket
	drop();

	// Move robot away from basket
	motor[motorD] = -50;
	motor[motorE] = -50;
	wait1Msec(500);

	// Spin robot parallel to teeter totter
	motor[motorD] = 50;
	motor[motorE] = -50;
	wait1Msec(1000);

	// drives us onto the ramp
	ramp(timesRun);
}
