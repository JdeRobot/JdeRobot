#include "progeo.h"

Progeo::Progeo()
{
}


void Progeo::xmlReader(std::string filename)
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
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"calibration_camera"))){

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
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdistx"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->fdistx = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdisty"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->fdisty = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"u0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->u0 = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"v0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->v0 = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"skew"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->skew = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rows"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->rows = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"columns"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					this->columns = atof((char*)key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"position"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position.vector(0) = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position.vector(1) = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->position.vector(2) = atof((char*)key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"foa"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa.vector(0) = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa.vector(1) = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->foa.vector(2) = atof((char*)key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"k_matrix"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k11"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k11 = atof((char*)key);
							K.getMatriz()(0,0) = this->k11;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k12 = atof((char*)key);
							K.getMatriz()(0,1) = this->k12;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k13 = atof((char*)key);
							K.getMatriz()(0,2) = this->k13;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k14 = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k21 = atof((char*)key);
							K.getMatriz()(1,0) = this->k21;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k22 = atof((char*)key);
							K.getMatriz()(1,1) = this->k22;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k23 = atof((char*)key);
							K.getMatriz()(1,2) = this->k23;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k24"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k24 = atof((char*)key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k31 = atof((char*)key);
							K.getMatriz()(2,0) = this->k31;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k32 = atof((char*)key);
							K.getMatriz()(2,1) = this->k32;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->k33 = atof((char*)key);
							K.getMatriz()(2,2) = this->k33;
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
                            RT.getMatrix()(0,0) = this->rt11;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt12 = atof((char*)key);
                            RT.getMatrix()(0,1) = this->rt12;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt13 = atof((char*)key);
                            RT.getMatrix()(0,2) = this->rt13;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt14 = atof((char*)key);
                            RT.getMatrix()(0,3) = this->rt14;
							xmlFree(key);

                        } else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt21 = atof((char*)key);
                            RT.getMatrix()(1,0) = this->rt21;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt22 = atof((char*)key);
                            RT.getMatrix()(1,1) = this->rt22;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt23 = atof((char*)key);
                            RT.getMatrix()(1,2) = this->rt23;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt24"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt24 = atof((char*)key);
                            RT.getMatrix()(1,3) = this->rt24;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt31 = atof((char*)key);
                            RT.getMatrix()(2,0) = this->rt31;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt32 = atof((char*)key);
                            RT.getMatrix()(2,1) = this->rt32;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt33 = atof((char*)key);
                            RT.getMatrix()(2,2) = this->rt33;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt34"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt34 = atof((char*)key);
                            RT.getMatrix()(2,3) = this->rt34;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt41"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt41 = atof((char*)key);
                            RT.getMatrix()(3,0) = this->rt41;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt42"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt42 = atof((char*)key);
                            RT.getMatrix()(3,1) = this->rt42;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt43"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt43 = atof((char*)key);
                            RT.getMatrix()(3,2) = this->rt43;
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt44"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							this->rt44 = atof((char*)key);
                            RT.getMatrix()(3,3) = this->rt44;
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

void Progeo::display_camerainfo(){
  printf("------------------------------------------------------\n");
  printf("Camera %s\n\n", this->name.c_str());
  printf("     Position: (X,Y,Z,H)=(%.1f,%.1f,%.1f,%.1f)\n",   position.getX(),
                                                               position.getY(),
                                                               position.getZ(),
                                                               position.getH());
  printf("     Focus of Attention: (x,y,z,h)=(%.1f,%.1f,%.1f,%.1f)\n\n",  foa.getX(),
                                                                          foa.getY(),
                                                                          foa.getZ(),
                                                                          foa.getH());
  printf("     Focus DistanceX(vertical): %.1f mm\n",fdistx);
  printf("     Focus DistanceY(horizontal): %.1f mm\n",fdisty);
  
  printf("     Skew: %.5f \n",skew);
  printf("     Optical Center: (x,y)=(%.1f,%.1f)\n\n",u0,v0);

//  std::cout << "\t\tK Matrix:\t" << k11 << " " << k12 << " " << k13 << " " << k14 << std::endl;
//  std::cout << "\t\t\t" << k21 << " " << k22 << " " << k23 << " " << k24 << std::endl;
//  std::cout << "\t\t\t" << k31 << " " << k32 << " " << k33 << " " << k34 << std::endl;
  //std::cout << "\t\t\t\t" << k41 << " " << k42 << " " << k43 << " " << k44 << std::endl;
  
  std::cout << "K Matrix: \n" << K.getMatriz() << std::endl;
  
//  std::cout << "\t\tR&T Matrix:\t" << rt11 << " " << rt12 << " " << rt13 << " " << rt14 << std::endl;
//  std::cout << "\t\t\t" << rt21 << " " << rt22 << " " << rt23 << " " << rt24 << std::endl;
//  std::cout << "\t\t\t" << rt31 << " " << rt32 << " " << rt33 << " " << rt34 << std::endl;
//  std::cout << "\t\t\t" << rt41 << " " << rt42 << " " << rt43 << " " << rt44 << std::endl;
  
  std::cout <<" R&T Matrix:\n"<<  RT.getMatrix() << std::endl;

  printf("------------------------------------------------------\n");
}

int Progeo::project(math::Vector3H in, math::Vector2H &out)
/* returns -1 if the point lies behind the camera,
   returns 1 if "in" 3Dpoint projects into a 2D finite point,
   0 otherwise */
{
    math::Vector3H a;
    a.vector = RT.getCopyMatrix()*in.vector;

    out.vector = K.getCopyMatriz()*a.vector.head(3);

    // optical 2 graphics
    out.setX(out.getX()/out.getH());
    out.setY(out.getY()/out.getH());
    out.setH(1.0);

    float aux =out.getX();
    out.setX(out.getY());
    out.setY(this->rows-1-aux);

    if (out.getH()!=0.){
        return 1;
    }else{
        return 0;
    }
}

void Progeo::backproject(math::Vector2H point, math::Vector3H& pro)
{
	
    //GRAPHIC_TO_OPTICAL
    int opX = this->rows -1 -point.getY();
    int opY = point.getX();

    math::Matriz3x3 ik;
    ik.getMatriz()= K.getCopyMatriz();
    ik.getMatriz() = ik.getCopyMatriz().inverse();

    math::Vector2H Pi(opX*this->k11/point.getH(), opY*this->k11/point.getH(),this->k11);

    math::Vector2H a;
    a.vector = ik.getCopyMatriz()*Pi.vector;

    math::Vector3H aH;
    aH.setX(a.getX());
    aH.setY(a.getY());
    aH.setZ(a.getH());
    aH.setH(1.0);

    math::Matriz4x4 RT2;
    RT2.getMatrix() = RT.getCopyMatrix();

    RT2.getMatrix()(0, 3) = .0;
    RT2.getMatrix()(1, 3) = .0;
    RT2.getMatrix()(2, 3) = .0;
    RT2.getMatrix()(3, 3) = 1.0;


    math::Vector3H b;

    b.vector = RT2.getCopyMatrix()*aH.vector;

    math::Matriz4x4 Translate(math::Matriz4x4::IDENTITY);
    Translate.getMatrix()(0, 3) = position.getX();
    Translate.getMatrix()(1, 3) = position.getY();
    Translate.getMatrix()(2, 3) = position.getZ();

    b.vector = Translate.getCopyMatrix()*b.vector;

    pro.setX(b.getX()/b.getH());
    pro.setY(b.getY()/b.getH());
    pro.setZ(b.getZ()/b.getH());
    pro.setH(b.getH());
}

void Progeo::update_camera_matrix()
{}
//{
//  float rc11,rc12,rc13,rc21,rc22,rc23,rc31,rc32,rc33;
//  float rab11,rab12,rab13,rab21,rab22,rab23,rab31,rab32,rab33;
//  float r;
//  /* a very small value but not infinite!! */
//  float SMALL=0.0001;
//  double t;
//  float ux,uy,uz,vx,vy,vz,wx,wy,wz;

//  camera->foa.H=1.;
//  camera->position.H=1;

//  /* Orientation model: focus of attention + roll */
//  rab31=camera->foa.X-camera->position.X;
//  rab32=camera->foa.Y-camera->position.Y;
//  rab33=camera->foa.Z-camera->position.Z;
//  r=(float)sqrt((double)(rab31*rab31+rab32*rab32+rab33*rab33));
//  rab31=rab31/r; rab32=rab32/r; rab33=rab33/r;

//  /* Second method:*/
//  wx=rab31;
//  wy=rab32;
//  wz=rab33;
//  t = atan2(-wx,wy);
//  vx=(float)cos(t);
//  vy=(float)sin(t);
//  vz=0.;
//  ux=vy*wz-wy*vz;
//  uy=-vx*wz+wx*vz;
//  uz=vx*wy-wx*vy;
//  if (uz<0.)
//    {vx=-vx; vy=-vy; vz=-vz;
//      ux=-ux; uy=-uy; uz=-uz;}
//  rab11=ux;
//  rab12=uy;
//  rab13=uz;
//  rab21=vx;
//  rab22=vy;
//  rab23=vz;

//  /* First method:
//   * this was commented in the previous version. only else branch of this if was valid. test it!!*

//  if ((rab31<SMALL) && (rab31>-SMALL) && (rab32<SMALL) && (rab32>-SMALL))
//    * u3 = OZ or FA=camera position *
//    {
//      rab11=1.; rab12=0.; rab13=0.;
//      rab21=0.; rab22=1.; rab23=0.;
//      rab31=0.; rab32=0.; rab33=1.;
//    }else{
//    rab11=rab31*rab33; rab12=rab32*rab33; rab13=-rab31*rab31-rab32*rab32;
//    r=(float)sqrt((double)(rab11*rab11+rab12*rab12+rab13*rab13));
//    rab11=rab11/r; rab12=rab12/r; rab13=rab13/r;
//    if (rab13<0.) {rab11=-rab11; rab12=-rab12; rab13=-rab13;}

//    rab21=rab32*rab13-rab12*rab33; rab22=rab11*rab33-rab31*rab13; rab23=rab31*rab12-rab11*rab32;
//    r=(float)sqrt((double)(rab21*rab21+rab22*rab22+rab23*rab23));
//    rab21=rab21/r; rab22=rab22/r; rab23=rab23/r;
//  }
//  */

//  rc11=cos(camera->roll); rc12=sin(camera->roll); rc13=0.;
//  rc21=-sin(camera->roll); rc22=cos(camera->roll); rc23=0.;
//  rc31=0.; rc32=0.; rc33=1.;

//  camera->rt11=rc11*rab11+rc12*rab21+rc13*rab31;
//  camera->rt12=rc11*rab12+rc12*rab22+rc13*rab32;
//  camera->rt13=rc11*rab13+rc12*rab23+rc13*rab33;
//  camera->rt21=rc21*rab11+rc22*rab21+rc23*rab31;
//  camera->rt22=rc21*rab12+rc22*rab22+rc23*rab32;
//  camera->rt23=rc21*rab13+rc22*rab23+rc23*rab33;
//  camera->rt31=rc31*rab11+rc32*rab21+rc33*rab31;
//  camera->rt32=rc31*rab12+rc32*rab22+rc33*rab32;
//  camera->rt33=rc31*rab13+rc32*rab23+rc33*rab33;

//  camera->rt14=-camera->position.X*camera->rt11-camera->position.Y*camera->rt12-camera->position.Z*camera->rt13;
//  camera->rt24=-camera->position.X*camera->rt21-camera->position.Y*camera->rt22-camera->position.Z*camera->rt23;
//  camera->rt34=-camera->position.X*camera->rt31-camera->position.Y*camera->rt32-camera->position.Z*camera->rt33;
//  camera->rt41=0.;
//  camera->rt42=0.;
//  camera->rt43=0.;
//  camera->rt44=1.;

//  /* intrinsics parameters */
//  camera->k11=camera->fdistx;  camera->k12=camera->skew*camera->fdisty; camera->k13=camera->u0; camera->k14=0.;
//  camera->k21=0.; camera->k22=camera->fdisty;  camera->k23=camera->v0; camera->k24=0.;
//  camera->k31=0.; camera->k32=0.; camera->k33=1.; camera->k34=0.;

//  if (debug==1) printf("Camera %s Located at (%.f,%.f,%.f)\n",camera->name,camera->position.X,camera->position.Y,camera->position.Z);
//  if (debug==1) printf("Camera %s Orientation: pointing towards FocusOfAtention (%.f,%.f,%.f), roll (%.f)\n",camera->name,camera->foa.X,camera->foa.Y,camera->foa.Z,camera->roll*360./(2*PI));
//  if (debug==1) printf("Camera %s fx= %.5f fy= %.5f skew= %.5f y0=%d x0=%d\n",camera->name, camera->fdistx, camera->fdisty, camera->skew,(int)camera->v0,(int)camera->u0);
//  if (debug==1) printf("Camera %s K matrix\n %.3f %.1f %.1f %.1f\n %.1f %.3f %.1f %.1f\n %.1f %.1f %.1f %.1f\n",camera->name,camera->k11,camera->k12,camera->k13,camera->k14,camera->k21,camera->k22,camera->k23,camera->k24,camera->k31,camera->k32,camera->k33,camera->k34);
//  if (debug==1) printf("Camera %s RT matrix\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n",camera->name,camera->rt11,camera->rt12,camera->rt13,camera->rt14,camera->rt21,camera->rt22,camera->rt23,camera->rt24,camera->rt31,camera->rt32,camera->rt33,camera->rt34,camera->rt41,camera->rt42,camera->rt43,camera->rt44);
//}
