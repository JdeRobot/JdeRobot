#include "Kinematics.h"

#include "Head.h"

Kinematics::Kinematics()
{
	this->escale = 1000.0;
	this->use_lower = 1;

	setFreqTime(SHORT_RATE);

	RTrt.resize(4,4);

	pthread_mutex_init(&mutex, NULL);
}

Kinematics::~Kinematics()
{
}

void
Kinematics::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	try {
		this->pmotion = parentBroker->getMotionProxy();
	} catch( ALError& e)	{
		cerr<<"[Kinematics:init()] " << e.toString() << endl;
	}

	updateKinematics();
}

TKinematics *
Kinematics::getKinematics()
{
	return &(this->mykinematics);
}

void
Kinematics::updateKinematics()
{

	MatrixCM RTneck(4,4), RTdiff(4,4), RTcam(4,4), foarel(4,1), foa(4,1);
	vector<float> neckpos;
	vector<float> neckRT, torso2neckRT;
	vector<float> footpos;
	double deg40 = -40.0*M_PI/180.0;

	torsoRT = this->pmotion->getTransform("Torso", 2, USE_SENSOR);

	/*Obtetemos la posición y orientación del cuello*/
	neckpos = this->pmotion->getPosition("Head", 2/*this->pmotion->SPACE_NAO*/, USE_SENSOR);

	/*Obtetemos la posición y orientación del cuello*/
	torso2neckRT = this->pmotion->getPosition("Head", 0, USE_SENSOR);

	/*Obtenemos la transformada homogenea, que nos permite calcular la traslación desde el cuello a la cámara teniendo en cuenta la inclinación de la cámara*/
	neckRT = this->pmotion->getTransform("Head", 2, USE_SENSOR);


	RTrt.sete(0,0,torsoRT[0]);
	RTrt.sete(0,1,torsoRT[1]);
	RTrt.sete(0,2,torsoRT[2]);
	RTrt.sete(0,3,torsoRT[3]);
	RTrt.sete(1,0,torsoRT[4]);
	RTrt.sete(1,1,torsoRT[5]);
	RTrt.sete(1,2,torsoRT[6]);
	RTrt.sete(1,3,torsoRT[7]);
	RTrt.sete(2,0,torsoRT[8]);
	RTrt.sete(2,1,torsoRT[9]);
	RTrt.sete(2,2,torsoRT[10]);
	RTrt.sete(2,3,torsoRT[11]);
	RTrt.sete(3,0,torsoRT[12]);
	RTrt.sete(3,1,torsoRT[13]);
	RTrt.sete(3,2,torsoRT[14]);
	RTrt.sete(3,3,torsoRT[15]);


	/*RT del cuello*/
	RTneck.sete(0,0,neckRT[0]);
	RTneck.sete(0,1,neckRT[1]);
	RTneck.sete(0,2,neckRT[2]);
	RTneck.sete(0,3,neckRT[3]);
	RTneck.sete(1,0,neckRT[4]);
	RTneck.sete(1,1,neckRT[5]);
	RTneck.sete(1,2,neckRT[6]);
	RTneck.sete(1,3,neckRT[7]);
	RTneck.sete(2,0,neckRT[8]);
	RTneck.sete(2,1,neckRT[9]);
	RTneck.sete(2,2,neckRT[10]);
	RTneck.sete(2,3,neckRT[11]);
	RTneck.sete(3,0,neckRT[12]);
	RTneck.sete(3,1,neckRT[13]);
	RTneck.sete(3,2,neckRT[14]);
	RTneck.sete(3,3,neckRT[15]);

	/*RT de la camara respecto al cuello*/
	if(this->use_lower) {				/*Lower camera*/
		RTdiff.sete(0,0,cos(deg40));
		RTdiff.sete(0,1,0.0);
		RTdiff.sete(0,2,-sin(deg40));
		RTdiff.sete(0,3,0.0488);				/*Profundidad*/
		RTdiff.sete(1,0,0.0);
		RTdiff.sete(1,1,1.0);
		RTdiff.sete(1,2,0.0);
		RTdiff.sete(1,3,0.0);					/*Lateral*/
		RTdiff.sete(2,0,sin(deg40));
		RTdiff.sete(2,1,0.0);
		RTdiff.sete(2,2,cos(deg40));
		RTdiff.sete(2,3,0.02381);				/*Altura*/
		RTdiff.sete(3,0,0.0);
		RTdiff.sete(3,1,0.0);
		RTdiff.sete(3,2,0.0);
		RTdiff.sete(3,3,1.0);	
	} else {							/*Top camera*/
		RTdiff.sete(0,0,cos(0.0));
		RTdiff.sete(0,1,0.0);
		RTdiff.sete(0,2,-sin(0.0));
		RTdiff.sete(0,3,0.0539);				/*Profundidad*/
		RTdiff.sete(1,0,0.0);
		RTdiff.sete(1,1,1.0);
		RTdiff.sete(1,2,0.0);
		RTdiff.sete(1,3,0.0);					/*Lateral*/
		RTdiff.sete(2,0,sin(0.0));
		RTdiff.sete(2,1,0.0);
		RTdiff.sete(2,2,cos(0.0));
		RTdiff.sete(2,3,0.0679);				/*Altura*/
		RTdiff.sete(3,0,0.0);
		RTdiff.sete(3,1,0.0);
		RTdiff.sete(3,2,0.0);
		RTdiff.sete(3,3,1.0);	
	}	

	/*RT de la camara*/
	RTcam = RTneck * RTdiff;

	/*Foa en relativas, mirando 1 metro hacia adelante*/
	foarel.sete(0,0,1.0);
	foarel.sete(1,0,0.0);
	foarel.sete(2,0,0.0);
	foarel.sete(3,0,1.0);

	/*Foa en absolutas*/
	foa = RTcam * foarel;

	/*Save kinematics*/
	this->mykinematics.x = (float)RTcam.e(0,3)*this->escale;		/*X*/
	this->mykinematics.y = (float)RTcam.e(1,3)*this->escale;		/*Y*/
	this->mykinematics.z = (float)RTcam.e(2,3)*this->escale;		/*Z*/
	this->mykinematics.pan = (float)neckpos[5];						/*Pan*/
	if(this->use_lower)
		this->mykinematics.tilt = (float)-(neckpos[4] - deg40);		/*Tilt*/
	else
		this->mykinematics.tilt = (float)-neckpos[4];				/*Tilt*/
	this->mykinematics.roll = (float)neckpos[3];					/*Roll*/
	this->mykinematics.foax = (float)foa.e(0,0)*this->escale;		/*FoaX*/
	this->mykinematics.foay = (float)foa.e(1,0)*this->escale;		/*FoaY*/
	this->mykinematics.foaz = (float)foa.e(2,0)*this->escale;		/*FoaZ*/

	/*Save RT*/
	this->mykinematics.RT[0] = (float)RTcam.e(0,0);
	this->mykinematics.RT[1] = (float)RTcam.e(0,1);
	this->mykinematics.RT[2] = (float)RTcam.e(0,2);
	this->mykinematics.RT[3] = (float)RTcam.e(0,3);
	this->mykinematics.RT[4] = (float)RTcam.e(1,0);
	this->mykinematics.RT[5] = (float)RTcam.e(1,1);
	this->mykinematics.RT[6] = (float)RTcam.e(1,2);
	this->mykinematics.RT[7] = (float)RTcam.e(1,3);
	this->mykinematics.RT[8] = (float)RTcam.e(2,0);
	this->mykinematics.RT[9] = (float)RTcam.e(2,1);
	this->mykinematics.RT[10] = (float)RTcam.e(2,2);
	this->mykinematics.RT[11] = (float)RTcam.e(2,3);
}

void
Kinematics::step(void)
{
}

void
Kinematics::forceStep(void)
{
	pthread_mutex_lock(&mutex);
	startDebugInfo();

	/*Update kinematics and camera*/
	this->updateKinematics();

	endDebugInfo();
	pthread_mutex_unlock(&mutex);
}
