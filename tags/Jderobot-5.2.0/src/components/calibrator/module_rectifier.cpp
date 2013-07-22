/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : Agustín Gallardo Díaz <agallard4@gmail.com> ; David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#include "module_rectifier.h" 
#include <iostream>
#include <cmath>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>


namespace calibrator{


const std::string gladepath = std::string(GLADE_DIR) + std::string("/rectifier.glade");

/* Images */
Glib::RefPtr<Gdk::Pixbuf> imgRectifiedBuff;
guint8* mybuffer;
guint8* mybuffer_rectified;

/* Points selected */
int counter_points_image = 0;
int counter_points_image_rectified = 0;

Module_Rectifier::Tpoint points_image[NUM_POINTS];
Module_Rectifier::Tpoint points_image_rectified[NUM_POINTS];

/* Usefull variables */
int system_of_linear_equations[NUM_EQU][9];
double solution_matrix[9];
bool flag_resolved = false;
int alfa = 1;


/****************************
	PUBLIC METHODS
****************************/


Module_Rectifier::Module_Rectifier(Ice::PropertiesPtr prop) {}

Module_Rectifier::~Module_Rectifier() {}
  

void Module_Rectifier::get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml){

		refXml->get_widget("image_notrectified", gtkimage_notrectified);
		refXml->get_widget("image_rectified", gtkimage_rectified);
		refXml->get_widget("reset_button_rectifier", reset_button_rectifier);
		refXml->get_widget("eventbox_notrectified",eventbox_notrectified);
		refXml->get_widget("eventbox_rectified",eventbox_rectified);
		/* Canvas body */
		reset_button_rectifier->signal_clicked().connect(sigc::mem_fun(this,&Module_Rectifier::on_clicked_reset_button_rectifier));
		eventbox_notrectified->signal_button_press_event().connect(sigc::mem_fun(this,&Module_Rectifier::on_button_press_image_notrectified));
		eventbox_rectified->signal_button_press_event().connect(sigc::mem_fun(this,&Module_Rectifier::on_button_press_image_rectified));
}

void Module_Rectifier::display( const colorspaces::Image& image)
{
	colorspaces::ImageRGB8 img_rgb8(image);//conversion will happen if needed
	Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
	Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
		Gdk::COLORSPACE_RGB,
		false,
		8,
		img_rgb8.width,
		img_rgb8.height,
		img_rgb8.step);


	/* Image */
	gtkimage_notrectified->clear(); /**/
	gtkimage_notrectified->set(imgBuff);
	mybuffer = imgBuff->get_pixels();
	
	/* Rectified Image */
	imgRectifiedBuff = imgBuff->copy();
	gtkimage_rectified->clear(); /**/
	gtkimage_rectified->set(imgRectifiedBuff);
	mybuffer_rectified = imgRectifiedBuff->get_pixels();


	/* if the user have selected 4 points on each image the application rectify the second image */
	if ((counter_points_image == NUM_POINTS) and (counter_points_image_rectified == NUM_POINTS)){

		/* Calculate the solution matrix */
		if (flag_resolved == false){
			solve_equation_system();
			flag_resolved = true;
		}
  
		/* Build the rectified image */
		build_rectified_image(mybuffer, mybuffer_rectified);

	}

	/* Draw selected points */
	drawSelectedPoints(counter_points_image, points_image, mybuffer);
	drawSelectedPoints(counter_points_image_rectified, points_image_rectified, mybuffer_rectified);

   
}


/****************************
	PRIVATE METHODS
****************************/

/* Show frame rate */
void Module_Rectifier::displayFrameRate()
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

void Module_Rectifier::build_rectified_image(guint8* mybuffer, guint8* mybuffer_rectified)
{
		Tpoint p;
		int i,j,offset,offset2;

		for (i=0; i<IMAGE_HEIGHT; i++){
			for (j=0; j<IMAGE_WIDTH; j++){

				calculate_allocation(j,i,&p);

				offset2 = i*IMAGE_WIDTH+j;
				if (mybuffer_rectified && mybuffer){
					if ((p.x>0) && (p.y>0) && (p.x<=IMAGE_WIDTH) && (p.y<=IMAGE_HEIGHT)){
						/* We copy pixels "known" */
						offset = p.y*IMAGE_WIDTH+p.x;

						mybuffer_rectified[offset2*3] = mybuffer[offset*3];
						mybuffer_rectified[offset2*3+1] = mybuffer[offset*3+1];
						mybuffer_rectified[offset2*3+2] = mybuffer[offset*3+2];
					}else{
						/* We paint pixels "unknown" with white colour */
						mybuffer_rectified[offset2*3] = 255;
						mybuffer_rectified[offset2*3+1] = 255;
						mybuffer_rectified[offset2*3+2] = 255;;

					}
				}
			}
		}
}

/* 
	Calculations 
****************************/

/* Resuelve el sistema sobredimensioando (un sistema con más ecuaciones de las
   necesarias) dando una solución que minimize el error cometido. Quiere decir
   minimize la suma de los cuadrados del error cometido en cada punto y su correpondiente
   calculado a traves de la matriz solución
*/

void Module_Rectifier::linear_multiple_regression(double a_data[NUM_EQU*8], double b_data[NUM_EQU]){

	gsl_matrix_view m = gsl_matrix_view_array (a_data, 8, 8);
	gsl_vector_view b = gsl_vector_view_array (b_data, 8);
	gsl_vector *x = gsl_vector_alloc (8);
       
	int s;
	gsl_permutation * p = gsl_permutation_alloc (8);
     
	gsl_linalg_LU_decomp (&m.matrix, p, &s);
	gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);

	for (int i=0; i<8; i++)
	{
		solution_matrix[i] = gsl_vector_get(x,i);
	}
	solution_matrix[8] = 1;

	gsl_permutation_free (p);
	gsl_vector_free (x);
}

/*
 Dados dos puntos p(x,y) y p'(x',y') obtiene dos
 ecuaciones fruto de la correspondencia entre los dos pnts
*/
void Module_Rectifier::get_equation(Tpoint p, Tpoint p_prima, int **ecuacion){
	
	/* Equation x */
	ecuacion[0][0] = p.x;
	ecuacion[0][1] = p.y;
	ecuacion[0][2] = 1;
	ecuacion[0][3] = 0;
	ecuacion[0][4] = 0;
	ecuacion[0][5] = 0;
	ecuacion[0][6] = -p_prima.x*p.x;
	ecuacion[0][7] = -p_prima.x*p.y;
	ecuacion[0][8] = p_prima.x;

	/*  Equation y */
	ecuacion[1][0] = 0;
	ecuacion[1][1] = 0;
	ecuacion[1][2] = 0;
	ecuacion[1][3] = p.x;
	ecuacion[1][4] = p.y;
	ecuacion[1][5] = 1;
	ecuacion[1][6] = -p_prima.y*p.x;
	ecuacion[1][7] = -p_prima.y*p.y;
	ecuacion[1][8] = p_prima.y;


}

void Module_Rectifier::solve_equation_system(){

	if ((counter_points_image_rectified == NUM_POINTS)
	and (counter_points_image_rectified == NUM_POINTS)){
		int k,i,j;
		int **linear_equation;
		double a_data[8*NUM_EQU];
		double b_data[NUM_EQU];

		linear_equation = (int**)malloc(2*sizeof(int*));
		linear_equation[0] = (int*)malloc(9*sizeof(int));
		linear_equation[1] = (int*)malloc(9*sizeof(int));
	
	  
		/* recorremos los dos arrays con los puntos almacenados, y vamos obteniendo 
		las ecuaciones para cada par de puntos. Cada par de puntos da lugar a dos ecuaciones.*/

		for (i=0; i<NUM_POINTS; i++){

			// FIXME
			Tpoint points_selected_over_input_image;
			points_selected_over_input_image.x = points_image[i].x;
			points_selected_over_input_image.y = points_image[i].y;
			Tpoint points_selected_over_rectified_image;
			points_selected_over_rectified_image.x = points_image_rectified[i].x;
			points_selected_over_rectified_image.y = points_image_rectified[i].y;

			get_equation(points_selected_over_rectified_image, points_selected_over_input_image, (int **)linear_equation);

			/** copiamos la ecuacion obtenida al sistema lineal sobredimensionado */
			for (j=0; j<9; j++){
				system_of_linear_equations[i*2][j] = linear_equation[0][j];
				system_of_linear_equations[i*2+1][j] = linear_equation[1][j];
			}

		}

		
		/** copy matrix "A" (system of linear equations) */
		k = 0;
		for (i=0; i<NUM_EQU; i++){
			for (j=0; j<8; j++){
				a_data[k++] = system_of_linear_equations[i][j];
			}
		}

		/** copy vector "b" (last column)*/
		for (j=0; j<NUM_EQU; j++)
			b_data[j] = system_of_linear_equations[j][8];

		/** resolve Ax=b */		
		linear_multiple_regression(a_data, b_data);

	}
}

/* 
	
*/
void Module_Rectifier::calculate_allocation(int x,int y,Tpoint *p){
  
  /*
    
    x' = (h1*x + h2*y + h3)/(h7*x + h8*y + h9)
    y' = (h4*x + h5*y + h6)/(h7*x + h8*y + h9)
 
  */
  
  p->x = (int) (solution_matrix[0]*x + solution_matrix[1]*y + solution_matrix[2])/
    (solution_matrix[6]*x + solution_matrix[7]*y + solution_matrix[8]);

  p->y = (int) (solution_matrix[3]*x + solution_matrix[4]*y + solution_matrix[5])/
    (solution_matrix[6]*x + solution_matrix[7]*y + solution_matrix[8]);

}


/* 
	Draw points on a buffer 
*/
void Module_Rectifier::drawSelectedPoints(int counter, Tpoint* points, guint8* buffer)
{

/*
	CvPoint p, q;

	/* Draw horizontal line 
	for(int k=0;k<counter;k++){
		/* Draw horizontal line 		
		p.x = points[k].x-5;
		p.y = points[k].y;
		q.x = points[k].x+5;
		q.y = points[k].y;
		cvLine(&buffer, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);

		/* Draw vertical line 		
		p.x = points[k].x;
		p.y = points[k].y-5;
		q.x = points[k].x;
		q.y = points[k].y+5;
		cvLine(&buffer, p, q, CV_RGB(0,0,0), 1, CV_AA, 0);

	}
*/
	for(int k=0;k<counter;k++){
		int height = points[k].y*IMAGE_WIDTH*3;
		int width = points[k].x*3;
		/* Draw horizontal line */
		for(int i=height-15;i<height+15;i++){
				buffer[i+width+0] = 255;
				buffer[i+width+1] = 255;
				buffer[i+width+2] = 255;
		}
		/* Draw vertica line */
		for(int j=height-(5*IMAGE_WIDTH*3);j<height+(5*IMAGE_WIDTH*3);j+=IMAGE_WIDTH*3){
				buffer[j+width+0] = 255;
				buffer[j+width+1] = 255;
				buffer[j+width+2] = 255;
		}
	}

}

/*
	Events 
****************************/

/** Canvas */
gboolean Module_Rectifier::on_button_press_image_notrectified(GdkEventButton* event)
{

	if (counter_points_image < NUM_POINTS){

		int x = event->x;
		int y = event->y;
		std::cout << "PRESS " << x << " " << y << std::endl;

		points_image[counter_points_image].x = event->x; 
		points_image[counter_points_image].y = event->y;
	
		counter_points_image++;

		return TRUE;
	}else{
		std::cout << "Clear the buffer" << std::endl;
	}
}
gboolean Module_Rectifier::on_button_press_image_rectified(GdkEventButton* event)
{

	if (counter_points_image_rectified < NUM_POINTS){

		int x = event->x;
		int y = event->y;

		points_image_rectified[counter_points_image_rectified].x = event->x; 
		points_image_rectified[counter_points_image_rectified].y = event->y;

		counter_points_image_rectified++;

		return TRUE;
	}else{
		std::cout << "Clear the buffer" << std::endl;
	}
}

/** Buttons */
void Module_Rectifier::on_clicked_reset_button_rectifier(){
	counter_points_image = 0;
	counter_points_image_rectified = 0;
	flag_resolved = false;
}
}/* namespace */
