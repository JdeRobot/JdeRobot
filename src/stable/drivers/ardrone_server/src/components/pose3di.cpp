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
#include "pose3di.h"
#include <cmath>

namespace pose3D
{
	Pose3DI::Pose3DI(ARDroneDriver *driver)
	{
		std::cout << "pose3d start" << std::endl;
		pose3D=new jderobot::Pose3DData();
		this->driver=driver;
	}
	
	Pose3DI::~Pose3DI()
	{

	}
		
	jderobot::Pose3DDataPtr Pose3DI::getPose3DData(const Ice::Current&)
	{
		vp_os_mutex_lock(&navdata_lock);
			navdata_unpacked_t navdata_raw = *shared_raw_navdata;
		vp_os_mutex_unlock(&navdata_lock);	

		float roll=navdata_raw.navdata_demo.phi / 1000.0;
		float pitch=-navdata_raw.navdata_demo.theta / 1000.0;
		float yaw=-navdata_raw.navdata_demo.psi / 1000.0;

		Eigen::Quaternion<float> q;
		Eigen::AngleAxis<float> aaZ(this->deg2rad(yaw), Eigen::Vector3f::UnitZ());
		Eigen::AngleAxis<float> aaY(this->deg2rad(pitch), Eigen::Vector3f::UnitY());
		Eigen::AngleAxis<float> aaX(this->deg2rad(roll), Eigen::Vector3f::UnitX());

		q = aaZ * aaY * aaX;	
		
		pose3D->q0=q.w();
		pose3D->q1=q.x();
		pose3D->q2=q.y();
		pose3D->q3=q.z();
						

		return pose3D;
	}
	
	float Pose3DI::deg2rad(float d)
	{
		float radians = (d / 360) * (2.0 * PI);
		return radians;
	}
	
	Ice::Int Pose3DI::setPose3DData(const jderobot::Pose3DDataPtr& data, const Ice::Current&)
	{
	
		pose3D->x=data->x;
		pose3D->y=data->y;
		pose3D->z=data->z;
		pose3D->h=data->h;
		pose3D->q0=data->q0;
		pose3D->q1=data->q1;
		pose3D->q2=data->q2;
		pose3D->q3=data->q3;														
		return 1;
	}

}
