/*
 *  Copyright (C) 1997-2013 JDE Developers TeamkinectViewer.camRGB
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
 *
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */

#include "util3d.h"


namespace rgbdViz{

util3d::util3d(myprogeo* p){
	mypro = p;
}

util3d::~util3d(){
}

int util3d::cvDrawline(cv::Mat image,Eigen::Vector3d p1, Eigen::Vector3d p2, cv::Scalar color,int cam){
  Eigen::Vector3d gooda,goodb;
  CvPoint pt1,pt2;
  Progeo::Progeo* camera;

	camera=mypro->getCamera(cam);
 /* if(camera->displayline(p1,p2,gooda,goodb)==1){
	pt1.x=(int)gooda(1);
	pt1.y=camera->getImageHeight()-1-(int)gooda(0);
	pt2.x=(int)goodb(1);
	pt2.y=camera->getImageWidth()-1-(int)goodb(0);
	cv::line(image,pt1,pt2,color,2,1,0);
	return 1;
  }*/
  return 0;
}

void util3d::draw_room(cv::Mat image,int cam, float lines[][8], int n_lines){
  	int i,j;
  	Eigen::Vector3d a,b;
  	Eigen::Vector4d a3A,a3B,a3C,a3D;
  	Progeo::Progeo* camera;

	camera=mypro->getCamera(cam);
  	for(i=0;i<n_lines;i++){
		a3A(0)=lines[i][0];
		a3A(1)=lines[i][1];
		a3A(2)=lines[i][2];
		a3A(3)=lines[i][3];
		a3B(0)=lines[i][4];
		a3B(1)=lines[i][5];
		a3B(2)=lines[i][6];
		a3B(3)=lines[i][7];
		camera->project(a3A,a);
		camera->project(a3B,b);
		cvDrawline(image,a,b,CV_RGB(0,0,0),cam);
  }
}


}
