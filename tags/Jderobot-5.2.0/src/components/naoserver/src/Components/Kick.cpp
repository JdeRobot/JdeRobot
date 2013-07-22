#include "Kick.h"

Kick::Kick()
{
	_Kinematics = Kinematics::getInstance();

	state = Initial;
}

Kick::~Kick()
{

}

void Kick::Initial_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Initial_state_code

//Primero ir a PoseInit
pmotion->setWalkTargetVelocity(0.0, 0.0, 0.0, 0.7);

ALValue path;
path.arraySetSize(6);
path[0] = 0.0;
path[2] = 0.0;
path[3] = 0.0;
path[4] = 0.0;
path[5] = 0.0;


path[1] = 0.03;
pmotion->positionInterpolation("LLeg", 2, path, 63, 0.5, false);
path[1] = -0.03;
pmotion->positionInterpolation("RLeg", 2, path, 63, 0.5, false);

HPoint3D ball3D;
/*HPoint2D ball2D;

ball2D.x = _BallDetectorOld->getX()*160.0+80.0;
ball2D.y = _BallDetectorOld->getY()*120.0+60.0;
ball2D.h = 1.0;

_Kinematics->get3DPositionZ(ball3D, ball2D, 0.04);

path[0] = 0.0;
path[2] = -0.03;
path[3] = 0.0;
path[4] = 0.0;
path[5] = 0.0;
*/

ball3D.Y = 1.0;

if(ball3D.Y > 0.0)
	path[1] = -0.07;
else
	path[1] = 0.07;

pmotion->positionInterpolation("Torso", 2, path, 63, 1.0, false);

//BUILDER COMMENT. DO NOT REMOVE. end Initial_state_code
}

void Kick::ReadyToKick_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin ReadyToKick_state_code
	ALValue path;
	path.arraySetSize(6);
	path[0] = -0.04;
	path[2] = 0.02;
	path[3] = 0.0;
	path[4] = 0.0;
	path[5] = 0.0;

	if(foot == LEFT)
	{
		path[1] = -0.03;
		pmotion->positionInterpolation("LLeg", 2, path, 63, 1.0, false);
	}else
	{
		path[1] = 0.03;
		pmotion->positionInterpolation("RLeg", 2, path, 63, 1.0, false);
	}

//BUILDER COMMENT. DO NOT REMOVE. end ReadyToKick_state_code
}

void Kick::Kicking_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Kicking_state_code
	ALValue path;
	path.arraySetSize(6);
	path[0] = 0.10;
	path[2] = 0.02;
	path[3] = 0.0;
	path[4] = 0.0;
	path[5] = 0.0;

	if(foot == LEFT)
	{
		path[1] = 0.03;
		pmotion->positionInterpolation("LLeg", 2, path, 63, 1.0, false);
	}else
	{
		path[1] = -0.03;
		pmotion->positionInterpolation("RLeg", 2, path, 63, 1.0, false);
	}

//BUILDER COMMENT. DO NOT REMOVE. end Kicking_state_code
}

void Kick::Recovering_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Recovering_state_code
	ALValue path;
	path.arraySetSize(6);
	path[0] = 0.0;
	path[2] = 0.0;
	path[3] = 0.0;
	path[4] = 0.0;
	path[5] = 0.0;
	if(foot == LEFT)
	{
		path[1] = 0.05;
		pmotion->positionInterpolation("LLeg", 2, path, 63, 1.5, false);
		path[1] = -0.05;
		pmotion->positionInterpolation("RLeg", 2, path, 63, 1.5, false);
	}else
	{
		path[1] = -0.05;
		pmotion->positionInterpolation("RLeg", 2, path, 63, 1.5, false);
		path[1] = 0.05;
		pmotion->positionInterpolation("LLeg", 2, path, 63, 1.5, false);
	}

	path[0] = 0.0;
	path[1] = 0.00;
	path[2] = -0.03;
	path[3] = 0.0;
	path[4] = 0.0;
	path[5] = 0.0;

	pmotion->positionInterpolation("Torso", 2, path, 63, 1.0, false);

//BUILDER COMMENT. DO NOT REMOVE. end Recovering_state_code
}

void Kick::Finished_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Finished_state_code
	fin = true;
//BUILDER COMMENT. DO NOT REMOVE. end Finished_state_code
}

bool Kick::Initial2ReadyToKick0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Initial2ReadyToKick0_transition_code
	return true;
if(pmotion->isRunning(tid))
	return false;
else
	return true;

//BUILDER COMMENT. DO NOT REMOVE. end Initial2ReadyToKick0_transition_code
}

bool Kick::ReadyToKick2Kicking0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin ReadyToKick2Kicking0_transition_code
	return true;
	if(pmotion->isRunning(tid))
		return false;
	else
		return true;
//BUILDER COMMENT. DO NOT REMOVE. end ReadyToKick2Kicking0_transition_code
}

bool Kick::Kicking2Recovering0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Kicking2Recovering0_transition_code
	return true;
	if(pmotion->isRunning(tid))
		return false;
	else
		return true;
//BUILDER COMMENT. DO NOT REMOVE. end Kicking2Recovering0_transition_code
}

bool Kick::Recovering2Finished0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Recovering2Finished0_transition_code
	return true;
	if(pmotion->isRunning(tid))
		return false;
	else
		return true;
//BUILDER COMMENT. DO NOT REMOVE. end Recovering2Finished0_transition_code
}

void
Kick::step(void)
{
	switch (state)
	{
	case Initial:
		_Kinematics->step();

		if (isTime2Run()) {
			Initial_state_code();

			if (Initial2ReadyToKick0_transition_code()) {
				state = ReadyToKick;
			}
		}

		break;
	case ReadyToKick:

		if (isTime2Run()) {
			ReadyToKick_state_code();

			if (ReadyToKick2Kicking0_transition_code()) {
				state = Kicking;
			}
		}

		break;
	case Kicking:

		if (isTime2Run()) {
			Kicking_state_code();

			if (Kicking2Recovering0_transition_code()) {
				state = Recovering;
			}
		}

		break;
	case Recovering:

		if (isTime2Run()) {
			Recovering_state_code();

			if (Recovering2Finished0_transition_code()) {
				state = Finished;
			}
		}

		break;
	case Finished:

		if (isTime2Run()) {
			Finished_state_code();

		}

		break;
	default:
		cout << "[Kick::step()] Invalid state\n";
	}
}
//BUILDER COMMENT. DO NOT REMOVE. auxcode begin
bool
Kick::finished(void)
{
	return fin;
}

void
Kick::restart()
{
	fin = false;
	state = Initial;
}

void
Kick::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);

	fin = false;

	try{
			pmotion = parentBroker->getMotionProxy();
	}catch( AL::ALError& e) {
		cerr << "[Kick ()::init(): " << e.toString() << endl;
	}


}
//BUILDER COMMENT. DO NOT REMOVE. auxcode end


