/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *             Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#include "module_dlt.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

namespace calibrator {

#define TESTPOINTS 4

	Module_DLT::Tpoint3D pnts_en_objeto_de_control[NUM_POINTSS+TESTPOINTS] = {
  
	  {10,10,10},
	  
	  {8,10,10},
	  {6,10,10},
	  {4,10,10},
	  {2,10,10},
	  {0,10,10},

	  {8,8,10},
	  {6,6,10},
	  {4,4,10},
	  {2,2,10},
	  {0,0,10},

	  {10,8,10},
	  {10,6,10},
	  {10,4,10},
	  {10,2,10},
	  {10,0,10},

	  {10,8,8},
	  {10,6,6},
	  {10,4,4},
	  {10,2,2},
	  {10,0,0},


	  {10,10,8},
	  {10,10,6},
	  {10,10,4},
	  {10,10,2},
	  {10,10,0},

	  {8,10,8},
	  {6,10,6},
	  {4,10,4},
	  {2,10,2},
	  {0,10,0},

	  {10,10,10},
	  {6,10,10},
	  {10,6,10},
	  {10,10,6},
	};

	bool DEBUG = true;

	Module_DLT::Module_DLT(Ice::PropertiesPtr prop) {
		this->gladepath = std::string(GLADE_DIR) + std::string("/calibrator.glade");
		std::cout << std::string(GLADE_DIR) + std::string("/calibrator.glade") << std::endl;
		this->init(prop);
	}

	Module_DLT::~Module_DLT() {
		//delete this->camera;
	}
  
	

	void Module_DLT::init(Ice::PropertiesPtr prop) {

		/*Default values for camera parameters*/
		this->camera.position.X = 0.;
		this->camera.position.Y = 0.;
		this->camera.position.Z = 0.;
		this->camera.position.H = 1.;
		this->camera.foa.X = 0.;
		this->camera.foa.Y = 0.;
		this->camera.foa.Z = 0.;
		this->camera.foa.H = 1.;
		this->camera.fdistx = 300.0;
		this->camera.fdisty = 300.0;
		this->camera.v0 = IMAGE_WIDTH/2;
		this->camera.u0 = IMAGE_HEIGHT/2;
		this->camera.roll = 0.0;
		this->camera.columns = IMAGE_WIDTH;
		this->camera.rows = IMAGE_HEIGHT;
		this->camera.skew = 0.0;

		update_camera_matrix(&(this->camera));

		// init
		K = gsl_matrix_alloc(3,3);
		R = gsl_matrix_alloc(3,3);
		X = gsl_vector_alloc(3);

	}


// Methods
	/* 
		Draw points on a buffer 
	*/
	void Module_DLT::display(const colorspaces::Image& image)
	{
			IplImage src;
			CvPoint p, q;
			HPoint3D p1, p2;
			int n=0;

			src=image;

			Glib::RefPtr<Gdk::Pixbuf> imgBuff_dlt = Gdk::Pixbuf::create_from_data((const guint8*)image.data,
				Gdk::COLORSPACE_RGB,
				false,
				8,
				image.width,
				image.height,
				image.step); 
			gtk_image->clear();
			gtk_image->set(imgBuff_dlt);

			/* Draw horizontal line */
			for(int k=0;k<this->counter_points_image;k++){
				/* Draw horizontal line */		
				p.x = points_image[k].x-5;
				p.y = points_image[k].y;
				q.x = points_image[k].x+5;
				q.y = points_image[k].y;
				cvLine(&src, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);

				/* Draw vertical line */		
				p.x = points_image[k].x;
				p.y = points_image[k].y-5;
				q.x = points_image[k].x;
				q.y = points_image[k].y+5;
				cvLine(&src, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);

			}

	}

	/* 
		Display Frame Rate
	*/
	void Module_DLT::displayFrameRate(Gtk::Label* fpslabel)
	{
		double diff;
		IceUtil::Time diffT;

		currentFrameTime = IceUtil::Time::now();
		diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
		if (diff < 1000.0)
			frameCount++;
		else{
			oldFrameTime = currentFrameTime;
			fps = frameCount*1000.0/diff;
			frameCount=0;
			/* Display the frame rate */
			std::stringstream fpsString;
			fpsString << "fps = " << int(fps);
			fpslabel->set_label(fpsString.str());
		}
	}


// Calculations

void Module_DLT::solve_equation_system(){

	if (counter_points_image == NUM_POINTSS){
		int k,i,j;
		int **linear_equation;
		double a_data[11*NUM_EQU];
		double b_data[NUM_EQU];

		linear_equation = (int**)malloc(2*sizeof(int*));
		linear_equation[0] = (int*)malloc(12*sizeof(int));
		linear_equation[1] = (int*)malloc(12*sizeof(int));
	
	  /*
		/* recorremos los dos arrays con los puntos almacenados, y vamos obteniendo 
		las ecuaciones para cada par de puntos. Cada par de puntos da lugar a dos ecuaciones.*/

		for (i=0; i<NUM_POINTSS; i++){
//			std::cout << "Punto" << pnts_en_objeto_de_control[i].x << " " << points_image[i].x   << std::endl;

			// FIXME
			Tpoint points_selected_over_input_image;
			points_selected_over_input_image.u = points_image[i].x;
			points_selected_over_input_image.v = points_image[i].y;

			get_equation(pnts_en_objeto_de_control[i], points_selected_over_input_image, (int **)linear_equation);

			/** copiamos la ecuacion obtenida al sistema lineal sobredimensionado */
			for (j=0; j<12; j++){
				system_of_linear_equations[i*2][j] = linear_equation[0][j];
				system_of_linear_equations[i*2+1][j] = linear_equation[1][j];
//				std::cout << "  i:"  << i << "  j:" << j << "  ecu1:" << system_of_linear_equations[i*2][j] << "  ecu2:" << system_of_linear_equations[i*2+1][j] << std::endl;
			}
			

		}

		//** free ? */
		free(linear_equation[0]);
		free(linear_equation[1]);
		free(linear_equation);

		/** copy matrix "A" (system of linear equations) */
//		std::cout << " Matrix A " << std::endl;
		k = 0;
		for (i=0; i<NUM_EQU; i++){
			for (j=0; j<11; j++){
				//std::cout << "i:"  << i << "  j:" << j << std::endl;
				a_data[k++] = system_of_linear_equations[i][j];
//				std::cout << system_of_linear_equations[i][j] << ", ";
			}
//			std::cout << std::endl;
		}

		/** copy vector "b" (last column) */
//		std::cout << " Vector b " << std::endl;
		for (j=0; j<NUM_EQU; j++){
			b_data[j] = system_of_linear_equations[j][11];
//			std::cout << b_data[j] << ", ";
		}
//		std::cout << std::endl;


		//** resolve Ax=b 
		linear_multiple_regression(a_data, b_data);

		rq_decomp(solution_matrix, K, R, X);

//		std::cout << "K" << std::endl;
		for (i=0; i<3; i++){
			for (j=0; j<3; j++){
				printf("%g ", gsl_matrix_get(K, i, j));
				//std::cout << gsl_matrix_get(K, i, j) << std::endl;
			}
//			std::cout << std::endl;
		}

//		std::cout << std::endl << "R" << std::endl;
		for (i=0; i<3; i++){
			for (j=0; j<3; j++){
				printf("%g ", gsl_matrix_get(R, i, j));
				//std::cout << gsl_matrix_get(R, i, j) << std::endl;
			}
//			std::cout << std::endl;
		}

//		std::cout << std::endl << "X" << std::endl;
		for (i=0; i<3; i++){
			printf("%g ", gsl_vector_get(X, i));
			//std::cout << gsl_matrix_get(R, i, j) << std::endl;
//			std::cout << std::endl;
		}

		std::cout << "Solution Matrix" << std::endl;
		for (i=0; i<3; i++){
			for (j=0; j<3; j++){
				std::cout << solution_matrix[i*3+j] << std::endl;
			}
			std::cout << std::endl;
		}

	}
}

/* Para llegar a la desomposicion RQ, hay que 
   invertir la matriz que queremos descomopner
   y luego aplicarle un QR, finalmente se invierte
   tanto R como Q para obtener la RQ

   1) M' = invert(M)
   2) M' = QR
   3) (M')' = (QR)' = R'Q'
   4) M = R'Q'
*/   

void Module_DLT::rq_decomp(double* solucion, 
	       gsl_matrix* R_prima,
	       gsl_matrix* Q_prima,
	       gsl_vector* x
	       ){
/*
	int i, j, lotkin_signum, frank_signum;
	int DIM = 3;
	gsl_matrix *lotkin_a, *frank_a;
	gsl_vector *x, *lotkin_b, *frank_b, *lotkin_x, *frank_x;
	gsl_vector *lotkin_tau, *frank_tau;

	/* allocate a, x, b 
	lotkin_a = gsl_matrix_alloc(DIM, DIM);
	frank_a = gsl_matrix_alloc(DIM, DIM);
	x = gsl_vector_alloc(DIM);
	lotkin_b = gsl_vector_alloc(DIM);
	frank_b = gsl_vector_alloc(DIM);
	lotkin_x = gsl_vector_alloc(DIM);
	frank_x = gsl_vector_alloc(DIM);

	/* set x = [1 2 ... DIM] 
	for(i = 0; i < DIM; i++)
		gsl_vector_set(x, i, (double)i);

	/* set Lotkin matrix                      */
	/* a_ij = 1 (i = 1) or 1/(i+j-1) (i != 1) 
	for(i = 0; i < DIM; i++)
		gsl_matrix_set(lotkin_a, 0, i, 1.0);
	for(i = 1; i < DIM; i++)
		for(j = 0; j < DIM; j++)
			gsl_matrix_set(lotkin_a, i, j, 1.0 / (double)(i + j + 1));

	/* set Frank matrix       
	/* a_ij = DIM - min(i,j) + 1 
	for(i = 0; i < DIM; i++)
		for(j = 0; j < DIM; j++)
			gsl_matrix_set(frank_a, i, j, (double)DIM - (double)GSL_MAX(i, j) );
	*/

	/* set A matrix                
	gsl_matrix_set(lotkin_a, 0, 0, 12);
	gsl_matrix_set(lotkin_a, 0, 1, 6);
	gsl_matrix_set(lotkin_a, 0, 2, -4);
	gsl_matrix_set(lotkin_a, 1, 0, -51);
	gsl_matrix_set(lotkin_a, 1, 1, 167);
	gsl_matrix_set(lotkin_a, 1, 2, 24);
	gsl_matrix_set(lotkin_a, 2, 0, 4);
	gsl_matrix_set(lotkin_a, 2, 1, -68);
	gsl_matrix_set(lotkin_a, 2, 2, -41);


	/* Print matrix 
	for(i = 0; i < DIM; i++)
	{
		printf("%3d: ", i);
		for(j = 0; j < DIM; j++)
			printf("%g ", gsl_matrix_get(lotkin_a, i, j));
		printf("\n");
	}
	printf("\n");


	/* b = A * x 
	gsl_blas_dgemv(CblasNoTrans, 1.0, lotkin_a, x, 0.0, lotkin_b);

	/* QR decomposition and solve 
	lotkin_tau = gsl_vector_alloc(DIM);
	gsl_linalg_QR_decomp(lotkin_a, lotkin_tau);
	gsl_linalg_QR_solve(lotkin_a, lotkin_tau, lotkin_b, lotkin_x);
	gsl_vector_free(lotkin_tau);

	/* Print solution matrix 
	for(i = 0; i < DIM; i++)
	{
		printf("%3d: ", i);
		for(j = 0; j < DIM; j++)
			printf("%g ", gsl_matrix_get(lotkin_a, i, j));
		printf("\n");
	}
	printf("\n");
	for(i = 0; i < DIM; i++)
	{
		printf("%3d: ", i);
		for(j = 0; j < DIM; j++)
			//printf("%g ", gsl_vector_get(lotkin_x, i, j));
		printf("\n");
	}

	/* free a, x, b 
	gsl_matrix_free(lotkin_a);
	gsl_vector_free(x);
	gsl_vector_free(lotkin_b);
	gsl_vector_free(lotkin_x);

*/

/*

  gsl_matrix* C = gsl_matrix_alloc(3,3);
  /* Compute C = A B 
  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
                  1.0, R_prima, Q_prima,
                  0.0, C);
  camera->rt11 = gsl_matrix_get(C, 0, 0);
  camera->rt12 = gsl_matrix_get(C, 0, 1);
  camera->rt13 = gsl_matrix_get(C, 0, 2);

  camera->rt21 = gsl_matrix_get(C, 1, 0);
  camera->rt22 = gsl_matrix_get(C, 1, 1);
  camera->rt23 = gsl_matrix_get(C, 1, 2);

  camera->rt31 = gsl_matrix_get(C, 2, 0);
  camera->rt32 = gsl_matrix_get(C, 2, 1);
  camera->rt33 = gsl_matrix_get(C, 2, 2);

  camera->rt41 = 0;
  camera->rt42 = 0;
  camera->rt43 = 0;
  camera->rt44 = 1;



**/

	std::cout << "RQ_Decomp" << std::endl;
	int n,mm,s,signum ;
	gsl_matrix *M,*Q,*R;
	gsl_vector* tau;
	double tmp,det;

	/* para invertir las matriz M,Q,R */
	gsl_permutation* p = gsl_permutation_alloc (3);
	gsl_permutation* p2 = gsl_permutation_alloc (3);
	gsl_permutation* p3 = gsl_permutation_alloc (3);
	gsl_matrix* M_prima = gsl_matrix_alloc(3,3);
	gsl_matrix* Q_prima_tmp = gsl_matrix_alloc(3,3);
  
	/* para resolver el centro de la camara usando Mx=C 
	donde C es el verctor p4 de la matriz P */
	gsl_vector* p4 = gsl_vector_alloc(3);
	
	gsl_matrix* temp = gsl_matrix_alloc(3,3);
	gsl_matrix* I_C = gsl_matrix_alloc(3,4);
	gsl_matrix* test = gsl_matrix_alloc(3,4);

	M = gsl_matrix_alloc(3,3);
	Q = gsl_matrix_alloc(3,3);
	R = gsl_matrix_alloc(3,3);
	tau = gsl_vector_alloc(3);

	/* Copiamos la submatriz 3x3 Izq de la solucion P a la matriz M */
	gsl_matrix_set(M,0,0,solucion[0]);
	gsl_matrix_set(M,0,1,solucion[1]);
	gsl_matrix_set(M,0,2,solucion[2]);

	gsl_matrix_set(M,1,0,solucion[4]);
	gsl_matrix_set(M,1,1,solucion[5]);
	gsl_matrix_set(M,1,2,solucion[6]);

	gsl_matrix_set(M,2,0,solucion[8]);
	gsl_matrix_set(M,2,1,solucion[9]);
	gsl_matrix_set(M,2,2,solucion[10]);

	/* Copiamos el vector p4 */
	gsl_vector_set(p4,0,solucion[3]);
	gsl_vector_set(p4,1,solucion[7]);
	gsl_vector_set(p4,2,solucion[11]);

	/* invertimos la matriz M */
	gsl_linalg_LU_decomp (M, p, &s);
	gsl_linalg_LU_solve(M,p,p4,x);
	gsl_linalg_LU_invert (M, p, M_prima);
  
  /* Hacemos una descomposicion a la matriz M invertida */
  gsl_linalg_QR_decomp (M_prima,tau);
  gsl_linalg_QR_unpack (M_prima,tau,Q,R);

  /* Invertimos R */
  gsl_linalg_LU_decomp (R, p2, &s);
  gsl_linalg_LU_invert (R, p2, R_prima);
  
  /* Invertimos Q */
  gsl_linalg_LU_decomp (Q, p3, &s);
  gsl_linalg_LU_invert (Q, p3, Q_prima);
  gsl_matrix_memcpy(Q_prima_tmp, Q_prima);


std::cout << "Calculamos" << std::endl;
      if (DEBUG) {
/** checking results: 
	
	If the rq decompsition is correct we should obtain
	the decomposed matrix:

	orig_matrix = K*R*T

	where T = (I|C)
*/
     

    gsl_matrix_set(I_C,0,3,gsl_vector_get(x,0));
    gsl_matrix_set(I_C,1,3,gsl_vector_get(x,1));
    gsl_matrix_set(I_C,2,3,gsl_vector_get(x,2));
    
    gsl_matrix_set(I_C,0,0,1);
    gsl_matrix_set(I_C,0,1,0);
    gsl_matrix_set(I_C,0,2,0);
    
    gsl_matrix_set(I_C,1,0,0);
    gsl_matrix_set(I_C,1,1,1);
    gsl_matrix_set(I_C,1,2,0);
    
    gsl_matrix_set(I_C,2,0,0);
    gsl_matrix_set(I_C,2,1,0);
    gsl_matrix_set(I_C,2,2,1);
    
    gsl_linalg_matmult(R_prima,Q_prima,temp);
    gsl_linalg_matmult(temp,I_C,test);
    
    printf(" Result -> \n");
    
    for (n=0; n<3; n++){
//      for (mm=0; mm<4; mm++){
      for (mm=0; mm<3; mm++){
	printf(" %g \t",gsl_matrix_get(temp,n,mm));
// se debe sacar test
      }
      printf("\n");
    }
  }
  
  /* El elemento (3,3) de la matriz R tiene que ser 1
     para ello tenemos que normalizar la matriz dividiendo
     entre este elemento
  */
  
  tmp = gsl_matrix_get(R_prima,2,2);
  for (n=0; n<3; n++)
    for (mm=0; mm<3; mm++){
      gsl_matrix_set(R_prima,n,mm, gsl_matrix_get(R_prima,n,mm)/tmp);
    }


  /*  Si obtenemos valores negativos en la
      diagonal de K tenemos que cambiar de signo la columna de K y la fila de Q
      correspondiente
  */
  
  if (DEBUG) 
    print_matrix(R_prima);
  if (DEBUG) 
    print_matrix(Q_prima);

  if (gsl_matrix_get(R_prima,0,0)<0){
  
    if (DEBUG) printf(" distancia focat 0,0 negativa\n");
    gsl_matrix_set(R_prima,0,0,
		   abs(gsl_matrix_get(R_prima,0,0))
		   );
    for (n=0;n<3;n++)
      gsl_matrix_set(Q_prima,0,n,
		     gsl_matrix_get(Q_prima,0,n)*-1
		     );
    
  }

  if (DEBUG)  printf("R_prima\n");
  print_matrix(R_prima);
  if (DEBUG) printf("Q_prima\n");
  print_matrix(Q_prima);

  if (gsl_matrix_get(R_prima,1,1)<0){
    if (DEBUG) printf(" distancia focal 1,1 negativa\n");
    for (n=0;n<3;n++){
      gsl_matrix_set(Q_prima,1,n,
		     gsl_matrix_get(Q_prima,1,n)*-1
		     );
      gsl_matrix_set(R_prima,n,1,
		     gsl_matrix_get(R_prima,n,1)*-1
		     );
    }
  }

  if (DEBUG) printf("R_prima\n");
  print_matrix(R_prima);
  if (DEBUG) printf("Q_prima\n");
  print_matrix(Q_prima);
  
  
  /*Finalmente, si Q queda con determinante -1 cambiamos de signo
    todos sus elementos para obtener una rotaciÃ³n sin "reflexion".
    
    NOTA: Este trozo de codigo lo he desactivado debido a que si lo
    hacemos obtenemos una orientacion equivocada a la hora de dibujarla
    con OGL
  */

  
  gsl_linalg_LU_decomp (Q_prima_tmp, p3, &s);
  signum=1;
  det = gsl_linalg_LU_det(Q_prima_tmp,signum);
    
  if (-1 == det && 0){
    if (DEBUG) printf("Q has a negatif det");
    for (n=0;n<3;n++)
      for (mm=0;mm<3;mm++)
	gsl_matrix_set(Q_prima,n,mm,gsl_matrix_get(Q_prima,n,mm)*-1);
    
  }  

}

void Module_DLT::print_matrix(gsl_matrix* mat){
 
  int n,mm;

  if (!DEBUG)
    return;

  for (n=0; n<3; n++){
    for (mm=0; mm<3; mm++){
      printf("%g ",gsl_matrix_get(mat,n,mm));
    }
    printf("\n");
  }
  
  printf("\n\n");
}



/*
 Dados dos puntos p(x,y) y p'(x',y') obtiene dos
 ecuaciones fruto de la correspondencia entre los dos pnts
*/
void Module_DLT::get_equation(Tpoint3D p, Tpoint p_prima, int **ecuacion){
//(Tpoint p, Tpoint p_prima, int **ecuacion){

	
	/* Equation x 
	ecuacion[0][0] = p.x;
	ecuacion[0][1] = p.y;
	ecuacion[0][2] = 1;
	ecuacion[0][3] = 0;
	ecuacion[0][4] = 0;
	ecuacion[0][5] = 0;
	ecuacion[0][6] = -p_prima.x*p.x;
	ecuacion[0][7] = -p_prima.x*p.y;
	ecuacion[0][8] = p_prima.x;

	/*  Equation y 
	ecuacion[1][0] = 0;
	ecuacion[1][1] = 0;
	ecuacion[1][2] = 0;
	ecuacion[1][3] = p.x;
	ecuacion[1][4] = p.y;
	ecuacion[1][5] = 1;
	ecuacion[1][6] = -p_prima.y*p.x;
	ecuacion[1][7] = -p_prima.y*p.y;
	ecuacion[1][8] = p_prima.y;

*/

  
  /** 
      Rellenamos la primera fila con los siguientes coeficientes Li :

      Ecuacion1 -> L1.x+L2.y+L3.z+L4+L5.0+L6.0+L7.0+L8.0-L9.x.u-L10.y.u-L11.z.u = u
      Ecuacion2 -> L1.0+L2.0+L3.0+L4.0+L5.x+L6.y+L7.z+L8-L9.x.v-L10.y.v-L11.z.v = v
  */

	  /* Ecuación 1 - lado izq */
	  ecuacion[0][0]  = p.x;
	  ecuacion[0][1]  = p.y;
	  ecuacion[0][2]  = p.z;
	  ecuacion[0][3]  = 1;
	  ecuacion[0][4]  = 0;
	  ecuacion[0][5]  = 0;
	  ecuacion[0][6]  = 0;
	  ecuacion[0][7]  = 0;
	  ecuacion[0][8]  = -p.x*p_prima.u;
	  ecuacion[0][9]  = -p.y*p_prima.u;
	  ecuacion[0][10] = -p.z*p_prima.u;

	  /* Ecuación 1 - lado derecho*/
//	  ecuacion[0][11] = p_prima.u*alfa;
	  ecuacion[0][11] = p_prima.u;

	  /* Ecuación 2 - lado derecho */
	  ecuacion[1][0]  = 0;
	  ecuacion[1][1]  = 0;
	  ecuacion[1][2]  = 0;
	  ecuacion[1][3]  = 0;
	  ecuacion[1][4]  = p.x;
	  ecuacion[1][5]  = p.y;
	  ecuacion[1][6]  = p.z;
	  ecuacion[1][7]  = 1;
	  ecuacion[1][8]  = -p.x*p_prima.v;
	  ecuacion[1][9]  = -p.y*p_prima.v;
	  ecuacion[1][10] = -p.z*p_prima.v;

	  /* Ecuación 2 - lado derecho */
//	  ecuacion[1][11] = p_prima.v*alfa;
	  ecuacion[1][11] = p_prima.v;



}
/* Resuelve el sistema sobredimensioando (un sistema con más ecuaciones de las
   necesarias) dando una solución que minimize el error cometido. Quiere decir
   minimize la suma de los cuadrados del error cometido en cada punto y su correpondiente
   calculado a traves de la matriz solución
*/
void Module_DLT::linear_multiple_regression(double a_data[NUM_EQU*11], double b_data[NUM_EQU]){

int i,j,k,aux;
  double chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *c;
  gsl_multifit_linear_workspace * work;
  
  X = gsl_matrix_alloc(NUM_EQU,11);
  y = gsl_vector_alloc(NUM_EQU);
  c = gsl_vector_alloc (11);
  cov = gsl_matrix_alloc (11,11);
  
  /* Prepramos la matriz de muestras 

     NOTA: El sistema de ecuaciones a resolver contiene
     11 incognitas en vez de 12. La 12 incognita es
     igual a alfa (constante), y hay que inicializarla sino
     La solucion siempre sera el vector nulo
  */
  
  for (i=0; i<NUM_EQU; i++){
    for (j=0; j<11; j++)
      {
	aux = i*11+j;
	gsl_matrix_set(X,i,j,a_data[aux]);
      }
  }
  
  /* Inicializamos el verctor de muestras */
  for (k=0; k<NUM_EQU; k++)
    {
      gsl_vector_set(y,k,b_data[k]);
    }
  
  /* Inicializamos y resolvemos el sistema sobredimensioando */
  work = gsl_multifit_linear_alloc (NUM_EQU,11);
  gsl_multifit_linear (X,  y, c, cov, &chisq, work);
  gsl_multifit_linear_free (work);
  
  /** copiamos la soluciÃ³n */
  for (i=0; i<11; i++)
    {
      solution_matrix[i] = gsl_vector_get(c,i);
    }
  
  solution_matrix[11] = 1;

  this->camera.rt11 = solution_matrix[0];
  this->camera.rt12 = solution_matrix[1];
  this->camera.rt13 = solution_matrix[2];
  this->camera.rt14 = solution_matrix[3];

  this->camera.rt21 = solution_matrix[4];
  this->camera.rt22 = solution_matrix[5];
  this->camera.rt23 = solution_matrix[6];
  this->camera.rt24 = solution_matrix[7];

  this->camera.rt31 = solution_matrix[8];
  this->camera.rt32 = solution_matrix[9];
  this->camera.rt33 = solution_matrix[10];
  this->camera.rt34 = solution_matrix[11];

  this->camera.rt41 = 0;
  this->camera.rt42 = 0;
  this->camera.rt43 = 0;
  this->camera.rt44 = 1;

  reverse_update_camera_matrix(&(this->camera));

}

double Module_DLT::sqr(double a){
  return a*a;
}

void Module_DLT::calcular_correspondencia_aux(Tpoint3D *p, Tpoint *punto_d, double* L){

	int D = 0;

//	D = L[8]*p->x+L[9]*p->y+L[10]*p->z+alfa;
	D = L[8]*p->x+L[9]*p->y+L[10]*p->z;

	punto_d->u = (int)((L[0]*p->x+L[1]*p->y+L[2]*p->z+L[3])/D);
	punto_d->v = (int)((L[4]*p->x+L[5]*p->y+L[6]*p->z+L[7])/D); 
}
void Module_DLT::calculate_allocation(int num_pnts,
			      Tpoint * correspondencias_2D,
			      double* matriz_calibracion){
//void calcular_correspondencia(int num_pnts,
//			      Tpoint2D * correspondencias_2D,
//			      double* matriz_calibracion){
	double error=0;

	/*
	Formulas Base para pasar de un punto (x,y,z) -> (u,v). Nuestro H(3x4) en este
	caso es "matriz_calib_cam1" que es un array plano de 12 posiciones

	u = (L1.x + L2.y + L3.z + L4) / (L9.x + L10.y + L11.z + 1)
	v = (L5.x + L6.y + L7.z + L8) / (L9.x + L10.y + L11.z + 1)

	*/

	double *L = matriz_calibracion;
	int i,D = 0;
	Tpoint3D p;

	for (i=0; i<num_pnts; i++){
		p = pnts_en_objeto_de_control[i];
//		D = L[8]*p.x+L[9]*p.y+L[10]*p.z+alfa;
		D = L[8]*p.x+L[9]*p.y+L[10]*p.z;
		calcular_correspondencia_aux(&p,&correspondencias_2D[i], matriz_calibracion); 

		/** El error se calcula solo con los puntos de control, los 
		de test no se cuentan.
		*/
		if (i<NUM_POINTSS){

			error += sqrt(
				sqr(correspondencias_2D[i].u-points_image[i].u)+
				sqr(correspondencias_2D[i].v-points_image[i].v)
			);
		}
	}
}




/* 

void Viewer::calculate_allocation(int x,int y,Tpoint *p){
  
  /*
    
    x' = (h1*x + h2*y + h3)/(h7*x + h8*y + h9)
    y' = (h4*x + h5*y + h6)/(h7*x + h8*y + h9)
 
//  
  
  p->x = (int) (solution_matrix[0]*x + solution_matrix[1]*y + solution_matrix[2])/
    (solution_matrix[6]*x + solution_matrix[7]*y + solution_matrix[8]);

  p->y = (int) (solution_matrix[3]*x + solution_matrix[4]*y + solution_matrix[5])/
    (solution_matrix[6]*x + solution_matrix[7]*y + solution_matrix[8]);

}

*/


/* GETTERS AND SETTERS */
	std::string Module_DLT::getGladePath() {
		return this->gladepath;
	}

	Module_DLT::Tpoint* Module_DLT::getPointsImage() {
		return this->points_image;
	}

	int Module_DLT::getCounterPointsImage() {
		return this->counter_points_image;
	}


/** Calibrator **/
/*
	Methods
****************************/
void Module_DLT::drawKRTMatrix(gsl_matrix *K, gsl_matrix *R, gsl_vector* X, Gtk::Label* k_aux[9], Gtk::Label* r_aux[9], Gtk::Label* x_aux[3]){

	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){
			std::stringstream labelString;
			labelString << gsl_matrix_get(K, i, j);
			k_aux[i*3+j]->set_label(labelString.str());
	
			
		}
	}

	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){
			std::stringstream labelString;
			labelString << gsl_matrix_get(R, i, j);
			r_aux[i*3+j]->set_label(labelString.str());

		}
	}

	for (int i=0; i<3; i++){
		std::stringstream labelString;
		labelString << gsl_vector_get(X, i);
		x_aux[i]->set_label(labelString.str());
	}

}
void Module_DLT::get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml){

		capture_on = 1;

		refXml->get_widget("dlt_image",gtk_image);
		for(int i=0; i<9;i++){
			std::stringstream labelString;
			labelString << "dlt_c1k" << int(i);
			refXml->get_widget(labelString.str(),k[i]);
		}
		for(int i=0; i<9;i++){
			std::stringstream labelString;
			labelString << "dlt_c1R" << int(i);
			refXml->get_widget(labelString.str(),r[i]);
		}

		for(int i=0; i<3;i++){
			std::stringstream labelString;
			labelString << "dlt_c1C" << int(i);
			refXml->get_widget(labelString.str(),x[i]);
		}


		refXml->get_widget("dlt_capture_button",capture_button);	
		refXml->get_widget("dlt_calibrate_button",calibrate_button);
		refXml->get_widget("dlt_unmark_button",unmark_button);
		refXml->get_widget("dlt_undo_button",undo_button);

		refXml->get_widget("dlt_eventbox_image",event_box);
		refXml->get_widget("dlt_eventbox_image_gl",event_box2);

		/* Canvas body */
		event_box->signal_button_press_event().connect(sigc::mem_fun(this,&Module_DLT::on_button_press_image));
		capture_button->signal_clicked().connect(sigc::mem_fun(this,&Module_DLT::on_capture_button_clicked));
		calibrate_button->signal_clicked().connect(sigc::mem_fun(this,&Module_DLT::on_calibrate_button_clicked));
		unmark_button->signal_clicked().connect(sigc::mem_fun(this,&Module_DLT::on_unmark_button_clicked));
		undo_button->signal_clicked().connect(sigc::mem_fun(this,&Module_DLT::on_undo_button_clicked));
}

/*
	Events 
****************************/
/** Canvas */
gboolean Module_DLT::on_button_press_image(GdkEventButton* event)
{


	if (this->getCounterPointsImage() < NUM_POINTSS){

		int x = event->x;
		int y = event->y;
		std::cout << "PRESS " << x << " " << y << std::endl;

		this->points_image[this->getCounterPointsImage()].x = event->x; 
		this->points_image[this->getCounterPointsImage()].y = event->y;
	
		this->counter_points_image++;

		return TRUE;
	}else{
		std::cout << "Clear the buffer" << std::endl;
	}
/*
	int mark[NUM_POINTSS*2] = {
				173, 87
				,160,82
				,146,79
				,132,75
				,117,68
				,99,64
				,162,98
				,152,110
				,140,124
				,129,138
				,112,158
				,174,99
				,177,115
				,181,132
				,184,148
				,188,162
				,186,94
				,204,102
				,222,110
				,243,118
				,264,130
				,184,81
				,197,71
				,211,65
				,225,55
				,240,45
				,172,76
				,172,62
				,171,47
				,171,29
				,171,10
				};


	this->counter_points_image=0;
	for(int i=0;i<NUM_POINTSS;i++){
		this->points_image[this->getCounterPointsImage()].x = mark[i*2]; 
		this->points_image[this->getCounterPointsImage()].y = mark[i*2+1];
	
		this->counter_points_image++;
	}
*/
}

void Module_DLT::on_capture_button_clicked()
{
	std::cout << "Clic: " << capture_on << std::endl;
	capture_on *= -1;
}


void Module_DLT::on_calibrate_button_clicked()
{
	if (this->getCounterPointsImage() == NUM_POINTSS){
		std::cout << "Clic calibrate: " << std::endl;
		this->solve_equation_system();

		drawKRTMatrix(this->K, this->R, this->X, k, r, x);
/*
		for (int i=0; i<9; i++)
		{
			std::cout << this->solution_matrix[i] << std::endl;

			std::stringstream labelString;
			labelString << solution_matrix[i];
			k[i]->set_label(labelString.str());
		}
*/
	}else{
		std::cout << "Tienes que selecionar todos los puntos" << std::endl;
	}

		std::cout << "Información invertida de la cámara" << std::endl;
		display_camerainfo(this->camera);

		/*Update camera with current values*/
		update_camera_matrix(&(this->camera));
		display_camerainfo(this->camera);

		reverse_update_camera_matrix(&(this->camera));
		display_camerainfo(this->camera);

}
void Module_DLT::on_undo_button_clicked()
{
	if (this->getCounterPointsImage() != 0){
		this->counter_points_image--;
		std::cout << "UNDO " << std::endl;
	}else{
		std::cout << "No tienes ningún punto seleccionado " << std::endl;
	}

}

void Module_DLT::on_unmark_button_clicked()
{
	this->counter_points_image = 0;
	std::cout << "Clic unmark: " << std::endl;
}


TPinHoleCamera Module_DLT::getCam(){
	return this->camera;
}


} /*namespace*/

