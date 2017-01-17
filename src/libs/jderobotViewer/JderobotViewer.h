/*
 * JderobotViewer.h
 *
 *  Created on: 19/03/2015
 *      Author: frivas
 */

#ifndef SRC_STABLE_LIBS_JDEROBOTVIEWER_JDEROBOTVIEWER_H_
#define SRC_STABLE_LIBS_JDEROBOTVIEWER_JDEROBOTVIEWER_H_

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <Eigen/Dense>
#include <pcl/common/common_headers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <logger/Logger.h>
#include <opencv2/core/core.hpp>


namespace jderobot {

class JderobotViewer {
public:
	JderobotViewer(const std::string& nameWindow, const double scale_in, const bool autoUpdate, std::string* worldFile=NULL);
	virtual ~JderobotViewer();

	void addPrism(const Eigen::Vector4d& pos, const Eigen::Vector4d& size,const std::string& type, const int prismID,const Eigen::Vector3d& color);
	void removeAllPointClouds();
	void removeAllShapes();
	void spinOnce(const int t);
	void addPointCloud( const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud, const std::string& id);
	void addPointCloud( const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud);
	void addPointCloud( const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud, const std::string& id);
	void addPointCloud( const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud);
	void addLine(const pcl::PointXYZ& p1, const pcl::PointXYZ& p2, const double r, const double g, const double b, const std::string& id);
	bool wasStopped();
	void saveScreenshot(const std::string& fileName);
    void getScreenshotImage(cv::Mat& image);
    void setShapeRenderingProperties(const pcl::visualization::RenderingProperties& property, const double value, const std::string& id );
	void drawWorld();
	void removePrism(const std::string& type, const int prismID);







private:
	const std::string title;
	bool exitSinal;
	boost::interprocess::interprocess_semaphore sem;
	boost::mutex new_data_mutex;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	boost::shared_ptr<boost::thread> viewerThread;
	std::vector<std::string> objectID;
	std::vector<std::string> cloudsID;

	int defaultLineWidth;
	double scale;
	int MAXWORLD;
	std::vector<std::vector<float> > worldLines;

	void refresh_thread();
	int addWorldFromFile(const std::string& worldfile);
	int load_line(FILE *myfile);
	bool signalSave;
	std::string name2save;
	bool autoUpdate;


};

} /* namespace jderobot */

#endif /* SRC_STABLE_LIBS_JDEROBOTVIEWER_JDEROBOTVIEWER_H_ */
