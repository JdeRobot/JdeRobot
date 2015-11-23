/*
 *  Copyright (C) 1997-2015 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *  Authors : 
 *       Alberto Martín Florido <almartinflorido@gmail.com>	
 */

#include "navdatai.h"

namespace navdata
{
	NavdataI::NavdataI()
	{
		std::cout << "navdata start" << std::endl;
		data=new jderobot::NavdataData();
		last_navdata_id = -1;
	    // Fill constant parts of IMU Message
	    // If no rosparam is set then the default value of 0.0 will be assigned to all covariance values

	    for (int i = 0; i < 9; i++)
	    {

	    }
	    readCovParams();


	    // Caliberation
	    max_num_samples = 50;
	    /*do_caliberation = (ros::param::get("~do_imu_caliberation", do_caliberation)) ? do_caliberation : false;
	    if (do_caliberation) {
		resetCaliberation();
		ROS_WARN("Automatic IMU Caliberation is active.");
	    }*/	
	}
	NavdataI::~NavdataI()
	{
	
	}
	jderobot::NavdataDataPtr NavdataI::getNavdata(Ice::Current const & c)
	{
	
		vp_os_mutex_lock(&navdata_lock);
			navdata_unpacked_t navdata_raw = *shared_raw_navdata;
		vp_os_mutex_unlock(&navdata_lock);

		if ((do_caliberation) && (!caliberated))
		{
			/*acc_samples[0].push_back(navdata_raw.navdata_phys_measures.phys_accs[ACC_X]);
			acc_samples[1].push_back(navdata_raw.navdata_phys_measures.phys_accs[ACC_Y]);
			acc_samples[2].push_back(navdata_raw.navdata_phys_measures.phys_accs[ACC_Z]);
			gyro_samples[0].push_back(navdata_raw.navdata_phys_measures.phys_gyros[GYRO_X]);
			gyro_samples[1].push_back(navdata_raw.navdata_phys_measures.phys_gyros[GYRO_Y]);
			gyro_samples[2].push_back(navdata_raw.navdata_phys_measures.phys_gyros[GYRO_Z]);
			vel_samples[0].push_back(navdata_raw.navdata_demo.vx);
			vel_samples[1].push_back(navdata_raw.navdata_demo.vy);
			vel_samples[2].push_back(navdata_raw.navdata_demo.vz);
			if (acc_samples[0].size() == max_num_samples)
			{
			    for (int j = 0; j < 3; j++)
			    {
				acc_bias[j] = calcAverage(acc_samples[j]);
				gyro_bias[j] = calcAverage(gyro_samples[j]);
				vel_bias[j] = calcAverage(vel_samples[j]);
			    }

			    caliberated = true;
			}*/
		}
		if ((do_caliberation) && (caliberated))
		{
			/*for (int j = 0; j < 3; j++)
			{
			    if (j != 2) navdata_raw.navdata_phys_measures.phys_accs[j] -= acc_bias[j];
			    navdata_raw.navdata_phys_measures.phys_gyros[j] -= gyro_bias[j];
			}
			navdata_raw.navdata_demo.vx -= vel_bias[0];
			navdata_raw.navdata_demo.vy -= vel_bias[1];
			navdata_raw.navdata_demo.vz -= vel_bias[2];*/
		}
		
		if (IS_ARDRONE2){
			data->vehicle=1;
		}else{
			data->vehicle=0;
		}
		
		data->batteryPercent=navdata_raw.navdata_demo.vbat_flying_percentage;
		data->state = (navdata_raw.navdata_demo.ctrl_state >> 16);

		data->rotX = navdata_raw.navdata_demo.phi / 1000.0; // tilt left/right
		data->rotY = -navdata_raw.navdata_demo.theta / 1000.0; // tilt forward/backward
		data->rotZ = -navdata_raw.navdata_demo.psi / 1000.0; // orientation

		data->altd = navdata_raw.navdata_demo.altitude; // cm
		data->vx = navdata_raw.navdata_demo.vx; // mm/sec
		data->vy = -navdata_raw.navdata_demo.vy; // mm/sec
		data->vz = -navdata_raw.navdata_demo.vz; // mm/sec

		data->tm = (navdata_raw.navdata_time.time & 0x001FFFFF) + (navdata_raw.navdata_time.time >> 21)*1000000;
		data->ax = navdata_raw.navdata_phys_measures.phys_accs[ACC_X] / 1000.0; // g
		data->ay = -navdata_raw.navdata_phys_measures.phys_accs[ACC_Y] / 1000.0; // g
		data->az = -navdata_raw.navdata_phys_measures.phys_accs[ACC_Z] / 1000.0; // g

		// New stuff
		if (IS_ARDRONE2)
		{
			data->magX = (int32_t)navdata_raw.navdata_magneto.mx;
			data->magY = (int32_t)navdata_raw.navdata_magneto.my;
			data->magZ = (int32_t)navdata_raw.navdata_magneto.mz;

			data->pressure = navdata_raw.navdata_pressure_raw.Pression_meas; // typo in the SDK!
			data->temp = navdata_raw.navdata_pressure_raw.Temperature_meas;

			data->windSpeed = navdata_raw.navdata_wind_speed.wind_speed;
			data->windAngle = navdata_raw.navdata_wind_speed.wind_angle;
			data->windCompAngle = navdata_raw.navdata_wind_speed.wind_compensation_phi;
		}
		else
		{
			data->magX = data->magY = data->magZ = 0;
			data->pressure = 0.0;
			data->temp = 0.0;
			data->windSpeed = 0.0;
			data->windAngle = 0.0;
			data->windCompAngle = 0.0;
		}

		// Tag Detection, need to clear vectors first because it's a member variable now
		data->tagsType.clear();
		data->tagsXc.clear();
		data->tagsYc.clear();
		data->tagsWidth.clear();
		data->tagsHeight.clear();
		data->tagsOrientation.clear();
		data->tagsDistance.clear();												


		data->tagsCount = navdata_raw.navdata_vision_detect.nb_detected;
		for (int i = 0; i < navdata_raw.navdata_vision_detect.nb_detected; i++)
		{
			/*
			 * The tags_type is in raw format. In order to extract the information
			 * macros from ardrone_api.h is needed.
			 *
			 * #define DETECTION_MAKE_TYPE(source,tag) ( ((source)<<16) | (tag) )
			 * #define DETECTION_EXTRACT_SOURCE(type)  ( ((type)>>16) & 0x0FF )
			 * #define DETECTION_EXTRACT_TAG(type)     ( (type) & 0x0FF )
			 *
			 * Please also note that the xc, yc, width and height are in [0,1000] range
			 * and must get converted back based on image resolution.
			 */
			data->tagsType.push_back(navdata_raw.navdata_vision_detect.type[i]);

			data->tagsXc.push_back(navdata_raw.navdata_vision_detect.xc[i]);
			data->tagsYc.push_back(navdata_raw.navdata_vision_detect.yc[i]);
			data->tagsWidth.push_back(navdata_raw.navdata_vision_detect.width[i]);
			data->tagsHeight.push_back(navdata_raw.navdata_vision_detect.height[i]);
			data->tagsOrientation.push_back(navdata_raw.navdata_vision_detect.orientation_angle[i]);
			data->tagsDistance.push_back(navdata_raw.navdata_vision_detect.dist[i]);
		}

		/* IMU */
		/*imu_msg.header.frame_id = droneFrameBase;
		imu_msg.header.stamp = navdata_receive_time;*/

		// IMU - Linear Acc
		/*imu_msg.linear_acceleration.x = legacynavdata_msg.ax * 9.8;
		imu_msg.linear_acceleration.y = legacynavdata_msg.ay * 9.8;
		imu_msg.linear_acceleration.z = legacynavdata_msg.az * 9.8;*/

		// IMU - Rotation Matrix


		// IMU - Gyro (Gyro is being sent in deg/sec)
		// TODO: Should Gyro be added to Navdata?
		/*imu_msg.angular_velocity.x = navdata_raw.navdata_phys_measures.phys_gyros[GYRO_X] * DEG_TO_RAD;
		imu_msg.angular_velocity.y = -navdata_raw.navdata_phys_measures.phys_gyros[GYRO_Y] * DEG_TO_RAD;
		imu_msg.angular_velocity.z = -navdata_raw.navdata_phys_measures.phys_gyros[GYRO_Z] * DEG_TO_RAD;

		mag_msg.header.frame_id = droneFrameBase;
		mag_msg.header.stamp = navdata_receive_time;
		const float mag_normalizer = sqrt( legacynavdata_msg.magX * legacynavdata_msg.magX + legacynavdata_msg.magY * legacynavdata_msg.magY + legacynavdata_msg.magZ * legacynavdata_msg.magZ );*/

		// TODO: Check if it is really needed that magnetometer message includes normalized value
		return data;
	}
	
	bool NavdataI::readCovParams()
	{
		return false;
	}
	double NavdataI::calcAverage(std::vector<double> &vec)
	{
	    double ret = 0.0;
	    for (unsigned int i = 0; i < vec.size(); i++)
	    {
		ret += vec.at(i);
	    }
	    return (ret / vec.size());	
	}
	
	void NavdataI::resetCaliberation()
	{
	    caliberated = false;
	    acc_samples.clear();
	    gyro_samples.clear();
	    vel_samples.clear();
	    for (int i = 0; i < 3; i++)
	    {
		acc_bias[i] = 0.0;
		vel_bias[i] = 0.0;
		gyro_bias[i] = 0.0;
		acc_samples.push_back(std::vector<double> ());
		gyro_samples.push_back(std::vector<double> ());
		vel_samples.push_back(std::vector<double> ());
	    }	
	}
}
