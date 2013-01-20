#include <trafficmonitor_config.h>
#include <map>

using namespace std;

namespace trafficmonitor {

const string prefix                            = "TrafficMonitor.";
const string MAX_VEHICLES                      = prefix+"MaxVehicles";
const string CONTROL_PLAY                      = prefix+"Control.play";
const string CONTROL_CLASIFY                   = prefix+"Control.classify";
const string CONTROL_USE_STATIC_ROAD           = prefix+"Control.useStaticRoad";
const string CONTROL_SWITCH_DETECTION_ZONE     = prefix+"Control.switchDetectionZone";
const string CONTROL_DETECTION_ZONE_PERCENTAGE = prefix+"Control.detectionZonePercentage";
const string CONTROL_KLT_TRACKING              = prefix+"Control.kltTrackingActive";
const string CONTROL_CAMERA_AUTO_CALIB         = prefix+"Control.cameraAutoCalibration";
const string CONTROL_GUI                       = prefix+"Control.gui";
const string CONTROL_BGALB                     = prefix+"Control.bgalg";
const string CONTROL_CAMERA_CALIB_FILE         = prefix+"Control.CameraCalibFile";
const string ROAD_LENGTH                       = prefix+"Road.Length";
const string ROAD_WIDTH                        = prefix+"Road.Width";
const string ROAD_A_X                          = prefix+"Road.A.x";
const string ROAD_A_Y                          = prefix+"Road.A.y";
const string ROAD_B_X                          = prefix+"Road.B.x";
const string ROAD_B_Y                          = prefix+"Road.B.y";
const string ROAD_C_X                          = prefix+"Road.C.x";
const string ROAD_C_Y                          = prefix+"Road.C.y";
const string ROAD_D_X                          = prefix+"Road.D.x";
const string ROAD_D_Y                          = prefix+"Road.D.y";

int TrafficMonitorAlgorithmConfig::str2int(const std::string str_value) const
{
   stringstream ss;
   int value;
   ss.clear();
   ss.str(str_value);
   ss >> value;

   return value;
}

double TrafficMonitorAlgorithmConfig::str2double(const std::string str_value) const
{
   stringstream ss;
   double value;
   ss.clear();
   ss.str(str_value);
   ss >> value;

   return value;
}

/**
 *
 */
void TrafficMonitorAlgorithmConfig::readProperties(Ice::PropertyDict props_dict)
{
   Tpoint2D p;

   max_vehicles = str2double(props_dict[MAX_VEHICLES]);
   road_length = str2double(props_dict[ROAD_LENGTH]);
   road_width = str2double(props_dict[ROAD_WIDTH]);
      
   play                  = str2int(props_dict[CONTROL_PLAY]);
   classify              = str2int(props_dict[CONTROL_CLASIFY]);
   useStaticRoad         = str2int(props_dict[CONTROL_USE_STATIC_ROAD]);
   switchDetectionZone   = str2int(props_dict[CONTROL_SWITCH_DETECTION_ZONE]);
   kltTrackingActive     = str2int(props_dict[CONTROL_KLT_TRACKING]);
   cameraAutoCalibration = str2int(props_dict[CONTROL_CAMERA_AUTO_CALIB]);
   
   gui                   = props_dict[CONTROL_GUI];
   bgalg                 = props_dict[CONTROL_BGALB];
   camera_conf_file      = props_dict[CONTROL_CAMERA_CALIB_FILE];

   p.x = str2int(props_dict[ROAD_A_X]);
   p.y = str2int(props_dict[ROAD_A_Y]);
   roadPoints.push_back(p);
   p.x = str2int(props_dict[ROAD_B_X]);
   p.y = str2int(props_dict[ROAD_B_Y]);
   roadPoints.push_back(p);
   p.x = str2int(props_dict[ROAD_C_X]);
   p.y = str2int(props_dict[ROAD_C_Y]);
   roadPoints.push_back(p);
   p.x = str2int(props_dict[ROAD_D_X]);
   p.y = str2int(props_dict[ROAD_D_Y]);
   roadPoints.push_back(p);
}

/**
 *
 */
void TrafficMonitorAlgorithmConfig::readProperties(const Ice::PropertiesPtr props)
{
   stringstream ss;
   Tpoint2D p;

   ss.clear();
   ss.str(props->getProperty(MAX_VEHICLES));
   ss >> max_vehicles;
      
   ss.clear();
   ss.str(props->getProperty(ROAD_LENGTH));
   ss >> road_length;
      
   ss.clear();
   ss.str(props->getProperty(ROAD_WIDTH));
   ss >> road_width;

   ss.clear();
   ss.str(props->getProperty(CONTROL_DETECTION_ZONE_PERCENTAGE));
   ss >> detectionZonePercentage;
   
   play                  = props->getPropertyAsInt(CONTROL_PLAY);
   classify              = props->getPropertyAsInt(CONTROL_CLASIFY);
   useStaticRoad         = props->getPropertyAsInt(CONTROL_USE_STATIC_ROAD);
   switchDetectionZone   = props->getPropertyAsInt(CONTROL_SWITCH_DETECTION_ZONE);
   kltTrackingActive     = props->getPropertyAsInt(CONTROL_KLT_TRACKING);
   cameraAutoCalibration = props->getPropertyAsInt(CONTROL_CAMERA_AUTO_CALIB);
   gui                   = props->getProperty(CONTROL_GUI);
   bgalg                 = props->getProperty(CONTROL_BGALB);
   camera_conf_file      = props->getProperty(CONTROL_CAMERA_CALIB_FILE);

   p.x = props->getPropertyAsInt(ROAD_A_X);
   p.y = props->getPropertyAsInt(ROAD_A_Y);
   roadPoints.push_back(p);
   p.x = props->getPropertyAsInt(ROAD_B_X);
   p.y = props->getPropertyAsInt(ROAD_B_Y);
   roadPoints.push_back(p);
   p.x = props->getPropertyAsInt(ROAD_C_X);
   p.y = props->getPropertyAsInt(ROAD_C_Y);
   roadPoints.push_back(p);
   p.x = props->getPropertyAsInt(ROAD_D_X);
   p.y = props->getPropertyAsInt(ROAD_D_Y);
   roadPoints.push_back(p);
}

/**
 *
 */
string TrafficMonitorAlgorithmConfig::dump() const
{
   stringstream ss;
   ss.clear();
   
   ss << MAX_VEHICLES                        << "=" << max_vehicles            << endl
      << ROAD_LENGTH                         << "=" << road_length             << endl
      << ROAD_WIDTH                          << "=" << road_width              << endl
      << ROAD_A_X                            << "=" << roadPoints[3].x         << endl
      << ROAD_A_Y                            << "=" << roadPoints[3].y         << endl
      << ROAD_B_X                            << "=" << roadPoints[0].x         << endl
      << ROAD_B_Y                            << "=" << roadPoints[0].y         << endl
      << ROAD_C_X                            << "=" << roadPoints[1].x         << endl
      << ROAD_C_Y                            << "=" << roadPoints[1].y         << endl
      << ROAD_D_X                            << "=" << roadPoints[2].x         << endl
      << ROAD_D_Y                            << "=" << roadPoints[2].y         << endl
      << CONTROL_PLAY                        << "=" << play                    << endl
      << CONTROL_CLASIFY                     << "=" << classify                << endl
      << CONTROL_USE_STATIC_ROAD             << "=" << useStaticRoad           << endl
      << CONTROL_SWITCH_DETECTION_ZONE       << "=" << switchDetectionZone     << endl
      << CONTROL_DETECTION_ZONE_PERCENTAGE   << "=" << detectionZonePercentage << endl
      << CONTROL_KLT_TRACKING                << "=" << kltTrackingActive       << endl
      << CONTROL_CAMERA_AUTO_CALIB           << "=" << cameraAutoCalibration   << endl
      << CONTROL_GUI                         << "=" << gui                     << endl
      << CONTROL_BGALB                       << "=" << bgalg                   << endl
      << CONTROL_CAMERA_CALIB_FILE           << "=" << camera_conf_file        << endl;

   return ss.str();
}

/**
 *
 */
std::string TrafficMonitorAlgorithmConfig::int2str(int value) const
{
   stringstream ss;
   ss.clear();
   ss << value;
   return ss.str();
}

/**
 *
 */
void TrafficMonitorAlgorithmConfig::writeProperties()
{
   Ice::PropertyDict props;

   props[ROAD_A_X] = int2str(roadPoints[3].x);
   props[ROAD_A_Y] = int2str(roadPoints[3].y);
   props[ROAD_B_X] = int2str(roadPoints[0].x);
   props[ROAD_B_Y] = int2str(roadPoints[0].y);
   props[ROAD_C_X] = int2str(roadPoints[1].x);
   props[ROAD_C_Y] = int2str(roadPoints[1].y);
   props[ROAD_D_X] = int2str(roadPoints[2].x);
   props[ROAD_D_Y] = int2str(roadPoints[2].y);
      
   props[CONTROL_PLAY]                      = int2str(play);
   props[CONTROL_CLASIFY]                   = int2str(classify);
   props[CONTROL_USE_STATIC_ROAD]           = int2str(useStaticRoad);
   props[CONTROL_SWITCH_DETECTION_ZONE]     = int2str(switchDetectionZone);
   props[CONTROL_DETECTION_ZONE_PERCENTAGE] = int2str(detectionZonePercentage );
   props[CONTROL_KLT_TRACKING]              = int2str(kltTrackingActive);
   props[CONTROL_CAMERA_AUTO_CALIB]         = int2str(cameraAutoCalibration);
   props[CONTROL_GUI]                       = gui;
   props[CONTROL_BGALB]                     = bgalg;
   props[CONTROL_CAMERA_CALIB_FILE]         = camera_conf_file;
   
   admin->setProperties(props);
}

}
