#include "SensorRecorderNao.h"

SensorRecorderNao::SensorRecorderNao()
{
	_Body = Body::getInstance();
	_Perception = Perception::getInstance();
	_Kinematics = Kinematics::getInstance();

	//
	this->file_root = "/home/nao/bica/video/";
	this->first_iteration = true;
}

SensorRecorderNao::~SensorRecorderNao()
{
}

void
SensorRecorderNao::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	Component::init(newName, parentBroker);
	this->setFreqTime(250);
}

void
SensorRecorderNao::step(void)
{
	stringstream ss;
	ofstream file_data;
	TKinematics * datak;
	float robotx, roboty, robott;

	struct timeval current_time;
	long diff_time;

	// Steps of its lower components
	_Perception->step();
	_Kinematics->step();
	_Body->step();

	if (!isTime2Run())
		return;

	if(this->first_iteration) {
		/*Initial time*/
		gettimeofday(&current_time,NULL);
		this->init_time=current_time.tv_sec*1000000+current_time.tv_usec;
		diff_time = 0;
		this->first_iteration = false;
	} else {
		/*Diff time*/
		gettimeofday(&current_time,NULL);
		diff_time=current_time.tv_sec*1000000+current_time.tv_usec;
		//cout << "init era " << this->init_time << " y el de ahora es " << 
		diff_time = diff_time - this->init_time;
	}

	/*Get Data*/
	this->src = cvCreateImage(cvSize(ImageInput::IMG_WIDTH,ImageInput::IMG_HEIGHT),	ImageInput::BYTES_PER_CHANNEL, ImageInput::IMG_CHANNELS);
	_Perception->getImageHandler()->getImageRGB(this->src->imageData, false, 0);

	_Body->getGlobalMovement(robotx, roboty, robott);
	datak = _Kinematics->getKinematics();	

	/*Save image*/
  ss << diff_time/1000;
	this->file_name_image = this->file_root + "camera1_" + ss.str() + ".jpg";
	cvSaveImage(this->file_name_image.c_str(), this->src);

	/*Release image*/
	cvReleaseImage(&this->src);

	/*Open data file*/
	this->file_name_data = this->file_root + "datos.txt";
	file_data.open(this->file_name_data.c_str(), ios::app);

	/*Save data*/
  file_data << diff_time/1000 << " localhost:9999:Camera1: images/camera1_" << diff_time/1000 << ".jpg" << endl;
  file_data << diff_time/1000 << " localhost:9999:Encoders: " << robotx << " " << roboty << " " << robott << endl;
  file_data << diff_time/1000 << " localhost:9999:Pose3DEncoders: " << datak->x << " " << datak->y << " " << datak->z << " " << datak->pan << " " << datak->tilt << " " << datak->roll << endl;	

	/*Close file*/
  file_data.close();
}
