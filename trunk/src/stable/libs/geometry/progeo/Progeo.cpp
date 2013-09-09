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
    foa(1) = 0.;
    foa(2) = 0.;
    foa(3) = 0.;
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

void Progeo::display_camerainfo() {
    printf("------------------------------------------------------\n");
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

    //  std::cout << "\t\tK Matrix:\t" << k11 << " " << k12 << " " << k13 << " " << k14 << std::endl;
    //  std::cout << "\t\t\t" << k21 << " " << k22 << " " << k23 << " " << k24 << std::endl;
    //  std::cout << "\t\t\t" << k31 << " " << k32 << " " << k33 << " " << k34 << std::endl;
    //std::cout << "\t\t\t\t" << k41 << " " << k42 << " " << k43 << " " << k44 << std::endl;

    std::cout << "K Matrix: \n" << K << std::endl;

    //  std::cout << "\t\tR&T Matrix:\t" << rt11 << " " << rt12 << " " << rt13 << " " << rt14 << std::endl;
    //  std::cout << "\t\t\t" << rt21 << " " << rt22 << " " << rt23 << " " << rt24 << std::endl;
    //  std::cout << "\t\t\t" << rt31 << " " << rt32 << " " << rt33 << " " << rt34 << std::endl;
    //  std::cout << "\t\t\t" << rt41 << " " << rt42 << " " << rt43 << " " << rt44 << std::endl;

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

    // optical 2 graphics
    out(0) = out(0)/out(2);
    out(1) = out(1)/out(2);
    out(2) = 1.0;

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
    int opX = this->rows -1 -point(1);
    int opY = point(0);

    Eigen::Matrix3d ik;
    ik = K;
    ik = ik.inverse().eval();

    Eigen::Vector3d Pi(opX*K(0,0)/point(2), opY*K(0,0)/point(2),K(0,0));

    Eigen::Vector3d a;
    a = ik*Pi;

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

    b = RT2*aH;

    Eigen::Matrix4d Translate;
    Translate.setIdentity();
    Translate(0, 3) = position(0);
    Translate(1, 3) = position(1);
    Translate(2, 3) = position(2);

    b = Translate*b;

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

  void Progeo::update_camera_matrix()
  {}

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
}
