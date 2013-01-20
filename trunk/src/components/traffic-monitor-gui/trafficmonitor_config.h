#ifndef _TRAFFICMONITOR_CONFIG_
#define _TRAFFICMONITOR_CONFIG_

#include <Ice/Ice.h>

namespace trafficmonitor{

struct Tpoint2D
{
   Tpoint2D(int a, int b):y(a),x(b){};
   Tpoint2D():y(0),x(0){};
   int x;
   int y;
};


/**
 *
 */
class TrafficMonitorAlgorithmConfig{
public:

   static const std::string FILE_VIEW;
   static const std::string GTK_VIEW;
   
   TrafficMonitorAlgorithmConfig(const Ice::PropertiesAdminPrx& props_admin):admin(props_admin){};

   std::string dump() const;
   void readProperties(const Ice::PropertiesPtr props);
   void readProperties(const Ice::PropertyDict props_dict);
   void writeProperties();

   const std::string get_camera_conf() const {return camera_conf_file;};
   unsigned int get_max_vehicles() const {return max_vehicles;};
   double get_road_length() const{return road_length;};
   double get_road_width() const{return road_width;};
   double get_detection_zone_percentage() const{return detectionZonePercentage;};
   const std::vector<Tpoint2D>& get_road_points() const{return roadPoints;};

   bool play;
   bool classify;
   bool useStaticRoad;
   bool switchDetectionZone;
   bool kltTrackingActive;
   bool cameraAutoCalibration;
   std::string gui;
   std::string bgalg;
   double detectionZonePercentage;
   
private:

   int str2int(std::string str_value) const;
   std::string int2str(int value) const;
   double str2double(std::string str_value) const;
   
   const Ice::PropertiesAdminPrx& admin;
   std::string camera_conf_file;
   unsigned int max_vehicles;
   double road_width;
   double road_length;
   std::vector<Tpoint2D> roadPoints;

};
}//namspace

#endif
