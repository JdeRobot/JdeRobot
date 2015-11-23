#include "sensors.h"

Sensors::Sensors(Ice::CommunicatorPtr ic)
{
	this-> ic = ic;
	Ice::PropertiesPtr prop = ic->getProperties();
	//Camera
	Ice::ObjectPrx basecamera = ic->propertyToProxy("UAVViewer.Camera.Proxy");
	if (0==basecamera)
		throw "Could not create proxy";

	/*cast to CameraPrx*/
	cprx = jderobot::CameraPrx::checkedCast(basecamera);
	if (0==cprx)
		throw "Camera -> Invalid proxy";

    jderobot::ImageDataPtr data = cprx->getImageData("RGB8");
	
	//Navdata
	Ice::ObjectPrx basenavdata = ic->propertyToProxy("UAVViewer.Navdata.Proxy");
	if (0==basenavdata)
		throw "Could not create proxy";

	/*cast to NavdataPrx*/
	navprx = jderobot::NavdataPrx::checkedCast(basenavdata);
	if (0==navprx)
		throw "Navdata -> Invalid proxy";
	//CMDVel
	Ice::ObjectPrx basecmd = ic->propertyToProxy("UAVViewer.CMDVel.Proxy");
	if (0==basecmd)
		throw "Could not create proxy";

	/*cast to CMDVelPrx*/
	cmdprx = jderobot::CMDVelPrx::checkedCast(basecmd);
	if (0==cmdprx)
		throw "CMDVel -> Invalid proxy";
	//Pose3D
	Ice::ObjectPrx basepose = ic->propertyToProxy("UAVViewer.Pose3D.Proxy");
	if (0==basepose)
		throw "Could not create proxy";

	/*cast to Pose3DPrx*/
	poseprx = jderobot::Pose3DPrx::checkedCast(basepose);
	if (0==poseprx)
		throw "Pose3D -> Invalid proxy";

    //Pose3D
    Ice::ObjectPrx baseextra = ic->propertyToProxy("UAVViewer.Extra.Proxy");
    if (0==baseextra)
        throw "Could not create proxy";

    /*cast to ArDronePrx*/
    arextraprx = jderobot::ArDroneExtraPrx::checkedCast(baseextra);
    if (0==arextraprx)
        throw "ArDroneExtra -> Invalid proxy";

	this->tracking=false;
	this->flying=false;	
	this->rst=false;
		
}

Sensors::~Sensors(){

}

void Sensors::sendVelocitiesToUAV(float vx,float vy,float vz,float roll,float pitch,float yaw)
{
	mutexDrone.lock();
        jderobot::CMDVelDataPtr vel=new jderobot::CMDVelData();
        vel->linearX=vx;
        vel->linearY=vy;
        vel->linearZ=vz;
        vel->angularZ=yaw;
        vel->angularX=roll;
        vel->angularY=pitch;
		
		cmdprx->setCMDVelData(vel);

    mutexDrone.unlock();    
}
void Sensors::update()
{
	mutex.lock();
        jderobot::ImageDataPtr data = cprx->getImageData("RGB8");
		image.create(data->description->height, data->description->width, CV_8UC3); 
		memcpy((unsigned char *) image.data ,&(data->pixelData[0]), image.cols*image.rows * 3);  
	mutex.unlock();
	mutexDrone.lock();
		navdata=navprx->getNavdata();
		pose3DDataPtr = poseprx->getPose3DData();		
	mutexDrone.unlock();

}

cv::Mat Sensors::getImage()
{
    mutex.lock();
		cv::Mat result = image.clone();
    mutex.unlock();
    if(result.cols>IMAGE_COLS_MAX)
    {
	    if(result.rows>IMAGE_ROWS_MAX)
	    {
	    	cv::Size size(IMAGE_COLS_MAX,IMAGE_ROWS_MAX);
	    	cv::resize(result,result,size);
	    }else{
	    	cv::Size size(IMAGE_COLS_MAX,result.cols);
	    	cv::resize(result,result,size);	    
	    }    	
    }else if(result.rows>IMAGE_ROWS_MAX)
    {
    	cv::Size size(result.cols,IMAGE_ROWS_MAX);
    	cv::resize(result,result,size);
    }
    return result;
}


void Sensors::takeOff(){
	mutexDrone.lock();
		if(!rst){
            arextraprx->takeoff();
			this->flying=true;
			std::cout << "takeoff"<<std::endl;	
		}
   	mutexDrone.unlock();
}

void Sensors::land(){
	mutexDrone.lock();
		if(!rst){
			jderobot::CMDVelDataPtr cmd=new jderobot::CMDVelData();
			cmd->linearX=cmd->linearY=cmd->linearZ=0;		
			cmd->angularX=cmd->angularY=cmd->angularZ=0;			
			cmdprx->setCMDVelData(cmd);		
            arextraprx->land();
			this->flying=false;
			this->tracking=false;
			std::cout << "land"<<std::endl;	
		}
   	mutexDrone.unlock();	
}

void Sensors::reset(){
	mutexDrone.lock();
        arextraprx->reset();
		this->flying=false;
		this->tracking=false;
		this->rst=!this->rst;
   	mutexDrone.unlock();	
}

void Sensors::toggleCam(){
	mutex.lock();
        arextraprx->toggleCam();
	mutex.unlock();
}

jderobot::NavdataDataPtr Sensors::getNavdata(){
	jderobot::NavdataDataPtr tmp;
	mutexDrone.lock();
		tmp=navdata;
	mutexDrone.unlock();
    return tmp;
}

jderobot::Pose3DDataPtr Sensors::getPose3DData(){
	jderobot::Pose3DDataPtr tmp = new jderobot::Pose3DData();
	mutexDrone.lock();
		tmp->x=pose3DDataPtr->x;
		tmp->y=pose3DDataPtr->y;
		tmp->z=pose3DDataPtr->z;
		tmp->h=pose3DDataPtr->h;
		tmp->q0=pose3DDataPtr->q0;
		tmp->q1=pose3DDataPtr->q1;
		tmp->q2=pose3DDataPtr->q2;
		tmp->q3=pose3DDataPtr->q3;
	mutexDrone.unlock();
    return tmp;
}

