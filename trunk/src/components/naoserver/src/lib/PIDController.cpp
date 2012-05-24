/*
 * Name: PIDController.cpp
 * @Author: Carlos Agüero (caguero@gsyc.es)
 *
 * Description: Class that implements a simple type P controller.
 *
 *	    		 |minOutput  |maxOutput
 *	 			 |		     |
 *	 		 	 v 	   	     v
 *	    		-----------------
 *  Ref[-1...1] |				|
 * 	  	--->	| P Controller	| ---> Output [-100%...100%]
 *				|				|
 *				-----------------
 * 	 			 ^     	      ^
 *	 			 | 		      |
 *       		 |minRef      |maxRef
 *
 * The reference should be normalized between [-1...1] and the output is generated as a percentage (positive or negative).
 *
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved.
 */

#include "PIDController.h"

const bool PIDController::debug	=	false;

/**
 * Class constructor that initializes the controller.
 *
 * @param minRef Sets the deadband of the controller (from -minRef to maxRef).
 * @param maxRef Sets the maximum reference of the controller. Greater values of the reference
 *  will not generate greater values in the output.
 * @param minOutput Sets the minimum percentage of output in case of not to stay into the death bandwith.
 * @param maxOutput Sets the maximum percentage of output.
 */


PIDController::PIDController( string name, float minRef, float maxRef, float minOutput, float maxOutput )
{
	this->minRef = minRef;
	this->maxRef = maxRef;
	this->minOutput = minOutput;
	this->maxOutput = maxOutput;
	this->name = name;
	ref = output = prev_error = int_error = 0.0;

	this->KP = 0.41;
	this->KI = 0.06;
	this->KD = 0.53;

	if(debug) cerr << "[PIDController::PIDController] New '" << name << "' controller created with next params: " <<
	endl << "MinRef: " << minRef << endl << "MaxRef: " << maxRef << endl << "MinOutput: " << minOutput <<
	endl << "MaxOutput: " << maxOutput << endl;
}

/**
 * Class destructor.
 **/
PIDController::~PIDController()
{
	if(debug) cerr << "[PIDController::~PIDController] '" << name << "' controller destroyed" << endl;
}

/**
 * setMinRef. Method that reconfigures the death band of the controller.
 * @param newMinRef New value for the controller's deadband (from -newMinRef to newMaxRef).
 **/
void
PIDController::setMinRef(float newMinRef)
{
	minRef = newMinRef;
	if(debug) cerr << "[PIDController::setMinRef] New minRef (" << minRef << ") for '" << name << "' controller " << endl;
}

void
PIDController::setPIDKs ( float nKP, float nKI, float nKD )
{
	this->KP = nKP;
	this->KI = nKI;
	this->KD = nKD;

}

/**
 * setMaxRef. Method that reconfigures the maximum reference of the controller. Greater values of the reference
 *  will not generate greater values in the output.
 * @param newMaxRef New value for the maximum controller's reference.
 **/
void
PIDController::setMaxRef(float newMaxRef)
{
	maxRef = newMaxRef;
	if(debug) cerr << "[PIDController::setMaxRef] New maxRef (" << maxRef << ") for '" << name << "' controller " << endl;
}

/**
 * setMinOutput. Method that reconfigures the minimum output of the controller in case of not to stay into the deadband.
 * @param newMinOutput New value for the minimum controller's output.
 **/
void
PIDController::setMinOutput(float newMinOutput)
{
	minOutput = newMinOutput;
	if(debug) cerr << "[PIDController::setMinOutput] New minOutput (" << minOutput << " %) for '" << name << "' controller " << endl;
}

/**
 * setMaxOutput. Method that reconfigures the maximum output of the controller.
 * @param newMaxOutput New value for the maximum controllers's output.
 **/
void
PIDController::setMaxOutput(float new_maxOutput)
{
	maxOutput = new_maxOutput;
	if(debug) cerr << "[PIDController::setMaxOutput] New maxOutput (" << maxOutput << " %) for '" << name << "' controller " << endl;
}

/**
 * setReference. Method that sets the new reference for the controller and updates the output.
 * @param newReference New input value for the controller.
 **/
void
PIDController::setReference(float newReference)
{
	ref = newReference;
	if(debug) cerr << "[PIDController::setReference] New reference (" << ref << ") for '" << name << "' controller " << endl;

	// Proportional Error
	direction = ref / fabs(ref);
	if (fabs(ref) < minRef)
		output = 0;
	else if (fabs(ref) > maxRef)
		output = direction * maxOutput;
	else
		output = direction * minOutput + ref * (maxOutput - minOutput);

	// Integral Error
	int_error = (int_error + output) * 2/3;

	// Derivative Error
	deriv_error = output - prev_error;
	prev_error = output;

	output = KP * output + KI * int_error + KD * deriv_error;
}

/**
 * getOutput. Method that returns the controller's output given the last reference.
 * @return the controller's output as a percentage between -100% and 100%.
 **/
float
PIDController::getOutput()
{
	if(debug) cerr << "[PIDController::getOutput]  (" << output << " %) at '" << name << "' controller " << endl;
	return output;
}

