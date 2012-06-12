/*
 *
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Authors : Jose María Cañas Plaza <jmplaza@gsyc.es>
 *            Eduardo Perdices García <eperdices@gsyc.es>
 *            Antonio Pineda
 */

#include "progeo.h"
#include <math.h>
#include <stdio.h>

#include <stdlib.h>
#include <errno.h>
/*xml*/
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#define PI 3.141592654
#define BIGNUM 1.0e4

int debug=0;

TPinHoleCamera xmlReader(TPinHoleCamera* camera, const char *docname)
{
	xmlDocPtr doc;
	xmlNodePtr cur, curAux, curAux_child;
	doc = xmlParseFile(docname);
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

					camera->name = a;
					xmlFree(key);
				}else */if ((!xmlStrcmp(curAux->name, (const xmlChar *)"roll"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->roll = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdistx"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->fdistx = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"fdisty"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->fdisty = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"u0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->u0 = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"v0"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->v0 = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"skew"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->skew = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rows"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->rows = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"columns"))) {
					key = xmlNodeListGetString(doc, curAux->xmlChildrenNode, 1);
					camera->columns = atof(key);
					xmlFree(key);
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"position"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->position.X = atof(key);
							xmlFree(key);
							fprintf(stderr,"\n Hemos leído X \n");

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->position.Y = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->position.Z = atof(key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"foa"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"x"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->foa.X = atof(key);
							xmlFree(key);
							fprintf(stderr,"\n Hemos leído X \n");

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"y"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->foa.Y = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"z"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->foa.Z = atof(key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				}else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"k_matrix"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k11"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k11 = atof(key);
							xmlFree(key);
							fprintf(stderr,"\n Hemos leído X \n");

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k12 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k13 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k14 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k14 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k21 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k22 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k23 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k24"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k24 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k31 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k32 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k33 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"k34"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->k34 = atof(key);
							xmlFree(key);

						}
						curAux_child = curAux_child->next;
					}
				} else if ((!xmlStrcmp(curAux->name, (const xmlChar *)"rt_matrix"))) {
					curAux_child = curAux->xmlChildrenNode;
					
					while (curAux_child != NULL) {
						if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt11"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt11 = atof(key);
							xmlFree(key);
							fprintf(stderr,"\n Hemos leído X \n");

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt12"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt12 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt13"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt13 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt14 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt14"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt14 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt21"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt21 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt22"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt22 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt23"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt23 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt24"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt24 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt31"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt31 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt32"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt32 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt33"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt33 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt34"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt34 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt41"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt41 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt42"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt42 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt43"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt43 = atof(key);
							xmlFree(key);

						} else if ((!xmlStrcmp(curAux_child->name, (const xmlChar *)"rt44"))) {
							key = xmlNodeListGetString(doc, curAux_child->xmlChildrenNode, 1);
							camera->rt44 = atof(key);
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
	
void xmlWriter(TPinHoleCamera camera, const char *filename){
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, node_position = NULL, node_foa = NULL, node_k_matrix = NULL, node_rt_matrix = NULL;/* node pointers */
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


	unsigned char vOut[32];

	sprintf(vOut, "%f", camera.position.X);
	xmlNewChild(node_position, NULL, BAD_CAST "x", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.position.Y);
	xmlNewChild(node_position, NULL, BAD_CAST "y", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.position.X);
	xmlNewChild(node_position, NULL, BAD_CAST "Z", BAD_CAST vOut);

	sprintf(vOut, "%f", camera.foa.X);
	xmlNewChild(node_foa, NULL, BAD_CAST "x", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.foa.Y);
	xmlNewChild(node_foa, NULL, BAD_CAST "y", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.foa.X);
	xmlNewChild(node_foa, NULL, BAD_CAST "Z", BAD_CAST vOut);


	sprintf(vOut, "%f", camera.roll);
	xmlNewChild(root_node, NULL, BAD_CAST "roll", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.fdistx);
	xmlNewChild(root_node, NULL, BAD_CAST "fdistx", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.fdisty);
	xmlNewChild(root_node, NULL, BAD_CAST "fdisty", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.u0);
	xmlNewChild(root_node, NULL, BAD_CAST "u0", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.v0);
	xmlNewChild(root_node, NULL, BAD_CAST "v0", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.skew);
	xmlNewChild(root_node, NULL, BAD_CAST "skew", BAD_CAST vOut);
	sprintf(vOut, "%d", camera.rows);
	xmlNewChild(root_node, NULL, BAD_CAST "rows", BAD_CAST vOut);
	sprintf(vOut, "%d", camera.columns);
	xmlNewChild(root_node, NULL, BAD_CAST "columns", BAD_CAST vOut);

	sprintf(vOut, "%f", camera.k11);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k11", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k12);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k12", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k13);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k13", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k14);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k14", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k21);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k21", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k22);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k22", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k23);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k23", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k24);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k24", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k31);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k31", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k32);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k32", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k33);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k33", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.k34);
	xmlNewChild(node_k_matrix, NULL, BAD_CAST "k34", BAD_CAST vOut);


	sprintf(vOut, "%f", camera.rt11);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt11", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt12);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt12", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt13);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt13", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt14);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt14", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt21);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt21", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt22);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt22", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt23);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt23", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt24);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt24", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt31);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt31", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt32);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt32", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt33);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt33", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt34);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt34", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt31);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt31", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt32);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt32", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt33);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt33", BAD_CAST vOut);
	sprintf(vOut, "%f", camera.rt34);
	xmlNewChild(node_rt_matrix, NULL, BAD_CAST "rt34", BAD_CAST vOut);

	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);

	xmlFreeDoc(doc);

	xmlCleanupParser();

}


void reverse_update_camera_matrix(TPinHoleCamera *camera)
{


	camera->foa.H=1.;
	camera->position.H=1;

	/* intrinsics parameters */
		/*Default values for camera parameters*/
		camera->fdistx = 300.0;
		camera->fdisty = 300.0;
		camera->skew = 0.0;

		/* K matrix*/
		camera->k11=camera->fdistx;
		camera->k12=camera->skew*camera->fdisty;
		camera->k13=camera->u0;
		camera->k14=0.;
		camera->k21=0.;
		camera->k22=camera->fdisty;
		camera->k23=camera->v0;
		camera->k24=0.;
		camera->k31=0.;
		camera->k32=0.;
		camera->k33=1.;
		camera->k34=0.;


	/* extrinsics parameters */

	int s;
	gsl_matrix *M;
	gsl_vector* x;	

	/* para invertir las matriz M,Q,R */
	gsl_permutation* p = gsl_permutation_alloc (3);
  
	/* para resolver el centro de la camara usando Mx=C 
	donde C es el verctor p4 de la matriz P */
	gsl_vector* p4 = gsl_vector_alloc(3);
	x =gsl_vector_alloc(3);
	
	M = gsl_matrix_alloc(3,3);

	/* Copiamos la submatriz 3x3 Izq de la solucion P a la matriz M */
	gsl_matrix_set(M,0,0,camera->rt11);
	gsl_matrix_set(M,0,1,camera->rt12);
	gsl_matrix_set(M,0,2,camera->rt13);

	gsl_matrix_set(M,1,0,camera->rt21);
	gsl_matrix_set(M,1,1,camera->rt22);
	gsl_matrix_set(M,1,2,camera->rt23);

	gsl_matrix_set(M,2,0,camera->rt31);
	gsl_matrix_set(M,2,1,camera->rt32);
	gsl_matrix_set(M,2,2,camera->rt33);

	/* Copiamos el vector p4 */
	gsl_vector_set(p4,0,camera->rt14);
	gsl_vector_set(p4,1,camera->rt24);
	gsl_vector_set(p4,2,camera->rt34);

	/* invertimos la matriz M */
	gsl_linalg_LU_decomp (M, p, &s);
	gsl_linalg_LU_solve(M,p,p4,x);


	printf("Posicion\n");
	  camera->position.X = -gsl_vector_get(x,0);
	  camera->position.Y = -gsl_vector_get(x,1);
	  camera->position.Z = -gsl_vector_get(x,2);



	/*
	El foa se puede sacar desde la KRT
	utilizando backproject, retroproyectas el centro óptico y lo unes con la
	posición de la cámara, coges cualquier punto de esa recta en 3D que esté
	delante de la cámara y listo.
	*/
	HPoint3D out;
	HPoint2D in;
	in.x = 0;
	in.y = 0;
	in.h = 0;

	backproject(&out, in, camera);

	camera->foa.X = out.X;
	camera->foa.Y = out.Y;
	camera->foa.Z = out.Z;


	/*
 	Roll: The angle between a line in RT matrix and RT matrix without roll

	1- Backproject para estar seguro de que no queda fuera el punto
	--2- Project el punto con la matriz RT parcial
	3- Project el punto con la matriz RT total
	4- Producto escalar de ambos vectores
	5- Paso a radianes

	*/

	HPoint3D p3;
	HPoint2D p2_rt_parcial;
	HPoint2D p2_rt_complete;
	p2_rt_complete.x = 1;
	p2_rt_complete.y = 1;
	p2_rt_complete.h = 0;

	backproject(&p3, p2_rt_complete, camera);

	TPinHoleCamera cameraAux; 
	cameraAux = *camera;

	update_camera_matrix(&cameraAux);
	
	project(p3, &p2_rt_parcial, cameraAux);

	camera->roll = 
		(float)acos ( 
			(p2_rt_parcial.x*p2_rt_complete.x + p2_rt_parcial.y*p2_rt_complete.y) 
			/ 
			(sqrt((double)p2_rt_parcial.x*p2_rt_parcial.x + p2_rt_parcial.y*p2_rt_parcial.y) 
				* sqrt((double)p2_rt_complete.x*p2_rt_complete.x + p2_rt_complete.y*p2_rt_complete.y)
			) 
		); 

		
}
void update_camera_matrix(TPinHoleCamera *camera)
{
  float rc11,rc12,rc13,rc21,rc22,rc23,rc31,rc32,rc33;
  float rab11,rab12,rab13,rab21,rab22,rab23,rab31,rab32,rab33;
  float r;
  /* a very small value but not infinite!! */
  float SMALL=0.0001;
  double t;
  float ux,uy,uz,vx,vy,vz,wx,wy,wz;
	
  camera->foa.H=1.;
  camera->position.H=1;
	
  /* Orientation model: focus of attention + roll */
  rab31=camera->foa.X-camera->position.X;
  rab32=camera->foa.Y-camera->position.Y; 
  rab33=camera->foa.Z-camera->position.Z;
  r=(float)sqrt((double)(rab31*rab31+rab32*rab32+rab33*rab33)); 
  rab31=rab31/r; rab32=rab32/r; rab33=rab33/r;

  /* Second method:*/
  wx=rab31;
  wy=rab32;
  wz=rab33;
  t = atan2(-wx,wy);
  vx=(float)cos(t);
  vy=(float)sin(t);
  vz=0.;
  ux=vy*wz-wy*vz;
  uy=-vx*wz+wx*vz;
  uz=vx*wy-wx*vy;
  if (uz<0.)
    {vx=-vx; vy=-vy; vz=-vz; 
      ux=-ux; uy=-uy; uz=-uz;}      
  rab11=ux;
  rab12=uy;
  rab13=uz;
  rab21=vx;
  rab22=vy;
  rab23=vz;

  /* First method:
   * this was commented in the previous version. only else branch of this if was valid. test it!!*	
  
  if ((rab31<SMALL) && (rab31>-SMALL) && (rab32<SMALL) && (rab32>-SMALL))
    * u3 = OZ or FA=camera position *
    {
      rab11=1.; rab12=0.; rab13=0.;
      rab21=0.; rab22=1.; rab23=0.;
      rab31=0.; rab32=0.; rab33=1.;
    }else{
    rab11=rab31*rab33; rab12=rab32*rab33; rab13=-rab31*rab31-rab32*rab32;
    r=(float)sqrt((double)(rab11*rab11+rab12*rab12+rab13*rab13));
    rab11=rab11/r; rab12=rab12/r; rab13=rab13/r;
    if (rab13<0.) {rab11=-rab11; rab12=-rab12; rab13=-rab13;}
		
    rab21=rab32*rab13-rab12*rab33; rab22=rab11*rab33-rab31*rab13; rab23=rab31*rab12-rab11*rab32;
    r=(float)sqrt((double)(rab21*rab21+rab22*rab22+rab23*rab23));
    rab21=rab21/r; rab22=rab22/r; rab23=rab23/r;
  }
  */
		
  rc11=cos(camera->roll); rc12=sin(camera->roll); rc13=0.;
  rc21=-sin(camera->roll); rc22=cos(camera->roll); rc23=0.;
  rc31=0.; rc32=0.; rc33=1.;

  camera->rt11=rc11*rab11+rc12*rab21+rc13*rab31;
  camera->rt12=rc11*rab12+rc12*rab22+rc13*rab32;
  camera->rt13=rc11*rab13+rc12*rab23+rc13*rab33;
  camera->rt21=rc21*rab11+rc22*rab21+rc23*rab31;
  camera->rt22=rc21*rab12+rc22*rab22+rc23*rab32;
  camera->rt23=rc21*rab13+rc22*rab23+rc23*rab33;
  camera->rt31=rc31*rab11+rc32*rab21+rc33*rab31;
  camera->rt32=rc31*rab12+rc32*rab22+rc33*rab32;
  camera->rt33=rc31*rab13+rc32*rab23+rc33*rab33;

  camera->rt14=-camera->position.X*camera->rt11-camera->position.Y*camera->rt12-camera->position.Z*camera->rt13;
  camera->rt24=-camera->position.X*camera->rt21-camera->position.Y*camera->rt22-camera->position.Z*camera->rt23;
  camera->rt34=-camera->position.X*camera->rt31-camera->position.Y*camera->rt32-camera->position.Z*camera->rt33;
  camera->rt41=0.;
  camera->rt42=0.;
  camera->rt43=0.;
  camera->rt44=1.;
	
  /* intrinsics parameters */
  camera->k11=camera->fdistx;  camera->k12=camera->skew*camera->fdisty; camera->k13=camera->u0; camera->k14=0.; 
  camera->k21=0.; camera->k22=camera->fdisty;  camera->k23=camera->v0; camera->k24=0.;
  camera->k31=0.; camera->k32=0.; camera->k33=1.; camera->k34=0.;
		 		
  if (debug==1) printf("Camera %s Located at (%.f,%.f,%.f)\n",camera->name,camera->position.X,camera->position.Y,camera->position.Z);
  if (debug==1) printf("Camera %s Orientation: pointing towards FocusOfAtention (%.f,%.f,%.f), roll (%.f)\n",camera->name,camera->foa.X,camera->foa.Y,camera->foa.Z,camera->roll*360./(2*PI));
  if (debug==1) printf("Camera %s fx= %.5f fy= %.5f skew= %.5f y0=%d x0=%d\n",camera->name, camera->fdistx, camera->fdisty, camera->skew,(int)camera->v0,(int)camera->u0);
  if (debug==1) printf("Camera %s K matrix\n %.3f %.1f %.1f %.1f\n %.1f %.3f %.1f %.1f\n %.1f %.1f %.1f %.1f\n",camera->name,camera->k11,camera->k12,camera->k13,camera->k14,camera->k21,camera->k22,camera->k23,camera->k24,camera->k31,camera->k32,camera->k33,camera->k34);
  if (debug==1) printf("Camera %s RT matrix\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n %.1f %.1f %.1f %.1f\n",camera->name,camera->rt11,camera->rt12,camera->rt13,camera->rt14,camera->rt21,camera->rt22,camera->rt23,camera->rt24,camera->rt31,camera->rt32,camera->rt33,camera->rt34,camera->rt41,camera->rt42,camera->rt43,camera->rt44);	
}

void update_stereocamera_matrix(TPinHoleStereocamera *stereo)
{
  float rc11,rc12,rc13,rc21,rc22,rc23,rc31,rc32,rc33;
  float rab11,rab12,rab13,rab21,rab22,rab23,rab31,rab32,rab33;
  float r,u21,u22,u23;
	
  /* Orientation model: focus of attention + roll */
  stereo->foa.H=1.;
  stereo->position.H=1;
  rab31=stereo->foa.X-stereo->position.X; 
  rab32=stereo->foa.Y-stereo->position.Y; 
  rab33=stereo->foa.Z-stereo->position.Z;
  r=(float)sqrt((double)(rab31*rab31+rab32*rab32+rab33*rab33));
	
  /* this was commented in the previous version. only else branch of this if was valid. test it!!*/
  if (r<000000001.){ rab31=0.; rab32=0.; rab33=1.; /*printf("Watchout C0: Camera and FocusOfAttention are the same\n");*/} 
  else{ rab31=rab31/r; rab32=rab32/r; rab33=rab33/r;}
	
  rab11=rab31*rab33; rab12=rab32*rab33; rab13=-rab31*rab31-rab32*rab32;
  r=(float)sqrt((double)(rab11*rab11+rab12*rab12+rab13*rab13));
  rab11=rab11/r; rab12=rab12/r; rab13=rab13/r;
  if (rab13<0.) {rab11=-rab11; rab12=-rab12; rab13=-rab13;}
	 
  rab21=rab32*rab13-rab12*rab33; rab22=rab11*rab33-rab31*rab13; rab23=rab31*rab12-rab11*rab32;
  r=(float)sqrt((double)(rab21*rab21+rab22*rab22+rab23*rab23));
  rab21=rab21/r; rab22=rab22/r; rab23=rab23/r;
	 
  rc11=cos(stereo->roll); rc12=sin(stereo->roll); rc13=0.;
  rc21=-sin(stereo->roll); rc22=cos(stereo->roll); rc23=0.;
  rc31=0.; rc32=0.; rc33=1.;
	 
  u21=rc21*rab11+rc22*rab21+rc23*rab31;
  u22=rc21*rab12+rc22*rab22+rc23*rab32;
  u23=rc21*rab13+rc22*rab23+rc23*rab33;
	
  stereo->camera1.position.X=stereo->position.X-stereo->baseline*u21/2.;
  stereo->camera1.position.Y=stereo->position.Y-stereo->baseline*u22/2.;
  stereo->camera1.position.Z=stereo->position.Z-stereo->baseline*u23/2.;
  stereo->camera1.position.H=stereo->position.H;
  stereo->camera2.position.X=stereo->position.X+stereo->baseline*u21/2.;
  stereo->camera2.position.Y=stereo->position.Y+stereo->baseline*u22/2.;
  stereo->camera2.position.Z=stereo->position.Z+stereo->baseline*u23/2.;
  stereo->camera2.position.H=stereo->position.H;
	 
  stereo->camera1.foa.X=stereo->foa.X-stereo->baseline*u21/2.;
  stereo->camera1.foa.Y=stereo->foa.Y-stereo->baseline*u22/2.;
  stereo->camera1.foa.Z=stereo->foa.Z-stereo->baseline*u23/2.;
  stereo->camera1.foa.H=stereo->foa.H;
  stereo->camera2.foa.X=stereo->foa.X+stereo->baseline*u21/2.;
  stereo->camera2.foa.Y=stereo->foa.Y+stereo->baseline*u22/2.;
  stereo->camera2.foa.Z=stereo->foa.Z+stereo->baseline*u23/2.;
  stereo->camera2.foa.H=stereo->foa.H;
  stereo->camera1.roll=stereo->roll;
  stereo->camera2.roll=stereo->roll;

  update_camera_matrix(&stereo->camera1);
  update_camera_matrix(&stereo->camera2);

  /*
    printf("Stereocamera %s Located at (%.f,%.f,%.f)\n",stereo->name,stereo->position.X,stereo->position.Y,stereo->position.Z);
    printf("Stereocamera %s Orientation: pointing towards FocusOfAtention (%.f,%.f,%.f), roll (%.f)\n",stereo->name,stereo->foa.X,stereo->foa.Y,stereo->foa.Z,stereo->roll*360./(2*PI));
    printf("Stereocamera cam %s Located at (%.f,%.f,%.f)\n",stereo->camera1.name,stereo->camera1.position.X,stereo->camera1.position.Y,stereo->camera1.position.Z);
    printf("Stereocamera cam %s Orientation: pointing towards FocusOfAtention (%.f,%.f,%.f), roll (%.f)\n",stereo->camera1.name,stereo->camera1.foa.X,stereo->camera1.foa.Y,stereo->camera1.foa.Z,stereo->camera1.roll*360./(2*PI));
    printf("Stereocamera cam %s Located at (%.f,%.f,%.f)\n",stereo->camera2.name,stereo->camera2.position.X,stereo->camera2.position.Y,stereo->camera2.position.Z);
    printf("Stereocamera cam %s Orientation: pointing towards FocusOfAtention (%.f,%.f,%.f), roll (%.f)\n",stereo->camera2.name,stereo->camera2.foa.X,stereo->camera1.foa.Y,stereo->camera2.foa.Z,stereo->camera2.roll*360./(2*PI));
  */	
}

int project(HPoint3D in, HPoint2D *out, TPinHoleCamera camera)
/* returns -1 if the point lies behind the camera,
   returns 1 if "in" 3Dpoint projects into a 2D finite point, 
   0 otherwise */
{
  int output=0;
  float a1,a2,a4,a3;
  if (out!=NULL)
    {
      a1=camera.rt11*in.X+camera.rt12*in.Y+camera.rt13*in.Z+camera.rt14*in.H;
      a2=camera.rt21*in.X+camera.rt22*in.Y+camera.rt23*in.Z+camera.rt24*in.H;
      a3=camera.rt31*in.X+camera.rt32*in.Y+camera.rt33*in.Z+camera.rt34*in.H;
      a4=camera.rt41*in.X+camera.rt42*in.Y+camera.rt43*in.Z+camera.rt44*in.H;
      out->x=camera.k11*a1+camera.k12*a2+camera.k13*a3;
      out->y=camera.k21*a1+camera.k22*a2+camera.k23*a3;
      out->h=camera.k31*a1+camera.k32*a2+camera.k33*a3;
		
      if (out->h!=0.){
	out->x=out->x/out->h;
	out->y=out->y/out->h;
	out->h=1.;
	
	/* a3/a4 is the number whose sign is interesting, but 
	   it has the same sign as a3*a4 and avoids to divide */
			
	if (a3*a4<=0.){out->h=-1.; output=-1;} /* point behind the focal plane */
	else output=1;
      }else output=0;
    }
  return(output);
}

int backproject(HPoint3D *out, HPoint2D in, TPinHoleCamera camera)
{
  int output=-1;
  float a1,a2,a3,a4,b1,b2,b3,b4;
  float ik11,ik12,ik13,ik21,ik22,ik23,ik31,ik32,ik33;
  float it11,it12,it13,it14,it21,it22,it23,it24,it31,it32,it33,it34,it41,it42,it43,it44;
  float ir11,ir12,ir13,ir14,ir21,ir22,ir23,ir24,ir31,ir32,ir33,ir34,ir41,ir42,ir43,ir44;
	 
  HPoint2D myin;
	
  if (in.h>0.) 
    {
      myin.h=camera.k11;
      myin.x=in.x*camera.k11/in.h;
      myin.y=in.y*camera.k11/in.h;
	
      ik11=(1./camera.k11); 
      ik12=-camera.k12/(camera.k11*camera.k22);
      ik13=(camera.k12*camera.k23-camera.k13*camera.k22)/(camera.k22*camera.k11); 
      ik21=0.; ik22=(1./camera.k22); ik23=-(camera.k23/camera.k22);
      ik31=0.; ik32=0.; ik33=1.;
	
      a1=ik11*myin.x+ik12*myin.y+ik13*myin.h;
      a2=ik21*myin.x+ik22*myin.y+ik23*myin.h;
      a3=ik31*myin.x+ik32*myin.y+ik33*myin.h;
      a4=1.;    
		 
      ir11=camera.rt11; ir12=camera.rt21; ir13=camera.rt31; ir14=0.;
      ir21=camera.rt12; ir22=camera.rt22; ir23=camera.rt32; ir24=0.;
      ir31=camera.rt13; ir32=camera.rt23; ir33=camera.rt33; ir34=0.;
      ir41=0.; ir42=0.; ir43=0.; ir44=1.;
	
      b1=ir11*a1+ir12*a2+ir13*a3+ir14*a4;
      b2=ir21*a1+ir22*a2+ir23*a3+ir24*a4;
      b3=ir31*a1+ir32*a2+ir33*a3+ir34*a4;
      b4=ir41*a1+ir42*a2+ir43*a3+ir44*a4; 
		 
      it11=1.; it12=0.; it13=0.; it14=camera.position.X;
      it21=0.; it22=1.; it23=0.; it24=camera.position.Y;
      it31=0.; it32=0.; it33=1.; it34=camera.position.Z;
      it41=0.; it42=0.; it43=0.; it44=1.;
		 
      out->X=it11*b1+it12*b2+it13*b3+it14*b4;
      out->Y=it21*b1+it22*b2+it23*b3+it24*b4;
      out->Z=it31*b1+it32*b2+it33*b3+it34*b4;
      out->H=it41*b1+it42*b2+it43*b3+it44*b4;
	
      if (out->H!=0.)
	{
	  out->X=out->X/out->H;
	  out->Y=out->Y/out->H;
	  out->Z=out->Z/out->H;
	  out->H=1.;
	  output=1;
	}else output=0;
    }
  return(output);
}

/* if p1 and p2 can't be drawed in a camera.cols X camera.rows image, then it will return 0.
   otherwise it will return 1 and gooda & goodb will be the correct points in the image to be drawn */
int displayline(HPoint2D p1, HPoint2D p2, HPoint2D *a, HPoint2D *b, TPinHoleCamera camera)
/* it takes care of important features: before/behind the focal plane, inside/outside the image frame */
{
  
  HPoint2D l,l0,l1,l2,l3; /* in fact lines in homogeneous coordinates */
  HPoint2D pa,pb,i0,i1,i2,i3,gooda,goodb;
  int mycase=0;
  float Xmax,Xmin,Ymax,Ymin;
  float papb=0.;
  
  Xmin=0.; Xmax=camera.rows-1.; Ymin=0.; Ymax=camera.columns-1.;
  /* Watchout!: they can't reach camera.rows or camera.columns, their are not valid values for the pixels */
  
  l0.x=0.; l0.y=1.; l0.h=-Ymin;
  l1.x=0.; l1.y=1.; l1.h=-Ymax;
  l2.x=1.; l2.y=0.; l2.h=-Xmax;
  l3.x=1.; l3.y=0.; l3.h=-Xmin;
  
  if ((p1.h<0.)&&(p2.h<0.)){
    /* both points behind the focal plane: don't display anything */
    mycase=10;

  }else{
    if ((p1.h>0.)&&(p2.h<0.)){
        /* p1 before the focal plane, p2 behind */
		/*Calculates p2 = p1 + -inf(p2-p1)*/
		p2.x = p1.x + (-BIGNUM)*(p2.x-p1.x);
		p2.y = p1.y + (-BIGNUM)*(p2.y-p1.y);
		p2.h=-p2.h;/* undo the "project" trick to get the right value */
    }else if ((p1.h<0.)&&(p2.h>0.)){
        /* p2 before the focal plane, p1 behind */
		/*Calculates p1 = p2 + -inf(p1-p2)*/
		p1.x = p2.x + (-BIGNUM)*(p1.x-p2.x);
		p1.y = p2.y + (-BIGNUM)*(p1.y-p2.y);
		p1.h=-p1.h;/* undo the "project" trick to get the right value */
    }  

    /* both points before the focal plane */
    if ((p1.x>=Xmin) && (p1.x<Xmax+1) && (p1.y>=Ymin) && (p1.y<Ymax+1) &&
	(p2.x>=Xmin) && (p2.x<Xmax+1) && (p2.y>=Ymin) && (p2.y<Ymax+1)){
      /* both inside the image limits */
      
      gooda.x=p1.x; gooda.y=p1.y; gooda.h=p1.h;
      goodb.x=p2.x; goodb.y=p2.y; goodb.h=p2.h;
      mycase=2;

    }else if ((p1.x>=Xmin) && (p1.x<Xmax+1) && (p1.y>=Ymin) && (p1.y<Ymax+1) &&
	      ((p2.x<Xmin) || (p2.x>=Xmax+1) || (p2.y<Ymin) || (p2.y>=Ymax+1))){
      /* p1 inside, p2 outside */
      gooda.x=p1.x; gooda.y=p1.y; gooda.h=p1.h;
      goodb.x=p1.x; goodb.y=p1.y; goodb.h=p1.h;
      pa.x=p1.x; pa.y=p1.y; pa.h=p1.h;
      pb.x=p2.x; pb.y=p2.y; pb.h=p2.h;
      mycase=3;

    }else if ((p2.x>=Xmin) && (p2.x<Xmax+1) && (p2.y>=Ymin) && (p2.y<Ymax+1) &&
	      ((p1.x<Xmin) || (p1.x>=Xmax+1) || (p1.y<Ymin) || (p1.y>=Ymax+1))){
      /* p2 inside, p1 outside */
	
      gooda.x=p2.x; gooda.y=p2.y; gooda.h=p2.h;
      goodb.x=p2.x; goodb.y=p2.y; goodb.h=p2.h;
      pa.x=p2.x; pa.y=p2.y; pa.h=p2.h;
      pb.x=p1.x; pb.y=p1.y; pb.h=p1.h;
      mycase=4;

    }else{
      /* both outside */    
      pa.x=p2.x; pa.y=p2.y; pa.h=p2.h;
      pb.x=p1.x; pb.y=p1.y; pb.h=p1.h;
      mycase=5;
    }
    l.x=pa.y*pb.h-pb.y*pa.h; l.y=pb.x*pa.h-pa.x*pb.h; l.h=pa.x*pb.y-pb.x*pa.y;
    i0.x=l.y*l0.h-l.h*l0.y; i0.y=l.h*l0.x-l.x*l0.h; i0.h=l.x*l0.y-l.y*l0.x;
    i1.x=l.y*l1.h-l.h*l1.y; i1.y=l.h*l1.x-l.x*l1.h; i1.h=l.x*l1.y-l.y*l1.x;
    i2.x=l.y*l2.h-l.h*l2.y; i2.y=l.h*l2.x-l.x*l2.h; i2.h=l.x*l2.y-l.y*l2.x;
    i3.x=l.y*l3.h-l.h*l3.y; i3.y=l.h*l3.x-l.x*l3.h; i3.h=l.x*l3.y-l.y*l3.x;
    if (i0.h!=0.) i0.x=i0.x/i0.h; i0.y=i0.y/i0.h; i0.h=1.;
    if (i1.h!=0.) i1.x=i1.x/i1.h; i1.y=i1.y/i1.h; i1.h=1.;
    if (i2.h!=0.) i2.x=i2.x/i2.h; i2.y=i2.y/i2.h; i2.h=1.;
    if (i3.h!=0.) i3.x=i3.x/i3.h; i3.y=i3.y/i3.h; i3.h=1.;
    
    papb=(pb.x-pa.x)*(pb.x-pa.x)+(pb.y-pa.y)*(pb.y-pa.y);

	float maxdot = -1;
    
    if (i0.h!=0.){ 
      if ((i0.x>=Xmin) && (i0.x<Xmax+1) && (i0.y>=Ymin) && (i0.y<Ymax+1)){
	if ((((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))>=0.) &&
	    (((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))<papb) &&
		(((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb.x=i0.x; goodb.y=i0.y; goodb.h=i0.h;
		maxdot = (pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y);

	  }else if (mycase==5){
	    gooda.x=i0.x; gooda.y=i0.y; gooda.h=i0.h;
	    goodb.x=i0.x; goodb.y=i0.y; goodb.h=i0.h; 
	    mycase=6;
	  }
	}
      }   
    }else; /* i0 at infinite, parallel lines */
           
    if (i1.h!=0.){ 
      if ((i1.x>=Xmin) && (i1.x<Xmax+1) && (i1.y>=Ymin) && (i1.y<Ymax+1)){
	if ((((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))>=0.)&&
	    (((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))<papb) &&
		(((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb.x=i1.x; goodb.y=i1.y; goodb.h=i1.h;
		maxdot = (pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y); 

	  }else if (mycase==5){
	    gooda.x=i1.x; gooda.y=i1.y; gooda.h=i1.h;
	    goodb.x=i1.x; goodb.y=i1.y; goodb.h=i1.h; 
	    mycase=6;
	  }
	}
      }   
    }else; /* i1 at infinite, parallel lines */
    
    if (i2.h!=0.){
      if ((i2.x>=Xmin) && (i2.x<Xmax+1) && (i2.y>=Ymin) && (i2.y<Ymax+1)){
	if ((((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))>=0.)&&
	    (((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))<papb) &&
	    (((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb.x=i2.x; goodb.y=i2.y; goodb.h=i2.h;
		maxdot = (pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y);

	  }else if (mycase==5){
	    gooda.x=i2.x; gooda.y=i2.y; gooda.h=i2.h;
	    goodb.x=i2.x; goodb.y=i2.y; goodb.h=i2.h; 
	    mycase=6;
	  }
	}
      }  
    }else; /* i2 at infinite, parallel lines */
	  
    if (i3.h!=0.){ 
      if  ((i3.x>=Xmin) && (i3.x<Xmax+1) && (i3.y>=Ymin) && (i3.y<Ymax+1)){
	if ((((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))>=0.) &&
	    (((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))<papb) &&
	    (((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y))>maxdot)){
	  if ((mycase==3)||(mycase==4)||(mycase==6)){
	    goodb.x=i3.x; goodb.y=i3.y; goodb.h=i3.h;
		maxdot = (pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y);

	  }else if (mycase==5){
	    gooda.x=i3.x; gooda.y=i3.y; gooda.h=i3.h;
	    goodb.x=i3.x; goodb.y=i3.y; goodb.h=i3.h; 
	    mycase=6;
	  }
	}
      }  
    }else; /* i3 at infinite, parallel lines */

  }

  if (debug==1){
    printf("p3: x=%.f y=%.f h=%.f\np2: x=%.f, y=%.f h=%.f\n",p1.x,p1.y,p1.h,p2.x,p2.y,p2.h);
    printf("case: %d\n i0: x=%.1f y=%.1f z=%.f dot=%.2f\n i1: x=%.1f y=%.1f z=%.f dot=%.2f\n i2: x=%.1f y=%.1f z=%.f dot=%.2f\n i3: x=%.1f y=%.1f z=%.f dot=%.2f\n",mycase,
	   i0.x,i0.y,i0.h,((pb.x-pa.x)*(i0.x-pa.x)+(pb.y-pa.y)*(i0.y-pa.y)),
	   i1.x,i1.y,i1.h,((pb.x-pa.x)*(i1.x-pa.x)+(pb.y-pa.y)*(i1.y-pa.y)),
	   i2.x,i2.y,i2.h,((pb.x-pa.x)*(i2.x-pa.x)+(pb.y-pa.y)*(i2.y-pa.y)),
	   i3.x,i3.y,i3.h,((pb.x-pa.x)*(i3.x-pa.x)+(pb.y-pa.y)*(i3.y-pa.y)));
    printf("gooda:  x=%.f y=%.f z=%.f\n",gooda.x,gooda.y,gooda.h);
    printf("goodb:  x=%.f y=%.f z=%.f\n",goodb.x,goodb.y,goodb.h);
  }
  
  a->x=gooda.x; b->x=goodb.x;
  a->y=gooda.y; b->y=goodb.y;
  a->h=gooda.h; b->h=goodb.h;

  if((mycase!=2)&&(mycase!=3)&&(mycase!=4)&&(mycase!=6)) return 0;
  else return 1;
}

void display_camerainfo(TPinHoleCamera camera){
  printf("------------------------------------------------------\n");
  printf("Camera %s\n\n",camera.name);
  printf("     Position: (X,Y,Z,H)=(%.1f,%.1f,%.1f,%.1f)\n",camera.position.X,camera.position.Y,camera.position.Z,camera.position.H);
  printf("     Focus of Attention: (x,y,z,h)=(%.1f,%.1f,%.1f,%.1f)\n\n",camera.foa.X,camera.foa.Y,camera.foa.Z,camera.foa.H);
  printf("     Focus DistanceX(vertical): %.1f mm\n",camera.fdistx);
  printf("     Focus DistanceY(horizontal): %.1f mm\n",camera.fdisty);
  printf("     Skew: %.5f \n",camera.skew);
  printf("     Optical Center: (x,y)=(%.1f,%.1f)\n\n",camera.u0,camera.v0);
  printf("     K Matrix: | %.1f %.1f %.1f %.1f |\n",camera.k11,camera.k12,camera.k13,camera.k14);
  printf("               | %.1f %.1f %.1f %.1f |\n",camera.k21,camera.k22,camera.k23,camera.k24);
  printf("               | %.1f %.1f %.1f %.1f |\n\n",camera.k31,camera.k32,camera.k33,camera.k34);
  printf("	 R&T Matrix: | %.1f %.1f %.1f %.1f |\n",camera.rt11,camera.rt12,camera.rt13,camera.rt14);
  printf("	             | %.1f %.1f %.1f %.1f |\n",camera.rt21,camera.rt22,camera.rt23,camera.rt24);
  printf("	             | %.1f %.1f %.1f %.1f |\n",camera.rt31,camera.rt32,camera.rt33,camera.rt34);
  printf("	             | %.1f %.1f %.1f %.1f |\n\n",camera.rt41,camera.rt42,camera.rt43,camera.rt44);
  printf("------------------------------------------------------\n");
}
