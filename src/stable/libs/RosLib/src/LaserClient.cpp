#include "LaserClient.h"

LaserClient::LaserClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosPublisher <RosIceMessage::Laser> (nodeName,1000);
    //addRosSubscriber(nodeName,1000,&LaserClient::rosCallback_Encoders,this);
    addRosSubscriber(nodeName,1000,&LaserClient::rosCallback_Pose3D,this);
    addRosImagePublisher(nodeName + "_image",1000);

    Laser3DRosNode = new ros::NodeHandle;

    Laser3DPublisher = new ros::Publisher(Laser3DRosNode->advertise<geometry_msgs::PolygonStamped>(nodeName + "_3D", 500));

    MarkerPublisher = new ros::Publisher(Laser3DRosNode->advertise<visualization_msgs::MarkerArray>(nodeName + "_marker", 500));



    PI = 4.0 * atan(1);
    DEGTORAD = PI / 180.0;

    laser_coord[0] = 19.;
    laser_coord[1] = 0.;
    laser_coord[2] = 0.;
    laser_coord[3] = cos(laser_coord[2] * DEGTORAD);
    laser_coord[4] = sin(laser_coord[2] * DEGTORAD);


    visualization_msgs::Marker marker;

    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    marker.ns = "basic_shapes";

    /// Marker.ID = 0 represents the laser scanner

    marker.id = 0;

    marker.type = visualization_msgs::Marker::MESH_RESOURCE;

    marker.mesh_resource = "package://RosIceGazebo/meshes/hokuyo.dae";
    marker.pose = getPose(2.0, 0.0, 3.2, 0, 0, 0);

    marker.scale.x = 10.0;
    marker.scale.y = 10.0;
    marker.scale.z = 10.0;

    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker_array.markers.push_back(marker);

    /// Marker.ID = 1 represents the robot body

    marker.id = 1;

    marker.type = visualization_msgs::Marker::MESH_RESOURCE;

    marker.mesh_resource = "package://RosIceGazebo/meshes/chassis.dae";

    marker.action = visualization_msgs::Marker::ADD;

    marker.scale.x = 10.0;
    marker.scale.y = 10.0;
    marker.scale.z = 10.0;

    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.pose = getPose(0.0, 0, 2.4, 0, 0, 0);


    marker_array.markers.push_back(marker);

    /// Marker.ID = 2 represents the back-wheel

    marker.id = 2;

    marker.type = visualization_msgs::Marker::SPHERE;


    marker.pose = getPose(-0.200, 0.0, -0.12, 0.0, 0.0, 0.0);

    marker.scale.x = 0.04;
    marker.scale.y = 0.04;
    marker.scale.z = 0.04;

    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;


    //marker_array.markers.push_back(marker);

    /// Marker.ID = 3 represents the left wheel

    marker.id = 3;

    marker.type = visualization_msgs::Marker::CYLINDER;

    marker.pose = getPose(0.1, -0.17, 0.0, 0, 1.5707, 1.5707);

    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.05;

    //marker_array.markers.push_back(marker);

    /// Marker.ID = 4 represents the right wheel

    marker.id = 4;

    marker.type = visualization_msgs::Marker::CYLINDER;

    marker.pose = getPose(0.1, 0.17, 0.0, 0, 1.5707, 1.5707);

    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.05;

    //marker_array.markers.push_back(marker);

    /// Marker.ID = 4 represents the camera sensor

    marker.id = 5;

    marker.type = visualization_msgs::Marker::MESH_RESOURCE;

    marker.mesh_resource = "package://RosIceGazebo/meshes/sonyvid30_mid.dae";
    marker.pose = getPose(0.5, 0.1, 0.3, -1.57, 0, 0);

    marker.scale.x = 10;
    marker.scale.y = 10;
    marker.scale.z = 10;

    //marker_array.markers.push_back(marker);



}

LaserClient::~LaserClient()
{

    delete Laser3DRosNode;
    delete Laser3DPublisher;
    delete MarkerPublisher;
}

void LaserClient::publishROS(RosIceMessage::EncodersData encodersMessage)
{

    jderobot::LaserDataPtr laserData= this->Proxy->getLaserData();
    RosIceMessage::Laser rosLaserData;

    rosLaserData.numLaser = laserData->numLaser;
    rosLaserData.distanceData = laserData->distanceData;

    rosLaserData.positionEncoders = encodersMessage;

    rosPublish(rosLaserData);

}

void LaserClient::publishROS(geometry_msgs::Pose pose3DMessage)
{
    jderobot::LaserDataPtr laserData= this->Proxy->getLaserData();
    RosIceMessage::Laser rosLaserData;

    rosLaserData.numLaser = laserData->numLaser;
    rosLaserData.distanceData = laserData->distanceData;

    rosLaserData.positionPose3D = pose3DMessage;

    double magnitude,w,x,y,z,squ,sqx,sqy,sqz;

    magnitude = sqrt(rosLaserData.positionPose3D.orientation.w * rosLaserData.positionPose3D.orientation.w + rosLaserData.positionPose3D.orientation.x * rosLaserData.positionPose3D.orientation.x + rosLaserData.positionPose3D.orientation.y * rosLaserData.positionPose3D.orientation.y + rosLaserData.positionPose3D.orientation.z * rosLaserData.positionPose3D.orientation.z);

    w = rosLaserData.positionPose3D.orientation.w / magnitude;
    x = rosLaserData.positionPose3D.orientation.x / magnitude;
    y = rosLaserData.positionPose3D.orientation.y / magnitude;
    z = rosLaserData.positionPose3D.orientation.z / magnitude;

    squ = w * w;
    sqx = x * x;
    sqy = y * y;
    sqz = z * z;

    double angle;

    angle = atan2( 2 * (x * y + w * z), squ + sqx - sqy - sqz) * 180.0 / PI;

    if(angle < 0)
    {
        angle += 360.0;
    }

    rosLaserData.robottheta = angle;


    rosPublish(rosLaserData);

}




void LaserClient::rosCallback_Pose3D(RosIceMessage::Laser laserMessage)
{
    cv::Mat laserImage(180,360, CV_8UC3, cv::Scalar(0,0,0));

    geometry_msgs::PolygonStamped laser_world;
    geometry_msgs::Point32 point_marker;


    float X,Y;

    laser_world.header.frame_id = "map";
    laser_world.header.stamp = ros::Time();
    laser_world.header.seq = 0;

    tf::Quaternion aux,point,rotation(laserMessage.positionPose3D.orientation.x,laserMessage.positionPose3D.orientation.y,laserMessage.positionPose3D.orientation.z,laserMessage.positionPose3D.orientation.w);
    rotation.normalize();

    point.setW(0.0);
    point.setZ(3.2);
    point_marker.z = 3.2;


    for( unsigned int i = 0 ; i < marker_array.markers.size(); ++i)
    {
        marker_array.markers[i].pose.orientation = laserMessage.positionPose3D.orientation;
    }




    cv::line(laserImage,cv::Point(180,165),cv::Point(180,180),cv::Scalar(255,0,0));


    for (int i = laserMessage.numLaser - 1; i > 0; --i)
    {
        cv::line(laserImage, cv::Point(180 + ((laserMessage.distanceData[i] / 45)*(cos((i) * DEGTORAD))), 180 - ((laserMessage.distanceData[i] / 45)*(sin((i) * DEGTORAD)))), cv::Point(180 + ((laserMessage.distanceData[i + 1] / 45)*(cos((i + 1) * DEGTORAD))), 180 - ((laserMessage.distanceData[i + 1] / 45)*(sin((i + 1) * DEGTORAD)))),cv::Scalar(255,255,255));

        X = laserMessage.distanceData[i] * cos(((float) i - 90.) * DEGTORAD);
        Y = laserMessage.distanceData[i] * sin(((float) i - 90.) * DEGTORAD );

        point.setX((laser_coord[0]*10. + X * laser_coord[3] - Y * laser_coord[4] + laserMessage.positionPose3D.position.x) / 100.0);
        point.setY((laser_coord[1] + Y * laser_coord[3] + X * laser_coord[4] + laserMessage.positionPose3D.position.y) / 100.0);

        aux = rotation * point * rotation.inverse();

        point_marker.x = aux.getX();
        point_marker.y = aux.getY();

        laser_world.polygon.points.push_back(point_marker);
    }

    point.setX((laser_coord[0] * 10.0 + laserMessage.positionPose3D.position.x)/ 100);
    point.setY((laser_coord[1] + laserMessage.positionPose3D.position.y)/ 100);

    aux = rotation * point * rotation.inverse();

    point_marker.x = aux.getX();
    point_marker.y = aux.getY();


    laser_world.polygon.points.push_back(point_marker);

    // The way the markers are moved, is that the position and orientation for the laser marker (ID = 0 ) is calculated such that it corresponds to the first
    // point of the Laser Polygon, afterwards the rest of the markers are moved such that that the body of the robot remains whole

    positionMarkers(point_marker.x - marker_array.markers[0].pose.position.x, point_marker.y - marker_array.markers[0].pose.position.y,rotation);


    sensor_msgs::ImagePtr image_message = cv_bridge::CvImage(std_msgs::Header(), "rgb8", laserImage).toImageMsg();
    rosImagePublish(image_message);

    Laser3DPublisher->publish(laser_world);

    MarkerPublisher->publish(marker_array);


}



void LaserClient::rosCallback_Encoders(RosIceMessage::Laser laserMessage)
{
    cv::Mat laserImage(180,360, CV_8UC3, cv::Scalar(0,0,0));

    geometry_msgs::PolygonStamped laser_world;
    geometry_msgs::Point32 point;


    float X,Y,costheta,sintheta,aux_x,aux_y;

    point.z = 3.2;

    costheta = cos(laserMessage.positionEncoders.robottheta * DEGTORAD);
    sintheta = sin(laserMessage.positionEncoders.robottheta * DEGTORAD);

    laser_world.header.frame_id = "map";
    laser_world.header.stamp = ros::Time();
    laser_world.header.seq = 0;

    tf::Quaternion rotation;

    rotation.setEuler(0,0,laserMessage.positionEncoders.robottheta * DEGTORAD);

    for( unsigned int i = 0 ; i < marker_array.markers.size(); ++i)
    {
        marker_array.markers[i].pose.orientation.x = rotation.getX();
        marker_array.markers[i].pose.orientation.y = rotation.getY();
        marker_array.markers[i].pose.orientation.z = rotation.getZ();
        marker_array.markers[i].pose.orientation.w = rotation.getW();
    }



    cv::line(laserImage,cv::Point(180,165),cv::Point(180,180),cv::Scalar(255,0,0));


    for (int i = laserMessage.numLaser - 1; i > 0; --i)
    {
        cv::line(laserImage, cv::Point(180 + ((laserMessage.distanceData[i] / 45)*(cos((i) * DEGTORAD))), 180 - ((laserMessage.distanceData[i] / 45)*(sin((i) * DEGTORAD)))), cv::Point(180 + ((laserMessage.distanceData[i + 1] / 45)*(cos((i + 1) * DEGTORAD))), 180 - ((laserMessage.distanceData[i + 1] / 45)*(sin((i + 1) * DEGTORAD)))),cv::Scalar(255,255,255));

        X = laserMessage.distanceData[i] * cos(((float) i - 90.) * DEGTORAD);
        Y = laserMessage.distanceData[i] * sin(((float) i - 90.) * DEGTORAD );

        point.x = (laser_coord[0]*10. + X * laser_coord[3] - Y * laser_coord[4] + laserMessage.positionEncoders.robotx) / 100;
        point.y = (laser_coord[1] + Y * laser_coord[3] + X * laser_coord[4] + laserMessage.positionEncoders.roboty) / 100;

        aux_x = costheta * point.x - sintheta * point.y;
        aux_y = sintheta * point.x + costheta * point.y;

        point.x = aux_x;
        point.y = aux_y;

        //ROS_INFO("Point %f %f \n", point.x, point.y);

        laser_world.polygon.points.push_back(point);
    }

    point.x = (laser_coord[0] * 10.0 + laserMessage.positionEncoders.robotx)/ 100;
    point.y = (laser_coord[1] + laserMessage.positionEncoders.roboty)/ 100;

    aux_x = costheta * point.x - sintheta * point.y;
    aux_y = sintheta * point.x + costheta * point.y;

    point.x = aux_x;
    point.y = aux_y;

    laser_world.polygon.points.push_back(point);


    // The way the markers are moved, is that the position and orientation for the laser marker (ID = 0 ) is calculated such that it corresponds to the first
    // point of the Laser Polygon, afterwards the rest of the markers are moved such that that the body of the robot remains whole

    positionMarkers(point.x - marker_array.markers[0].pose.position.x, point.y - marker_array.markers[0].pose.position.y,costheta,sintheta);




    sensor_msgs::ImagePtr image_message = cv_bridge::CvImage(std_msgs::Header(), "rgb8", laserImage).toImageMsg();
    rosImagePublish(image_message);

    Laser3DPublisher->publish(laser_world);

    MarkerPublisher->publish(marker_array);


}



void LaserClient::positionMarkers(float x, float y, float costheta, float sintheta)
{

    float aux_x,aux_y;

    aux_x = costheta * 0.0 - sintheta * 7.5;
    aux_y = sintheta * 0.0 + costheta * 7.5;

    marker_array.markers[1].pose.position.x = aux_x;
    marker_array.markers[1].pose.position.y = aux_y;


    aux_x = costheta * 0.0 + sintheta * 7.5;
    aux_y = sintheta * 0.0 - costheta * 7.5;

    marker_array.markers[2].pose.position.x = aux_x;
    marker_array.markers[2].pose.position.y = aux_y;

    marker_array.markers[0].pose.position.x += x;
    marker_array.markers[0].pose.position.y += y;



    for(int i = 1; i < marker_array.markers.size(); ++i)
    {
        marker_array.markers[i].pose.position.x += marker_array.markers[0].pose.position.x;
        marker_array.markers[i].pose.position.y += marker_array.markers[0].pose.position.y;
    }




}

void LaserClient::positionMarkers(float x, float y, tf::Quaternion rotation)
{
    tf::Quaternion aux,point1(0.0, 0.0, 2.4,0.0),point2(0.0,-7.5,3.2,0.0);

    aux = rotation * point1 * rotation.inverse();


    marker_array.markers[1].pose.position.x = aux.getX();
    marker_array.markers[1].pose.position.y = aux.getY();

    aux = rotation * point2 * rotation.inverse();

    marker_array.markers[2].pose.position.x = aux.getX();
    marker_array.markers[2].pose.position.y = aux.getY();

    marker_array.markers[0].pose.position.x += x;
    marker_array.markers[0].pose.position.y += y;



    for(int i = 1; i < marker_array.markers.size(); ++i)
    {
        marker_array.markers[i].pose.position.x += marker_array.markers[0].pose.position.x;
        marker_array.markers[i].pose.position.y += marker_array.markers[0].pose.position.y;
    }


}

geometry_msgs::Pose LaserClient::getPose(double x, double y, double z, double roll, double pitch, double yaw)
{
    geometry_msgs::Pose pose;

    pose.position.x = x;
    pose.position.y = y;
    pose.position.z = z;

    tf::Quaternion tf_q;

    tf_q.setEuler(yaw,pitch,roll);

    pose.orientation.w = tf_q.getW();
    pose.orientation.x = tf_q.getX();
    pose.orientation.y = tf_q.getY();
    pose.orientation.z = tf_q.getZ();


    return pose;
}
