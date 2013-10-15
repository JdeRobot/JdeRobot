/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  Authors : Alejandro Hernández <ahcorde [at] gmail [dot] com>
 *            Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *            Eduardo Perdices <eperdices [at] gsyc [dot] es>
 *
 */

#include "Progeo.h"
#include <math.h>


namespace Progeo {

Progeo::Progeo()
{
}

Progeo::Progeo(Eigen::Vector4d posCamera,
		Eigen::Matrix3d KMatrix,
		Eigen::Matrix4d RTMatrix,
		int width, int height)
{

	setPosition(posCamera);
	setKMatrix(KMatrix);
	setRTMatrix(RTMatrix);
	setImageSize(width, height);
	foa(0) = 0.;
	foa(1) = 1.;
	foa(2) = 0.;
	foa(3) = 1.;

	roll = 0.;
	skew = 0.;

}

Progeo::Progeo(std::string filename)
{
	xmlDocPtr doc;
	xmlNodePtr cur, curAux, curAux_child;
	doc = xmlParseFile(filename.c_str());
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "calibration_camera")) {
		fprintf(stderr,"document of the wrong type, root node != calibration_camera");
		xmlFreeDoc(doc);
		return;
	}

	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"calibration_camera"))) {

			xmlChar *key;
			curAux = cur->xmlChildrenNode;

			while (curAux != NULL) {
				/*
                if ((!xmlStrcmp(curAux->name, (const xmlChar *)"name"))) {
                    key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);

                    char a[((string)key).size()+1];
                    a[key.size()]=0;
                    memcpy(a,key.c_str(),s.size());

                    this->name = a;
                    xmlFree(key);
                }else */if ((!xmlStrcmp(curAux->name, (const xmlChar *)"roll"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->roll = atof((char*)(char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdistx"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->fdistx = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdisty"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->fdisty = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"u0"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->u0 = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"v0"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->v0 = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"skew"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->skew = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rows"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->rows = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"columns"))) {
                	key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
                	this->columns = atof((char*)key);
                	xmlFree(key);
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"position"))) {
                	curAux_child = curAux->xmlChildrenNode;

                	while (curAux_child != NULL) {
                		if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->position(0) = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->position(1) = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->position(2) = atof((char*)key);
                			xmlFree(key);

                		}
                		curAux_child = curAux_child->next;
                	}
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"foa"))) {
                	curAux_child = curAux->xmlChildrenNode;

                	while (curAux_child != NULL) {
                		if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->foa(0) = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->foa(1) = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->foa(2) = atof((char*)key);
                			xmlFree(key);

                		}
                		curAux_child = curAux_child->next;
                	}
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"k_matrix"))) {
                	curAux_child = curAux->xmlChildrenNode;

                	while (curAux_child != NULL) {
                		if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k11"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k11 = atof((char*)key);
                			K(0,0) = this->k11;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k12"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k12 = atof((char*)key);
                			K(0,1) = this->k12;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k13"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k13 = atof((char*)key);
                			K(0,2) = this->k13;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k14"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k14 = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k21"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k21 = atof((char*)key);
                			K(1,0) = this->k21;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k22"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k22 = atof((char*)key);
                			K(1,1) = this->k22;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k23"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k23 = atof((char*)key);
                			K(1,2) = this->k23;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k24"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k24 = atof((char*)key);
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k31"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k31 = atof((char*)key);
                			K(2,0) = this->k31;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k32"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k32 = atof((char*)key);
                			K(2,1) = this->k32;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k33"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k33 = atof((char*)key);
                			K(2,2) = this->k33;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k34"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->k34 = atof((char*)key);
                			xmlFree(key);

                		}
                		curAux_child = curAux_child->next;
                	}
                } else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rt_matrix"))) {
                	curAux_child = curAux->xmlChildrenNode;

                	while (curAux_child != NULL) {
                		if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt11"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt11 = atof((char*)key);
                			RT(0,0) = this->rt11;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt12"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt12 = atof((char*)key);
                			RT(0,1) = this->rt12;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt13"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt13 = atof((char*)key);
                			RT(0,2) = this->rt13;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt14"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt14 = atof((char*)key);
                			RT(0,3) = this->rt14;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt21"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt21 = atof((char*)key);
                			RT(1,0) = this->rt21;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt22"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt22 = atof((char*)key);
                			RT(1,1) = this->rt22;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt23"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt23 = atof((char*)key);
                			RT(1,2) = this->rt23;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt24"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt24 = atof((char*)key);
                			RT(1,3) = this->rt24;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt31"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt31 = atof((char*)key);
                			RT(2,0) = this->rt31;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt32"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt32 = atof((char*)key);
                			RT(2,1) = this->rt32;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt33"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt33 = atof((char*)key);
                			RT(2,2) = this->rt33;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt34"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt34 = atof((char*)key);
                			RT(2,3) = this->rt34;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt41"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt41 = atof((char*)key);
                			RT(3,0) = this->rt41;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt42"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt42 = atof((char*)key);
                			RT(3,1) = this->rt42;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt43"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt43 = atof((char*)key);
                			RT(3,2) = this->rt43;
                			xmlFree(key);

                		} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt44"))) {
                			key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
                			this->rt44 = atof((char*)key);
                			RT(3,3) = this->rt44;
                			xmlFree(key);

                		}
                		curAux_child = curAux_child->next;
                	}
                }
                curAux = curAux->next;
			}
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
	return;
}

void Progeo::displayCameraInfo() {

	printf("\n----------------------- PROGEO C++ ---------------------\n");
	printf("Camera %s\n\n", this->name.c_str());
	printf("     Position: (X,Y,Z,H)=(%.1f,%.1f,%.1f,%.1f)\n",   position(0),
			position(1),
			position(2),
			position(3));
	printf("     Focus of Attention: (x,y,z,h)=(%.1f,%.1f,%.1f,%.1f)\n\n",  foa(0),
			foa(1),
			foa(2),
			foa(3));
	printf("     Focus DistanceX(vertical): %.1f mm\n",fdistx);
	printf("     Focus DistanceY(horizontal): %.1f mm\n",fdisty);

	printf("     Skew: %.5f \n",skew);
	printf("     Optical Center: (x,y)=(%.1f,%.1f)\n\n",u0,v0);

	std::cout << "K Matrix: \n" << K << std::endl;
	std::cout <<" R&T Matrix:\n"<<  RT << std::endl;

	printf("------------------------------------------------------\n");
}

int Progeo::project(Eigen::Vector4d in, Eigen::Vector3d &out)
/* returns -1 if the point lies behind the camera,
   returns 1 if "in" 3Dpoint projects into a 2D finite point,
   0 otherwise */
{
	Eigen::Vector4d a;
	a = RT*in;

	out = K*a.head(3);

	
	out(0) = out(0)/out(2);
	out(1) = out(1)/out(2);
	out(2) = 1.0;

	// optical 2 graphics
	double aux = out(0);
	out(0) = out(1);
	out(1) = this->rows-1-aux;

	if (out(2)!=0.) {
		return 1;
	} else {
		return 0;
	}
}

void Progeo::backproject(Eigen::Vector3d point, Eigen::Vector4d& pro)
{
	//GRAPHIC_TO_OPTICAL
	//int opX = this->rows -1 -point(1);
	//int opY = point(0);

	Eigen::Matrix3d ik;
	ik = K;
	ik = ik.inverse().eval();

	//std::cout << "punto: " << point(0) << ", " << point(1) << ", " << point(2) << std::endl;

	Eigen::Vector3d Pi(point(0)/point(2), point(1)/point(2),1);

	Eigen::Vector3d a;
	a = ik*Pi;

	//std::cout << "a: " << a(0) << ", " << a(1) << ", " << a(2) << std::endl;

	Eigen::Vector4d aH;
	aH(0) = a(0);
	aH(1) = a(1);
	aH(2) = a(2);
	aH(3) = 1.0;

	Eigen::Matrix4d RT2;
	RT2 = RT;

	RT2(0, 3) = .0;
	RT2(1, 3) = .0;
	RT2(2, 3) = .0;
	RT2(3, 3) = 1.0;


	Eigen::Vector4d b;

	b = RT2.transpose()*aH;

	//std::cout << "b: " << b(0) << ", " << b(1) << ", " << b(2) << std::endl;

	Eigen::Matrix4d Translate;
	Translate.setIdentity();
	Translate(0, 3) = RT(0,3)/RT(3,3);
	Translate(1, 3) = RT(1,3)/RT(3,3);
	Translate(2, 3) = RT(2,3)/RT(3,3);

	b = Translate*b;


	/*OJO*/
	pro(0) = b(0)/b(3);
	pro(1) = b(1)/b(3);
	pro(2) = b(2)/b(3);
	pro(3) = b(3);
	
}


void Progeo::backprojectCV(Eigen::Vector3d point, Eigen::Vector4d& pro)
{
	//GRAPHIC_TO_OPTICAL
	//int opX = this->rows -1 -point(1);
	//int opY = point(0);

	/*Eigen::Matrix3d ik;
	ik = K;
	ik = ik.inverse().eval();
*/
	//Eigen::Vector3d Pi(point(0)/point(2), point(1)/point(2),1);




//float k;
	std::string line;
	float p;

	cv::Point3f foa;
	cv::Mat myK = cv::Mat(cv::Size(3,3),CV_32FC1);
	cv::Mat myR = cv::Mat(cv::Size(3,3),CV_32FC1);
		
	

	myK.at<float>(0,0)=K(0,0);
	myK.at<float>(0,1)=K(0,1);
	myK.at<float>(0,2)=K(0,2);
	myK.at<float>(1,0)=K(1,0);
	myK.at<float>(1,1)=K(1,1);
	myK.at<float>(1,2)=K(1,2);
	myK.at<float>(2,0)=K(2,0);
	myK.at<float>(2,1)=K(2,1);
	myK.at<float>(2,2)=K(2,2);


	myR.at<float>(0,0)=RT(0,0);
	myR.at<float>(0,1)=RT(0,1);
	myR.at<float>(0,2)=RT(0,2);
	myR.at<float>(1,0)=RT(1,0);
	myR.at<float>(1,1)=RT(1,1);
	myR.at<float>(1,2)=RT(1,2);
	myR.at<float>(2,0)=RT(2,0);
	myR.at<float>(2,1)=RT(2,1);
	myR.at<float>(2,2)=RT(2,2);


	cv::Mat ik=cv::Mat(myK.size(), myK.type());
	cv::invert(myK,ik);

	
	cv::Mat Pi=cv::Mat(cv::Size(1,3),CV_32FC1);
	Pi.at<float>(0,0)=(float)point(0)/point(2);
	Pi.at<float>(0,1)=(float)point(1)/point(2);
	Pi.at<float>(0,2)=1.;
	
	cv::Mat a;
	a=ik*Pi;

	cv::Mat aH=cv::Mat(cv::Size(1,4), CV_32FC1);
	aH.at<float>(0,0)=a.at<float>(0,0);
	aH.at<float>(0,1)=a.at<float>(0,1);
	aH.at<float>(0,2)=a.at<float>(0,2);
	aH.at<float>(0,3)=1.;

	std::cout << " -------------------- " << std::endl;
	std::cout << "a= " << a << std::endl;
	std::cout << aH << std::endl;
//	std::cout 

	cv::Mat rT;
	cv::transpose(myR,rT);

	cv::Mat rTH=cv::Mat(cv::Size(4,4),CV_32FC1, cv::Scalar(0));
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			rTH.at<float>(i,j)=rT.at<float>(i,j);
	rTH.at<float>(3,3)=1.;
	
	
	cv::Mat b;
	b=rTH*aH;

	std::cout << "b= " << a << std::endl;
	
	cv::Mat iT=cv::Mat(cv::Size(4,4),CV_32FC1, cv::Scalar(0));
	iT.at<float>(0,0)=1.;
	iT.at<float>(1,1)=1;
	iT.at<float>(2,2)=1;
	iT.at<float>(3,3)=1;
	iT.at<float>(0,3)=position(0);
	iT.at<float>(1,3)=position(1);
	iT.at<float>(2,3)=position(2);
	cv::Mat Pw;
	Pw=iT*b;
	

	pro(0)=Pw.at<float>(0,0);
	pro(1)=Pw.at<float>(0,1);
	pro(2)=Pw.at<float>(0,2);
	pro(3) = 1;



	/*pro(0) = b(0)/b(3);
	pro(1) = b(1)/b(3);
	pro(2) = b(2)/b(3);
	pro(3) = b(3);*/

	/*OJO*/
	/*pro(0) = b(0)/b(3);
	pro(1) = b(1)/b(3);
	pro(2) = b(2)/b(3);
	pro(3) = b(3);*/
	
}



void Progeo::setPosition (Eigen::Vector4d pos)
{
	position = pos;
}

void Progeo::setKMatrix (Eigen::Matrix3d KMatrix)
{
	K = KMatrix;
	fdistx = K(0,0);
	fdisty = K(1,1);
	u0 = K(0,2);
	v0 = K(1,2);
}

void Progeo::setRTMatrix (Eigen::Matrix4d RTMatrix)
{
	RT = RTMatrix;
}

void Progeo::setImageSize (int width, int height)
{
	rows = height;
	columns = width;
}

void Progeo::setFoa(Eigen::Vector4d Foa){
	this->foa=Foa;
}


void Progeo::setRoll( float Roll)
{
	this->roll=Roll;
}

void Progeo::updateRTMatrix()
{
	Eigen::Matrix4d RC;
	Eigen::Matrix4d RAB;
	float r;
	double t;

	Eigen::Vector3d u,v,w;

	// Set homogeneous coordinate of camera and FOA
	foa(3) = 1.0;
	position(3) = 1.0;

	// Orientation model: focus of attention + roll

	RAB(2,0) = double(foa(0) - position(0));
	RAB(2,1) = double(foa(1) - position(1));
	RAB(2,2) = double(foa(2) - position(2));

	r=(float)sqrt((double)(RAB(2,0)*RAB(2,0)+RAB(2,1)*RAB(2,1)+RAB(2,2)*RAB(2,2)));

	RAB(2,0) = double(RAB(2,0) / r);
	RAB(2,1) = double(RAB(2,1) / r);
	RAB(2,2) = double(RAB(2,2) / r);

	w(0) = RAB(2,0);
	w(1) = RAB(2,1);
	w(2) = RAB(2,2);

	t = atan2((double)-w(0),(double)w(1));

	v(0) = (float) cos(t);
	v(1) = (float) sin(t);
	v(2) = 0.;

	u(0) = v(1)*w(2)-w(1)*v(2);
	u(1) = -v(0)*w(2)+w(0)*v(2);
	u(2) = v(0)*w(1)-w(0)*v(1);

	if (u(2) <0.)
	{
		v(0) = -v(0);
		v(1) = -v(1);
		v(2) = -v(2);
		u(0) = -u(0);
		u(1) = -u(1);
		u(2) = -u(2);
	}

	RAB(0,0) = double(u(0));
	RAB(0,1) = double(u(1));
	RAB(0,2) = double(u(2));

	RAB(1,0) = double(v(0));
	RAB(1,1) = double(v(1));
	RAB(1,2) = double(v(2));


	RC(0,0) = double(cos(roll));
	RC(0,1) = double(sin(roll));
	RC(0,2) = 0.;
	RC(1,0) = double(-sin(roll));
	RC(1,1) = double(cos(roll));
	RC(1,2) = 0.;
	RC(2,0) = 0.;
	RC(2,1) = 0.;
	RC(2,2) = 1.;

	RT(0,0) = double(RC(0,0)*RAB(0,0) + RC(0,1)*RAB(1,0) + RC(0,2)*RAB(2,0));
	RT(0,1) = double(RC(0,0)*RAB(0,1) + RC(0,1)*RAB(1,1) + RC(0,2)*RAB(2,1));
	RT(0,2) = double(RC(0,0)*RAB(0,2) + RC(0,1)*RAB(1,2) + RC(0,2)*RAB(2,2));
	RT(0,3) = position(0);

	RT(1,0) = double(RC(1,0)*RAB(0,0) + RC(1,1)*RAB(1,0) + RC(1,2)*RAB(2,0));
	RT(1,1) = double(RC(1,0)*RAB(0,1) + RC(1,1)*RAB(1,1) + RC(1,2)*RAB(2,1));
	RT(1,2) = double(RC(1,0)*RAB(0,2) + RC(1,1)*RAB(1,2) + RC(1,2)*RAB(2,2));
	RT(1,3) = position(1);

	RT(2,0) = double(RC(2,0)*RAB(0,0) + RC(2,1)*RAB(1,0) + RC(2,2)*RAB(2,0));
	RT(2,1) = double(RC(2,0)*RAB(0,1) + RC(2,1)*RAB(1,1) + RC(2,2)*RAB(2,1));
	RT(2,2) = double(RC(2,0)*RAB(0,2) + RC(2,1)*RAB(1,2) + RC(2,2)*RAB(2,2));
	RT(2,3) = position(2);

    RT(3,0) = 0.;
    RT(3,1) = 0.;
    RT(3,2) = 0.;
    RT(3,3) = 1.;
}

void Progeo::updateKMatrix()
{

	K(0,0) = fdistx;
	K(0,1) = skew*fdisty;
	K(0,2) = u0;


	K(1,0) = 0.;
    K(1,1) = fdisty;
    K(1,2) = v0;


    K(2,0) = 0.;
    K(2,1) = 0.;
    K(2,2) = 1.;


}

void Progeo::pixel2optical (Eigen::Vector3d &point)
{
	double aux = point(0);
	point(0) = rows-1-point(1);
	point(1) = aux;
}

void Progeo::optical2pixel(Eigen::Vector3d &point)
{
	double aux = point(1);
	point(1) = rows-1-point(0);
	point(0) = aux;
}

const char* Progeo::double2char(double d)
{
	std::stringstream ss;
	ss << d;
	return ss.str().c_str();
}

void Progeo::saveToFile (std::string filename)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	xmlNodePtr node_position = NULL;
	xmlNodePtr node_foa = NULL;
	xmlNodePtr node_k_matrix = NULL;
	xmlNodePtr node_rt_matrix = NULL;

	updateKMatrix();
	updateRTMatrix();

	char buff[256];
	int i, j;

	LIBXML_TEST_VERSION;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "calibration_camera");
	node_position = xmlNewChild(root_node, NULL, BAD_CAST "position", NULL);
	node_foa = xmlNewChild(root_node, NULL, BAD_CAST "foa", NULL);
	node_k_matrix = xmlNewChild(root_node, NULL, BAD_CAST "k_matrix", NULL);
	node_rt_matrix = xmlNewChild(root_node, NULL, BAD_CAST "rt_matrix", NULL);

	xmlDocSetRootElement(doc, root_node);

	// Camera position
	xmlNewChild(node_position, NULL, BAD_CAST "x", BAD_CAST double2char(position(0)));
	xmlNewChild(node_position, NULL, BAD_CAST "y", BAD_CAST double2char(position(1)));
	xmlNewChild(node_position, NULL, BAD_CAST "z", BAD_CAST double2char(position(2)));

	// Foa position
	xmlNewChild(node_foa, NULL, BAD_CAST "x", BAD_CAST double2char(foa(0)));
	xmlNewChild(node_foa, NULL, BAD_CAST "y", BAD_CAST double2char(foa(1)));
	xmlNewChild(node_foa, NULL, BAD_CAST "z", BAD_CAST double2char(foa(2)));


	// Intrisic parameters
	xmlNewChild(root_node, NULL, BAD_CAST "roll", BAD_CAST double2char(roll));
	xmlNewChild(root_node, NULL, BAD_CAST "fdistx", BAD_CAST double2char(fdistx));
	xmlNewChild(root_node, NULL, BAD_CAST "fdisty", BAD_CAST double2char(fdisty));
	xmlNewChild(root_node, NULL, BAD_CAST "u0", BAD_CAST double2char(u0));
	xmlNewChild(root_node, NULL, BAD_CAST "v0", BAD_CAST double2char(v0));
	xmlNewChild(root_node, NULL, BAD_CAST "skew", BAD_CAST double2char(skew));
	xmlNewChild(root_node, NULL, BAD_CAST "rows", BAD_CAST double2char(rows));
	xmlNewChild(root_node, NULL, BAD_CAST "columns", BAD_CAST double2char(columns));

	// K Matrix
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k11", BAD_CAST double2char(K(0,0)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k12", BAD_CAST double2char(K(0,1)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k13", BAD_CAST double2char(K(0,2)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k21", BAD_CAST double2char(K(1,0)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k22", BAD_CAST double2char(K(1,1)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k23", BAD_CAST double2char(K(1,2)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k31", BAD_CAST double2char(K(2,0)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k32", BAD_CAST double2char(K(2,1)));
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k33", BAD_CAST double2char(K(2,2)));


	// RT Matrix
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt11", BAD_CAST double2char(RT(0,0)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt12", BAD_CAST double2char(RT(0,1)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt13", BAD_CAST double2char(RT(0,2)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt14", BAD_CAST double2char(RT(0,3)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt21", BAD_CAST double2char(RT(1,0)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt22", BAD_CAST double2char(RT(1,1)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt23", BAD_CAST double2char(RT(1,2)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt24", BAD_CAST double2char(RT(1,3)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt31", BAD_CAST double2char(RT(2,0)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt32", BAD_CAST double2char(RT(2,1)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt33", BAD_CAST double2char(RT(2,2)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt34", BAD_CAST double2char(RT(2,3)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt41", BAD_CAST double2char(RT(3,0)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt42", BAD_CAST double2char(RT(3,1)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt43", BAD_CAST double2char(RT(3,2)));
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt44", BAD_CAST double2char(RT(3,3)));

	xmlSaveFormatFileEnc(filename.c_str(), doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

void Progeo::readFromFile(std::string filename){
	xmlDocPtr doc;
	xmlNodePtr cur, curAux, curAux_child;
	doc = xmlParseFile(filename.c_str());
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
	return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *) "calibration_camera")) {
		fprintf(stderr,"document of the wrong type, root node != calibration_camera");
		xmlFreeDoc(doc);
		return;
	}
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"calibration_camera"))){

			xmlChar *key;
			curAux = cur->xmlChildrenNode;

			while (curAux != NULL) {
				if ((!xmlStrcmp(curAux->name, (const xmlChar *)"roll"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->roll = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdistx"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->fdistx = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdisty"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->fdisty = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"u0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->u0 = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"v0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->v0 = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"skew"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->skew = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rows"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->rows = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"columns"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->columns = atof((const char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"position"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position(0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position(1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position(2) = atof((const char*)key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"foa"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa(0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa(1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa(2) = atof((const char*)key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"k_matrix"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k11"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(0,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(0,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(0,2) = atof((const char*)key);
							xmlFree(key);


						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(1,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(1,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(1,2) = atof((const char*)key);
							xmlFree(key);


						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(2,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(2,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->K(2,2) = atof((const char*)key);
							xmlFree(key);


						}
						curAux_child = curAux_child->next;
					}
				} else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rt_matrix"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt11"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(0,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(0,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(0,2) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(0,3) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(1,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(1,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(1,2) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt24"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(1,3) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(2,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(2,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(2,2) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt34"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(2,3) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt41"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(3,0) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt42"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(3,1) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt43"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(3,2) = atof((const char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt44"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->RT(3,3) = atof((const char*)key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}
				curAux = curAux->next;
			}
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
}


}
