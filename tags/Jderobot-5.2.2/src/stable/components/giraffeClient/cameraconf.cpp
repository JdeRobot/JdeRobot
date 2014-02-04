/*
 *  Copyright (C) 2010 Eduardo Perdices García
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "cameraconf.h"

namespace giraffeClient {

	CameraConf::CameraConf(RoboCompJointMotor::JointMotorPrx jprx) {

		this->jprx = jprx;

		/*Set Extrinsecs camera parameters*/
		this->camera.position.X=0.0;
    this->camera.position.Y=0.0;
    this->camera.position.Z=0.0;
    this->camera.position.H=1.0;		
		this->camera.foa.X=0.0;
    this->camera.foa.Y=0.0;
    this->camera.foa.Z=0.0;
    this->camera.foa.H=1.0;
		this->camera.roll=0.0;	

		/*Set Intrinsecs camera parameters*/
		this->camera.u0=120; 
		this->camera.v0=160;
		this->camera.fdistx=320.0;
		this->camera.fdisty=320.0;
		this->camera.skew=0.0;
		this->camera.rows = 240;
		this->camera.columns = 320;
		update_camera_matrix(&(this->camera));
	}

  CameraConf::~CameraConf() {
  }
  
	void
	CameraConf::drawWorld(IplImage * src) {
		HPoint3D p1, p2;

		this->calcCameraPos();

		p1.X = 0.0;		p1.Y = 0.0;		p1.Z = 0.0;		p1.H = 1.0;
		p2.X = 500.0;		p2.Y = 0.0;		p2.Z = 0.0;		p2.H = 1.0;
		this->drawLine(src, p1, p2);

		p1.X = 0.0;		p1.Y = -300.0;		p1.Z = 0.0;		p1.H = 1.0;
		p2.X = 500.0;		p2.Y = -300.0;		p2.Z = 0.0;		p2.H = 1.0;
		this->drawLine(src, p1, p2);

		p1.X = 0.0;		p1.Y = 300.0;		p1.Z = 0.0;		p1.H = 1.0;
		p2.X = 500.0;		p2.Y = 300.0;		p2.Z = 0.0;		p2.H = 1.0;
		this->drawLine(src, p1, p2);
	}

	void
	CameraConf::calcCameraPos() {

		RoboCompJointMotor::MotorStateMap motorsstate;
		gsl_matrix * init, * motor_pan, * RT_pan;
		gsl_matrix * neck, * RT_neck, * motor_tilt, *RT_tilt;
		gsl_matrix * cam_right, * RT_cam_right;
		gsl_matrix * foa_rel, * foa;
		float pan_value, tilt_value;

		init = gsl_matrix_alloc(4,4);
		motor_pan = gsl_matrix_alloc(4,4);
		RT_pan = gsl_matrix_alloc(4,4);
		neck = gsl_matrix_alloc(4,4);
		RT_neck = gsl_matrix_alloc(4,4);
		motor_tilt = gsl_matrix_alloc(4,4);
		RT_tilt = gsl_matrix_alloc(4,4);
		cam_right = gsl_matrix_alloc(4,4);
		RT_cam_right = gsl_matrix_alloc(4,4);
		foa_rel = gsl_matrix_alloc(4,1);
		foa = gsl_matrix_alloc(4,1);

		/*Get motors state*/
		jprx->getAllMotorState(motorsstate);
		pan_value = motorsstate["neck"].pos;
		tilt_value = -motorsstate["tilt"].pos;

		/*Initial matrix*/
		gsl_matrix_set(init,0,0,1.0);
		gsl_matrix_set(init,0,1,0.0);
		gsl_matrix_set(init,0,2,0.0);
		gsl_matrix_set(init,0,3,INIT_X);	//Profundidad
		gsl_matrix_set(init,1,0,0.0);
		gsl_matrix_set(init,1,1,1.0);
		gsl_matrix_set(init,1,2,0.0);
		gsl_matrix_set(init,1,3,INIT_Y);	//Lateral (izq positivo)
		gsl_matrix_set(init,2,0,0.0);
		gsl_matrix_set(init,2,1,0.0);
		gsl_matrix_set(init,2,2,1.0);
		gsl_matrix_set(init,2,3,INIT_Z);	//Altura
		gsl_matrix_set(init,3,0,0.0);
		gsl_matrix_set(init,3,1,0.0);
		gsl_matrix_set(init,3,2,0.0);
		gsl_matrix_set(init,3,3,1.0);

		/*Pan motor turn in Z axis*/
		gsl_matrix_set(motor_pan,0,0,cos(pan_value));
		gsl_matrix_set(motor_pan,0,1,-sin(pan_value));
		gsl_matrix_set(motor_pan,0,2,0.0);
		gsl_matrix_set(motor_pan,0,3,0.0);	//Profundidad
		gsl_matrix_set(motor_pan,1,0,sin(pan_value));
		gsl_matrix_set(motor_pan,1,1,cos(pan_value));
		gsl_matrix_set(motor_pan,1,2,0.0);
		gsl_matrix_set(motor_pan,1,3,0.0);	//Lateral (izq positivo)
		gsl_matrix_set(motor_pan,2,0,0.0);
		gsl_matrix_set(motor_pan,2,1,0.0);
		gsl_matrix_set(motor_pan,2,2,1.0);
		gsl_matrix_set(motor_pan,2,3,0.0);	//Altura
		gsl_matrix_set(motor_pan,3,0,0.0);
		gsl_matrix_set(motor_pan,3,1,0.0);
		gsl_matrix_set(motor_pan,3,2,0.0);
		gsl_matrix_set(motor_pan,3,3,1.0);

		gsl_linalg_matmult(init, motor_pan, RT_pan);

		/*Neck length*/
		gsl_matrix_set(neck,0,0,1.0);
		gsl_matrix_set(neck,0,1,0.0);
		gsl_matrix_set(neck,0,2,0.0);
		gsl_matrix_set(neck,0,3,0.0);	//Profundidad
		gsl_matrix_set(neck,1,0,0.0);
		gsl_matrix_set(neck,1,1,1.0);
		gsl_matrix_set(neck,1,2,0.0);
		gsl_matrix_set(neck,1,3,0.0);	//Lateral (izq positivo)
		gsl_matrix_set(neck,2,0,0.0);
		gsl_matrix_set(neck,2,1,0.0);
		gsl_matrix_set(neck,2,2,1.0);
		gsl_matrix_set(neck,2,3,NECK_LENGTH);	//Altura
		gsl_matrix_set(neck,3,0,0.0);
		gsl_matrix_set(neck,3,1,0.0);
		gsl_matrix_set(neck,3,2,0.0);
		gsl_matrix_set(neck,3,3,1.0);	

		gsl_linalg_matmult(RT_pan, neck, RT_neck);

		/*Tilt motor turn in Y axis*/	
		gsl_matrix_set(motor_tilt,0,0,cos(tilt_value));
		gsl_matrix_set(motor_tilt,0,1,0.0);
		gsl_matrix_set(motor_tilt,0,2,sin(tilt_value));
		gsl_matrix_set(motor_tilt,0,3,0.0);	//Profundidad
		gsl_matrix_set(motor_tilt,1,0,0.0);
		gsl_matrix_set(motor_tilt,1,1,1.0);
		gsl_matrix_set(motor_tilt,1,2,0.0);
		gsl_matrix_set(motor_tilt,1,3,0.0);	//Lateral (izq positivo)
		gsl_matrix_set(motor_tilt,2,0,-sin(tilt_value));
		gsl_matrix_set(motor_tilt,2,1,0.0);
		gsl_matrix_set(motor_tilt,2,2,cos(tilt_value));
		gsl_matrix_set(motor_tilt,2,3,0.0);	//Altura
		gsl_matrix_set(motor_tilt,3,0,0.0);
		gsl_matrix_set(motor_tilt,3,1,0.0);
		gsl_matrix_set(motor_tilt,3,2,0.0);
		gsl_matrix_set(motor_tilt,3,3,1.0);	

		gsl_linalg_matmult(RT_neck, motor_tilt, RT_tilt);

		/*Camera right position*/
		gsl_matrix_set(cam_right,0,0,1.0);
		gsl_matrix_set(cam_right,0,1,0.0);
		gsl_matrix_set(cam_right,0,2,0.0);
		gsl_matrix_set(cam_right,0,3,CAM_RIGHT_X);	//Profundidad
		gsl_matrix_set(cam_right,1,0,0.0);
		gsl_matrix_set(cam_right,1,1,1.0);
		gsl_matrix_set(cam_right,1,2,0.0);
		gsl_matrix_set(cam_right,1,3,CAM_RIGHT_Y);	//Lateral (izq positivo)
		gsl_matrix_set(cam_right,2,0,0.0);
		gsl_matrix_set(cam_right,2,1,0.0);
		gsl_matrix_set(cam_right,2,2,1.0);
		gsl_matrix_set(cam_right,2,3,CAM_RIGHT_Z);	//Altura
		gsl_matrix_set(cam_right,3,0,0.0);
		gsl_matrix_set(cam_right,3,1,0.0);
		gsl_matrix_set(cam_right,3,2,0.0);
		gsl_matrix_set(cam_right,3,3,1.0);

		gsl_linalg_matmult(RT_tilt, cam_right, RT_cam_right);		

		/*Calc foa (1 meter in front of the camera*/
		gsl_matrix_set(foa_rel,0,0,1.0);
		gsl_matrix_set(foa_rel,1,0,0.0);
		gsl_matrix_set(foa_rel,2,0,0.0);
		gsl_matrix_set(foa_rel,3,0,1.0);

		gsl_linalg_matmult(RT_cam_right,foa_rel,foa);

		/*Update extrinsecs camera parameters*/
		this->camera.position.X=(float)gsl_matrix_get(RT_cam_right,0,3)*1000;
    this->camera.position.Y=(float)gsl_matrix_get(RT_cam_right,1,3)*1000;
    this->camera.position.Z=(float)gsl_matrix_get(RT_cam_right,2,3)*1000;
    this->camera.position.H=1.0;		
		this->camera.foa.X=(float)gsl_matrix_get(foa,0,0)*1000;
    this->camera.foa.Y=(float)gsl_matrix_get(foa,1,0)*1000;
    this->camera.foa.Z=(float)gsl_matrix_get(foa,2,0)*1000;
    this->camera.foa.H=1.0;
		this->camera.roll=0.0;	
		update_camera_matrix(&(this->camera));	

		gsl_matrix_free(init);
		gsl_matrix_free(motor_pan);
		gsl_matrix_free(RT_pan);
		gsl_matrix_free(neck);
		gsl_matrix_free(RT_neck);
		gsl_matrix_free(motor_tilt);
		gsl_matrix_free(RT_tilt);
		gsl_matrix_free(cam_right);
		gsl_matrix_free(RT_cam_right);
		gsl_matrix_free(foa_rel);
		gsl_matrix_free(foa);
	}

	void
	CameraConf::printMatrix(gsl_matrix * m, int rows, int cols) {
		int i, j;

		std::cout << "-------------" << std::endl;

		for(i=0;i<rows;i++) {
			std::cout << "|\t";
			for(j=0;j<cols;j++) {
					std::cout << gsl_matrix_get(m,i,j) << "\t|\t";
			}
			std::cout << std::endl;
		}
	}

	void
	CameraConf::drawLine(IplImage * src, HPoint3D pini, HPoint3D pend) {
		HPoint2D p1, p2;
		HPoint2D gooda,goodb;
		CvPoint pt1, pt2;

		project(pini,&p1,this->camera);
    project(pend,&p2,this->camera);

		if(displayline(p1,p2,&gooda,&goodb,this->camera)==1) {

			/*From optical to pixels*/
			pt1.x=(int)gooda.y;
			pt1.y=this->camera.rows-1-(int)gooda.x;
			pt2.x=(int)goodb.y;
			pt2.y=this->camera.rows-1-(int)goodb.x;
      
			cvLine(src, pt1, pt2, cvScalar(255, 0, 255, 0), 2, 8, 0);
    }
	}

	void
	CameraConf::getAngles(float x, float y, float z, float &pan, float &tilt) {

		float rx, ry, rz;

		/*Set pan with X-Y*/
		if(x == 0.0)
			pan = MAX_PAN;
		else
			pan = atan(y/x);

		/*Calc relative position of point from tilt motor*/
		rx = x - CAM_RIGHT_X*1000;
		ry = y - CAM_RIGHT_Y*1000;
		rz = z - CAM_RIGHT_Z*1000 - NECK_LENGTH*1000;

		/*Set tilt with X-Z*/
		if(rx == 0.0)
			tilt = MAX_TILT;	
		else
			tilt = atan(rz/rx);
	}

} /*namespace*/

