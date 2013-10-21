#include "Body.h"

//BUILDER COMMENT. DO NOT REMOVE. auxinclude begin
const float	Body::MIN_MOVEMENT = 10.0;
const float	Body::MAX_MOVEMENT = 1000.0;
const float	Body::MIN_MOVEMENT_THETA = 0.01;

//Webots: freq=0.3 -> factor=0.86, freq=1.0 -> factor0.7
//Real: freq=1.0 -> factor=1.0
const float	Body::ODOMETRY_FACTOR_X = 1.0;
const float	Body::ODOMETRY_FACTOR_Y = 1.0;
const float	Body::ODOMETRY_FACTOR_T = 1.0;
//BUILDER COMMENT. DO NOT REMOVE. auxinclude end

Body::Body()
{
	state = Initial;

/*	if(USE_SENSOR)
		cerr<<"USANDO SENSORES"<<endl;
	else
		cerr<<"NO USANDO SENSORES"<<endl;*/
}

Body::~Body()
{

}

void Body::Initial_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Initial_state_code
v = w = l = 0.0;
freq = 1.0;
last_f = last_v = last_w = last_l = -1000.0;

calcMovement();

// Init mutex
pthread_mutex_init(&mutex, NULL);

//BUILDER COMMENT. DO NOT REMOVE. end Initial_state_code
}

void Body::Walking_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Walking_state_code
	//cerr<<"Walking ("<<v<<", "<<w<<", "<<l<<")"<<endl;
	
	// Lock mutex
	pthread_mutex_lock(&mutex);

	if(kick != NOKICK)
	{
		//pmotion->post.stopWalk();
		last_w = last_v = last_l = 0.0;
	}else
	if((v != last_v) || (w != last_w) || (l != last_l) || (freq != last_f))
	{ 
		pmotion->setWalkTargetVelocity(v, l, w, freq);

		last_w = w;
		last_v = v;
		last_l = l;
		last_f = freq;
	}

	// Unlock mutex
	pthread_mutex_unlock(&mutex);

	calcMovement();


	/*//cerr<<"["<<_Fallen->getFallen()<<"]";
	if(_Fallen->getFallen() == string("Back"))
	{
		//cerr<<"GBACK!!";
		//kick = TXTSTANDB;
	}
	if(_Fallen->getFallen() == string("Belly"))
	{
		//cerr<<"GFRONT!!";
		//kick = TXTSTANDF;
	}*/

//BUILDER COMMENT. DO NOT REMOVE. end Walking_state_code
}

void Body::Stopped_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Stopped_state_code
calcMovement();
//cerr<<"["<<_Fallen->getFallen()<<"]";

	/*if(_Fallen->getFallen() == string("Back"))
	{
		//cerr<<"GBACK!!";
		//kick = TXTSTANDB;
	}
	if(_Fallen->getFallen() == string("Belly"))
	{
		//cerr<<"GFRONT!!";
		//kick = TXTSTANDF;
	}*/

//BUILDER COMMENT. DO NOT REMOVE. end Stopped_state_code
}

void Body::Moving_state_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Moving_state_code
//BUILDER COMMENT. DO NOT REMOVE. end Moving_state_code
}

bool Body::Initial2Stopped0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Initial2Stopped0_transition_code

	pmotion->post.stopWalk();
	return true;
//BUILDER COMMENT. DO NOT REMOVE. end Initial2Stopped0_transition_code
}

bool Body::Stopped2Walking0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Stopped2Walking0_transition_code
if(((fabs(v)>0.0) || (fabs(w)>0.0)||(fabs(l)>0.0)) && (kick==NOKICK))
{
	//pmotion->post.walkInit();
	return true;
}
else
	return false;
//BUILDER COMMENT. DO NOT REMOVE. end Stopped2Walking0_transition_code
}

bool Body::Walking2Stopped0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Walking2Stopped0_transition_code
if(((v==0.0) && (w==0.0) && (l==0.0)) || (kick != NOKICK))
{
	pmotion->post.stopWalk();
	return true;
}
else
	return false;
//BUILDER COMMENT. DO NOT REMOVE. end Walking2Stopped0_transition_code
}

bool Body::Stopped2Moving0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Stopped2Moving0_transition_code
if(pmotion->walkIsActive() || (kick == NOKICK))
	return false;
else
{

	AL::ALValue jointName  = "Body";

	vector<float> stiffnesses = pmotion->getStiffnesses(jointName);

	cerr<<"stiffness: "<<stiffnesses[0]<<endl;
	if(stiffnesses[0]<0.8)
		pmotion->setStiffnesses(jointName, AL::ALValue(1.0));


	movet.clear();

	kicktime = 10000;
	switch(kick)
	{
	case LFOOT:
	case RFOOT:
		poseInit(0.6);
		doParamKick(kick);
		kicktime = 5000;
		break;
	case SLFOOT:
	case SRFOOT:
		poseInit(0.2);
		poseInit(1.0);
		doKickL(kick);
		//poseInit(0.2);
		kicktime = 5000;
		break;
	case LLFOOT:
	case LRFOOT:
		poseInit(1.0);
		doKickL2(kick);
		kicktime = 5000;
		//poseInit(0.2);
		break;
/*	case GBACK:
		poseInit(0.2);
		StandUpBack();
		poseInit(0.3);
		kicktime = 15000;
		break;
	case GFRONT:
		poseInit(0.2);
		StandUpFront();
		poseInit(0.3);
		kicktime = 15000;
		break;*/
	case POSEINIT:
		poseInit(0.3);
		kicktime = 2000;
		break;
	case FIXED:
		poseInit(0.2);
		doFixMove(fixkick);
		poseInit(0.1);
		kicktime = timeMove;

		break;
	};



	return true;
}
//BUILDER COMMENT. DO NOT REMOVE. end Stopped2Moving0_transition_code
}

bool Body::Moving2Stopped0_transition_code(void)
{
//BUILDER COMMENT. DO NOT REMOVE. begin Moving2Stopped0_transition_code


	if(getStopWatch() < 1000)
		return false;

	vector<string> joints;

	vector<string> jauxlegs;
	vector<string> jauxarms;

	jauxlegs.push_back(string("LKneePitch"));
	jauxlegs.push_back(string("RKneePitch"));
	/*jauxarms.push_back(string("RShoulderPitch"));
	jauxarms.push_back(string("RShoulderRoll"));
	jauxarms.push_back(string("RElbowYaw"));
	jauxarms.push_back(string("RElbowRoll"));
	jauxarms.push_back(string("LShoulderPitch"));
	jauxarms.push_back(string("LShoulderRoll"));
	jauxarms.push_back(string("LElbowYaw"));
	jauxarms.push_back(string("LElbowRoll"));*/
	//ALValue tl = pmotion->getTaskList();

	//AL::ALValue jointName  = "Body";

	//if(tl.getSize()==0)
	if(pmotion->areResourcesAvailable(jauxlegs))// && pmotion->areResourcesAvailable(jauxarms))
	{
		pmotion->post.stopWalk();
		//cerr<<"FINALIZADO EL KICK"<<endl;
		kick = NOKICK;
		return true;
	}
	else
	{
		//cerr<<"ESPERANDO POR KICK"<<endl;
		return false;
	}
//BUILDER COMMENT. DO NOT REMOVE. end Moving2Stopped0_transition_code
}

void
Body::step(void)
{
	switch (state)
	{
	case Initial:

		if (isTime2Run()) {
			startDebugInfo();
			Initial_state_code();

			if (Initial2Stopped0_transition_code()) {
				state = Stopped;
			}
			endDebugInfo();
		}

		break;
	case Walking:

		if (isTime2Run()) {
			startDebugInfo();
			Walking_state_code();

			if (Walking2Stopped0_transition_code()) {
				state = Stopped;
			}
			endDebugInfo();
		}

		break;
	case Stopped:

		if (isTime2Run()) {
			startDebugInfo();
			Stopped_state_code();

			if (Stopped2Walking0_transition_code()) {
				state = Walking;
			}
			else if (Stopped2Moving0_transition_code()) {
				state = Moving;
				resetStopWatch();
			}
			endDebugInfo();
		}

		break;
	case Moving:

		if (isTime2Run()) {
			startDebugInfo();
			Moving_state_code();

			if (Moving2Stopped0_transition_code()) {
				state = Stopped;
			}
			endDebugInfo();
		}

		break;
	default:
		cout << "[Body::step()] Invalid state\n";
	}

}

//BUILDER COMMENT. DO NOT REMOVE. auxcode begin

void
Body::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	try
	{
		pmotion = parentBroker->getMotionProxy();
		pmotion->setStiffnesses("Body", 1.0);
		pmotion->setWalkArmsEnable(true, true);

	} catch (AL::ALError& e) {
		cerr << "Coach::Coach [motion]" << e.toString() << endl;
	}

	v = w = l = 0.0;
	kick = NOKICK;
	fdebug = 1.0;
	freq = 0.8;
	movet.clear();

	initFixMoves();

	this->setFreqTime(SHORT_RATE); //CAMBIAR a MEDIUM_RATE

	//selKick(POSEINIT);
}

void
Body::stop()
{
	setVel(0.0, 0.0, 0.0);
	//pmotion->killWalk();
}

void
Body::setVel(float v, float w, float l)
{
	// Lock mutex
	pthread_mutex_lock(&mutex);

	//Only one decimal (to avoid continous calls to almotion)

	v = ((float)((int)(v *30.0)))/30.0;
	w = ((float)((int)(w *30.0)))/30.0;
	l = ((float)((int)(l *30.0)))/30.0;


	if(l < -1.0 ) l = -1.0;
	if(l > 1.0 ) l = 1.0;
	if(v < -1.0 ) v = -1.0;
	if(v > 1.0 ) v = 1.0;
	if(w < -1.0 ) w = -1.0;
	if(w > 1.0 ) w = 1.0;
	
	this->v = v;
	this->w = w;
	this->l = l;
	// Unlock mutex
	pthread_mutex_unlock(&mutex);
}

void
Body::setVelV(float v)
{
	setVel(v, this->w, this->l);
}

void
Body::setVelW(float w)
{
	setVel(this->v, w, this->l);
}

void
Body::setVelL(float l)
{
	setVel(this->v, this->w, l);
}

void
Body::setFreq(float freq)
{
	this->freq = freq;
	//cerr<<"Changed freq to: "<<this->freq<<endl;
	if(freq < 0.0 ) freq = 0.0;
	if(freq > 1.0 ) freq = 1.0;
}

void
Body::calcMovement()
{
	vector<float> pos = pmotion->getPosition("Torso", 1, USE_SENSOR);
	vector<float> pos2 = pmotion->getRobotPosition(USE_SENSOR);

	movx = pos[0]*1000.0;
	movy = pos[1]*1000.0;
	movtheta = pos[5];

	float movx2 = pos2[0]*1000.0;
	float movy2 = pos2[1]*1000.0;
	float movtheta2 = pos2[5];

	//cerr<<"["<<movx<<" ,"<<movy<<" ,"<<movtheta<<"] vs ["<<movx2<<" ,"<<movy2<<" ,"<<movtheta<<endl;


}

void
Body::getGlobalMovement(float &x, float &y, float &theta)
{
	x = this->movx;
	y = this->movy;
	theta = this->movtheta;
}

bool
Body::hasMoved(float lastx, float lasty, float lastt)
{
	bool minok;
	bool maxok;

	/*Move a minimal value*/
	minok = fabs(this->movx - lastx) > MIN_MOVEMENT || fabs(this->movy - lasty) > MIN_MOVEMENT ||
		fabs(this->movtheta - lastt) > MIN_MOVEMENT_THETA;
	/*Values don't have errors*/
	maxok = fabs(this->movx - lastx) < MAX_MOVEMENT && fabs(this->movy - lasty) < MAX_MOVEMENT;

	return 	minok && maxok;
}

void
Body::getRelativeMovement(float lastx, float lasty, float lastt, float &movx, float &movy, float &movt)
{
	/*From absolute to relative: angle negative*/
	float diffx, diffy, difft;

	diffx = (this->movx - lastx)*ODOMETRY_FACTOR_X;
	diffy = (this->movy - lasty)*ODOMETRY_FACTOR_Y;
	difft = (this->movtheta - lastt)*ODOMETRY_FACTOR_T;

	movx = diffx*cos(-lastt) - diffy*sin(-lastt);
	movy = diffx*sin(-lastt) + diffy*cos(-lastt);
	movt = difft;

	/*movx = 0.0f;
	movy = 0.0f;
	movt = 0.0f;*/
}

void
Body::getAbsoluteMovement(float relx, float rely, float relt, float absx, float absy, float abst, float &movx, float &movy, float &movt)
{
	/*From relative to absolute: angle positive*/
	/*Calculate RT*/
	movx = relx*cos(abst) - rely*sin(abst) + absx;
	movy = relx*sin(abst) + rely*cos(abst) + absy;
	movt = abst + relt;
}

void
Body::poseInit(float speed)
{
	int f;
	float duration;

	if(speed>0.9) speed = 0.9;
	if(speed<0.0) speed = 0.0;
	duration = 2.0*(1.0-speed);

	ALValue path2;
	path2.arraySetSize(6);
		path2[0] = 0.0170;
		path2[1] = 0.0;
		path2[2] = 0.3097;
		path2[3] = 0.00296;
		path2[4] = -0.00671;
		path2[5] = -0.00089;

		f = pmotion->post.positionInterpolation("Torso", 2, path2, 63, duration*fdebug, true);
		movet.push_back(f);


	ALValue path0;
	path0.arraySetSize(6);
	path0[0] = 0.0;
	path0[1] = -0.05;
	path0[2] = 0.0;
	path0[3] = 0.0;
	path0[4] = 0.0;
	path0[5] = 0.0;

	f = pmotion->post.positionInterpolation("RLeg", 2, path0, 63, duration*fdebug, true);
	movet.push_back(f);

	ALValue path1;
	path1.arraySetSize(6);
	path1[0] = 0.0;
	path1[1] = 0.05;
	path1[2] = 0.0;
	path1[3] = 0.0;
	path1[4] = 0.0;
	path1[5] = 0.0;

	f = pmotion->post.positionInterpolation("LLeg", 2, path1, 63, duration*fdebug, true);
	movet.push_back(f);


	/*ALValue path3;
	path3.arraySetSize(6);
	path3[0] = 0.0;
	path3[1] = 0.0;
	path3[2] = 0.0;
	path3[3] = toRadians(-20.0);
	path3[4] = toRadians(-20.0);
	path3[5] = toRadians(-45.0);

	f = pmotion->post.setPosition("Head", 2, path3, 0.5, 56);
		movet.push_back(f);*/

}


void
Body::selKick(int kicksel)
{
	kick = kicksel;
}

void
Body::selKick(string kicksel)
{
	kick = FIXED;
	fixkick = kicksel;
}

void
Body::setDebugMov(bool d)
{
	if(d)
		fdebug = 4.0;
	else
		fdebug = 1.0;
}
void
Body::doParamKick(int foot)
{
	int f;

	string foots;
	if(foot == LFOOT)
		foots = "LLeg";
	else
		foots = "RLeg";

	ALValue path0;
	path0.arraySetSize(6);
	path0[0] = 0.0;
	path0[2] = -0.03;
	path0[3] = 0.0;
	path0[4] = 0.0;
	path0[5] = 0.0;

	if(foot == LFOOT)
		path0[1] = -0.085;
	else
		path0[1] = 0.085;

	f = pmotion->post.positionInterpolation("Torso", 2, path0, 63, 1.0*fdebug, false);
	movet.push_back(f);

	ALValue path1;
	path1.arraySetSize(6);
	path1[0] = -0.04;
	path1[2] = 0.02;
	path1[3] = 0.0;
	path1[4] = 0.0;
	path1[5] = 0.0;

	if(foot == LFOOT)
	{
		path1[1] = 0.01;
		path1[5] = 0.0;
	}
	else
	{
		path1[1] = -0.01;
		path1[5] = 0.0;
	}
	f = pmotion->post.positionInterpolation(foots, 2, path1, 63, 1.5*fdebug, true);
	movet.push_back(f);



	ALValue path2;
	path2.arraySetSize(6);
	path2[0] = +0.10;
	path2[2] = 0.02;
	path2[3] = 0.0;
	path2[4] = 0.0;

	if(foot == LFOOT)
	{
		path2[1] = 0.01;
		path2[5] = 0.0;
	}
	else
	{
		path2[1] = -0.01;
		path2[5] = 0.0;
	}


	f = pmotion->post.positionInterpolation(foots, 2, path2, 63, 0.17*fdebug, true);
	movet.push_back(f);

	ALValue path3;
	path3.arraySetSize(6);
	path3[0] = 0.0;
	path3[2] = 0.02;
	path3[3] = 0.0;
	path3[4] = 0.0;

	if(foot == LFOOT)
	{
		path3[1] = +0.02;
		path3[5] = 0.0;
	}
	else
	{
		path3[1] = -0.02;
		path3[5] = 0.0;
	}

	f = pmotion->post.positionInterpolation(foots, 2, path3, 63, 0.9*fdebug, true);
	movet.push_back(f);

	ALValue path4;
	path4.arraySetSize(6);
	path4[0] = 0.0;
	path4[2] = 0.0;
	path4[3] = 0.0;
	path4[4] = 0.0;
	path4[5] = 0.0;

	if(foot == LFOOT)
		path4[1] = -0.065;
	else
		path4[1] = 0.065;

	f = pmotion->post.positionInterpolation("Torso", 2, path4, 63, 1.0*fdebug, false);
	movet.push_back(f);

	ALValue path5;
	path5.arraySetSize(6);
	path5[0] = 0.0;
	path5[2] = -0.025;
	path5[3] = 0.0;
	path5[4] = 0.0;

	if(foot == LFOOT)
	{
		path5[1] = +0.05;
		path5[5] = 0.0;
	}
	else
	{
		path5[1] = -0.05;
		path5[5] = 0.0;
	}

	f = pmotion->post.positionInterpolation(foots, 2, path5, 63, 0.9*fdebug, true);
	movet.push_back(f);
}

void
Body::doFixMove(string move)
{
	tmov m = fixmovs[move];
	timeMove = m.time;
	for(int i=0; i<m.steps; i++)
	{
		cerr << move << ": " << i << endl;
		ALValue angles;
		angles.arraySetSize(m.numjoints);
		ALValue times;
		times.arraySetSize(m.numjoints);
		for(int j=0; j<m.numjoints;j++)
		{
			ALValue auxa, auxt;
			auxa.arraySetSize(1);
			auxt.arraySetSize(1);
			auxa[0] = m.angles[i][j];
			auxt[0] = m.times[i][j];
			angles[j] = auxa;
			times[j] = auxt;
		}
		movet.push_back(pmotion->post.angleInterpolation(m.joints, angles, times, true));
	}

}

void Body::setStiffness(float val){
	vector<string> pJointName;
	vector<float> pStiffness;
	if (val > 1.0) val =1.0; else if (val < 0.0 ) val = 0.0;
/*	pJointName.push_back("Body");
	pStiffness.push_back(val);
	pmotion->setStiffnesses(pJointName,pStiffness);*/

	pmotion->setStiffnesses("Body", val);


}

void
Body::initFixMoves()
{
	fixmovs.clear();

	ifstream infile;
	string line;

	infile.open ("/home/nao/bica/movs/movs.index", ifstream::in);

	getline(infile, line);
	while (infile.good())
	{
		if(line.length()>0)
		{
			loadMove(line+string(".mov"));
		}
		getline(infile, line);
	}

	infile.close();

}

void
Body::loadMove(string MoveFile)
{
	//cerr<<"Loading mov: "<<MoveFile<<endl;
	ifstream infile;
	string line;
	char MoveFilename[64];
	unsigned int num_joints;
	int steps=0;
	float time=0.0;
	tmov *aux = new tmov;

	infile.open (string(("/home/nao/bica/movs/") + MoveFile).c_str(), ifstream::in);

	getline(infile, line);
	sscanf(line.c_str(), "%s %d %f", MoveFilename, &steps, &time);

	if(time == 0.0) time = 4.0; //por si no tiene el fichero el tiempo. Default = 4000 ms

	//Steps
	//Joints
	//cerr<<"Joints: ";
	if(infile.good())
		getline(infile, line);

	TokenizeS(line, aux->joints);

	aux->steps = steps;
	aux->time = (long)(time*1000.0);

	aux->angles = new float*[steps];
	aux->times = new float*[steps];

	// Vamos a leer los ÃÂ¡ngulos
	// en cada instante, va a hebar un

	num_joints = aux->joints.getSize();
	aux->numjoints = num_joints;

	//[ [j0_t0] [j1_t0] [j2_t0] ... [jn_t0] ]
	//[ [j0_t1] [j1_t1] [j2_t1] ... [jn_t1] ]
	//...
	//[ [j0_tk] [j1_tk] [j2_tk] ... [jn_tk] ]

	for(int s=0; s<steps; s++)
	{
		aux->angles[s] = new float[num_joints];
		aux->times[s] = new float[num_joints];
	}

	for(unsigned int i=0; i<num_joints; i++)
	{
		if(infile.good())
			getline(infile, line);
		ALValue auxal;
		auxal.clear();
		Tokenize(line, auxal);
		for(int s=0; s<steps; s++)
			aux->angles[s][i] = auxal[s];
	}


	for(unsigned int i=0; i<num_joints; i++)
	{
		if(infile.good())
			getline(infile, line);

		ALValue auxal;
		auxal.clear();

		Tokenize(line, auxal);

		for(int s=0; s<steps; s++)
			aux->times[s][i] = auxal[s];
	}

	for(int s=steps-1; s>0; s--)
	{
		for(unsigned int i=0; i<num_joints; i++)
		{
			//cerr<<"("<<s<<","<<i<<") = "<< aux->times[s][i]<< " - "<<aux->times[s][i-1]<<endl;
			aux->times[s][i] = aux->times[s][i] - aux->times[s-1][i];
		}
	}

	fixmovs[string(MoveFilename)] = *aux;

	infile.close();

}

void
Body::TokenizeS(const string& str, ALValue& tokens, const string& delimiters)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.arrayPush(str.substr(lastPos, pos - lastPos));
		//cerr<<" "<<str.substr(lastPos, pos - lastPos);
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void
Body::Tokenize(const string& str, ALValue& tokens, const string& delimiters)
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.arrayPush(atof(str.substr(lastPos, pos - lastPos).c_str()));
		//cerr<<" "<<atof(str.substr(lastPos, pos - lastPos).c_str());
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void
Body::setVel(float v, float w, float l, const Ice::Current& c)
{
	setVel(v, w, l);
}

void
Body::poseInit(const Ice::Current& c)
{
	selKick(POSEINIT);
}

bool
Body::isWalking(const Ice::Current& c)
{
	if (state == Walking) return true;
	else return false;
}

bool
Body::isMoving(const Ice::Current& c)
{
	if (state == Moving) return true;
	else return false;
}

void
Body::doMove(const string& kick, const Ice::Current& c)
{
}

void
Body::doKickL(int foot)
{
	int f;

	string foots;
	if(foot == SLFOOT)
		foots = "LLeg";
	else
		foots = "RLeg";

	ALValue path0;
	path0.arraySetSize(6);
	path0[0] = 0.0;
	path0[2] = -0.03;
	path0[3] = 0.0;
	path0[4] = 0.0;
	path0[5] = 0.0;

	if(foot == SLFOOT)
		path0[1] = -0.07;
	else
		path0[1] = 0.07;

	f = pmotion->post.positionInterpolation("Torso", 2, path0, 63, 1.5, false);
	movet.push_back(f);

	ALValue path1;
	path1.arraySetSize(6);
	path1[0] = 0.0;
	path1[2] = 0.0;
	path1[3] = 0.0;
	path1[4] = 0.0;

	if(foot == SLFOOT)
	{
		path1[1] = 0.06;
	}
	else
	{
		path1[1] = -0.06;
	}
		if(foot == SLFOOT)
	{
		path1[5] = 0.8;
	}
	else
	{
		path1[5] = -0.8;
	}

//path1[5] = 0.8;
	f = pmotion->post.positionInterpolation(foots, 2, path1, 63, 1.5, true);
	movet.push_back(f);
}

void
Body::doKickL2(int foot)
{
	cerr<<"ejecutando doKickL2"<<endl;
	int f;
	int time = 1;

	string foots;
	string arms;
	if(foot == LLFOOT){
		foots = "LLeg";
		arms = "RArm";
	}
	else {
		foots = "RLeg";
		arms = "LArm";
	}

	ALValue path0;
	path0.arraySetSize(6);
	path0[0] = 0.0;
	path0[1] = 0.0; //por el comentario
	path0[2] = -0.065;//-0.08;
	path0[3] = 0.0;
	path0[4] = 0.0;
	path0[5] = 0.0;

	if(foot == LLFOOT)
		path0[1] = -0.02;
	else
		path0[1] = 0.02;

	f = pmotion->post.positionInterpolation("Torso", 2, path0, 63, 1.0*time, false);
	movet.push_back(f);

	ALValue path1;
	path1.arraySetSize(6);
	path1[0] = 0.0;
	path1[2] = 0.0;
	path1[3] = 0.0;
	path1[4] = 0.0;

	if(foot == LLFOOT)
	{
		path1[1] = 0.19;
	}
	else
	{
		path1[1] = -0.19;
	}
	if(foot == LLFOOT)
	{
		path1[5] = 0.8;
	}
	else
	{
		path1[5] = -0.8;
	}

	f = pmotion->post.positionInterpolation(foots, 2, path1, 63, 1.0*time, true);
	//movet.push_back(f);
	movet.push_back(f); //para que haga los 2 movs
}
//BUILDER COMMENT. DO NOT REMOVE. auxcode end




