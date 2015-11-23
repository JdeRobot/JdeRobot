#include <stdexcept>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "parallelIce/cameraClient.h"
#include "gui.h"
#include "sharer.h"

#define cycle_camera 100000 //microseconds
#define cycle_gui 50000 //microseconds
#define cycle_speed 20000 //microseconds

// Global Memory
navigatorCamera::Sharer *sharer;


inline long cycleWait(long cycle, long diff)
{
	diff = (diff > cycle)?cycle:diff;
	diff = (diff < 0)?cycle:cycle - diff;

	usleep(diff);

	return diff;
} //end cycleWait

// ################################## Threads ##################################

void *showGui(void* gladeFile_ptr)
{
	struct timeval a, b;
	long totala, totalb;
	long diff;
	navigatorCamera::Gui *gui;
	std::string gladeFile = *(std::string *)gladeFile_ptr;

	gui = new navigatorCamera::Gui(sharer, gladeFile);

	while ( gui->isVisible() )
	{
		gettimeofday(&a, NULL);
		totala = a.tv_sec * 1000000 + a.tv_usec;

		gui->display();

		//Sleep Algorithm
		gettimeofday(&b, NULL);
		totalb = b.tv_sec * 1000000 + b.tv_usec;
		diff = (totalb - totala);

		cycleWait(cycle_gui, diff);
	}

	sharer->setGuiVisible(false);

	return NULL;
} //*showGui

void *updateCamera(void* camRGB_ptr)
{
	struct timeval a, b;
	long totala, totalb;
	long diff;
	jderobot::cameraClient* camRGB = (jderobot::cameraClient*)camRGB_ptr;
	cv::Mat rgb;

	while ( sharer->getGuiVisible() )
	{
		gettimeofday(&a, NULL);
		totala = a.tv_sec * 1000000 + a.tv_usec;

		// Get RGB Image data.
		if ( camRGB != NULL )
			camRGB->getImage(rgb);

		if ( rgb.rows != 0 )
		{
			sharer->setImage(rgb);
		}

		//Sleep Algorithm
		gettimeofday(&b, NULL);
		totalb = b.tv_sec * 1000000 + b.tv_usec;
		diff = (totalb - totala);

		cycleWait(cycle_camera, diff);
	}

	return NULL;
} //*updateCamera

void *updateSpeed(void*)
{
	struct timeval a, b;
	long totala, totalb;
	long diff;

	while ( sharer->getGuiVisible() )
	{
		gettimeofday(&a, NULL);
		totala = a.tv_sec * 1000000 + a.tv_usec;

		sharer->changePose3dTranslationSpeed();


		//Sleep Algorithm
		gettimeofday(&b, NULL);
		totalb = b.tv_sec * 1000000 + b.tv_usec;
		diff = (totalb - totala);

		cycleWait(cycle_speed, diff);
	}

	return NULL;
} //*updateCamera

// ################################ End Threads ################################

int main(int argc, char** argv)
{
	int status;

	// ICE
	Ice::CommunicatorPtr ic;
	jderobot::cameraClient* camRGB = NULL;			// parallelIce RGB Image
	jderobot::Pose3DPrx pose3Dprx = 0;				// ICE Pose3D proxy

	IceUtil::ThreadControl rgbTc;	// RGB Image Thread Control

	pthread_t thr_gui;			// GUI thread
	pthread_t thr_camera;		// Update Camera thread
	pthread_t thr_speed;		//speed control

	std::string prefix("navigatorCamera");	// Component Prefix
	std::string gladeFile;					// Path to the glade file

	bool guiActivated;		// GUI activation flag
	bool controlActivated;	// Control activation flag
	bool speedActivated;

	int pose3dFps;		// Frequency to update the Pose3D.
	long cycle_pose3d;	// Time cycle for control the update of Pose3D.

	sharer = new navigatorCamera::Sharer();

	struct timeval a, b;
	long totala, totalb;
	long diff;

	//--------------------INPUT ARGUMENTS--------------------//
	if (argc != 2)
	{
		std::cerr << "\nUSE: ./" << prefix << " --Ice.Config=" << prefix << ".cfg\n" << std::endl;
		return 1;
	}
	//------------------END INPUT ARGUMENTS------------------//

	try
	{
		//--------------------ICE--------------------//
		ic = Ice::initialize(argc,argv);
		Ice::PropertiesPtr prop = ic->getProperties();

		gladeFile = prop->getPropertyWithDefault(prefix + ".gladeFile", "./" + prefix + ".glade");

		guiActivated = prop->getPropertyAsIntWithDefault(prefix + ".guiActivated",1);
		controlActivated = prop->getPropertyAsIntWithDefault(prefix + ".controlActivated",0);
		speedActivated = prop->getPropertyAsIntWithDefault(prefix + ".speedActivated",0);

		pose3dFps = prop->getPropertyAsIntWithDefault(prefix + ".Pose3D.Fps",25);
		cycle_pose3d = (long)( (1./(float)pose3dFps) * 1000000); //microseconds

		sharer->setTranslationStep(atof(prop->getPropertyWithDefault(prefix + ".TranslationStep","0.1").c_str()));
		sharer->setRotationStep(atof(prop->getPropertyWithDefault(prefix + ".RotationStep","0.1").c_str()));

		// Contact to RGB Image interface
		camRGB = new jderobot::cameraClient(ic, prefix + ".CameraRGB.");
		if (camRGB != NULL){
			rgbTc = camRGB->start();
		}
		else{
			throw prefix + ": failed to load RGB Camera";
		}

		// Contact to Pose3D interface
		Ice::ObjectPrx basePose3D = ic->propertyToProxy(prefix + ".Pose3D.Proxy");
		if ( basePose3D == 0 )
			throw "Could not create proxy with Pose3D.";
		// Cast to Pose3D
		pose3Dprx = jderobot::Pose3DPrx::checkedCast(basePose3D);
		if ( pose3Dprx == 0 )
			throw std::invalid_argument("Invalid proxy " + prefix + ".Pose3D.Proxy");

		//------------------END ICE------------------//

		sharer->setGuiVisible(guiActivated);
		sharer->setControlActive(controlActivated);
		if ( guiActivated )
			pthread_create(&thr_gui, NULL, &showGui, static_cast<void *>(&gladeFile));

		pthread_create(&thr_camera, NULL, &updateCamera, static_cast<void *>(camRGB));
		

		// Captures Pose3D ICE data.
		jderobot::Pose3DDataPtr p3dData = pose3Dprx->getPose3DData();
		sharer->setPose3D(p3dData);

		if ( speedActivated ) {
			pthread_create(&thr_speed, NULL, &updateSpeed, NULL);
		}

		while ( sharer->getControlActive() || sharer->getGuiVisible() )
		{
			gettimeofday(&a, NULL);
			totala = a.tv_sec * 1000000 + a.tv_usec;

			// Update the current Pose3D ICE data.
			pose3Dprx->setPose3DData(sharer->getPose3D());

			// Sleep Algorithm
			gettimeofday(&b, NULL);
			totalb = b.tv_sec * 1000000 + b.tv_usec;
			diff = (totalb - totala);

			cycleWait(cycle_pose3d, diff);
		}

		if ( guiActivated )
			pthread_join(thr_gui, NULL);

		pthread_join(thr_camera, NULL);
		camRGB->stop_thread();

	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		status = 1;

	} catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		status = 1;

	} catch (const char* msg) {
		std::cerr << "Error: " << msg << std::endl;
		status = 1;

	}

	rgbTc.join();

	if ( ic )
		ic->destroy();

	return status;
} //end main
