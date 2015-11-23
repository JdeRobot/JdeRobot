#include "quadrotor_config.h"

QuadrotorConfig::QuadrotorConfig()
{
    //ctor
}

QuadrotorConfig::~QuadrotorConfig()
{
    //dtor
}

std::ostream& operator<<(std::ostream &strm, const QuadrotorConfig &conf) {
  return strm << "Configuration: \n"
  "\tInterface camera: \n"
  "\t\tEndPoint:\t\t"<< conf.interface_camera_end_point <<"\n\t\tAdapter:\t\t"<<conf.interface_camera_adapter << "\n\t\tName:\t\t\t" << conf.interface_camera_name << "\n\t\tFramerateN:\t\t" << conf.interface_camera_framerateN << "\n\t\tFramerateD:\t\t" << conf.interface_camera_framerateD << "\n\t\tFormat:\t\t\t" << conf.interface_camera_format << "\n\t\tImage width:\t\t" << conf.interface_camera_image_width << "\n\t\tImage heigth:\t\t" << conf.interface_camera_image_height << 
  "\n\tInterface control:\n"
  "\t\tEndPoint:\t\t"<< conf.interface_control_end_point <<"\n\t\tAdapter:\t\t"<<conf.interface_control_adapter <<
  "\n\t\tName:\t\t\t"<< conf.interface_control_name <<
  "\n\tInterface remote config:\n"
  "\t\tEndPoint:\t\t"<< conf.interface_remote_config_end_point <<"\n\t\tAdapter:\t\t"<<conf.interface_remote_config_adapter <<
  "\n\t\tName:\t\t\t"<< conf.interface_remote_config_name <<
  "\n\tQuadrotor config:\n"
  "\t\tdefault_camera:\t\t\t"<< conf.quadrotor_default_camera <<"\n\t\toutdoor:\t\t\t"<< conf.quadrotor_outdoor << "\n\t\tmax_bitrate:\t\t\t"<< conf.quadrotor_max_bitrate << "\n\t\tnavdata_demo:\t\t\t"<< conf.quadrotor_navdata_demo<<
  "\n\t\tflight_without_shell:\t\t"<<conf.quadrotor_flight_without_shell << "\n\t\taltitude_max:\t\t\t"<<conf.quadrotor_altitude_max << "\n\t\taltitude_min:\t\t\t"<< conf.quadrotor_altitude_min<<
  "\n\t\teuler_angle_max:\t\t"<<conf.quadrotor_euler_angle_max << "\n\t\tcontrol_vz_max:\t\t\t"<< conf.quadrotor_control_vz_max << "\n\t\tcontrol_yaw:\t\t\t"<<conf.quadrotor_control_yaw;
}

