#ifndef LASERCLIENT_H
#define LASERCLIENT_H

#include "Ros_Ice.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <visualization_msgs/MarkerArray.h>
#include <geometry_msgs/PolygonStamped.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <laser.h>
#include <RosIceMessage/Laser.h>
#include <RosIceMessage/EncodersData.h>
//#include "EncodersData.h"
//#include "Laser.h"
#include <geometry_msgs/Pose.h>
//#include <geometry_msgs/Vector3.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <tf/tf.h>

/**
 * @brief The LaserClient class is used to plot the 3Dworld Laser-Window in rviz. Unlike the other "Clients", it contains other members in order to create the 3D representation
 * of the pioneer.
 */


class LaserClient : public Ros_Ice < jderobot::LaserPrx >
{

public:

    /**
     * @brief This constructor initializes the ROS topic using the nodeName parameter.
     * Besides the ROS Publishers and ROS Subscribers it also creates the visual Markers of the pioneer robot in order to view it in rviz
     */


    LaserClient(int argc, char **argv, std::string nodeName);
    ~LaserClient();

    /**
     * @brief Method to get the Laser-Ice data from the JdeRobot simulator and publish it to the ROS topic
     * NOTE: it also takes the position of the pioneer as a parameter in order to visualize it in rviz. This method makes use of the Encoders class
     */

    void publishROS(RosIceMessage::EncodersData encodersMessage);


    /**
     * @brief Method to get the Laser-Ice data from the JdeRobot simulator and publish it to the ROS topic
     * NOTE: it also takes the position of the pioneer as a parameter in order to visualize it in rviz. This method makes use of the Encoders class
     */

    void publishROS(geometry_msgs::Pose pose3DMessage);

    /**
     * @brief This callback method computes the data needed by rviz in order to plot the pioneer robot. This method makes use of the Encoders class
     */

    void rosCallback_Encoders(RosIceMessage::Laser laserMessage);

    /**
     * @brief This callback method computes the data needed by rviz in order to plot the pioneer robot. This method makes use of the built-in geometry_msgs/Pose class
     */

    void rosCallback_Pose3D(RosIceMessage::Laser laserMessage);

private:

    /**
     * @brief This method is used modify the position and orientation of every Marker of the pioneer robot
     */

    void positionMarkers(float x, float y, float costheta, float sintheta);

    /**
     * @brief This method is used modify the position and orientation of every Marker of the pioneer robot
     */

    void positionMarkers(float x, float y, tf::Quaternion rotation);

    /**
     * @brief This method is used to calculate the geometry_msgs/Pose given the position and the roll,pitch,yaw angles

     */
    geometry_msgs::Pose getPose(double x, double y, double z, double roll, double pitch, double yaw);



    float laser_coord[5];
    double PI;
    double DEGTORAD;

    ros::NodeHandle* Laser3DRosNode;


    /**
    * @brief This publisher is used to plot the laser data as a Polygon
    */

    ros::Publisher* Laser3DPublisher;


    /**
    * @brief This publisher is used to plot the body of the pioneer robot
    */

    ros::Publisher* MarkerPublisher;

    float robotx;
    float roboty;
    float robottheta;

    /**
     * @brief This private member contains the the markers used to plot the pioneer robot
     */

    visualization_msgs::MarkerArray marker_array;





};

#endif // LASERCLIENT_H
