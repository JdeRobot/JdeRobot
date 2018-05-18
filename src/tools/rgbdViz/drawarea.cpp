/*
*  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *             Julio Vega <julio.vega@urjc.es>
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *
 */

#include "drawarea.h"

#define eval2(N) (N*N)


namespace rgbdViz {
	const float DrawArea::MAXWORLD = 50.;
	const float DrawArea::PI = 3.141592654;

	DrawArea::DrawArea(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& builder)
	: Gtk::DrawingArea(cobject), Gtk::GL::Widget<DrawArea>() {


		this->refresh_time = 50; //ms
		this->numlines=0;
		this->post_p=0;
		jamb_p=0;
		circle_p=0;
		triangle_p=0;
		sq_p=0;
		numextra_lines=0;
		draw_kinect_points=false;
		draw_kinect_with_color=false;
		num_camera_lines=0;

		Glib::RefPtr<Gdk::GL::Config> glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_DOUBLE);
		if (!glconfig) {
			std::cerr << "*** Cannot find the double-buffered visual.\n" << "*** Trying single-buffered visual.\n";

			// Try single-buffered visual
			glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH);
			if (!glconfig) {
				std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
				std::exit(1);
			}
		}

		/*Set OpenGL-capability to the widget.*/
		this->unrealize();
		if(!this->set_gl_capability(glconfig) || !this->is_gl_capable()) {
			std::cerr << "No Gl capability\n";
			std::exit(1);
		}
		this->realize();

		/*Add events*/
		this->add_events(	Gdk::BUTTON1_MOTION_MASK    |
											Gdk::BUTTON2_MOTION_MASK    |
											Gdk::BUTTON3_MOTION_MASK    |
											Gdk::BUTTON_PRESS_MASK      |
											Gdk::BUTTON_RELEASE_MASK    |
											Gdk::VISIBILITY_NOTIFY_MASK);

		this->signal_motion_notify_event().connect(sigc::mem_fun(this,&DrawArea::on_motion_notify));
		this->signal_scroll_event().connect(sigc::mem_fun(this,&DrawArea::on_drawarea_scroll));

		/*Call to expose_event*/
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &DrawArea::on_timeout), this->refresh_time);

		/*Init Glut*/
		int val_init = 0;
		glutInit(&val_init, NULL);
 		glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

		/*GL Camera Position and FOA*/
		this->glcam_pos.x=-100.;
		this->glcam_pos.y=-100.;
		this->glcam_pos.z=200.;
		this->glcam_foa.x=100;
		this->glcam_foa.y=100.;
		this->glcam_foa.z=20.;

    this->scale = 50;
    this->radius = 20.0;
    this->lati = 0.2;
    this->longi = -1.0;
    this->old_x = 0.0;
    this->old_y = 0.0; 

	}

	DrawArea::~DrawArea() {	}

	void DrawArea::setToCamera1 () {
		this->glcam_pos.x=0.;
		this->glcam_pos.y=(int)MAXWORLD*10/1.2;
		this->glcam_pos.z=150.;

		this->glcam_foa.x=0.;
		this->glcam_foa.y=0.;
		this->glcam_foa.z=0.;
	}

	void DrawArea::setToCamera2 () {
		this->glcam_pos.x=-(int)MAXWORLD*10/1.2;
		this->glcam_pos.y=0.;
		this->glcam_pos.z=150.;

		this->glcam_foa.x=0.;
		this->glcam_foa.y=0.;
		this->glcam_foa.z=0.;
	}

	void DrawArea::setToCamera3 () {
		this->glcam_pos.x=0.;
		this->glcam_pos.y=-(int)MAXWORLD*10/1.2;
		this->glcam_pos.z=150.;

		this->glcam_foa.x=0.;
		this->glcam_foa.y=0.;
		this->glcam_foa.z=0.;
	}

	void DrawArea::setToCamera4 () {
		this->glcam_pos.x=(int)MAXWORLD*10/1.2;
		this->glcam_pos.y=0.;
		this->glcam_pos.z=150.;

		this->glcam_foa.x=0.;
		this->glcam_foa.y=0.;
		this->glcam_foa.z=0.;
	}

	void DrawArea::readFile(std::string path){
		read_world_file((char*)path.c_str());
	}

	void DrawArea::setToPioneerCamera () {
		this->glcam_pos.x = ((10000.)*(cos(this->robottheta)) - (0.*sin(this->robottheta)) + this->robotx)/100.;
		this->glcam_pos.y = ((10000.)*(-sin(this->robottheta)) + (0.+cos(this->robottheta)) + this->roboty)/100.;
		this->glcam_pos.z = 150.;
		this->glcam_foa.x = this->robotx/100.;
		this->glcam_foa.y = this->roboty/100.;
		this->glcam_foa.z = 0.;
	}

	void DrawArea::InitOGL (int w, int h) {
		GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
		GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
		GLfloat local_view[] = {0.0};

		glViewport(0,0,(GLint)w,(GLint)h);  
		glDrawBuffer(GL_BACK);
		glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* With this, the pioneer appears correctly, but the cubes don't */
		glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv (GL_LIGHT0, GL_POSITION, position);
		glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
		glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
		glEnable (GL_LIGHT0);
		// glEnable (GL_LIGHTING);

		glEnable(GL_TEXTURE_2D);     // Enable Texture Mapping
		glEnable (GL_AUTO_NORMAL);
		glEnable (GL_NORMALIZE);  
		glEnable(GL_DEPTH_TEST);     // Enables Depth Testing
		glDepthFunc(GL_LESS);  
		glShadeModel(GL_SMOOTH);     // Enables Smooth Color Shading
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	bool DrawArea::on_expose_event(GdkEventExpose* event) {
		return true;
	}

	bool DrawArea::my_expose_event(){
		//std::cout << "expose" << std::endl;
		Gtk::Allocation allocation = get_allocation();
		GLfloat width, height;

		Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
		glwindow->gl_begin(get_gl_context());

		glDrawBuffer(GL_BACK);
		glClearColor(0.6f, 0.8f, 1.0f, 0.0f);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		width = allocation.get_width();
		height = allocation.get_height();

		this->InitOGL (width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*Angulo	ratio		znear, zfar*/
		gluPerspective(50.0, 	width/height, 	1.0, 50000.0);	

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();

		/*pos cam		pto central	vector up*/
		gluLookAt((double)this->glcam_pos.x, (double)this->glcam_pos.y, (double)this->glcam_pos.z,
							(double)this->glcam_foa.x, (double)this->glcam_foa.y, (double)this->glcam_foa.z,
							0., 0., 1.);

		/*Draw world*/
		this->drawScene();

		/*Swap buffers*/
		if (glwindow->is_double_buffered())
			glwindow->swap_buffers();
		else
			glFlush();

		glwindow->gl_end();

		return true;
	}

	bool DrawArea::on_timeout() {
		/*Force our program to redraw*/
		Glib::RefPtr<Gdk::Window> win = get_window();
		if (win) {
			Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
			win->invalidate_rect(r, false);
		}
		return true;
	}

	void DrawArea::drawScene() {
		int i;
		float r,lati,longi,dx,dy,dz;
		float matColors[4];

		// Absolute Frame of Reference
		// floor
		glColor3f( 1, 1, 1 );
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		for(i=0;i<((int)MAXWORLD+1);i++) {
			v3f(-(int)MAXWORLD*10/2.+(float)i*10,-(int)MAXWORLD*10/2.,0.);
			v3f(-(int)MAXWORLD*10/2.+(float)i*10,(int)MAXWORLD*10/2.,0.);
			v3f(-(int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.);
			v3f((int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.);
		}
		glEnd();

		// absolute axis
		glLineWidth(3.0f);
		glColor3f( 0.7, 0., 0. );
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 10.0, 0.0, 0.0 );
		glEnd();
		glColor3f( 0.,0.7,0. );
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 0.0, 10.0, 0.0 );
		glEnd();
		glColor3f( 0.,0.,0.7 );
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 0.0, 0.0, 10.0 );
		glEnd();
		glLineWidth(3.0f);

		// Robot Frame of Reference
		mypioneer.posx=0/100.;
		mypioneer.posy=0/100.;
		mypioneer.posz=0.;
		mypioneer.foax=0/100.;
		mypioneer.foay=0/100.;
		mypioneer.foaz=10.;
		mypioneer.roll=this->robottheta;

		glTranslatef(mypioneer.posx,mypioneer.posy,mypioneer.posz);
		dx=(mypioneer.foax-mypioneer.posx);
		dy=(mypioneer.foay-mypioneer.posy);
		dz=(mypioneer.foaz-mypioneer.posz);
		longi=(float)atan2(dy,dx)*360./(2.*PI);
		glRotatef(longi,0.,0.,1.);
		r=sqrt(dx*dx+dy*dy+dz*dz);
		if (r<0.00001) lati=0.;
		else lati=acos(dz/r)*360./(2.*PI);
		glRotatef(lati,0.,1.,0.);
		glRotatef(mypioneer.roll,0.,0.,1.);

		// X axis
		glColor3f( 1., 0., 0. );
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 5.0, 0.0, 0.0 );
		glEnd();

		// Y axis
		glColor3f( 0., 1., 0. );  
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 0.0, 5.0, 0.0 );
		glEnd();

		// Z axis
		glColor3f( 0., 0., 1.);
		glBegin(GL_LINES);
		v3f( 0.0, 0.0, 0.0 );   
		v3f( 0.0, 0.0, 5.0 );
		glEnd();

		// robot body
		glEnable (GL_LIGHTING);
		glPushMatrix();
		glTranslatef(1.,0.,0.);

		// the body it is not centered. With this translation we center it
		glPopMatrix();
		glDisable (GL_LIGHTING);

		// lasers
		glLineWidth(1.0f);
		glEnable (GL_LIGHTING); // luces... entramos en parte de dibujado del pioneer con texturas
		matColors[0] = 1.0;
		matColors[1] = 0.0;
		matColors[2] = 0.0;
		matColors[3] = 0.5;
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);


		glDisable (GL_LIGHTING);

		/* ADD YOUR CODE HERE */

		glColor3f( 0, 0, 0 );
		glLineWidth(2.0f);
		for(int i=0;i<numlines;i++) {
			glBegin(GL_LINES);
				glVertex3f(lines[i][0]/scale, lines[i][1]/scale, lines[i][2]/scale);
				glVertex3f(lines[i][4]/scale, lines[i][5]/scale, lines[i][6]/scale);
			glEnd();	
		}
		for(i=0;i<numextra_lines;i++) {
			glBegin(GL_LINES);
				glVertex3f(extra_lines[i][0]/scale, extra_lines[i][1]/scale, extra_lines[i][2]/scale);
				glVertex3f(extra_lines[i][4]/scale, extra_lines[i][5]/scale, extra_lines[i][6]/scale);
			glEnd();
		}
		glLineWidth(3.0f);
		glColor3f( 0, 0.5, 0.5 );
		for(i=0;i<num_camera_lines;i++) {
			glBegin(GL_LINES);
				glVertex3f(camera_lines[i][0]/scale, camera_lines[i][1]/scale, camera_lines[i][2]/scale);
				glVertex3f(camera_lines[i][4]/scale, camera_lines[i][5]/scale, camera_lines[i][6]/scale);
			glEnd();
		}
		glColor3f( 0, 0, 0 );
		if (draw_kinect_points){
			for (std::vector<jderobot::RGBPoint>::iterator it = cloud.begin(); it != cloud.end(); ++it){
				if (draw_kinect_with_color)
					glColor3f( it->r, it->g, it->b );
				glBegin(GL_POINTS);
					glVertex3f(it->x/scale,it->y/scale,it->z/scale);
				glEnd();
			}
				
        			/* points connected */
				/*if (i < (img_width*(img_height-1))){
					//i , i + img_width
					float sq;
					sq = sqrt((kinect_points[i][0] - kinect_points[i+img_width][0])*(kinect_points[i][0] - kinect_points[i+img_width][0]) + (kinect_points[i][1] - kinect_points[i+img_width][1])*(kinect_points[i][1] - kinect_points[i+img_width][1]) + (kinect_points[i][2] - kinect_points[i+img_width][2])*(kinect_points[i][2] - kinect_points[i+img_width][2]));
					if ((sq < 3)&&(sq!=0)){

						float tt;
						tt = (kinect_points[i][0] - kinect_points[i+img_width][0]);

						std::cout << "pinto" << tt  << std::endl;
						glBegin(GL_LINES);
							glVertex3f(kinect_points[i][0]/scale, kinect_points[i][1]/scale, kinect_points[i][2]/scale);
							glVertex3f(kinect_points[i+img_width][0]/scale, kinect_points[i+img_width][1]/scale, kinect_points[i+img_width][2]/scale);
						glEnd();	
					}
				}*/
		}
		
		
		
		
	}

	bool DrawArea::on_motion_notify(GdkEventMotion* event) {
		float desp = 0.01;
		float x=event->x;
		float y=event->y;

		/* if left mouse button is toggled */
		if (event->state & GDK_BUTTON1_MASK) {
			if ((x - old_x) > 0.0) longi -= desp;
			else if ((x - old_x) < 0.0) longi += desp;

			if ((y - old_y) > 0.0) lati += desp;
			else if ((y - old_y) < 0.0) lati -= desp;

			this->glcam_pos.x=radius*cosf(lati)*cosf(longi) + this->glcam_foa.x;
			this->glcam_pos.y=radius*cosf(lati)*sinf(longi) + this->glcam_foa.y;
			this->glcam_pos.z=radius*sinf(lati) + this->glcam_foa.z;
		}

		/* if right mouse button is toggled */
		if (event->state & GDK_BUTTON3_MASK) {
			if ((x - old_x) > 0.0) longi -= desp;
			else if ((x - old_x) < 0.0) longi += desp;

			if ((y - old_y) > 0.0) lati += desp;
			else if ((y - old_y) < 0.0) lati -= desp;

			this->glcam_foa.x=-radius*cosf(lati)*cosf(longi) + this->glcam_pos.x;
			this->glcam_foa.y=-radius*cosf(lati)*sinf(longi) + this->glcam_pos.y;
			this->glcam_foa.z=-radius*sinf(lati) + this->glcam_pos.z;
		}

		old_x=x;
		old_y=y;
		return true;
	}

	bool DrawArea::on_drawarea_scroll(GdkEventScroll * event) {
		float vx, vy, vz;

		vx = (this->glcam_foa.x - this->glcam_pos.x)/radius;
		vy = (this->glcam_foa.y - this->glcam_pos.y)/radius;
		vz = (this->glcam_foa.z - this->glcam_pos.z)/radius;

		if (event->direction == GDK_SCROLL_UP) {
			this->glcam_foa.x = this->glcam_foa.x + vx;
			this->glcam_foa.y = this->glcam_foa.y + vy;
			this->glcam_foa.z = this->glcam_foa.z + vz;

			this->glcam_pos.x = this->glcam_pos.x + vx;
			this->glcam_pos.y = this->glcam_pos.y + vy;
			this->glcam_pos.z = this->glcam_pos.z + vz;
		}

		if (event->direction == GDK_SCROLL_DOWN) {
			this->glcam_foa.x = this->glcam_foa.x - vx;
			this->glcam_foa.y = this->glcam_foa.y - vy;
			this->glcam_foa.z = this->glcam_foa.z - vz;

			this->glcam_pos.x = this->glcam_pos.x - vx;
			this->glcam_pos.y = this->glcam_pos.y - vy;
			this->glcam_pos.z = this->glcam_pos.z - vz;
		}
		return true;
	}

int
DrawArea::load_line(FILE *myfile)
{
	const int limit = 256;	
	char word1[limit],words[10][limit];
	int i=0;
	char buffer_file[limit];  
	int number;

	buffer_file[0]=fgetc(myfile);
	if (feof(myfile)) 
		return EOF;
	if (buffer_file[0]==(char)255) 
		return EOF; 
	if (buffer_file[0]=='#'){
		while(fgetc(myfile)!='\n'); 
		return 0;
	}
	if (buffer_file[0]==' '){
		while(buffer_file[0]==' ') 
			buffer_file[0]=fgetc(myfile);
	}
  	if (buffer_file[0]=='\t'){
		while(buffer_file[0]=='\t') 
			buffer_file[0]=fgetc(myfile);
	}

  /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
	while ((buffer_file[i]!='\n')&&(buffer_file[i] != (char)255) && (i<limit-1)) {
		buffer_file[++i]=fgetc(myfile);
	}

  
	if (i >= limit-1) { 
		printf("%s...\n", buffer_file); 
		printf ("Line too long in config file!\n"); 
		exit(-1);
	}
	buffer_file[++i]='\0';

	if (sscanf(buffer_file,"%s",word1)!=1) 
		return 0; 
	/* return EOF; empty line*/
	else {
		number=sscanf(buffer_file,"%s %s %s %s %s %s %s %s %s %s %s",word1,words[0],words[1],words[2],words[3],words[4],words[5],words[6],words[7],words[8],words[9]);
		if (strcmp(word1,"worldline")==0){
			if (number == 9){
				if (numlines <MAX_LINES){
					lines[numlines][0] = (float)atof(words[0]);
					lines[numlines][1] = (float)atof(words[1]);
					lines[numlines][2] = (float)atof(words[2]);
					lines[numlines][3] = (float)atof(words[3]);
					lines[numlines][4] = (float)atof(words[4]);
					lines[numlines][5] = (float)atof(words[5]);
					lines[numlines][6] = (float)atof(words[6]);
					lines[numlines][7] = (float)atof(words[7]);
					numlines++;
				}
				else{
					printf("rgbdViz: error, too much lines in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, worldfile line not valid: %s\n",buffer_file);
			}
		}
		else if (strcmp(word1, "post")==0){
			if (number==11){
				if (post_p < MAX_POST){
					post[post_p][0]= (float)atof(words[0]);
					post[post_p][1]= (float)atof(words[1]);
					post[post_p][2]= (float)atof(words[2]);
					post[post_p][3]= (float)atof(words[3]);
					post[post_p][4]= (float)atof(words[4]);
					post[post_p][5]= (float)atof(words[5]);
					post[post_p][6]= (float)atof(words[6]);
					post[post_p][7]= (float)atof(words[7]);
					post[post_p][8]= (float)atof(words[8]);
					post[post_p][9]= (float)atof(words[9]);
					post_p++;
				}
				else{
					printf("rgbdViz: error, too much posts in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, post line not valid: %s\n",buffer_file);
			}
		}
		else if (strcmp(word1, "jamb")==0){
			if (number==11){
				if (jamb_p<MAX_JAMB){
					jamb[jamb_p][0]= (float)atof(words[0]);
					jamb[jamb_p][1]= (float)atof(words[1]);
					jamb[jamb_p][2]= (float)atof(words[2]);
					jamb[jamb_p][3]= (float)atof(words[3]);
					jamb[jamb_p][4]= (float)atof(words[4]);
					jamb[jamb_p][5]= (float)atof(words[5]);
					jamb[jamb_p][6]= (float)atof(words[6]);
					jamb[jamb_p][7]= (float)atof(words[7]);
					jamb[jamb_p][8]= (float)atof(words[8]);
					jamb[jamb_p][9]= (float)atof(words[9]);
					jamb_p++;
				}
				else{
					printf("rgbdViz: error, too much jambs in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, post line not valid: %s\n",buffer_file);
			}
		}
		else if (strcmp(word1, "circle")==0){
			if (number==4){
				if (circle_p<MAX_CIRCLE){
					circle[circle_p][0]=(float)atof(words[0]);
					circle[circle_p][1]=(float)atof(words[1]);
					circle[circle_p][2]=(float)atof(words[2]);
					circle_p++;
				}
				else{
					printf("rgbdViz: error, too much circles in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, circle line not valid: %s\n",buffer_file);
			}
		}
		else if (strcmp(word1, "triangle")==0){
			if (number==4){
				if (triangle_p<MAX_TRIANGLE){
					triangle[triangle_p][0]=(float)atof(words[0]);
					triangle[triangle_p][1]=(float)atof(words[1]);
					triangle[triangle_p][2]=(float)atof(words[2]);
					triangle_p++;
				}
				else{
					printf("rgbdViz: error, too much triangles in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, triangle line not valid: %s\n",buffer_file);
			}
		}
		else if (strcmp(word1, "sq")==0){
			if (number==4){
				if (sq_p<MAX_SQUARE){
					sq[sq_p][0]=(float)atof(words[0]);
					sq[sq_p][1]=(float)atof(words[1]);
					sq[sq_p][2]=(float)atof(words[2]);
					sq_p++;
				}
				else{
					printf("rgbdViz: error, too much sqs in the world file configuration.\n");
				}
			}
			else{
				printf("rgbdViz: error, sq line not valid: %s\n",buffer_file);
			}
		}
	}
	return 1;
}

/****************************************************************************************/
/****************************************************************************************/

int 
DrawArea::read_world_file(char* worldfile){
	FILE *myfile;
	int i;

	if ((myfile=fopen(worldfile,"r"))==NULL){
		std::cout << "rgbdViz: cannot find world file" << std::endl;
		return -1;
	}

	do{
		i=load_line(myfile);
	} while(i!=EOF);

	fclose(myfile);
    return 0;
}

void
DrawArea::add_line(float x0,float y0, float z0, float x1, float y1, float z1){
	if (numextra_lines <MAX_LINES){
		extra_lines[numextra_lines][0] = x0;
		extra_lines[numextra_lines][1] = y0;
		extra_lines[numextra_lines][2] = z0;
		extra_lines[numextra_lines][3] = 0;
		extra_lines[numextra_lines][4] = x1;
		extra_lines[numextra_lines][5] = y1;
		extra_lines[numextra_lines][6] = z1;
		extra_lines[numextra_lines][7] = 0;
		numextra_lines++;
	}
	else{
		printf("NaoVision: error, too much lines in the world file configuration.\n");
	}
}

void
DrawArea::add_camera_line(float x0,float y0, float z0, float x1, float y1, float z1){
	if (num_camera_lines <MAX_LINES){
		camera_lines[num_camera_lines][0] = x0;
		camera_lines[num_camera_lines][1] = y0;
		camera_lines[num_camera_lines][2] = z0;
		camera_lines[num_camera_lines][3] = 0;
		camera_lines[num_camera_lines][4] = x1;
		camera_lines[num_camera_lines][5] = y1;
		camera_lines[num_camera_lines][6] = z1;
		camera_lines[num_camera_lines][7] = 0;
		num_camera_lines++;
	}
	else{
		printf("NaoVision: error, too much camera lines in the world file configuration.\n");
	}
}

void
DrawArea::clear_camera_lines(){
	num_camera_lines=0;
}



void
DrawArea::add_kinect_point(float x,float y, float z,int r,int g, int b){
		jderobot::RGBPoint p;

		p.x=x;
		p.y=y;
		p.z=z;
		p.r = (float)r/255;
		p.g = (float)g/255;
		p.b = (float)b/255;
		cloud.push_back(p);
} 

void 
DrawArea::clear_points(){
	cloud.clear();
}

void 
DrawArea::clearExtraLines(){
	numextra_lines=0;
}

void
DrawArea::setCamerasResolution(int width, int height){
	img_width=width;
	img_height=height;
}

} // namespace

