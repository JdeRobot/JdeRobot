/*
 * Name: PIDController.h
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
 * The 'minRef' sets the deadband of the controller.
 * The 'maxRef' sets the maximum reference of the controller. Greater values of the reference
 * will not generate greater values in the output.
 * The 'minOutput' sets the minimum percentage of output in case of not to stay into the death bandwith.
 * The 'maxOutput' sets the maximum percentage of output.
 *
 * Copyright (C) 2008-2009 Universidad Rey Juan Carlos
 * All Rights Reserved.
 */

#ifndef PIDController_H
#define PIDController_H

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class PIDController
{

public:
	PIDController(string name, float minRef, float maxRef, float minOutput, float maxOutput );
	virtual ~PIDController();

	void setMinRef  	( float new_minRef );
	void setMaxRef  	( float new_maxRef );
	void setMinOutput 	( float new_minOutput );
	void setMaxOutput 	( float new_maxOutput );
	void setReference 	( float newReference );

	void setPIDKs 	( float nKP, float nKI, float nKD );

	float  getOutput    	();

private:

	float KP;
	float KI;
	float KD;
 	static const bool debug;

	float ref, minRef, maxRef, minOutput, maxOutput, output, int_error, deriv_error, prev_error;
	string name;
	int direction;
};
#endif // PIDController_H
