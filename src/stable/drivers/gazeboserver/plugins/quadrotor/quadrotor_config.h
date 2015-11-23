#ifndef ARDRONECONFIG_H
#define ARDRONECONFIG_H

#include <iostream>
class QuadrotorConfig
{
    public:
        QuadrotorConfig();
        virtual ~QuadrotorConfig();
	//Interface camera
        std::string get_interface_camera_end_point() { return interface_camera_end_point; }
        void set_interface_camera_end_point(std::string val) { interface_camera_end_point = val; }
        std::string get_interface_camera_adapter() { return interface_camera_adapter; }
        void set_interface_camera_adapter(std::string val) { interface_camera_adapter = val; }
	std::string get_interface_camera_name() { return interface_camera_name; }
        void set_interface_camera_name(std::string val) { interface_camera_name = val; }
        int get_interface_camera_framerateN() { return interface_camera_framerateN; }
        void set_interface_camera_framerateN(int val) { interface_camera_framerateN = val; }
        int get_interface_camera_framerateD() { return interface_camera_framerateD; }
        void set_interface_camera_framerateD(int val) { interface_camera_framerateD = val; }
        std::string get_interface_camera_format() { return interface_camera_format; }
        void set_interface_camera_format(std::string val) { interface_camera_format = val; }
        int get_interface_camera_image_width() { return interface_camera_image_width; }
        void set_interface_camera_image_width(int val) { interface_camera_image_width = val; }
        int get_interface_camera_image_height() { return interface_camera_image_height; }
        void set_interface_camera_image_height(int val) { interface_camera_image_height = val; }
	//Interface control
        std::string get_interface_control_end_point() { return interface_control_end_point; }
        void set_interface_control_end_point(std::string val) { interface_control_end_point = val; }
        std::string get_interface_control_adapter() { return interface_control_adapter; }
        void set_interface_control_adapter(std::string val) { interface_control_adapter = val; }
        std::string get_interface_control_name() { return interface_control_name; }
        void set_interface_control_name(std::string val) { interface_control_name = val; }
	//Interface remote config
        std::string get_interface_remote_config_end_point() { return interface_remote_config_end_point; }
        void set_interface_remote_config_end_point(std::string val) { interface_remote_config_end_point = val; }
        std::string get_interface_remote_config_adapter() { return interface_remote_config_adapter; }
        void set_interface_remote_config_adapter(std::string val) { interface_remote_config_adapter = val; }
        std::string get_interface_remote_config_name() { return interface_remote_config_name; }
        void set_interface_remote_config_name(std::string val) { interface_remote_config_name = val; }
	//Quadrotor config
        int get_quadrotor_default_camera() { return quadrotor_default_camera; }
        void set_quadrotor_default_camera(int val) { quadrotor_default_camera = val; }
        bool get_quadrotor_outdoor() { return quadrotor_outdoor; }
        void set_quadrotor_outdoor(bool val) { quadrotor_outdoor = val; }
        int get_quadrotor_max_bitrate() { return quadrotor_max_bitrate; }
        void set_quadrotor_max_bitrate(int val) { quadrotor_max_bitrate = val; }
        int get_quadrotor_bitrate() { return quadrotor_bitrate; }
        void set_quadrotor_bitrate(int val) { quadrotor_bitrate = val; }
        int get_quadrotor_navdata_demo() { return quadrotor_navdata_demo; }
        void set_quadrotor_navdata_demo(int val) { quadrotor_navdata_demo = val; }
        bool get_quadrotor_flight_without_shell() { return quadrotor_flight_without_shell; }
        void set_quadrotor_flight_without_shell(bool val) { quadrotor_flight_without_shell = val; }
        int get_quadrotor_altitude_max() { return quadrotor_altitude_max; }
        void set_quadrotor_altitude_max(int val) { quadrotor_altitude_max = val; }
        int get_quadrotor_altitude_min() { return quadrotor_altitude_min; }
        void set_quadrotor_altitude_min(int val) { quadrotor_altitude_min = val; }
        float get_quadrotor_euler_angle_max() { return quadrotor_euler_angle_max; }
        void set_quadrotor_euler_angle_max(float val) { quadrotor_euler_angle_max = val; }
        int get_quadrotor_control_vz_max() { return quadrotor_control_vz_max; }
        void set_quadrotor_control_vz_max(int val) { quadrotor_control_vz_max = val; }
        float get_quadrotor_control_yaw() { return quadrotor_control_yaw; }
        void set_quadrotor_control_yaw(float val) { quadrotor_control_yaw = val; }
        //Video record
        bool get_video_record(){ return video_record;}
        void set_video_record(bool val){ video_record=val;}
        std::string get_video_path(){ return video_path;} 
        void set_video_path(std::string val){ video_path=val;}
        int get_video_fps(){ return video_fps;}
        void set_video_fps(int val){ video_fps=val;}       
    protected:
    private:
        friend std::ostream& operator<<(std::ostream&, const QuadrotorConfig&);
        std::string interface_camera_end_point;
        std::string interface_camera_adapter;
        std::string interface_camera_name;
        int interface_camera_framerateN;
        int interface_camera_framerateD;
        std::string interface_camera_format;
        int interface_camera_image_width;
        int interface_camera_image_height;

        std::string interface_control_end_point;
        std::string interface_control_adapter;
        std::string interface_control_name;

        std::string interface_remote_config_end_point;
        std::string interface_remote_config_adapter;
        std::string interface_remote_config_name;

        int quadrotor_default_camera;
        bool quadrotor_outdoor;
        int quadrotor_max_bitrate;
        int quadrotor_bitrate;
        int quadrotor_navdata_demo;
        bool quadrotor_flight_without_shell;
        int quadrotor_altitude_max;
        int quadrotor_altitude_min;
        float quadrotor_euler_angle_max;
        int quadrotor_control_vz_max;
        float quadrotor_control_yaw;
        
        bool video_record;
        std::string video_path;
        int video_fps;
};

#endif // ARDRONECONFIG_H

