/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef NAOFOLLOWBALL_H
#define NAOFOLLOWBALL_H

class NaoFollowBall : public Singleton<NaoFollowBall>, public jderobot::NaoFollowBall {
public:
    // Constructor
    NaoFollowBall ();
        
    // Destructor
    virtual ~NaoFollowBall ();
    
    // Another functions
    void init ( const std::string newName, AL::ALPtr<AL::ALBroker> parentBroker, float stiffness, float speed );
    
    /*NaoFollowBall*/
    void setParams ( int rMin, int rMax, int gMin, int gMax, int bMin, int bMax, const Ice::Current& );
    void setParams ( int rMin, int gMin, int bMin, const Ice::Current& );
    void setParams ( int rMax, int gMax, int bMax, const Ice::Current& );
    void start ( const Ice::Current& );
    void stop ( const Ice::Current& );

private:
    std::string name;
    float stiffness, speed;
    AL::ALPtr<AL::ALMotionProxy> motion;
    JointControl *pan, *tilt;
    NaoServerCamera* naoservercamera;
    
    bool doTheFollow;
    
    int rMin, gMin, bMin;
    int rMax, gMax, bMax;
    
    float getTiltLimit ( float currentYaw, float currentVy );
    float target;
    
    void move ();
    
    static const float	MAX_PANU = 80.0f;
	static const float	MAX_PANL = 40.0f;
	static const float 	MAX_TILT = 45.0f;
	static const float 	MIN_TILT = -39.0f;
	static const float	MIN_PAN_ANGLE = 5.0f;
	static const float	MAX_PAN_ANGLE = 70.0f;
	static const float  MIN_TILT_ANGLE = 5.0f;
	static const float 	MAX_TILT_ANGLE = 30.0f;

	static const int    MAX_TILT_VECTOR =	25;
	static const float	MAXTILT[MAX_TILT_VECTOR];

	//Velocities are expressed in %  (18,12 is ok)
	static const float	VEL_PAN_MIN;
	static const float	VEL_PAN_MAX;		// Real robot
	static const float	VEL_TILT_MIN;
	static const float	VEL_TILT_MAX;		// Real robot

};

#endif // NAOFOLLOWBALL_H
