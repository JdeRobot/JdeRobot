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
 *            Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#ifndef CALIBRATOR_CONTROLLER_H
#define CALIBRATOR_CONTROLLER_H

#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <progeo/progeo.h>
#include <gtkmm.h>
#include <colorspaces/colorspacesmm.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <opencv/cv.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>
#include "drawarea.h"



using namespace std;

namespace calibrator {
	class Module_DLT {

		public:
			Module_DLT(Ice::PropertiesPtr prop);
			virtual ~Module_DLT();

			void display(const colorspaces::Image& image);
			void get_widgets(Glib::RefPtr<Gnome::Glade::Xml> refXml);
			TPinHoleCamera getCam();

			typedef struct point3D{
			  int x;
			  int y;
			  int z;
			} Tpoint3D;

			

		  private:
			#define IMAGE_WIDTH 320
			#define IMAGE_HEIGHT 240
			#define CALIBRATION_PI 3.1415926

			#define REF_AXE_POINTS 5
			#define NORMAL_POINTS 25 
			#define NUM_POINTSS (1+REF_AXE_POINTS+NORMAL_POINTS)
			/* Each point two equations */
			#define NUM_EQU NUM_POINTSS*2

			/* Data Types */
			struct Tpoint{
				int x;
				int y;
				int u;
				int v;
			} ;

			Tpoint points_image[NUM_POINTSS];
			Tpoint points_image_rectified[NUM_POINTSS];

			int counter_points_image;
			int counter_points_image_rectified;
			double solution_matrix[12];
			gsl_matrix *K,*R;
			gsl_vector* X;


			/* Methods */
			std::string getGladePath();
			Tpoint* getPointsImage();
			void setPointsImage(Tpoint* points);
			int getCounterPointsImage();
			
			double sqr(double a);

			
			void displayFrameRate(Gtk::Label* fpslabel);
			void solve_equation_system();
			void calculate_allocation(int num_pnts, Tpoint* correspondencias_2D, double* matriz_calibracion);
			void calcular_correspondencia_aux(Tpoint3D *p, Tpoint *punto_d, double* L);
			void rq_decomp(double* solucion, gsl_matrix* R_prima, gsl_matrix* Q_prima, gsl_vector* x);

			void print_matrix(gsl_matrix* mat);

		/*
				void drawWorld(const colorspaces::Image& image);
				HPoint3D * getPos();
				HPoint3D * getFoa();
				float getFdistX();
				float getFdistY();
				float getU0();
				float getV0();
				float getRoll();*/
		/*
				void setPos(float x, float y, float z);
				void setFoa(float x, float y, float z);
				void setFdistX(float value);
				void setFdistY(float value);
				void setU0(float value);
				void setV0(float value);	
				void setRoll(float value);
				void changeDrawCenter();
				int saveParameters(const char* fileName);
				int load_world();
				int load_world_line(FILE *myfile);
				void resetLines();
				*/


			/** Calibrator **/
			void on_capture_button_clicked();
			void on_calibrate_button_clicked();
			void on_undo_button_clicked();
			void on_unmark_button_clicked();
			void on_progeo_button_clicked();
			gboolean on_button_press_image(GdkEventButton* event);
			    
			// Functions
			int capture_on;

			void drawSelectedPoints(int counter, Module_DLT::Tpoint* points, guint8* mybuffer_rectfied);
			void drawKRTMatrix(gsl_matrix *K, gsl_matrix *R, gsl_vector* X, Gtk::Label* k[9], Gtk::Label* r[9], Gtk::Label* x[3]);

			Gtk::EventBox *event_box;
			Gtk::EventBox *event_box2;

			Gtk::Label* k[9];
			Gtk::Label* r[9];
			Gtk::Label* x[3];

			Gtk::Image *gtk_image;

			Gtk::Button* capture_button;
			Gtk::Button* calibrate_button;
			Gtk::Button* unmark_button;
			Gtk::Button* undo_button;
			Gtk::Button* progeo_button;

			/* Usefull variables */
			TPinHoleCamera camera;

			//! time variables for calculating number of frames per second 
			IceUtil::Time currentFrameTime,oldFrameTime;
			double fps;
			int frameCount;

			std::string gladepath;
			//		float lastx, lasty, lastz;
			//		bool drawCenter;
			//		vector<HPoint3D> lines;
			//int load_camera_config(Ice::PropertyDict pd);
			//void drawLine(IplImage * src, HPoint3D pini, HPoint3D pend);
			//std::string world;
			//std::string camOut;

			/* Usefull variables */
			int system_of_linear_equations[NUM_EQU][12];



			/* Methods */
			void init(Ice::PropertiesPtr prop);
			void get_equation(Tpoint3D p, Tpoint p_prima, int **ecuacion);
			//(Tpoint p, Tpoint p_prima, int **ecuacion);
			void linear_multiple_regression(double a_data[NUM_EQU*11], double b_data[NUM_EQU]);

  };

} /*namespace*/

#endif /*GIRAFFECLIENT_CONTROLLER_H*/
