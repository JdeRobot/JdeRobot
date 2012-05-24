#ifndef Kick_H
#define Kick_H

#include "Component.h"
#include "Kinematics.h"
#include "Singleton.h"

//BUILDER COMMENT. DO NOT REMOVE. auxinclude begin
#include "alproxies/almotionproxy.h"
//BUILDER COMMENT. DO NOT REMOVE. auxinclude end

class Kick : public Component, public Singleton<Kick>
{
public:

	Kick();
	~Kick();

	void step();
private:

	static const int Initial	= 0;
	static const int ReadyToKick	= 1;
	static const int Kicking	= 2;
	static const int Recovering	= 3;
	static const int Finished	= 4;
	int state;

	Kinematics *_Kinematics;

	void Initial_state_code(void);
	void ReadyToKick_state_code(void);
	void Kicking_state_code(void);
	void Recovering_state_code(void);
	void Finished_state_code(void);
	bool Initial2ReadyToKick0_transition_code(void);
	bool ReadyToKick2Kicking0_transition_code(void);
	bool Kicking2Recovering0_transition_code(void);
	bool Recovering2Finished0_transition_code(void);
//BUILDER COMMENT. DO NOT REMOVE. auxcode begin
public:
	void restart();
	bool finished();
	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);
private:
	AL::ALPtr<AL::ALMotionProxy> pmotion;
	bool  fin;
	int tid;
	int foot;

	static const int LEFT = 0;
	static const int RIGHT = 1;
//BUILDER COMMENT. DO NOT REMOVE. auxcode end
};

#endif

