/*
 * JderobotViewer.cpp
 *
 *  Created on: 19/03/2015
 *      Author: frivas
 */

#include <jderobotViewer/JderobotViewer.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>   //to involve opencv2 cvtColor module
#include <opencv2/objdetect/objdetect.hpp>

namespace jderobot {

/**
 *
 * @param nameWindow
 * @param scale_in
 * @param autoUpdate
 * @param worldFile
 */
JderobotViewer::JderobotViewer(const std::string& nameWindow, const double scale_in, const bool autoUpdate, std::string* worldFile):title(nameWindow),sem(0) {
	this->exitSinal=false;
	this->defaultLineWidth=2;
	this->scale=scale_in;
	this->MAXWORLD=50;
	this->signalSave=false;
	this->autoUpdate=autoUpdate;

	if (autoUpdate){
		//dynamic update
		this->viewerThread=boost::shared_ptr<boost::thread>(new boost::thread(&JderobotViewer::refresh_thread, this));
		this->sem.wait();
	}
	else{
		jderobot::Logger::getInstance()->info("This implementation may not work in all infrastructures, if the rendering is not updating please try with the dynamic one");
		//by callback update
		this->viewer=boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer(this->title));
	}

	if (worldFile){
		addWorldFromFile(*worldFile);
	}
	// TODO Auto-generated constructor stub

}

JderobotViewer::~JderobotViewer() {
	// TODO Auto-generated destructor stub
}

void JderobotViewer::refresh_thread(){

	this->viewer=boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer(this->title));


	pcl::visualization::Camera cam;
	cam.pos[0]=-62.8165;
	cam.pos[1]=-183.627;
	cam.pos[2]=152.41;
	cam.view[0]=0.211166;
	cam.view[1]=0.376454;
	cam.view[2]=0.902048;
	cam.focal[0]=23.9582;
	cam.focal[1]=13.4239;
	cam.focal[2]=43.2956;
	cam.window_pos[0]=377;
	cam.window_pos[1]=276;
	cam.window_size[0]=683;
	cam.window_size[1]=384;
	cam.clip[0]=0.01;
	cam.clip[1]=1000.01;
	cam.fovy=0.47;
	this->viewer->setCameraParameters(cam);
	this->viewer->setBackgroundColor(153.0/255.0,204.0/255.0,255.0/255.0);
	viewer->addCoordinateSystem(0.05);
	this->sem.post();
	while (!viewer->wasStopped() || this->exitSinal)
   {
		this->new_data_mutex.lock();
		viewer->spinOnce (100);
		if (this->signalSave){
			this->viewer->saveScreenshot(name2save);
			this->signalSave=false;
		}
		this->new_data_mutex.unlock();
		boost::this_thread::sleep (boost::posix_time::microseconds (100000));
   }
}


int JderobotViewer::addWorldFromFile(const std::string& worldfile){
	FILE *myfile;
	int i;

	if ((myfile=fopen(worldfile.c_str(),"r"))==NULL){
		printf("JDEROBOT: cannot find config file\n");
		return -1;
	}

	do{
		i=load_line(myfile);
	} while(i!=EOF);

	fclose(myfile);
	std::stringstream ss;
	ss << "Loaded: " << this->worldLines.size() << " lines from workFile";
	jderobot::Logger::getInstance()->info(ss.str());

	return 0;
}

int JderobotViewer::load_line(FILE *myfile)
{
	const int limit = 256;
	char word1[limit],words[11][limit];
	int i=0;
	char buffer_file[limit];
	int number;

	buffer_file[0]=fgetc(myfile);
	if (feof(myfile))
		return EOF;
	if (buffer_file[0]==(char)255)
		return EOF;
	if (buffer_file[0]=='#'){
		while(fgetc(myfile)!='\n');
		return 0;
	}
	if (buffer_file[0]==' '){
		while(buffer_file[0]==' ')
			buffer_file[0]=fgetc(myfile);
	}
	if (buffer_file[0]=='\t'){
		while(buffer_file[0]=='\t')
			buffer_file[0]=fgetc(myfile);
	}

	/* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
	while ((buffer_file[i]!='\n')&&(buffer_file[i] != (char)255) && (i<limit-1)) {
		buffer_file[++i]=fgetc(myfile);
	}


	if (i >= limit-1) {
		printf("%s...\n", buffer_file);
		printf ("Line too long in config file!\n");
		exit(-1);
	}
	buffer_file[++i]='\0';

	if (sscanf(buffer_file,"%s",word1)!=1)
		return 0;
	/* return EOF; empty line*/
	else {
		number=sscanf(buffer_file,"%s %s %s %s %s %s %s %s %s %s %s %s",(char*)word1,(char*)words[0],(char*)words[1],(char*)words[2],(char*)words[3],(char*)words[4],(char*)words[5],(char*)words[6],(char*)words[7],(char*)words[8],(char*)words[9],(char*)words[10]);
		if (strcmp(word1,"worldline")==0){
			if (number == 9){
				std::vector<float> line;
				line.push_back((float)atof(words[0]));
				line.push_back((float)atof(words[1]));
				line.push_back((float)atof(words[2]));
				line.push_back((float)atof(words[3]));
				line.push_back((float)atof(words[4]));
				line.push_back((float)atof(words[5]));
				line.push_back((float)atof(words[6]));
				line.push_back((float)atof(words[7]));
				this->worldLines.push_back(line);
			}
			else{
				jderobot::Logger::getInstance()->warning("Too much lines in the world file configuration!");
			}
		}
	}
	return 1;
}


void JderobotViewer::drawWorld(){

	this->new_data_mutex.lock();

	//ground
	for(int i=0;i<((int)MAXWORLD+1);i++) {
		std::stringstream ss1;
		ss1 << "+ground" << i;
		this->viewer->addLine(pcl::PointXYZ( -(int)MAXWORLD*10/2.+(float)i*10,-(int)MAXWORLD*10/2.,0. ),
				pcl::PointXYZ(-(int)MAXWORLD*10/2.+(float)i*10,(int)MAXWORLD*10/2.,0. ),255,255,255,ss1.str());
		std::stringstream ss2;
		ss2 << "-world" << i;
		this->viewer->addLine(pcl::PointXYZ(-(int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.),
				pcl::PointXYZ((int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.), 255,255,255,ss2.str());
	}
	//worldlines
	for (std::vector<std::vector<float> >::iterator it = this->worldLines.begin(); it!=this->worldLines.end();  it++){
		std::stringstream ss;
		ss << "world" << std::distance(this->worldLines.begin(),it);
		pcl::PointXYZ p1((*it)[0]/scale,(*it)[1]/scale,(*it)[2]/scale);
		pcl::PointXYZ p2((*it)[4]/scale,(*it)[5]/scale,(*it)[6]/scale);
		this->viewer->addLine(p1,p2,0,0,0,ss.str());
		this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss.str());
	}
	this->new_data_mutex.unlock();

}

void JderobotViewer::removePrism(const std::string& type, const int prismID){

	this->new_data_mutex.lock();
	for (int i=1; i<=12; i++){
		std::stringstream ss;
		ss << type << prismID << "l" << i;
		std::vector<std::string>::iterator itFind = std::find(this->objectID.begin(), this->objectID.end(),ss.str());
		if (itFind!= this->objectID.end()){
			this->objectID.erase(itFind);
			this->viewer->removeShape(ss.str());
		}
	}
	this->new_data_mutex.unlock();


}

void JderobotViewer::addPrism(const Eigen::Vector4d& pos, const Eigen::Vector4d& size, const std::string& textID, const int prismID,const Eigen::Vector3d& color){

	pcl::PointXYZ p1;
	p1.x=(pos[0]-size[0])/scale;
	p1.y=(pos[1]-size[1])/scale;
	p1.z=(pos[2]-size[2])/scale;

	pcl::PointXYZ p2;
	p2.x=(pos[0]-size[0])/scale;
	p2.y=(pos[1]+size[1])/scale;
	p2.z=(pos[2]-size[2])/scale;

	pcl::PointXYZ p3;
	p3.x=(pos[0]+size[0])/scale;
	p3.y=(pos[1]+size[1])/scale;
	p3.z=(pos[2]-size[2])/scale;

	pcl::PointXYZ p4;
	p4.x=(pos[0]+size[0])/scale;
	p4.y=(pos[1]-size[1])/scale;
	p4.z=(pos[2]-size[2])/scale;

	pcl::PointXYZ p5;
	p5.x=(pos[0]-size[0])/scale;
	p5.y=(pos[1]-size[1])/scale;
	p5.z=(pos[2]+size[2])/scale;

	pcl::PointXYZ p6;
	p6.x=(pos[0]-size[0])/scale;
	p6.y=(pos[1]+size[1])/scale;
	p6.z=(pos[2]+size[2])/scale;

	pcl::PointXYZ p7;
	p7.x=(pos[0]+size[0])/scale;
	p7.y=(pos[1]+size[1])/scale;
	p7.z=(pos[2]+size[2])/scale;

	pcl::PointXYZ p8;
	p8.x=(pos[0]+size[0])/scale;
	p8.y=(pos[1]-size[1])/scale;
	p8.z=(pos[2]+size[2])/scale;


	this->new_data_mutex.lock();

	for (int i=1; i<=12; i++){
		std::stringstream ss;
		ss << textID << prismID << "l" << i;
		std::vector<std::string>::iterator itFind = std::find(this->objectID.begin(), this->objectID.end(),ss.str());
		if (itFind!= this->objectID.end()){
			this->objectID.erase(itFind);
			this->viewer->removeShape(ss.str());
		}
	}



	std::stringstream ss1;
	ss1 << textID << prismID << "l1";
	this->viewer->addLine(p1,p2,(double)color[0],(double)color[1],(double)color[2],ss1.str());
	this->objectID.push_back(ss1.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss1.str());


	std::stringstream ss2;
	ss2 << textID << prismID << "l2";
	this->viewer->addLine(p2,p3,(double)color[0],(double)color[1],(double)color[2],ss2.str());
	this->objectID.push_back(ss2.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss2.str());


	std::stringstream ss3;
	ss3 << textID << prismID << "l3";
	this->viewer->addLine(p3,p4,(double)color[0],(double)color[1],(double)color[2],ss3.str());
	this->objectID.push_back(ss3.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss3.str());


	std::stringstream ss4;
	ss4 << textID << prismID << "l4";
	this->viewer->addLine(p4,p1,(double)color[0],(double)color[1],(double)color[2],ss4.str());
	this->objectID.push_back(ss4.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss4.str());


	std::stringstream ss5;
	ss5 << textID << prismID << "l5";
	this->viewer->addLine(p5,p6,(double)color[0],(double)color[1],(double)color[2],ss5.str());
	this->objectID.push_back(ss5.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss5.str());


	std::stringstream ss6;
	ss6 << textID << prismID << "l6";
	this->viewer->addLine(p6,p7,(double)color[0],(double)color[1],(double)color[2],ss6.str());
	this->objectID.push_back(ss6.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss6.str());


	std::stringstream ss7;
	ss7 << textID << prismID << "l7";
	this->viewer->addLine(p7,p8,(double)color[0],(double)color[1],(double)color[2],ss7.str());
	this->objectID.push_back(ss7.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss7.str());


	std::stringstream ss8;
	ss8 << textID << prismID << "l8";
	this->viewer->addLine(p8,p5,(double)color[0],(double)color[1],(double)color[2],ss8.str());
	this->objectID.push_back(ss8.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss8.str());


	std::stringstream ss9;
	ss9 << textID << prismID << "l9";
	this->viewer->addLine(p1,p5,(double)color[0],(double)color[1],(double)color[2],ss9.str());
	this->objectID.push_back(ss9.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss9.str());


	std::stringstream ss10;
	ss10 << textID << prismID << "l10";
	this->viewer->addLine(p2,p6,(double)color[0],(double)color[1],(double)color[2],ss10.str());
	this->objectID.push_back(ss10.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss10.str());


	std::stringstream ss11;
	ss11 << textID << prismID << "l11";
	this->viewer->addLine(p3,p7,(double)color[0],(double)color[1],(double)color[2],ss11.str());
	this->objectID.push_back(ss11.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss11.str());


	std::stringstream ss12;
	ss12 << textID << prismID << "l12";
	this->viewer->addLine(p4,p8,(double)color[0],(double)color[1],(double)color[2],ss12.str());
	this->objectID.push_back(ss12.str());
	this->viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH, this->defaultLineWidth,ss12.str());
	this->new_data_mutex.unlock();

}

void JderobotViewer::removeAllShapes(){
	this->new_data_mutex.lock();

	for (std::vector<std::string>::iterator it = this->objectID.begin(); it != this->objectID.end(); it++){
		this->viewer->removeShape(*it);
	}
	this->objectID.clear();
	this->new_data_mutex.unlock();

}

void JderobotViewer::removeAllPointClouds(){
	this->new_data_mutex.lock();
	this->viewer->removeAllPointClouds();
	this->new_data_mutex.unlock();

}

void JderobotViewer::spinOnce(const int t){
	this->new_data_mutex.lock();
	this->viewer->spinOnce(t);
	this->new_data_mutex.unlock();
}

void JderobotViewer::addPointCloud( const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud, const std::string& id){
	this->new_data_mutex.lock();
	if (std::find(this->cloudsID.begin(), this->cloudsID.end(),id) != this->cloudsID.end()){
		this->cloudsID.push_back(id);
		this->viewer->addPointCloud(cloud,id);
	}
	else{
		this->viewer->removePointCloud(id);
		this->viewer->addPointCloud(cloud,id);
	}
	this->new_data_mutex.unlock();

}
void JderobotViewer::addPointCloud( const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud){
	this->new_data_mutex.lock();
	this->viewer->addPointCloud(cloud);
	this->viewer->spinOnce (100);
	this->new_data_mutex.unlock();

}

void JderobotViewer::addPointCloud( const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud, const std::string& id){
	this->new_data_mutex.lock();
	this->viewer->addPointCloud(cloud,id);
	this->new_data_mutex.unlock();

}
void JderobotViewer::addPointCloud( const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud){
	this->new_data_mutex.lock();
	this->viewer->addPointCloud(cloud);
	this->new_data_mutex.unlock();
}

void JderobotViewer::addLine(const pcl::PointXYZ& p1, const pcl::PointXYZ& p2, const double r, const double g, const double b, const std::string& id){
	this->new_data_mutex.lock();
	this->viewer->addLine(p1,p2,r,g,b,id);
	this->objectID.push_back(id);
	this->new_data_mutex.unlock();
}




bool JderobotViewer::wasStopped() {
	bool value;
	this->new_data_mutex.lock();
	value= this->viewer->wasStopped();
	this->new_data_mutex.unlock();
	return value;
}

void JderobotViewer::saveScreenshot(const std::string& fileName){
	this->new_data_mutex.lock();
	if (this->autoUpdate){
		this->signalSave=true;
		this->name2save=fileName;
	}
	else{
		this->viewer->saveScreenshot(fileName);
	}
	this->new_data_mutex.unlock();
}
void JderobotViewer::setShapeRenderingProperties(const pcl::visualization::RenderingProperties& property, const double value, const std::string& id ){
	this->new_data_mutex.lock();
	this->viewer->setShapeRenderingProperties(property, value,id);
	this->new_data_mutex.unlock();

}


} /* namespace jderobot */
