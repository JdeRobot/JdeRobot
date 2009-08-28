/*
 *  Copyright (C) 1997-2009 JDE Developers Team
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
 *  Authors : Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
 */




#include <jde.h>
#include <stdio.h>
#include "naooperator.h"
#include <unistd.h> 
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <graphics_gtk.h>
#include <glade/glade-xml.h> 
#include <interfaces/varcolor.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>



int finish_flag=0; 

int naooperator_id=0; 
int naooperator_brothers[MAX_SCHEMAS];
arbitration naooperator_callforarbitration;
int naooperator_cycle=150; /* ms */ 
GladeXML *xml;
GtkWidget *win;
deletedisplay mydelete_displaycallback;
registerdisplay myregister_displaycallback;
pthread_mutex_t main_mutex;
char **mycolorA=NULL;
Varcolor *myAA;
runFn color_run;
stopFn color_stop;

/*imported variables*/
float* v=NULL;
float* w=NULL;
float* my_pan_angle=NULL;
float* my_tilt_angle=NULL;
float *mylongitude=NULL;
float *mylatitude=NULL;
float *max_longitude=NULL;
float *max_latitude=NULL;
float *min_longitude=NULL;
float *min_latitude=NULL;
float *longitude_speed=NULL;
float *latitude_speed=NULL;
unsigned long int *my_clock;


/*my local values */
float longitude;
float latitude;


int naobody_image_runs=0; /* image provided and where is it */
int naobody_runs=0; /* body provided*/
int ptencoders_run=0;
int ptmotors_run=0;
int image_source=0; /* where is the image provided*/


/*images to show on gui*/
char* image;

int cargado=0;



/* for the head positioner */
GtkWidget *canvas_teleoperator;
float joystick_x, joystick_y;
float pt_joystick_x, pt_joystick_y;
int update_positioner=0; // 1 if the change on head is by the positioner 2 if the change is by the buttons

/* for the body positioner */
GtkWidget *canvas_body;
float joystick_x_body, joystick_y_body;
float pt_joystick_x_body, pt_joystick_y_body;
int update_positioner_body=0; // 1 if the change on head is by the positioner 2 if the change is by the buttons




/*test variales, I have to import these values from the motor driver*/
float minw=-100;
float maxw=100;
float maxv=100;


/*radios*/
int radio_process=1;
int radio_changes=0;



void load_image() {
	pthread_mutex_lock(&main_mutex);
	if (naobody_image_runs!=2){
		printf("NaoOperator: error, not image provided\n");
		pthread_mutex_unlock(&main_mutex);
		return;
	}
	printf("%d,%d\n",(*myAA).width, (*myAA).height);
	image=(char *)malloc((*myAA).width*(*myAA).height*3);
	{
		GdkPixbuf *imgBuff;
		GtkImage *img=(GtkImage *)glade_xml_get_widget(xml, "image7");
		imgBuff = gdk_pixbuf_new_from_data((unsigned char *)image,GDK_COLORSPACE_RGB,0,8,(*myAA).width,(*myAA).height,(*myAA).width*3,NULL,NULL);
		gtk_image_clear(img);
		gtk_image_set_from_pixbuf(img, imgBuff);
		gtk_widget_queue_draw(GTK_WIDGET(img));
	}
	pthread_mutex_unlock(&main_mutex);
}


void naooperator_iteration()
{	
	int i;
	pthread_mutex_lock(&main_mutex);
	if(naobody_image_runs==2) {
		for (i=0;i<((*myAA).width*(*myAA).height); i++){
			image[i*3]=((*myAA).img)[i*3+2];
		  	image[i*3+1]=((*myAA).img)[i*3+1];
		  	image[i*3+2]=((*myAA).img)[i*3];
					
		}
	}
	if (ptmotors_run==2){
		*mylongitude=longitude;
		*mylatitude=latitude;
	}
	pthread_mutex_unlock(&main_mutex);
}

void naooperator_terminate()
{
	pthread_mutex_lock(&(all[naooperator_id].mymutex));
	put_state(naooperator_id,slept);
	printf("NaoOperator: off\n");
	pthread_mutex_unlock(&(all[naooperator_id].mymutex));
} 



/* stop a schema*/
int
naooperator_stop_schema(int a){

	int j=0;
	while (j<num_schemas){
		if (a==1){
			if (strcmp(all[j].name,"varcolorA")==0){
				printf("NaoOperator: varcolorA is stopped\n");
				naobody_image_runs=1;
				break;
			}
		}
		else if (a==2){
			if (strcmp(all[j].name,"varcolorB")==0){
				printf("NaoOperator: varcolorB is stopped\n");
				naobody_image_runs=1;
				break;
			}
		}
		else if (a==3){
			if (strcmp(all[j].name,"varcolorC")==0){
				printf("NaoOperator: varcolorC is stopped\n");
				naobody_image_runs=1;
				break;
			}
		}
		else if (a==4){
			if (strcmp(all[j].name,"varcolorD")==0){
				printf("NaoOperator: varcolorD is stopped\n");
				naobody_image_runs=1;
				break;
			}
		}
		else if (a==5){
			if (strcmp(all[j].name,"varcolorE")==0){
				printf("NaoOperator: varcolorE is stopped\n");
				naobody_image_runs=1;
				break;
			}
		}
		else if (a==6){
			if (strcmp(all[j].name,"motors")==0){
				printf("NaoOperator: motors is stopped\n");
				naobody_runs=1;
				break;
			}
		}
		else if (a==7){
			if (strcmp(all[j].name,"ptencoders")==0){
				printf("NaoOperator: ptencoders is stopped\n");
				ptencoders_run=1;
				break;
			}
		}
		else if (a==8){
			if (strcmp(all[j].name,"ptmotors")==0){
				printf("NaoOperator: ptmotors is stopped\n");
				ptmotors_run=1;
				break;
			}
		}
			
		j++;
	}
	if (j<num_schemas){
		stopFn st;
		st=(stopFn)all[j].stop;
		st();
	}
}


/* run a schema*/
int
naooperator_run_schema(int a){

	int j=0;
	while (j<num_schemas){
		if (a==1){
			if (strcmp(all[j].name,"varcolorA")==0){
				printf("NaoOperator: varcolorA is loaded\n");
				break;
			}
		}
		else if (a==2){
			if (strcmp(all[j].name,"varcolorB")==0){
				printf("NaoOperator: varcolorB is loaded\n");
				break;
			}
		}
		else if (a==3){
			if (strcmp(all[j].name,"varcolorC")==0){
				printf("NaoOperator: varcolorC is loaded\n");
				break;
			}
		}
		else if (a==4){
			if (strcmp(all[j].name,"varcolorD")==0){
				printf("NaoOperator: varcolorD is loaded\n");
				break;
			}
		}
		else if (a==5){
			if (strcmp(all[j].name,"varcolorE")==0){
				printf("NaoOperator: varcolorE is loaded\n");
				break;
			}
		}
		else if (a==6){
			if (strcmp(all[j].name,"motors")==0){
				printf("NaoOperator: motors is loaded\n");
				break;
			}
		}
		else if (a==7){
			if (strcmp(all[j].name,"ptencoders")==0){
				printf("NaoOperator: ptencoders is loaded\n");
				break;
			}
		}
		else if (a==8){
			if (strcmp(all[j].name,"ptmotors")==0){
				printf("NaoOperator: ptmotors is loaded\n");
				break;
			}
		}
		
		j++;
	}
	if (j<num_schemas){
		runFn r;
		r=(runFn)all[j].run;
		r(naooperator_id,NULL,NULL);
		return 1;
	}
	return 0;
}


void 
naooperator_imports(int a){
	switch (a){
		case 1:
			if (naobody_image_runs){
				if (naooperator_run_schema(image_source)){
					switch (image_source){
						case 1: 
								myAA=(Varcolor *)myimport("varcolorA","varcolorA");
								naobody_image_runs=2;
								break;	
						case 2: 
								myAA=(Varcolor *)myimport("varcolorB","varcolorB");
								naobody_image_runs=2;
								break;
						case 3: 
								myAA=(Varcolor *)myimport("varcolorC","varcolorC");
								naobody_image_runs=2;
								break;					
						case 4: 
								myAA=(Varcolor *)myimport("varcolorD","varcolorD");
								naobody_image_runs=2;
								break;
						case 5: 
								myAA=(Varcolor *)myimport("varcolorE","varcolorE");
								naobody_image_runs=2;
								break;
						default:
								printf("NaoOperator: image source not valid.\n");
								break;
					}
				}
			}
			break;
		case 2:
			if (naobody_runs==1){
				if (naooperator_run_schema(6)){
					naobody_runs=2;
					v=(float *)myimport("motors","v");
					w=(float *)myimport("motors","w");
				}
			}
			break;
		case 3:
			if (ptencoders_run==1){
				if (naooperator_run_schema(7)){
					ptencoders_run=2;
					my_pan_angle=(float *)myimport("ptencoders","pan_angle");
					my_tilt_angle=(float *)myimport("ptencoders","tilt_angle");
					my_clock=(unsigned long int *)myimport("ptencoders","clock");
				}
			}
			if (ptmotors_run==1){
				if (naooperator_run_schema(8)){
					ptmotors_run=2;
					mylongitude=(int *)myimport("ptmotors","longitude");
					mylatitude=(float *)myimport("ptmotors","latitude");
					max_longitude=(float *)myimport("ptmotors","max_longitude");
					min_longitude=(float *)myimport("ptmotors","min_longitude");
					max_latitude=(float *)myimport("ptmotors","max_latitude");
					min_latitude=(float *)myimport("ptmotors","min_latitude");
					latitude_speed=(float *)myimport("ptmotors","latitude_speed");
					longitude_speed=(float *)myimport("ptmotors","longitude_speed");
				}
				else{
					printf("NaoBody: could not load ptmotors\n");
				}
				
			}
			break;
	}
}


void naooperator_run(int father, int *brothers, arbitration fn)
{
	int i;	

 
	pthread_mutex_lock(&(all[naooperator_id].mymutex));
	/* this schema resumes its execution with no children at all */
	for(i=0;i<MAX_SCHEMAS;i++) 
		all[naooperator_id].children[i]=FALSE;
	all[naooperator_id].father=father;
	if (brothers!=NULL) {
		for(i=0;i<MAX_SCHEMAS;i++) 
			naooperator_brothers[i]=-1;
    		i=0;
    		while(brothers[i]!=-1) {naooperator_brothers[i]=brothers[i];i++;}
  	}
	naooperator_callforarbitration=fn;
	put_state(naooperator_id,notready);
	printf("NaoOperator: on\n");
	pthread_cond_signal(&(all[naooperator_id].condition));
	pthread_mutex_unlock(&(all[naooperator_id].mymutex));
	if (naobody_runs)
		naooperator_imports(2); 
	if ((ptmotors_run)||(ptencoders_run))
		naooperator_imports(3);
	
} 

void *naooperator_thread(void *not_used) 
{
	struct timeval a,b;
	long diff, next;

	for(;finish_flag==0;) {
		pthread_mutex_lock(&(all[naooperator_id].mymutex));
		if (all[naooperator_id].state==slept) {
			pthread_cond_wait(&(all[naooperator_id].condition),&(all[naooperator_id].mymutex));
			pthread_mutex_unlock(&(all[naooperator_id].mymutex));
		}
		else {
			/* check preconditions. For now, preconditions are always satisfied*/
			if (all[naooperator_id].state==notready) put_state(naooperator_id,ready);
			/* check brothers and arbitrate. For now this is the only winner */
			else if (all[naooperator_id].state==ready)	{ 
				put_state(naooperator_id,winner);
			}	  
			else if (all[naooperator_id].state==winner);

			if (all[naooperator_id].state==winner) {
				/* I'm the winner and must execute my iteration */
				pthread_mutex_unlock(&(all[naooperator_id].mymutex));
				gettimeofday(&a,NULL);
				naooperator_iteration();
				gettimeofday(&b,NULL);  
				diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
				next = naooperator_cycle*1000-diff-10000; 
				if (next>0) 
					/* discounts 10ms taken by calling usleep itself */
					usleep(naooperator_cycle*1000-diff);
				else {
					/* just let this iteration go away. overhead time negligible */
					printf("time interval violated: NaoOperator\n"); 
					usleep(naooperator_cycle*1000);
				}
			}
			else {
				pthread_mutex_unlock(&(all[naooperator_id].mymutex));
				usleep(naooperator_cycle*1000);
			}
		}
	}
	pthread_exit(0);
} 

void naooperator_stop()
{
	naooperator_stop_schema(image_source);
	if (naobody_runs)
		naooperator_stop_schema(6);
	if (ptencoders_run){
		naooperator_stop_schema(7);
	}
	if (ptmotors_run){
		naooperator_stop_schema(8);
	}
  pthread_mutex_lock(&(all[naooperator_id].mymutex));
  put_state(naooperator_id,slept);
  printf("NaoOperator: off\n");
  pthread_mutex_unlock(&(all[naooperator_id].mymutex));
}



int naooperator_parseconf(char *configfile){
	
	int end_parse=0; int end_section=0; int driver_config_parsed=0;
	FILE *myfile;
	const int limit = 256;


	if ((myfile=fopen(configfile,"r"))==NULL){
		printf("NaoOperator: cannot find config file\n");
		return -1;
	}
	do{
    
		char word[256],word2[256],buffer_file[256];
		int i=0; int j=0;

		buffer_file[0]=fgetc(myfile);
    
		/* end of file */
		if (feof(myfile)){
			end_section=1;
			end_parse=1;
      
		/* line comment */
		}
		else if (buffer_file[0]=='#') {
			while(fgetc(myfile)!='\n');
      
			/* white spaces */
		}
		else if (buffer_file[0]==' ') {
			while(buffer_file[0]==' ') 
				buffer_file[0]=fgetc(myfile);

			/* tabs */
		}
		else if(buffer_file[0]=='\t') {
			while(buffer_file[0]=='\t') 
				buffer_file[0]=fgetc(myfile);
			/* storing line in buffer */
		}
		else{
      
			while(buffer_file[i]!='\n') 
				buffer_file[++i]=fgetc(myfile);
			buffer_file[++i]='\0';
			if (i >= limit-1) {
				printf("%s...\n", buffer_file);
				printf ("NaoOperator: line too long in config file!\n");
				return -1;
			}
      
			/* first word of the line */
			if (sscanf(buffer_file,"%s",word)==1){
				if (strcmp(word,"schema")==0) {
					while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) 
						j++;
					sscanf(&buffer_file[j],"%s",word2);
	  
					
					/* checking if this section matchs naobody */
					if (strcmp(word2,"naooperator")==0){
						/* the sections match */
						do{
                     			char buffer_file2[256],word3[256],word4[256];
                     			char word5[256],word6[256], word7[256];
                     			int k=0; int z=0;
							buffer_file2[0]=fgetc(myfile);
	      
							/* end of file */
							if (feof(myfile)){
								end_section=1;
								end_parse=1;
		
							/* line comment */
							}
							else if (buffer_file2[0]=='#') {
								while(fgetc(myfile)!='\n');
	      
							/* white spaces */
							}
							else if (buffer_file2[0]==' ') {
								while(buffer_file2[0]==' ') 
									buffer_file2[0]=fgetc(myfile);

							/* tabs */
 							}
							else if(buffer_file2[0]=='\t') {
								while(buffer_file2[0]=='\t') 
									buffer_file2[0]=fgetc(myfile);

							/* storing line in buffer */
							}
							else{
								while(buffer_file2[k]!='\n') 
									buffer_file2[++k]=fgetc(myfile);
								buffer_file2[++k]='\0';
		
								/* first word of the line */
								if (sscanf(buffer_file2,"%s",word3)==1){
									if (strcmp(word3,"end_schema")==0) {
										while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) 
											z++;
										driver_config_parsed=1;
										end_section=1;
										//end_parse=1;
		    
									}
									else if (strcmp(word3,"driver")==0) {
										while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&& (buffer_file2[z]!='\0')&& (buffer_file2[z]!='\t')) 
											z++;
										printf("NaoOperator: error in config file.\n'end_section' keyword required before starting new driver section.\n");
										end_section=1; 
										//end_parse=1;

									}
									else if(strcmp(word3,"uses")==0){
										
										int words;
										while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&& (buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) 
											z++;
										words=sscanf(buffer_file2,"%s %s %s %s %s",word3,word4,word5,word6,word7);
										if (words){
											if(strcmp(word4,"motors")==0){
												naobody_runs=1;
											}
											else if(strcmp(word4,"ptencoders")==0){
												ptencoders_run=1;
											}
											else if(strcmp(word4,"ptmotors")==0){
												ptmotors_run=1;
											}
											else if(strcmp(word4,"varcolorA")==0){
												image_source=1;
											}
											else if(strcmp(word4,"varcolorB")==0){
												image_source=2;
											}
											else if(strcmp(word4,"varcolorC")==0){
												image_source=3;
											}
											else if(strcmp(word4,"varcolorD")==0){
												image_source=4;
											}
											else if(strcmp(word4,"varcolorE")==0){
												image_source=5;
											}
										}
									}
								}
							}
						} while(end_section==0);
						end_section=0;
					}
				}
			}
		}
   }while(end_parse==0);
	if (image_source){
		naobody_image_runs=1;
	}
	/* checking if a driver section was read */
	if(driver_config_parsed==1){
		if(naobody_runs==0)
			printf("NaoOperator: WARNING! not body motion provided\n");
		if(ptmotors_run==0)
			printf("NaoOperator: WARNING! not Head motion provided.\n");
		if(ptmotors_run==0)
			printf("NaoOperator: WARNING! not Head encoders provided.\n");
		if(naobody_image_runs==0)
			printf("NaoOperator: WARNING! not Imagen provided.\n");
		return 0;
	} else 
		return -1;
}



void naooperator_guiinit(){
   if (myregister_displaycallback==NULL){
      if ((myregister_displaycallback=
           (registerdisplay)myimport ("graphics_gtk",
            "register_displaycallback"))==NULL)
      {
         printf ("I can't fetch register_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=
           (deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))
           ==NULL)
      {
         printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
   }
}


void naooperator_init(char *configfile)
{
	pthread_mutex_lock(&(all[naooperator_id].mymutex));
	myexport("naooperator","id",&naooperator_id);
	myexport("naooperator","resume",(void *)&naooperator_run);
	myexport("naooperator","suspend",(void *)&naooperator_terminate);
	printf("naooperator started up\n");
	if(naooperator_parseconf(configfile)==-1){
		printf("NaoOperator: configfile parsing error, system will be unestable.\n");
	}
	put_state(naooperator_id,slept);
	pthread_create(&(all[naooperator_id].mythread),NULL,naooperator_thread,NULL);
	pthread_mutex_unlock(&(all[naooperator_id].mymutex));
	naooperator_guiinit();
}




void naooperator_guidisplay(){
	GdkGC *gc;
	GdkGC *gc_body;
	GdkColormap *colormap;
	GdkColor color_white,color_black;
	float posy,posx;
	char buff[50];
	GtkWidget *entry;

	pthread_mutex_lock(&main_mutex);
	if (naobody_image_runs==2){
		GtkImage *img = GTK_IMAGE(glade_xml_get_widget(xml, "image7"));
		gtk_widget_queue_draw(GTK_WIDGET(img));
	}
	gdk_threads_enter();
	if (ptencoders_run==2){
			entry= glade_xml_get_widget(xml, "entry_realy");
			sprintf(buff,"%f",*my_pan_angle);
			gtk_entry_set_text(entry,buff);
			entry= glade_xml_get_widget(xml, "entry_realp");
			sprintf(buff,"%f",*my_tilt_angle);
			gtk_entry_set_text(entry,buff);
			if (my_clock!=NULL){
				entry= glade_xml_get_widget(xml, "entry_clock");
				sprintf(buff,"%ld",*my_clock);
				gtk_entry_set_text(entry,buff);
			}
	}



	if (ptmotors_run==2){	
			*longitude_speed=gtk_range_get_value(glade_xml_get_widget(xml, "hscale_longitude_speed"));
			*latitude_speed=gtk_range_get_value(glade_xml_get_widget(xml, "hscale_latitude_speed"));
			if (update_positioner==1){
				colormap = gdk_colormap_get_system ();
				gdk_color_black (colormap, & color_black);
				gdk_color_white (colormap, & color_white);
				gc = gdk_gc_new (canvas_teleoperator->window);
				gdk_gc_set_foreground (gc, & color_white);
				gdk_draw_rectangle (canvas_teleoperator->window,gc,TRUE,0,0,canvas_teleoperator->allocation.width,canvas_teleoperator->allocation.height);
				gdk_gc_set_foreground (gc, & color_black);
				gdk_draw_line(canvas_teleoperator->window,gc,0,pt_joystick_y,canvas_teleoperator->allocation.width,pt_joystick_y);
				gdk_draw_line(canvas_teleoperator->window,gc,pt_joystick_x,0,pt_joystick_x,canvas_teleoperator->allocation.height);
			}
			else if (update_positioner==2){
				posx=((longitude/(2*(*max_longitude)))+0.5)*canvas_teleoperator->allocation.width;
				posy=((latitude/(2*(*max_latitude)))+0.5)*canvas_teleoperator->allocation.height;
				posy=canvas_teleoperator->allocation.height-posy;
				colormap = gdk_colormap_get_system ();
				gdk_color_black (colormap, & color_black);
				gdk_color_white (colormap, & color_white);
				gc = gdk_gc_new (canvas_teleoperator->window);
				gdk_gc_set_foreground (gc, & color_white);
				gdk_draw_rectangle (canvas_teleoperator->window,gc,TRUE,0,0,canvas_teleoperator->allocation.width,canvas_teleoperator->allocation.height);
				gdk_gc_set_foreground (gc, & color_black);
				gdk_draw_line(canvas_teleoperator->window,gc,0,posy,canvas_teleoperator->allocation.width,posy);
				gdk_draw_line(canvas_teleoperator->window,gc,posx,0,posx,canvas_teleoperator->allocation.height);
			}
			
			if (update_positioner){
				entry= glade_xml_get_widget(xml, "entry_p");
				sprintf(buff,"%f",latitude);
				gtk_entry_set_text(entry,buff);
				entry= glade_xml_get_widget(xml, "entry_y");
				sprintf(buff,"%f",longitude);
				gtk_entry_set_text(entry,buff);
				update_positioner=0;
			}
	}
			
			
	if (naobody_runs==2){
			if (update_positioner_body==1){
				colormap = gdk_colormap_get_system ();
				gdk_color_black (colormap, & color_black);
				gdk_color_white (colormap, & color_white);
				gc_body = gdk_gc_new (canvas_body->window);
				gdk_gc_set_foreground (gc_body, & color_white);
				gdk_draw_rectangle (canvas_body->window,gc_body,TRUE,0,0,canvas_body->allocation.width,canvas_body->allocation.height);
				gdk_gc_set_foreground (gc_body, & color_black);
				gdk_draw_line(canvas_body->window,gc_body,0,pt_joystick_y_body,canvas_body->allocation.width,pt_joystick_y_body);
				gdk_draw_line(canvas_body->window,gc_body,pt_joystick_x_body,0,pt_joystick_x_body,canvas_body->allocation.height);
			}
			else if (update_positioner_body==2){
				posy=canvas_body->allocation.width-(*v)-1;
				posx=canvas_body->allocation.width- (*w-minw)/(maxw-minw)*canvas_body->allocation.width;
				posx=canvas_body->allocation.height-posx;
				colormap = gdk_colormap_get_system ();
				gdk_color_black (colormap, & color_black);
				gdk_color_white (colormap, & color_white);
				gc_body = gdk_gc_new (canvas_body->window);
				gdk_gc_set_foreground (gc_body, & color_white);
				gdk_draw_rectangle (canvas_body->window,gc_body,TRUE,0,0,canvas_body->allocation.width,canvas_body->allocation.height);
				gdk_gc_set_foreground (gc_body, & color_black);
				gdk_draw_line(canvas_body->window,gc_body,0,posy,canvas_body->allocation.width,posy);
				gdk_draw_line(canvas_body->window,gc_body,posx,0,posx,canvas_body->allocation.height);
			}
			if (update_positioner_body){
				entry= glade_xml_get_widget(xml, "entry_v");
				sprintf(buff,"%f",*v);
				gtk_entry_set_text(entry,buff);
				entry= glade_xml_get_widget(xml, "entry_w");
				sprintf(buff,"%f",*w);
				gtk_entry_set_text(entry,buff);
				update_positioner_body=0;
				gtk_range_set_value(glade_xml_get_widget(xml, "vscale_v"),*v);
				gtk_range_set_value(glade_xml_get_widget(xml, "hscale_w"),*w);
			}
			
		}
	gdk_threads_leave();
   	pthread_mutex_unlock(&main_mutex);
}

/****************************/
/* GUI BUTTONS CONFIGURATION*/
/****************************/

gboolean 
on_delete_naooperator (GtkWidget *widget, GdkEvent *event, gpointer user_data){
	gtk_widget_hide(glade_xml_get_widget(xml, "NaoOperator"));
   return TRUE;
}

void 
on_button_exit_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	jdeshutdown(0);
}

void
on_camera1_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if (naobody_image_runs==1){
		naooperator_imports(1);
		load_image();
		gtk_widget_show(glade_xml_get_widget(xml, "hbox_image"));
	}
	else if (naobody_image_runs==2){
		naooperator_stop_schema(image_source);
		gtk_widget_hide(glade_xml_get_widget(xml, "hbox_image"));
	}
	else{
		printf("NaoOperator: not image provided por NaoBody.\n");
	}
}


void 
on_button_up_v_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	float v_aux;
	v_aux=*v;
	v_aux++;
	*v=v_aux;
	char buff[50];
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_v");
	sprintf(buff,"%f",*v);
	gtk_entry_set_text(entry,buff);
	update_positioner_body=2;
}

void 
on_button_down_v_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	float v_aux;
	v_aux=*v;
	v_aux--;
	*v=v_aux;
	char buff[50];
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_v");
	sprintf(buff,"%f",*v);
	gtk_entry_set_text(entry,buff);
	update_positioner_body=2;
}


void 
on_button_up_w_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	float w_aux;
	w_aux=*w;
	w_aux=w_aux+0.5;
	*w=w_aux;
	char buff[50];
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_w");
	sprintf(buff,"%f",*w);
	gtk_entry_set_text(entry,buff);
	update_positioner_body=2;
}

void 
on_button_down_w_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	float w_aux;
	w_aux=*w;
	w_aux=w_aux-0.5;
	*w=w_aux;
	char buff[50];
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_w");
	sprintf(buff,"%f",*w);
	gtk_entry_set_text(entry,buff);
	update_positioner_body=2;
}

void 
on_button_stop_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	*w=0;
	*v=0;
	char buff[50];
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_w");
	sprintf(buff,"%f",*w);
	gtk_entry_set_text(entry,buff);
	entry= glade_xml_get_widget(xml, "entry_v");
	sprintf(buff,"%f",*v);
	gtk_entry_set_text(entry,buff);
	update_positioner_body=2;
}





void 
on_button_up_p_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	latitude++;
	update_positioner=2;
}

void 
on_button_down_p_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	latitude--;
	update_positioner=2;
}


void 
on_button_up_y_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	longitude++;
	update_positioner=2;
}


void 
on_button_down_y_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	longitude--;
	update_positioner=2;
}

void 
on_button_go_body_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_v");
	*v=atof(gtk_entry_get_text(entry));
	entry= glade_xml_get_widget(xml, "entry_w");
	*w=atof(gtk_entry_get_text(entry));
	update_positioner_body=2;

}

void 
on_button_go_head_clicked(GtkCheckMenuItem *menu_item, gpointer user_data){
	GtkWidget *entry;
	entry= glade_xml_get_widget(xml, "entry_y");
	longitude=atof(gtk_entry_get_text(entry));
	entry= glade_xml_get_widget(xml, "entry_p");
	latitude=atof(gtk_entry_get_text(entry));
	update_positioner=2;
	
}



static gboolean expose_event_teleoperator (GtkWidget *widget, GdkEventButton *event, gpointer data){

	GdkGC *gc;
	GdkColormap *colormap;
	GdkColor color_white,color_black;

	colormap = gdk_colormap_get_system ();
	gdk_color_black (colormap, & color_black);
	gdk_color_white (colormap, & color_white);
	gc = gdk_gc_new (canvas_teleoperator->window);
	gdk_gc_set_foreground (gc, & color_white);

	gdk_draw_rectangle (canvas_teleoperator->window,gc,TRUE,0,0,canvas_teleoperator->allocation.width,canvas_teleoperator->allocation.height);
	gdk_gc_set_foreground (gc, & color_black);
	gdk_draw_line(canvas_teleoperator->window,gc,0,pt_joystick_y,canvas_teleoperator->allocation.width,pt_joystick_y);
	gdk_draw_line(canvas_teleoperator->window,gc,pt_joystick_x,0,pt_joystick_x,canvas_teleoperator->allocation.height);

	return TRUE;
}

static gboolean button_press_event_teleoperator (GtkWidget *widget, GdkEventButton *event, gpointer data){

	float delta,deltapos;

	if(GDK_BUTTON1_MASK)
	{
		pt_joystick_x=event->x;
		pt_joystick_y=event->y;
		joystick_x=pt_joystick_x/canvas_teleoperator->allocation.width;
		joystick_y=1-(pt_joystick_y/canvas_teleoperator->allocation.height);
		delta = (joystick_x-0.5)*2; /* entre +-1 */
		if (delta<0){
			longitude=-1*delta*(*min_longitude);
		}
		else{
			longitude=delta*(*max_longitude);
		}
		delta = joystick_y; /* entre 0 y 1 */
		delta = (joystick_y-0.5)*2; /* entre +-1 */
		if (delta<0){
			latitude=-1*delta*(*min_latitude);
		}
		else{
			latitude=(float)(delta*(*max_latitude));
		}
		update_positioner=1;
	}
	//expose_event_teleoperator (widget,event,data);
	return TRUE;
}


static gboolean expose_event_body (GtkWidget *widget, GdkEventButton *event, gpointer data){

	GdkGC *gc_body;
	GdkColormap *colormap;
	GdkColor color_white,color_black;

	colormap = gdk_colormap_get_system ();
	gdk_color_black (colormap, & color_black);
	gdk_color_white (colormap, & color_white);
	gc_body = gdk_gc_new (canvas_body->window);
	gdk_gc_set_foreground (gc_body, & color_white);

	gdk_draw_rectangle (canvas_body->window,gc_body,TRUE,0,0,canvas_body->allocation.width,canvas_body->allocation.height);
	gdk_gc_set_foreground (gc_body, & color_black);
	gdk_draw_line(canvas_body->window,gc_body,0,pt_joystick_y_body,canvas_body->allocation.width,pt_joystick_y_body);
	gdk_draw_line(canvas_body->window,gc_body,pt_joystick_x_body,0,pt_joystick_x_body,canvas_body->allocation.height);

	return TRUE;
}

static gboolean button_press_event_body (GtkWidget *widget, GdkEventButton *event, gpointer data){

	float delta,deltapos;

	if(GDK_BUTTON1_MASK)
	{
		pt_joystick_x_body=event->x;
		pt_joystick_y_body=event->y;
		joystick_x_body=pt_joystick_x_body/canvas_body->allocation.width;
		joystick_y_body=1-(pt_joystick_y_body/canvas_body->allocation.height);
		delta = (joystick_x_body-0.5)*2; /* entre +-1 */
		if (delta<0){
			*w=-1*delta*(minw);
		}
		else{
			*w=delta*maxw;
		}
		


		delta = (joystick_y_body); /* entre 0 y 1 */
		*v=delta*(maxv);




		update_positioner_body=1;
	}
	//expose_event_teleoperator (widget,event,data);
	return TRUE;
}







gboolean
on_hscale_w_change_value(GtkCheckMenuItem *menu_item, gpointer user_data){
	*w=gtk_range_get_value(glade_xml_get_widget(xml, "hscale_w"));
	update_positioner_body=2;
	return FALSE;
}

gboolean
on_vscale_v_change_value(GtkCheckMenuItem *menu_item, gpointer user_data){
	*v=gtk_range_get_value(glade_xml_get_widget(xml, "vscale_v"));
	update_positioner_body=2;
	return FALSE;
}



void
on_button_origin_clicked (GtkCheckMenuItem *menu_item, gpointer user_data){
	latitude=0;
	longitude=0;
	update_positioner=2;
}


/**************************************/
/*    END GUI BUTTONS CONFIGURATION    */
/**************************************/

void 
naooperator_show()
{	
	
	static pthread_mutex_t naooperator_gui_mutex;
	pthread_mutex_lock(&naooperator_gui_mutex);
	if (!cargado){
		loadglade ld_fn;
		cargado=1;
		pthread_mutex_unlock(&naooperator_gui_mutex);
		gdk_threads_enter();
		if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
			fprintf (stderr,"NaoOperator: I can't fetch 'load_glade' from 'graphics_gtk'.\n");
			jdeshutdown(1);
		}
		xml = ld_fn ("naooperator.glade");
		if (xml==NULL){
			fprintf(stderr, "NaoOperator: Error loading graphical interface\n");
			jdeshutdown(1);
		}

		/* Head canvas*/
		canvas_teleoperator = glade_xml_get_widget(xml, "canvas_teleoperator");
		gtk_widget_unrealize(canvas_teleoperator);
		gtk_widget_realize(canvas_teleoperator);
		pt_joystick_x=canvas_teleoperator->allocation.width/2;
		pt_joystick_y=canvas_teleoperator->allocation.height/2;
		gtk_widget_set_child_visible (GTK_WIDGET(canvas_teleoperator), TRUE);
		gtk_widget_add_events(canvas_teleoperator,GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_VISIBILITY_NOTIFY_MASK);

		/*body canvas*/
		canvas_body = glade_xml_get_widget(xml, "canvas_body");
		gtk_widget_unrealize(canvas_body);
		gtk_widget_realize(canvas_body);
		pt_joystick_x_body=canvas_body->allocation.width/2;
		pt_joystick_y_body=canvas_body->allocation.height-1;
		gtk_widget_set_child_visible (GTK_WIDGET(canvas_body), TRUE);
		gtk_widget_add_events(canvas_body,GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_VISIBILITY_NOTIFY_MASK);


		win = glade_xml_get_widget(xml, "NaoOperator");
		g_signal_connect(G_OBJECT(win), "delete-event",G_CALLBACK(on_delete_naooperator), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_exit")),"clicked", G_CALLBACK(on_button_exit_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "exit1")),"button_press_event", G_CALLBACK(on_button_exit_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "camera1")),"toggled", G_CALLBACK(on_camera1_toggled), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_up_v")),"clicked", G_CALLBACK(on_button_up_v_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_down_v")),"clicked", G_CALLBACK(on_button_down_v_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_up_w")),"clicked", G_CALLBACK(on_button_up_w_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_down_w")),"clicked", G_CALLBACK(on_button_down_w_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_stop")),"clicked", G_CALLBACK(on_button_stop_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_up_p")),"clicked", G_CALLBACK(on_button_up_p_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_down_p")),"clicked", G_CALLBACK(on_button_down_p_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_up_y")),"clicked", G_CALLBACK(on_button_up_y_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_down_y")),"clicked", G_CALLBACK(on_button_down_y_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_go_body")),"clicked", G_CALLBACK(on_button_go_body_clicked), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_go_head")),"clicked", G_CALLBACK(on_button_go_head_clicked), NULL);
		g_signal_connect(G_OBJECT (canvas_teleoperator), "button_press_event",G_CALLBACK (button_press_event_teleoperator), NULL);
		g_signal_connect(G_OBJECT (canvas_body), "button_press_event",G_CALLBACK (button_press_event_body), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "hscale_w")),"change_value", G_CALLBACK(on_hscale_w_change_value), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "vscale_v")),"change_value", G_CALLBACK(on_vscale_v_change_value), NULL);
		g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "button_origin")),"clicked", G_CALLBACK(on_button_origin_clicked), NULL);


	//	gtk_widget_hide(glade_xml_get_widget(xml, "hbox_image"));
		if (!naobody_runs)
			gtk_widget_hide(glade_xml_get_widget(xml, "hbox_body"));
		if (!ptmotors_run)
			gtk_widget_hide(glade_xml_get_widget(xml, "hbox_motors"));
		if (!ptencoders_run){
			gtk_widget_hide(glade_xml_get_widget(xml, "table_encoders"));
		}



		
		


		char buff[50];
		GtkWidget *entry;
		if (naobody_runs==2){
			gtk_range_set_value(glade_xml_get_widget(xml, "hscale_w"),*w);
			gtk_range_set_value(glade_xml_get_widget(xml, "vscale_v"),*v);
			g_signal_connect(G_OBJECT (canvas_body), "expose_event", G_CALLBACK (expose_event_body), NULL);
			entry= glade_xml_get_widget(xml, "entry_v");
			sprintf(buff,"%f",*v);
			gtk_entry_set_text(entry,buff);
			entry= glade_xml_get_widget(xml, "entry_w");
			sprintf(buff,"%f",*w);
			gtk_entry_set_text(entry,buff);
		}
		if (ptmotors_run==2){
			gtk_range_set_value(glade_xml_get_widget(xml, "hscale_longitude_speed"),*longitude_speed);
			gtk_range_set_value(glade_xml_get_widget(xml, "hscale_latitude_speed"),*latitude_speed);
			g_signal_connect(G_OBJECT (canvas_teleoperator), "expose_event", G_CALLBACK (expose_event_teleoperator), NULL);
			entry= glade_xml_get_widget(xml, "entry_p");
			sprintf(buff,"%f",*mylatitude);
			gtk_entry_set_text(entry,buff);
			entry= glade_xml_get_widget(xml, "entry_y");
			sprintf(buff,"%f",*mylongitude);
			gtk_entry_set_text(entry,buff);
		}


		gdk_threads_leave();
	}
	else{
		pthread_mutex_unlock(&naooperator_gui_mutex);
		gdk_threads_enter();
		gtk_widget_show(win);
		gtk_widget_queue_draw(GTK_WIDGET(win));
		gdk_threads_leave();
	}
	
	myregister_displaycallback(naooperator_guidisplay);
	all[naooperator_id].guistate=pending_on;
}
void naooperator_hide()
{
	mydelete_displaycallback(naooperator_guidisplay);
	/* Here we will put the things that we want the schema do when hide the GUI*/
	if (win!=NULL){
		gdk_threads_enter();	
		gtk_widget_hide(win);
		gdk_threads_leave();
	}
   	all[naooperator_id].guistate=pending_off;
}




