#include "Perception.h"

const string Perception::UPPER_CAMERA_CONFIG_FILE =
		"/home/nao/bica/conf/ballPerception/cameraUpper.conf";
const string Perception::LOWER_CAMERA_CONFIG_FILE =
		"/home/nao/bica/conf/ballPerception/cameraLower.conf";


jderobot::ImageDataPtr
Perception::getImageData(const Ice::Current& c)
{
	jderobot::ImageDataPtr reply(new jderobot::ImageData);
	reply->description = new jderobot::ImageDescription();
	reply->description->width = ImageInput::IMG_WIDTH;
	reply->description->height = ImageInput::IMG_HEIGHT;
	reply->description->size = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS;
	reply->description->format = "RGB8";
	
	int imgSize = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS * sizeof(char);

	IceUtil::Time t = IceUtil::Time::now();
	reply->timeStamp.seconds = (long)t.toSeconds();
	reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
	reply->pixelData.resize(imgSize);

	IplImage* src;

	this->newImage();
	src = cvCreateImage(cvSize(ImageInput::IMG_WIDTH,ImageInput::IMG_HEIGHT),
				ImageInput::BYTES_PER_CHANNEL, ImageInput::IMG_CHANNELS);

	_ImageHandler->getImageRGBInverted(src->imageData, false, 0);

	memmove( &(reply->pixelData[0]), (char *) src->imageData, imgSize);//copy data to reply

	cvReleaseImage(&src);

	return reply;
}

jderobot::ImageDescriptionPtr
Perception::getImageDescription(const Ice::Current& c)
{
	return imgDescription;
}

void
Perception::setCam (CameraType cam, const Ice::Current& c)
{
	if (cam == UPPERCAMERA)
		setCam(ImageInput::UPPER_CAMERA);
	else
		setCam(ImageInput::LOWER_CAMERA);
}

Perception::Perception() {

	colorFilter = SimpleColorFilter::getInstance();
	_Kinematics = Kinematics::getInstance();
	_ImageHandler = ImageHandler::getInstance();

	camera_initialized = false;

	camConfUpper.createDictionary();
	camConfLower.createDictionary();

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutexstep, NULL);

	setFreqTime(SHORT_RATE);

	this->imgDebug.arrayReserve(ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS * sizeof(char));
	this->colorSrc = new char[ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS];

	jderobot::ImageDescriptionPtr imgDescription(new jderobot::ImageDescription);
	imgDescription->width = ImageInput::IMG_WIDTH;
	imgDescription->height = ImageInput::IMG_HEIGHT;
	imgDescription->size = ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS;
	imgDescription->format = "YUV888";

	newImageTaken = false;

	struct timeval current;
	long current_m;

	gettimeofday(&current, NULL);
	current_m = current.tv_sec * 1000000 + current.tv_usec;

	lastTimeStamp = current_m;//newImage->fTimeStamp;
	wtime=0;

}

Perception::~Perception() {

	try {
		pcamera->unsubscribe(fLEMname);
	} catch (ALError& e) {
		cerr << "Unable to unsubscribe Lem: " << e.toString() << endl;
	}

	cvReleaseImage(&cvImage);
	cvReleaseImage(&cvAux);

	delete this->colorSrc;
}

void
Perception::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);

	initCamera();

	int rc;

	rc =  pthread_create(&capture_thread, NULL, Perception::EntryPoint, this);

    if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
    }
}

/*Camera parameters*/
void
Perception::loadCameraParams(const string upper_file, const string lower_file)
{
	Dictionary *conf;
	ostringstream s;

	camConfUpper.loadFromFile(upper_file.c_str());
	camConfLower.loadFromFile(lower_file.c_str());

	int newkCameraBrightnessID, newkCameraContrastID, newkCameraSaturationID,
	newkCameraHueID, newkCameraRedChromaID, newkCameraBlueChromaID,
	newkCameraGainID, newkCameraLensXID, newkCameraLensYID,
	newkCameraAutoGainID, newkCameraAutoExpositionID,
	newkCameraAutoWhiteBalanceID;

	for (int cameraID = 1; cameraID >= 0; cameraID--) {

		if (cameraID == ImageInput::LOWER_CAMERA)
			conf = &camConfLower;
		else if (cameraID == ImageInput::UPPER_CAMERA)
			conf = &camConfUpper;
		else
			s << "[Perception::loadCameraParams()] Bad camera identifier (" << cameraID << endl;

		// Read the camera parameters
		if ((conf->getValueInt("kCameraBrightnessID", &newkCameraBrightnessID)) &&
				(conf->getValueInt("kCameraContrastID", &newkCameraContrastID)) &&
				(conf->getValueInt("kCameraSaturationID", &newkCameraSaturationID)) &&
				(conf->getValueInt("kCameraHueID", &newkCameraHueID)) &&
				(conf->getValueInt("kCameraRedChromaID", &newkCameraRedChromaID)) &&
				(conf->getValueInt("kCameraBlueChromaID", &newkCameraBlueChromaID)) &&
				(conf->getValueInt("kCameraGainID", &newkCameraGainID)) &&
				(conf->getValueInt("kCameraLensXID", &newkCameraLensXID)) &&
				(conf->getValueInt("kCameraLensYID", &newkCameraLensYID)) &&
				(conf->getValueInt("kCameraAutoGainID",	&newkCameraAutoGainID)) &&
				(conf->getValueInt("kCameraAutoExpositionID", &newkCameraAutoExpositionID)) &&
				(conf->getValueInt("kCameraAutoWhiteBalanceID", &newkCameraAutoWhiteBalanceID)))
			s << "[Perception] Loaded camera parameters for camera " << cameraID << endl;
		else
			s << "[Perception] Problem loading parameters for camera " << cameraID << endl;

		// Set the camera parameters
		try {
			pcamera->setParam(kCameraSelectID, cameraID);
			usleep(250000);

			pcamera->setParam(kCameraBrightnessID, newkCameraBrightnessID);
			pcamera->setParam(kCameraContrastID, newkCameraContrastID);
			pcamera->setParam(kCameraSaturationID, newkCameraSaturationID);
			pcamera->setParam(kCameraHueID, newkCameraHueID);
			pcamera->setParam(kCameraRedChromaID, newkCameraRedChromaID);
			pcamera->setParam(kCameraBlueChromaID, newkCameraBlueChromaID);
			pcamera->setParam(kCameraGainID, newkCameraGainID);
			pcamera->setParam(kCameraLensXID, newkCameraLensXID);
			pcamera->setParam(kCameraLensYID, newkCameraLensYID);

			// Automatic values
			pcamera->setParam(kCameraAutoGainID, newkCameraAutoGainID);
			pcamera->setParam(kCameraAutoExpositionID, newkCameraAutoExpositionID);
			pcamera->setParam(kCameraAutoWhiteBalanceID, newkCameraAutoWhiteBalanceID);
		} catch (ALError& e) {
			s << "Could not set cam param: " << e.toString() << endl;
		}
	}
	writeLog(s.str());
}

bool
Perception::setCamParam(const int param, const int value)
{
	ostringstream s;
	try {
		pcamera->setParam(param, value);
		return true;
	} catch (ALError& e) {
		s << "Could not set cam param: " << e.toString() << endl;
		return false;
	}
}

int
Perception::getCamParam(const int param)
{
	ostringstream s;
	try {
		return pcamera->getParam(param);
	} catch (ALError& e) {
		s << "Could not set cam param: " << e.toString() << endl;
		return -1;
	}
}

void
Perception::initCamera(void)
{
	try {
		pcamera = new ALVideoDeviceProxy(getParentBroker());
		cam = pcamera->getParam(kCameraSelectID);
		fLEMname = pcamera->subscribe(string("playerGVM"), kQVGA,
				kYUV422InterlacedColorSpace, 30);
	} catch (ALError& e) {
		cerr << "[Perception::initCamera()] " << e.toString() << endl;
	}

	filterParams.arraySetSize(6);

	cvImage = cvCreateImage(cvSize(ImageInput::IMG_WIDTH, ImageInput::IMG_HEIGHT), ImageInput::BYTES_PER_CHANNEL, 2);
	cvAux = cvCreateImage(cvSize(ImageInput::IMG_WIDTH, ImageInput::IMG_HEIGHT), ImageInput::BYTES_PER_CHANNEL, ImageInput::IMG_CHANNELS);

	camera_initialized = true;

	for (int i = 0; i < 2; i++)
		loadCameraParams(UPPER_CAMERA_CONFIG_FILE, LOWER_CAMERA_CONFIG_FILE);

	setCam(ImageInput::LOWER_CAMERA);
}

long
Perception::getWaitingTime()
{
	return wtime;
};

void *Perception::EntryPoint(void * pthis)
{
	Perception *pt = Perception::getInstance();
    pt->Capture();
}
void Perception::Capture()
{
	struct timeval s1;

	while(true)
	{

	#ifdef PLAYER_IS_REMOTE_OFF		//Get new image (local mode)

		ALImage* newImage = NULL;
		try {
			newImage = (ALImage*) (pcamera->getDirectRawImageLocal(fLEMname));
		} catch( ALError& e) {
			cerr << "[Perception::getImageLocal()] Error: " << e.toString() << endl;
		}

		pthread_mutex_lock(&mutex);
		memcpy(imgBuff, (char*)(newImage->getFrame()), ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV);

		cerr<<"*";
		pthread_mutex_unlock(&mutex);

		gettimeofday(&s1, NULL);
		newImageTaken = true;
		image_ts = s1.tv_sec * 1000000 + s1.tv_usec;

		try {
			pcamera->releaseDirectRawImage(fLEMname);
		}catch (ALError& e) {
			cerr << "Error in releasing image: " << e.toString() << endl;
		}

	#else							//Get new image (remote mode)
		ALValue newImage;

		try {
			newImage = pcamera->getImageRemote(fLEMname);

		} catch (ALError& e) {
			cerr << "[Perception::getImageRemote()] Error: " << e.toString() << endl;
		}

		pthread_mutex_lock(&mutex);
		memcpy(imgBuff, (char*) static_cast<const char*> (newImage[6].GetBinary()), ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV);
		pthread_mutex_unlock(&mutex);

		gettimeofday(&s1, NULL);
		newImageTaken = true;
		image_ts = s1.tv_sec * 1000000 + s1.tv_usec;

	#endif

	}

}

bool
Perception::newImage()
{

	if(newImageTaken)
	{

		long end, start;
		struct timeval s1;

		pthread_mutex_lock(&mutex);

		gettimeofday(&s1, NULL);
		start = s1.tv_sec * 1000000 + s1.tv_usec;
		_ImageHandler->setImage(cam, imgBuff, colorFilter);
		gettimeofday(&s1, NULL);
		end = s1.tv_sec * 1000000 + s1.tv_usec;

		_Kinematics->forceStep();

		pthread_mutex_unlock(&mutex);

		newImageTaken = false;

		wtime = (end-start);
		return true;
	}else
	{
		return false;
	}

}

void
Perception::step(void)
{
	pthread_mutex_lock(&mutexstep);

	if (isTime2Run()) {
		startDebugInfo();

		/*Get a new image*/
		this->newImage();

		endDebugInfo();
	}
	pthread_mutex_unlock(&mutexstep);

}

void Perception::setCam(const int cam) {
	if (!camera_initialized)
		initCamera();

	try {
		pcamera->setParam(kCameraSelectID, cam);
		this->cam = cam;
	} catch (ALError& e) {
		cerr << "Unable to change camera: " << e.toString() << endl;
	}
}
