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

#define BIGNUM 1.0e4

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
	/*double aux = out(0);
	out(0) = out(1);
	out(1) = this->rows-1-aux;*/

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

std::string Progeo::double2char(double d)
{
	std::stringstream ss;
	ss << d;
	return ss.str();
}

void Progeo::saveToFile (std::string filename, bool updateMatrix)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	xmlNodePtr node_position = NULL;
	xmlNodePtr node_foa = NULL;
	xmlNodePtr node_k_matrix = NULL;
	xmlNodePtr node_rt_matrix = NULL;

	if (updateMatrix)
	{
		updateKMatrix();
		updateRTMatrix();
	}

	LIBXML_TEST_VERSION;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "calibration_camera");
	node_position = xmlNewChild(root_node, NULL, BAD_CAST "position", NULL);
	node_foa = xmlNewChild(root_node, NULL, BAD_CAST "foa", NULL);
	node_k_matrix = xmlNewChild(root_node, NULL, BAD_CAST "k_matrix", NULL);
	node_rt_matrix = xmlNewChild(root_node, NULL, BAD_CAST "rt_matrix", NULL);

	xmlDocSetRootElement(doc, root_node);

	// Camera position
	xmlNewChild(node_position, NULL, BAD_CAST "x", BAD_CAST double2char((double)(position(0))).c_str());
	xmlNewChild(node_position, NULL, BAD_CAST "y", BAD_CAST double2char((double)(position(1))).c_str());
	xmlNewChild(node_position, NULL, BAD_CAST "z", BAD_CAST double2char((double)(position(2))).c_str());

	// Foa position
	xmlNewChild(node_foa, NULL, BAD_CAST "x", BAD_CAST double2char((double)foa(0)).c_str());
	xmlNewChild(node_foa, NULL, BAD_CAST "y", BAD_CAST double2char((double)foa(1)).c_str());
	xmlNewChild(node_foa, NULL, BAD_CAST "z", BAD_CAST double2char((double)foa(2)).c_str());


	// Intrisic parameters
	xmlNewChild(root_node, NULL, BAD_CAST "roll", BAD_CAST double2char(roll).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "fdistx", BAD_CAST double2char(fdistx).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "fdisty", BAD_CAST double2char(fdisty).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "u0", BAD_CAST double2char(u0).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "v0", BAD_CAST double2char(v0).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "skew", BAD_CAST double2char(skew).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "rows", BAD_CAST double2char(rows).c_str());
	xmlNewChild(root_node, NULL, BAD_CAST "columns", BAD_CAST double2char(columns).c_str());

	// K Matrix
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k11", BAD_CAST double2char((double)K(0,0)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k12", BAD_CAST double2char((double)K(0,1)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k13", BAD_CAST double2char((double)K(0,2)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k21", BAD_CAST double2char((double)K(1,0)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k22", BAD_CAST double2char((double)K(1,1)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k23", BAD_CAST double2char((double)K(1,2)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k31", BAD_CAST double2char((double)K(2,0)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k32", BAD_CAST double2char((double)K(2,1)).c_str());
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k33", BAD_CAST double2char((double)K(2,2)).c_str());


	// RT Matrix
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt11", BAD_CAST double2char((double)RT(0,0)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt12", BAD_CAST double2char((double)RT(0,1)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt13", BAD_CAST double2char((double)RT(0,2)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt14", BAD_CAST double2char((double)RT(0,3)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt21", BAD_CAST double2char((double)RT(1,0)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt22", BAD_CAST double2char((double)RT(1,1)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt23", BAD_CAST double2char((double)RT(1,2)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt24", BAD_CAST double2char((double)RT(1,3)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt31", BAD_CAST double2char((double)RT(2,0)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt32", BAD_CAST double2char((double)RT(2,1)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt33", BAD_CAST double2char((double)RT(2,2)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt34", BAD_CAST double2char((double)RT(2,3)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt41", BAD_CAST double2char((double)RT(3,0)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt42", BAD_CAST double2char((double)RT(3,1)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt43", BAD_CAST double2char((double)RT(3,2)).c_str());
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt44", BAD_CAST double2char((double)RT(3,3)).c_str());

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

int Progeo::displayline(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d& a, Eigen::Vector3d& b)
/* it takes care of important features: before/behind the focal plane, inside/outside the image frame */
{

	Eigen::Vector3d l,l0,l1,l2,l3; /* in fact lines in homogeneous coordinates */
	Eigen::Vector3d pa,pb,i0,i1,i2,i3,gooda,goodb;
  int mycase=0;
  float Xmax,Xmin,Ymax,Ymin;
  float papb=0.;

  Xmin=0.;
  Xmax=this->rows-1.;
  Ymin=0.;
  Ymax=this->columns-1.;
  /* Watchout!: they can't reach camera.rows or camera.columns, their are not valid values for the pixels */

  l0(1)=0.; l0(1)=1.; l0(2)=-Ymin;
  l1(1)=0.; l1(1)=1.; l1(2)=-Ymax;
  l2(1)=1.; l2(1)=0.; l2(2)=-Xmax;
  l3(1)=1.; l3(1)=0.; l3(2)=-Xmin;

  if ((p1(2)<0.)&&(p2(2)<0.)){
    /* both points behind the focal plane: don't display anything */
    mycase=10;

  }else{
    if ((p1(2)>0.)&&(p2(2)<0.)){
        /* p1 before the focal plane, p2 behind */
		/*Calculates p2 = p1 + -inf(p2-p1)*/
		p2(0) = p1(1) + (-BIGNUM)*(p2(1)-p1(1));
		p2(1) = p1(1) + (-BIGNUM)*(p2(1)-p1(1));
		p2(2)=-p2(2);/* undo the "project" trick to get the right value */
    }else if ((p1(2)<0.)&&(p2(2)>0.)){
        /* p2 before the focal plane, p1 behind */
		/*Calculates p1 = p2 + -inf(p1-p2)*/
		p1(1) = p2(1) + (-BIGNUM)*(p1(1)-p2(1));
		p1(1) = p2(1) + (-BIGNUM)*(p1(1)-p2(1));
		p1(2)=-p1(2);/* undo the "project" trick to get the right value */
    }

    /* both points before the focal plane */
    if ((p1(1)>=Xmin) && (p1(1)<Xmax+1) && (p1(1)>=Ymin) && (p1(1)<Ymax+1) &&
	(p2(1)>=Xmin) && (p2(1)<Xmax+1) && (p2(1)>=Ymin) && (p2(1)<Ymax+1)){
      /* both inside the image limits */

      gooda(1)=p1(1); gooda(1)=p1(1); gooda(2)=p1(2);
      goodb(1)=p2(1); goodb(1)=p2(1); goodb(2)=p2(2);
      mycase=2;

    }else if ((p1(1)>=Xmin) && (p1(1)<Xmax+1) && (p1(1)>=Ymin) && (p1(1)<Ymax+1) &&
	      ((p2(1)<Xmin) || (p2(1)>=Xmax+1) || (p2(1)<Ymin) || (p2(1)>=Ymax+1))){
      /* p1 inside, p2 outside */
      gooda(1)=p1(1); gooda(1)=p1(1); gooda(2)=p1(2);
      goodb(1)=p1(1); goodb(1)=p1(1); goodb(2)=p1(2);
      pa(1)=p1(1); pa(1)=p1(1); pa(2)=p1(2);
      pb(1)=p2(1); pb(1)=p2(1); pb(2)=p2(2);
      mycase=3;

    }else if ((p2(1)>=Xmin) && (p2(1)<Xmax+1) && (p2(1)>=Ymin) && (p2(1)<Ymax+1) &&
	      ((p1(1)<Xmin) || (p1(1)>=Xmax+1) || (p1(1)<Ymin) || (p1(1)>=Ymax+1))){
      /* p2 inside, p1 outside */

      gooda(1)=p2(1); gooda(1)=p2(1); gooda(2)=p2(2);
      goodb(1)=p2(1); goodb(1)=p2(1); goodb(2)=p2(2);
      pa(1)=p2(1); pa(1)=p2(1); pa(2)=p2(2);
      pb(1)=p1(1); pb(1)=p1(1); pb(2)=p1(2);
      mycase=4;

    }else{
      /* both outside */
      pa(1)=p2(1); pa(1)=p2(1); pa(2)=p2(2);
      pb(1)=p1(1); pb(1)=p1(1); pb(2)=p1(2);
      mycase=5;
    }
    l(1)=pa(1)*pb(2)-pb(1)*pa(2); l(1)=pb(1)*pa(2)-pa(1)*pb(2); l(2)=pa(1)*pb(1)-pb(1)*pa(1);
    i0(1)=l(1)*l0(2)-l(2)*l0(1); i0(1)=l(2)*l0(1)-l(1)*l0(2); i0(2)=l(1)*l0(1)-l(1)*l0(1);
    i1(1)=l(1)*l1(2)-l(2)*l1(1); i1(1)=l(2)*l1(1)-l(1)*l1(2); i1(2)=l(1)*l1(1)-l(1)*l1(1);
    i2(1)=l(1)*l2(2)-l(2)*l2(1); i2(1)=l(2)*l2(1)-l(1)*l2(2); i2(2)=l(1)*l2(1)-l(1)*l2(1);
    i3(1)=l(1)*l3(2)-l(2)*l3(1); i3(1)=l(2)*l3(1)-l(1)*l3(2); i3(2)=l(1)*l3(1)-l(1)*l3(1);
    if (i0(2)!=0.) i0(1)=i0(1)/i0(2); i0(1)=i0(1)/i0(2); i0(2)=1.;
    if (i1(2)!=0.) i1(1)=i1(1)/i1(2); i1(1)=i1(1)/i1(2); i1(2)=1.;
    if (i2(2)!=0.) i2(1)=i2(1)/i2(2); i2(1)=i2(1)/i2(2); i2(2)=1.;
    if (i3(2)!=0.) i3(1)=i3(1)/i3(2); i3(1)=i3(1)/i3(2); i3(2)=1.;

    papb=(pb(1)-pa(1))*(pb(1)-pa(1))+(pb(1)-pa(1))*(pb(1)-pa(1));

	float maxdot = -1;

    if (i0(2)!=0.){
      if ((i0(1)>=Xmin) && (i0(1)<Xmax+1) && (i0(1)>=Ymin) && (i0(1)<Ymax+1)){
	if ((((pb(1)-pa(1))*(i0(1)-pa(1))+(pb(1)-pa(1))*(i0(1)-pa(1)))>=0.) &&
	    (((pb(1)-pa(1))*(i0(1)-pa(1))+(pb(1)-pa(1))*(i0(1)-pa(1)))<papb) &&
		(((pb(1)-pa(1))*(i0(1)-pa(1))+(pb(1)-pa(1))*(i0(1)-pa(1)))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb(1)=i0(1); goodb(1)=i0(1); goodb(2)=i0(2);
		maxdot = (pb(1)-pa(1))*(i0(1)-pa(1))+(pb(1)-pa(1))*(i0(1)-pa(1));

	  }else if (mycase==5){
	    gooda(1)=i0(1); gooda(1)=i0(1); gooda(2)=i0(2);
	    goodb(1)=i0(1); goodb(1)=i0(1); goodb(2)=i0(2);
	    mycase=6;
	  }
	}
      }
    }else; /* i0 at infinite, parallel lines */

    if (i1(2)!=0.){
      if ((i1(1)>=Xmin) && (i1(1)<Xmax+1) && (i1(1)>=Ymin) && (i1(1)<Ymax+1)){
	if ((((pb(1)-pa(1))*(i1(1)-pa(1))+(pb(1)-pa(1))*(i1(1)-pa(1)))>=0.)&&
	    (((pb(1)-pa(1))*(i1(1)-pa(1))+(pb(1)-pa(1))*(i1(1)-pa(1)))<papb) &&
		(((pb(1)-pa(1))*(i1(1)-pa(1))+(pb(1)-pa(1))*(i1(1)-pa(1)))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb(1)=i1(1); goodb(1)=i1(1); goodb(2)=i1(2);
		maxdot = (pb(1)-pa(1))*(i1(1)-pa(1))+(pb(1)-pa(1))*(i1(1)-pa(1));

	  }else if (mycase==5){
	    gooda(1)=i1(1); gooda(1)=i1(1); gooda(2)=i1(2);
	    goodb(1)=i1(1); goodb(1)=i1(1); goodb(2)=i1(2);
	    mycase=6;
	  }
	}
      }
    }else; /* i1 at infinite, parallel lines */

    if (i2(2)!=0.){
      if ((i2(1)>=Xmin) && (i2(1)<Xmax+1) && (i2(1)>=Ymin) && (i2(1)<Ymax+1)){
	if ((((pb(1)-pa(1))*(i2(1)-pa(1))+(pb(1)-pa(1))*(i2(1)-pa(1)))>=0.)&&
	    (((pb(1)-pa(1))*(i2(1)-pa(1))+(pb(1)-pa(1))*(i2(1)-pa(1)))<papb) &&
	    (((pb(1)-pa(1))*(i2(1)-pa(1))+(pb(1)-pa(1))*(i2(1)-pa(1)))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb(1)=i2(1); goodb(1)=i2(1); goodb(2)=i2(2);
		maxdot = (pb(1)-pa(1))*(i2(1)-pa(1))+(pb(1)-pa(1))*(i2(1)-pa(1));

	  }else if (mycase==5){
	    gooda(1)=i2(1); gooda(1)=i2(1); gooda(2)=i2(2);
	    goodb(1)=i2(1); goodb(1)=i2(1); goodb(2)=i2(2);
	    mycase=6;
	  }
	}
      }
    }else; /* i2 at infinite, parallel lines */

    if (i3(2)!=0.){
      if  ((i3(1)>=Xmin) && (i3(1)<Xmax+1) && (i3(1)>=Ymin) && (i3(1)<Ymax+1)){
	if ((((pb(1)-pa(1))*(i3(1)-pa(1))+(pb(1)-pa(1))*(i3(1)-pa(1)))>=0.) &&
	    (((pb(1)-pa(1))*(i3(1)-pa(1))+(pb(1)-pa(1))*(i3(1)-pa(1)))<papb) &&
	    (((pb(1)-pa(1))*(i3(1)-pa(1))+(pb(1)-pa(1))*(i3(1)-pa(1)))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb(1)=i3(1); goodb(1)=i3(1); goodb(2)=i3(2);
		maxdot = (pb(1)-pa(1))*(i3(1)-pa(1))+(pb(1)-pa(1))*(i3(1)-pa(1));

	  }else if (mycase==5){
	    gooda(1)=i3(1); gooda(1)=i3(1); gooda(2)=i3(2);
	    goodb(1)=i3(1); goodb(1)=i3(1); goodb(2)=i3(2);
	    mycase=6;
	  }
	}
      }
    }else; /* i3 at infinite, parallel lines */

  }

  /*if (debug==1){
    printf("p3: x=%.f y=%.f h=%.f\np2: x=%.f, y=%.f h=%.f\n",p1(1),p1(1),p1(2),p2(1),p2(1),p2(2));
    printf("case: %d\n i0: x=%.1f y=%.1f z=%.f dot=%.2f\n i1: x=%.1f y=%.1f z=%.f dot=%.2f\n i2: x=%.1f y=%.1f z=%.f dot=%.2f\n i3: x=%.1f y=%.1f z=%.f dot=%.2f\n",mycase,
	   i0(1),i0(1),i0(2),((pb(1)-pa(1))*(i0(1)-pa(1))+(pb(1)-pa(1))*(i0(1)-pa(1))),
	   i1(1),i1(1),i1(2),((pb(1)-pa(1))*(i1(1)-pa(1))+(pb(1)-pa(1))*(i1(1)-pa(1))),
	   i2(1),i2(1),i2(2),((pb(1)-pa(1))*(i2(1)-pa(1))+(pb(1)-pa(1))*(i2(1)-pa(1))),
	   i3(1),i3(1),i3(2),((pb(1)-pa(1))*(i3(1)-pa(1))+(pb(1)-pa(1))*(i3(1)-pa(1))));
    printf("gooda:  x=%.f y=%.f z=%.f\n",gooda(1),gooda(1),gooda(2));
    printf("goodb:  x=%.f y=%.f z=%.f\n",goodb(1),goodb(1),goodb(2));
  }*/

  a(0)=gooda(1); b(0)=goodb(1);
  a(1)=gooda(1); b(1)=goodb(1);
  a(2)=gooda(2); b(2)=goodb(2);

  if((mycase!=2)&&(mycase!=3)&&(mycase!=4)&&(mycase!=6)) return 0;
  else return 1;
}


}
