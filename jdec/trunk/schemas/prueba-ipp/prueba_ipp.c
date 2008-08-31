/*
*  Copyright (C) 2006 José María Cañas Plaza 
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
*  Authors : Gonzalo Lahera Reviriego <glahera@gmail.com>
*/

#include "jde.h"
#include "prueba_ipp.h"
#include "graphics_gtk.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <ipp.h>
//#include <math.h>
//#include <colorspaces.h>

int prueba_ipp_id=0;
int prueba_ipp_brothers[MAX_SCHEMAS];
arbitration prueba_ipp_callforarbitration;

#define MAX_COLOR 4
#define SIFNTSC_COLUMNS 320
#define SIFNTSC_ROWS 240
#define SIFNTSC 76800
#define NUM_FEAT_MAX 300
#define square(a)  ((a)*(a))
static const double pi = 3.14159265358979323846;
static const double pi2 = 6.283185308;
static const int colorMax = 220;
static const int colorMin = 220;

typedef struct{
	IppiPoint_32f punto;
	Ipp32f valor;
}t_pInteres;
float rate=2.0f, alpha=0.375f;
int ksize=5, wsize=9;

/*Imported variables*/
int* width[MAX_COLOR];
int* height[MAX_COLOR];
char** mycolor[MAX_COLOR];
resumeFn myresume[MAX_COLOR];
suspendFn mysuspend[MAX_COLOR];
char *mensajes[MAX_COLOR]={"Mostrando colorA","Mostrando colorB","Mostrando colorC","Mostrando colorD"};

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
char *image;
char image2AuxAnterior[SIFNTSC];
char image2Aux[SIFNTSC];

Ipp8u *buffer;
int image_selected=-1;
int tipo_selected=0;
int image_anterior=-5;
pthread_mutex_t main_mutex;
int contexto_pila;
int red_bajo = 0;
int green_bajo = 0;
int blue_bajo = 0;
int red_alto = 255;
int green_alto = 255;
int blue_alto = 255;
static int inicializado = 0;
Ipp8u mask[SIFNTSC_COLUMNS*SIFNTSC_COLUMNS*3];
Ipp8u masked_old[SIFNTSC*3];

//Variable globales para el calculo de flujo optico
enum tformatos {ORIGEN_DESTINO, DESTINO_ORIGEN};
int nfeat=0;
IppiPoint_32f *prev=0; /* coordinates on previous frame*/
IppiPoint_32f *next=0; /* hint to coordinates on next frame*/
Ipp8s *opflow_status=0; /*indicates if opflow have been found*/
Ipp32f *opflow_error=0; /*indicates opflow calc error*/
int opflow_id=0;
Ipp8u export_old1[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
Ipp8u export_old2[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
Ipp8u *export_old=NULL;
Ipp8u *export_old_work=NULL;
//Export variables
t_opflow *opflow_img=NULL;
/*Variables del gui*/
double threshold=0.3;
int layers=3;
int num_iter=12;
int show_arrows=1;
int show_interesting_p=1;
float min_eig=1.0;
float max_eig=2.0;
float min_mov=1.5;
float max_err=15;
int formato=ORIGEN_DESTINO;
int show_mask=0;
Ipp8u show_old[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];//Para mostrar en el display


/* exported variables */
int prueba_ipp_cycle=40; /* ms */

/*GUI Variables*/
pthread_t hilo_gtk;
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/

void cambiar_imagen(int i){
//	int stepBytes;
	int tamImagen;
	IppiSize roi;
	int size;
	if ((width[i]!=NULL) && (height[i]!=NULL) && (mycolor[i]!=NULL) &&
	(myresume[i]!=NULL) && (mysuspend[i]!=NULL))
	{
		pthread_mutex_lock(&main_mutex);
		if (image_selected!=-1){
			mysuspend[image_selected]();
			free(image);
			ippiFree(buffer);
		}
		image_anterior=image_selected;
		image_selected=i;
		myresume[image_selected](prueba_ipp_id,NULL,NULL);
		tamImagen = (width[image_selected][0]*height[image_selected][0]*3);
		image=(char *)malloc(tamImagen);
		roi.width = 320;
		roi.height = 240;
		ippiCannyGetSize(roi, &size);
		buffer = ippsMalloc_8u(size);

		{
		GdkPixbuf *imgBuff;

		GtkImage *img=(GtkImage *)glade_xml_get_widget(xml, "image");

		imgBuff = gdk_pixbuf_new_from_data((unsigned char *)image,
											GDK_COLORSPACE_RGB,0,8,
											width[image_selected][0],height[image_selected][0],
											width[image_selected][0]*3,NULL,NULL);
		gtk_image_clear(img);
		gtk_image_set_from_pixbuf(img, imgBuff);
		gtk_widget_queue_draw(GTK_WIDGET(img));
		gtk_window_resize (GTK_WINDOW(win),1,1);
		gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"), contexto_pila, mensajes[i]);
		}
		pthread_mutex_unlock(&main_mutex);
	}
}

/************************************************************/
/*Callbacks*/
void cambiar_imagen_filtror_bajo(GtkWidget *widget, gpointer user_data){
	red_bajo = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void cambiar_imagen_filtrog_bajo(GtkWidget *widget, gpointer user_data){
	green_bajo = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void cambiar_imagen_filtrob_bajo(GtkWidget *widget, gpointer user_data){
	blue_bajo = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void cambiar_imagen_filtror_alto(GtkWidget *widget, gpointer user_data){
	red_alto = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void cambiar_imagen_filtrog_alto(GtkWidget *widget, gpointer user_data){
	green_alto = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void cambiar_imagen_filtrob_alto(GtkWidget *widget, gpointer user_data){
	blue_alto = (guint)gtk_range_get_value (GTK_RANGE(widget));
}
void on_img_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
	/*Hay que comprobar el valor que tiene*/
	char *valor;
	valor=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);
	/*Importar los valores oportunos y modificar la selección*/
	printf (valor);
	printf("\n");
	if (strcmp(valor,"colorA")==0){
		if (image_selected!=0){
			inicializado = 0;
			width[0]=myimport("colorA","width");
			height[0]=myimport("colorA","height");
			mycolor[0]=myimport("colorA","colorA");
			myresume[0]=(resumeFn)myimport("colorA","resume");
			mysuspend[0]=(suspendFn)myimport("colorA","suspend");
			cambiar_imagen(0);
		}
	}
	else if (strcmp(valor,"colorB")==0){
		if (image_selected!=1){
			inicializado = 0;
			width[1]=myimport("colorB","width");
			height[1]=myimport("colorB","height");
			mycolor[1]=myimport("colorB","colorB");
			myresume[1]=(resumeFn)myimport("colorB","resume");
			mysuspend[1]=(suspendFn)myimport("colorB","suspend");
			cambiar_imagen(1);
		}
	}
	else if (strcmp(valor,"colorC")==0){
		if (image_selected!=2){
			inicializado = 0;
			width[2]=myimport("colorC","width");
			height[2]=myimport("colorC","height");
			mycolor[2]=myimport("colorC","colorC");
			myresume[2]=(resumeFn)myimport("colorC","resume");
			mysuspend[2]=(suspendFn)myimport("colorC","suspend");
			cambiar_imagen(2);
		}
	}
	else if (strcmp(valor,"colorD")==0){
		if (image_selected!=3){
			inicializado = 0;
			width[3]=myimport("colorD","width");
			height[3]=myimport("colorD","height");
			mycolor[3]=myimport("colorD","colorD");
			myresume[3]=(resumeFn)myimport("colorD","resume");
			mysuspend[3]=(suspendFn)myimport("colorD","suspend");
			cambiar_imagen(3);
		}
	}
}
void on_img_tipo_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
	/*Hay que comprobar el valor que tiene*/
	char *valor;
	valor=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);
	/*Importar los valores oportunos y modificar la selección*/
	printf (valor);
	printf("\n");
	if (strcmp(valor,"Original")==0){
		tipo_selected = 0;
	}
	else if (strcmp(valor,"Filtro De Color")==0){
		tipo_selected = 1;
	}
	else if (strcmp(valor,"Blanco Y negro")==0){
		tipo_selected = 2;
	}
	else if (strcmp(valor,"Filtro De Bordes")==0){
		tipo_selected = 3;
	}
	else if (strcmp(valor,"Flujo Optico")==0){
		inicializado = 0;
		tipo_selected = 4;
	}
	else if (strcmp(valor,"Puntos Relevantes")==0){
		inicializado = 0;
		tipo_selected = 5;
	}
}

void pyroptflow(const Ipp8u *prevFrame, // previous frame
				int prevStep, // its row step
				const Ipp8u *nextFrame, // next frame
				int nextStep, // its row step
				IppiSize roiSize, // frame size
				int numLevel, // pyramid level number (5)
				float rate, // pyramid rate (2.0f)
				Ipp16s *pKernel, // pyramid kernel
				int kerSize, // pyramid kernel size (5)
				IppiPoint_32f *prevPt, // coordinates on previous frame
				IppiPoint_32f *nextPt, // hint to coordinates on next frame
				Ipp8s *pStatus, // result indicators
				Ipp32f *pError, // differences
				int numFeat, // point number
				int winSize, // search window size (41)
				int numIter, // iteration number (5)
				float threshold) // threshold (0.0001f)
{
	IppiPyramid *pPyr1, *pPyr2;
	IppiOptFlowPyrLK *pOF;
	ippiPyramidInitAlloc (&pPyr1, numLevel, roiSize, rate);
	ippiPyramidInitAlloc (&pPyr2, numLevel, roiSize, rate);
	{
		IppiPyramidDownState_8u_C1R **pState1 = (IppiPyramidDownState_8u_C1R**)&(pPyr1->pState);
		IppiPyramidDownState_8u_C1R **pState2 = (IppiPyramidDownState_8u_C1R**)&(pPyr2->pState);
		Ipp8u **pImg1 = pPyr1->pImage;
		Ipp8u **pImg2 = pPyr2->pImage;
		int *pStep1 = pPyr1->pStep;
		int *pStep2 = pPyr2->pStep;
		IppiSize *pRoi1 = pPyr1->pRoi;
		IppiSize *pRoi2 = pPyr2->pRoi;
		IppHintAlgorithm hint=ippAlgHintFast;
		int i,level = pPyr1->level;
		ippiPyramidLayerDownInitAlloc_8u_C1R(pState1, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR);
		ippiPyramidLayerDownInitAlloc_8u_C1R(pState2, roiSize, rate, pKernel, kerSize, IPPI_INTER_LINEAR);
		pImg1[0] = (Ipp8u*)prevFrame;
		pImg2[0] = (Ipp8u*)nextFrame;
		pStep1[0] = prevStep;
		pStep2[0] = nextStep;
		pRoi1[0] = pRoi2[0] = roiSize;
		for (i=1; i<=level; i++) {
			pPyr1->pImage[i] = ippiMalloc_8u_C1(pRoi1[i].width,pRoi1[i].height, pStep1+i);
			pPyr2->pImage[i] = ippiMalloc_8u_C1(pRoi2[i].width,pRoi2[i].height, pStep2+i);
			ippiPyramidLayerDown_8u_C1R(pImg1[i-1], pStep1[i-1], pRoi1[i-1], pImg1[i], pStep1[i], pRoi1[i], *pState1);
			ippiPyramidLayerDown_8u_C1R(pImg2[i-1], pStep2[i-1], pRoi2[i-1], pImg2[i], pStep2[i], pRoi2[i], *pState2);
		}
		ippiOpticalFlowPyrLKInitAlloc_8u_C1R (&pOF,roiSize,winSize,hint);
		ippiOpticalFlowPyrLK_8u_C1R (pPyr1, pPyr2, prevPt, nextPt, pStatus, pError,  numFeat, winSize, numLevel, numIter, threshold, pOF);
		ippiOpticalFlowPyrLKFree_8u_C1R(pOF);
		for (i=level; i>0; i--) {
			if (pImg2[i]) ippiFree(pImg2[i]);
			if (pImg1[i]) ippiFree(pImg1[i]);
		}
		ippiPyramidLayerDownFree_8u_C1R(*pState1);
		ippiPyramidLayerDownFree_8u_C1R(*pState2);
	}
	ippiPyramidFree (pPyr2);
	ippiPyramidFree (pPyr1);
}
int lineinimage(Ipp8u *img_salida, int xa, int ya, int xb, int yb, int thiscolor){
	float L;
	int i,imax,r,g,b;
	int lastx,lasty,thisx,thisy,lastcount;
	int threshold=1;
	int Xmax,Xmin,Ymax,Ymin;
	int punto;
	//printf("Haciendo linea\n");
	Xmin=0; Xmax=SIFNTSC_COLUMNS-1; Ymin=0; Ymax=SIFNTSC_ROWS-1;
	/* In this image always graf coordinates x=horizontal, y=vertical, starting
	at the top left corner of the image. They can't reach 240 or 320, their are
	not valid values for the pixels.  */

	if (thiscolor==1) {r=255;g=0;b=0;}
	else if (thiscolor==2) {r=0;g=255;b=0;}
/*	else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
	else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
	else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
	else if (thiscolor==FL_GREEN) {r=0;g=255;b=0;}
	else {r=0;g=0;b=0;}*/
	//r=255;g=0;b=0;

	/* first, check both points are inside the limits and draw them */
	/* draw both points */
	if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1)){
		img_salida[(SIFNTSC_COLUMNS*ya+xa)*3]=r;
		img_salida[(SIFNTSC_COLUMNS*ya+xa)*3+1]=g;
		img_salida[(SIFNTSC_COLUMNS*ya+xa)*3+2]=b;
	}
	if ((xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)){
		img_salida[(SIFNTSC_COLUMNS*yb+xb)*3]=r;
		img_salida[(SIFNTSC_COLUMNS*yb+xb)*3+1]=g;
		img_salida[(SIFNTSC_COLUMNS*yb+xb)*3+2]=b;
	}

	L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
	imax=3*(int)L+1;
	lastx=xa; lasty=ya; lastcount=0;
	for(i=0;i<=imax;i++)
	{
		thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
		thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));

		if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
		else
		{
		if (lastcount>=threshold)
		{ /* draw that point in the image */
			if ((lastx>=Xmin)&&(lastx<=Xmax)&&(lasty>=Ymin)&&(lasty<=Ymax)){
			punto=(SIFNTSC_COLUMNS*lasty+lastx)*3;
			img_salida[punto]=r;
			img_salida[punto+1]=g;
			img_salida[punto+2]=b;
			}
		}
		lasty=thisy;
		lastx=thisx;
		lastcount=0;
		}
	}
	return 0; 
}

void prueba_ipp_iteration(){
	int i, j;
	int ancho, alto;
	IppiSize imgtam;
	IppiPoint punto;
	IppiSize roi;
	IppStatus sts;
	Ipp32f low=1.0f;
	Ipp32f high=200.0f;
	static Ipp32f *kernel=NULL, *kernel_16s=NULL;
	Ipp16u image3X[SIFNTSC];
	Ipp16u image3Y[SIFNTSC];
	Ipp8u image3Aux[SIFNTSC];	char original[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
	Ipp8u *arr[3];
	speedcounter(prueba_ipp_id);

	pthread_mutex_lock(&main_mutex);
	ancho = SIFNTSC_COLUMNS;//width[image_selected][0];
	alto = SIFNTSC_ROWS;//height[image_selected][0];
	imgtam.width = ancho;
	imgtam.height = alto;

/*	//		sts = ippiFilterLaplace_8u_C1R(image2Aux, ancho, image4Aux, ancho, roi, ippMskSize3x3);
	//		sts = ippiFilterGauss_8u_C1R(image2Aux, ancho, image4Aux, ancho, roi, ippMskSize3x3);
	//		sts = ippiFilterRobertsUp_8u_C1R(image2Aux, ancho, image4Aux, ancho, roi);
	//		sts = ippiFilterHipass_8u_C1R(image2Aux, ancho, image4Aux, ancho, roi, ippMskSize3x3);
	//		sts = ippiFilterSharpen_8u_C1R(image2Aux, ancho, image4Aux, ancho, roi);
	//		sts = ippiFilterSobelCross_8u16s_C1R(image2Aux, ancho, image4Aux, ancho, roi, ippMskSize3x3);
	//		sts = ippiFilterLaplacianBorder_8u8s_C1R(image2Aux, ancho, image4Aux, ancho, roi, ippMskSize3x3, ippBorderConst, 0, 75, buffer);
*/

	if (image_selected!=-1){
		for (i=0;i<ancho*alto; i++){
			original[i*3]	= (*mycolor[image_selected])[i*3+2];
			original[i*3+1] = (*mycolor[image_selected])[i*3+1];
			original[i*3+2] = (*mycolor[image_selected])[i*3];
		}
		switch (tipo_selected){
		case 0:{//Imagen original
			ippiCopy_8u_C3R((Ipp8u*)original, SIFNTSC_COLUMNS*3, (Ipp8u*)image, SIFNTSC_COLUMNS*3, imgtam);
			}
			break;
		case 1:{//Filtro de color
			unsigned char b1;
			unsigned char b2;
			unsigned char b3;
			int f1;
			int f2;
			int f3;
			for (i=0;i<ancho*alto; i++){
				b1 = original[i*3];
				b2 = original[i*3+1];
				b3 = original[i*3+2];
				f1 = (int)b1;
				f2 = (int)b2;
				f3 = (int)b3;
				if (f1>red_alto || f1<red_bajo){
					image[i*3] = 0x00;
					image[i*3+1] = 0x00;
					image[i*3+2] = 0x00;
				}else if (f2>green_alto || f2<green_bajo){
					image[i*3] = 0x00;
					image[i*3+1] = 0x00;
					image[i*3+2] = 0x00;
				}else if (f3>blue_alto || f3<blue_bajo){
					image[i*3] = 0x00;
					image[i*3+1] = 0x00;
					image[i*3+2] = 0x00;
				}else{
					image[i*3] = b1;
					image[i*3+1] = b2;
					image[i*3+2] = b3;
				}
			}
			}
			break;
		case 2:{//Escala de grises
			ippiRGBToGray_8u_C3C1R ((Ipp8u*)original, ancho*3,
											(Ipp8u*)image2Aux, ancho, imgtam);
			arr[0] = (Ipp8u*)image2Aux;
			arr[1] = (Ipp8u*)image2Aux;
			arr[2] = (Ipp8u*)image2Aux;
			ippiCopy_8u_P3C3R(arr, ancho, (Ipp8u*)image, ancho*3, imgtam);
			}
			break;
		case 3:{//Bordes
			IppiSize kernelSize;
			IppiPoint anchor;
			ippiRGBToGray_8u_C3C1R ((Ipp8u*)original, ancho*3,
											(Ipp8u*)image2Aux, ancho, imgtam);
			roi.width = 320;
			roi.height = 240;
			kernelSize.width = 3;
			kernelSize.height = 3;
			anchor.x = 0;
			anchor.y = 0;

			low = 1.0f;
			high = 200.0f;
			sts = ippiFilterSobelNegVertBorder_8u16s_C1R ((Ipp8u*)image2Aux, ancho, image3X, ancho*2, roi,
																			ippMskSize3x3, ippBorderConst, 0, buffer);//631
			sts = ippiFilterSobelHorizBorder_8u16s_C1R ((Ipp8u*)image2Aux, ancho, image3Y, ancho*2, roi,
																		ippMskSize3x3, ippBorderConst, 0, buffer);
			sts = ippiCanny_16s8u_C1R(image3X, ancho*2, image3Y, ancho*2, (Ipp8u*)image3Aux, ancho, roi, low, high, buffer);

			//sts = ippiFilter_8u_C1R(image2Aux, ancho, image3Aux, ancho, roi, pKernel, kernelSize, anchor, 1);
			//pgs 936 del manual 2: "ippiman.pdf"
			if (sts == ippStsNullPtrErr){
				printf("Error : ippStsNullPtrErr\n");
			}
			if (sts == ippStsSizeErr){
				printf("Error : ippStsSizeErr\n");
			}
			if (sts == ippStsStepErr){
				printf("Error : ippStsStepErr %i\n", kernelSize);
			}
			if (sts == ippStsBadArgErr){
				printf("Error : ippStsBadArgErr\n");
			}
			if (sts == ippStsNotEvenStepErr){
				printf("Error : ippStsNotEvenStepErr\n");
			}
			arr[0] = (Ipp8u*)image3Aux;
			arr[1] = (Ipp8u*)image3Aux;
			arr[2] = (Ipp8u*)image3Aux;
			ippiCopy_8u_P3C3R(arr, ancho, (Ipp8u*)image, ancho*3, imgtam);
			}
			break;
		case 4:{//Flujo optico
				static Ipp8u img_bw1[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static Ipp8u img_bw2[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static Ipp8u *old_bw=NULL;
				static Ipp8u *new_bw=NULL;
//				int max=SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;

				static IppiPoint_32f prev1[NUM_FEAT_MAX];
				static IppiPoint_32f prev2[NUM_FEAT_MAX];
				static IppiPoint_32f next1[NUM_FEAT_MAX];
				static IppiPoint_32f next2[NUM_FEAT_MAX];
				static IppiPoint_32f *next_work=NULL;
				static IppiPoint_32f *prev_work=NULL;

				static Ipp8s status1[NUM_FEAT_MAX];
				static Ipp8s status2[NUM_FEAT_MAX];
				static Ipp32f error1[NUM_FEAT_MAX];
				static Ipp32f error2[NUM_FEAT_MAX];
				static Ipp8s *status_work=NULL;
				static Ipp32f *error_work=NULL;

				static t_opflow opflow_img_1[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static t_opflow opflow_img_2[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static t_opflow *opflow_img_work=NULL;

				static IppiSize imgtam;

				/*tamaño del buffer para el cálculo de los puntos de interés*/
				static int eigen_buff_tam;

				static int nfeat_temp; /*Número de puntos de interés provisionales*/
				punto.x = 0;
				punto.y = 0;
				ippiRGBToGray_8u_C3C1R ((Ipp8u*)original, ancho*3,
											(Ipp8u*)image2Aux, ancho, imgtam);
				/*Comienzo de la iteración*/
				speedcounter(opflow_id);
				/*Inicialización de los punteros de imagenes*/
				if (inicializado==0){
					old_bw=img_bw1;
					new_bw=img_bw2;
					prev=prev1;
					next=next1;
					prev_work=prev2;
					next_work=next2;
					opflow_error=error1;
					error_work=error2;
					opflow_status=status1;
					status_work=status2;
					imgtam.width= SIFNTSC_COLUMNS;
					imgtam.height= SIFNTSC_ROWS;
					if (ippiMinEigenValGetBufferSize_8u32f_C1R(imgtam, 3, 3, &eigen_buff_tam)!=ippStsNoErr){
						fprintf (stderr, "Error calculating size of temp space for calculate eigen values\n");
						exit(-1);
					}
					opflow_img_work=opflow_img_1;
					opflow_img=NULL;
				}

				/*Obtain image*/
				ippiCopy_8u_C3R((Ipp8u*)image, SIFNTSC_COLUMNS*3, (Ipp8u*)export_old_work, SIFNTSC_COLUMNS*3, imgtam);
	//			ippiMulScale_8u_C3R((Ipp8u*)image, SIFNTSC_COLUMNS*3, (Ipp8u *)mask, SIFNTSC_COLUMNS*3,
	//										(Ipp8u*)masked_old, SIFNTSC_COLUMNS*3, imgtam);
				ippiRGBToGray_8u_C3C1R ((Ipp8u*)export_old_work, SIFNTSC_COLUMNS*3, (Ipp8u*)new_bw, SIFNTSC_COLUMNS, imgtam);
				/*Ahora se tiene capturada una imagen, habrá que tener dos para ejecutar
					el algoritmo, se espera a la siguiente iteración si es necesario*/
				if (inicializado){
					int i,j;
					static Ipp8u* eig_buffer=NULL;
					static Ipp32f *eig_val=NULL;
					static int eig_val_s;
	
					int ksize=5, wsize=9;
					float rate=2.0f, alpha=0.375f;
	
					static t_pInteres interes[NUM_FEAT_MAX];
					/*Variable temporal para obtener los puntos de interés*/
	
					if (eig_buffer==NULL){
					//printf ("Reserva memoria para eig_buffer step=%d\n",eig_buff_step);
						eig_buffer=(Ipp8u*)malloc(sizeof (Ipp8u)*eigen_buff_tam);
						if (eig_buffer==NULL)
							printf ("Falla la llamada a malloc %d\n",eigen_buff_tam);
					}
					if (eig_val==NULL){
						eig_val=ippiMalloc_32f_C1 (SIFNTSC_COLUMNS, SIFNTSC_ROWS, &eig_val_s);
						if (eig_val==NULL){
							printf ("Falla la llamada a malloc2\n");
						}
					}
					nfeat_temp=0;
					switch (ippiMinEigenVal_8u32f_C1R((Ipp8u*)old_bw, SIFNTSC_COLUMNS,
																	eig_val, SIFNTSC_COLUMNS*sizeof(Ipp32f),
																	imgtam, ippKernelScharr, 3, 3, eig_buffer))
					{
						case ippStsNoErr:
							for (j=0;j<SIFNTSC_ROWS;j++){
							for (i=0; i<SIFNTSC_COLUMNS; i++){
								int val=i+j*SIFNTSC_COLUMNS;
								Ipp32f valor= eig_val[val];
						//Se utiliza también para reiniciar la matriz resultado
								opflow_img_work[val].calc=0;
						//En este punto hay que umbralizar y ordenar
								if (valor>min_eig && valor<max_eig){
									if (nfeat_temp<NUM_FEAT_MAX){
										int index1;
										int index2;
										if (nfeat_temp==0){
											interes[0].punto.x=i;
											interes[0].punto.y=j;
											interes[0].valor=valor;
										}
										else{
											for (index1=0; index1<nfeat_temp; index1++){
												if (interes[index1].valor<valor){
													/*Desplazar hacia la derecha*/
													for (index2=nfeat_temp; index2>index1; index2--){
														interes[index2]=interes[index2-1];
													}
													interes[index1].punto.x=i;
													interes[index1].punto.y=j;
													interes[index1].valor=valor;
													break;
												}
											}
										}
										nfeat_temp++;
									}
									else{
										if (interes[NUM_FEAT_MAX-1].valor<valor){
										/*Insertar en el lugar adecuado*/
										int index1;
										int index2;
										for (index1=0; index1<nfeat_temp; index1++){
											if (interes[index1].valor<valor){
												/*Desplazar hacia la derecha*/
												for (index2=NUM_FEAT_MAX-1; index2>index1; index2--){
													interes[index2]=interes[index2-1];
												}
												interes[index1].punto.x=i;
												interes[index1].punto.y=j;
												interes[index1].valor=valor;
												break;
											}
										}
										}
									}
								}
							}
							}
							for (i=0;i<nfeat_temp; i++){
								prev_work[i]=interes[i].punto;
							}
	
							break;
						case ippStsNullPtrErr:
							printf ("Puntero nulo.\n");
							if (old_bw==NULL){
							printf ("old_bw nulo\n");
							}
							if (eig_val==NULL){
							printf ("eig_val nulo\n");
							}
							if (eig_buffer==NULL){
							printf ("eig_buffer nulo\n");
							}
							break;
						case ippStsSizeErr:
							printf ("Parámetros erróneos.\n");
							break;
						case ippStsStepErr:
							printf ("Step inválido.\n");
							printf ("width=%d, COLUMS=%d\n", imgtam.width, SIFNTSC_COLUMNS);
							break;
						case ippStsNotEvenStepErr:
							printf ("Step no divisible /4.\n");
							break;
						default:
							printf ("Error desconocido.\n");
							break;
					}

					/*Crea un núcleo para la creación de las pirámides*/
					if (kernel==NULL){
						Ipp32f sum;
						int i;
						kernel=(Ipp32f*)malloc(sizeof(Ipp32f)*ksize);
						kernel_16s=(Ipp16s*)malloc(sizeof(Ipp16s)*ksize);
						switch (ksize) {
							case 3:
							kernel[1] = alpha;
							kernel[0] = kernel[2] = 0.5f*(1.0f - alpha);
							break;
							case 5:
							kernel[2] = alpha;
							kernel[1] = kernel[3] = 0.25f;
							kernel[0] = kernel[4] = 0.5f*(0.5f - alpha);
							break;
							default:
							sum = 0;
							for (i=0; i<ksize; i++) {
								kernel[i] = (Ipp32f)exp(alpha*(ksize/2-i)*(ksize/2-i)*0.5f);
								sum += kernel[i];
							}
							for (i=0; i<ksize; i++) {
								kernel[i] /= sum;
							}
						}
						ippsConvert_32f16s_Sfs(kernel, kernel_16s, ksize, ippRndNear, -15);
					}
					/*Copia de los puntos de interés a next_work*/
					memcpy(next_work, prev_work, nfeat_temp*sizeof(IppiPoint_32f));
					/*Se llama a la función que calcula el flujo*/
					pyroptflow(old_bw, SIFNTSC_COLUMNS, new_bw, SIFNTSC_COLUMNS, imgtam,
						layers, rate, kernel_16s, ksize, prev_work, next_work,
						status_work, error_work, nfeat_temp, wsize, num_iter,
						threshold);
					ippiCopy_8u_C3R((Ipp8u*)original, SIFNTSC_COLUMNS*3, (Ipp8u*)image, SIFNTSC_COLUMNS*3, imgtam);
	
					if (formato==ORIGEN_DESTINO){
						for (i=0;i<nfeat_temp; i++){
							int x=(int)prev_work[i].x;
							int y=(int)prev_work[i].y;
							int valor;
							if (status_work[i]==0 && error_work[i]< max_err && x<SIFNTSC_COLUMNS && y<SIFNTSC_ROWS && y>=0 && x>=0){
								int x1 = (int) prev_work[i].x;
								int y1 = (int) prev_work[i].y;
								int x2 = (int) next_work[i].x;
								int y2 = (int) next_work[i].y;
								valor = x+y*SIFNTSC_COLUMNS;
	
								opflow_img_work[valor].hyp = /*sqrt*/( square(y1 - y2) + square(x1 - x2) );
								if (opflow_img_work[valor].hyp > min_mov){
									opflow_img_work[valor].calc=1;
									opflow_img_work[valor].status=status_work[i];
									opflow_img_work[valor].error=error_work[i];
									opflow_img_work[valor].dest.x=next_work[i].x;
									opflow_img_work[valor].dest.y=next_work[i].y;
									opflow_img_work[valor].angle = atan2( (double) (y1 - y2), (double) (x1 - x2) );
								}
								else{
									opflow_img_work[valor].calc=0;
								}
							}
						}
					}
					else{ /*formato == DESTINO_ORIGEN */
					for (i=0;i<nfeat_temp; i++){
							int x=(int)next_work[i].x;
							int y=(int)next_work[i].y;
							int valor;
							if (status_work[i]==0 && error_work[i]< max_err && x<SIFNTSC_COLUMNS && y<SIFNTSC_ROWS && y>=0 && x>=0){
								int x1 = (int) next_work[i].x;
								int y1 = (int) next_work[i].y;
								int x2 = (int) prev_work[i].x;
								int y2 = (int) prev_work[i].y;
	
								valor= x+y*SIFNTSC_COLUMNS;
	
								opflow_img_work[valor].hyp = /*sqrt*/( square(y1 - y2) + square(x1 - x2) );
								if (opflow_img_work[valor].hyp > min_mov){
									opflow_img_work[valor].calc=1;
									opflow_img_work[valor].status=status_work[i];
									opflow_img_work[valor].error=error_work[i];
									opflow_img_work[valor].dest.x=prev_work[i].x;
									opflow_img_work[valor].dest.y=prev_work[i].y;
									opflow_img_work[valor].angle = atan2( (double) (y1 - y2), (double) (x1 - x2) );
								}
								else{
									opflow_img_work[valor].calc=0;
								}
							}
						}
					}
				}

				inicializado=1;
				/*Cambiar el buffer de las imágenes en 1 canal*/
				if (old_bw==img_bw1){
					old_bw=img_bw2;
					new_bw=img_bw1;
				}
				else{
					old_bw=img_bw1;
					new_bw=img_bw2;
				}
				/*Cambiar el buffer de trabajo de prev y next*/
				if (prev==prev1){
					prev_work=prev1;
					next_work=next1;
					status_work=status1;
					error_work=error1;
					prev=prev2;
					next=next2;
					opflow_status=status2;
					opflow_error=error2;
				} else{
					prev_work=prev2;
					next_work=next2;
					status_work=status2;
					error_work=error2;
					prev=prev1;
					next=next1;
					opflow_status=status1;
					opflow_error=error1;
				}
				nfeat=nfeat_temp;
				if (export_old==export_old1){
					export_old=export_old2;
					export_old_work=export_old1;
				} else{
					export_old=export_old1;
					export_old_work=export_old2;
				}
	
				if (opflow_img_work==opflow_img_1){
					opflow_img=opflow_img_1;
					opflow_img_work=opflow_img_2;
				} else{
					opflow_img=opflow_img_2;
					opflow_img_work=opflow_img_1;
				}
	
	
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
				for (i=0;i<SIFNTSC_COLUMNS;i++){
					for (j=0; j<SIFNTSC_ROWS; j++){
						if (show_arrows){
							int val=i+j*SIFNTSC_COLUMNS;
							int color;
							int x1, y1, x2, y2;
							double angle, hypotenuse;
	
							if (opflow_img[val].status!=0 || opflow_img[val].error > max_err || opflow_img[val].calc==0)
								continue;
	
							if (opflow_img[val].error >(max_err/2) ){
								color= 1;//Rojo
							}else{
								color = 2;//Verde
							}
	
							x1 = (int) i;
							y1 = (int) j;
							x2 = (int) opflow_img[val].dest.x;
							y2 = (int) opflow_img[val].dest.y;
	
							hypotenuse = opflow_img[val].hyp;
	
							if (hypotenuse<square(min_mov))
								continue;
	
							hypotenuse=sqrt(hypotenuse);
							angle = opflow_img[val].angle;
							angle = opflow_img[val].angle;
							if (formato == ORIGEN_DESTINO){
								//Darle longitud al segmento que se pinta
								x2 = (int) (x1 - 2 * hypotenuse * cos(angle));
								y2 = (int) (y1 - 2 * hypotenuse * sin(angle));
								//Dibujar la línea principal de la flecha
								lineinimage(image, x1, y1, x2, y2,color);
								//Ahora las puntas de las flechas
								x1 = (int) (x2 + (hypotenuse/4) * cos(angle + pi / 4));
								y1 = (int) (y2 + (hypotenuse/4) * sin(angle + pi / 4));
								lineinimage(image, x1, y1, x2, y2, color);
								x1 = (int) (x2 + (hypotenuse/4) * cos(angle - pi / 4));
								y1 = (int) (y2 + (hypotenuse/4) * sin(angle - pi / 4));
								lineinimage(image, x1, y1, x2, y2, color);
							}else{ //formato == DESTINO_ORIGEN
								x2 = (int) (x1 - (hypotenuse/4) * cos(angle + pi / 4));
								y2 = (int) (y1 - (hypotenuse/4) * sin(angle + pi / 4));
								lineinimage(image, x1, y1, x2, y2, color);
								x2 = (int) (x1 - (hypotenuse/4) * cos(angle - pi / 4));
								y2 = (int) (y1 - (hypotenuse/4) * sin(angle - pi / 4));
								lineinimage(image, x1, y1, x2, y2, color);
								x2 = (int) (x1 - 2 * hypotenuse * cos(angle));
								y2 = (int) (y1 - 2 * hypotenuse * sin(angle));
								//Dibujar la línea principal de la flecha
								lineinimage(image, x1, y1, x2, y2,color);
							}
						}
					}
				}
			}
			break;
		case 5:{//Puntos relevantes
				static Ipp8u img_bw1[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static Ipp8u img_bw2[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static Ipp8u *old_bw=NULL;
				static Ipp8u *new_bw=NULL;
				int max=SIFNTSC_COLUMNS*SIFNTSC_ROWS*3;

				static IppiPoint_32f prev1[NUM_FEAT_MAX];
				static IppiPoint_32f prev2[NUM_FEAT_MAX];
				static IppiPoint_32f next1[NUM_FEAT_MAX];
				static IppiPoint_32f next2[NUM_FEAT_MAX];
				static IppiPoint_32f *next_work=NULL;
				static IppiPoint_32f *prev_work=NULL;

				static Ipp8s status1[NUM_FEAT_MAX];
				static Ipp8s status2[NUM_FEAT_MAX];
				static Ipp32f error1[NUM_FEAT_MAX];
				static Ipp32f error2[NUM_FEAT_MAX];
				static Ipp8s *status_work=NULL;
				static Ipp32f *error_work=NULL;

				static t_opflow opflow_img_1[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static t_opflow opflow_img_2[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
				static t_opflow *opflow_img_work=NULL;

				static IppiSize imgtam;

				/*tamaño del buffer para el cálculo de los puntos de interés*/
				static int eigen_buff_tam;

				static int nfeat_temp; /*Número de puntos de interés provisionales*/
				punto.x = 0;
				punto.y = 0;
				ippiRGBToGray_8u_C3C1R ((Ipp8u*)original, ancho*3,
											(Ipp8u*)image2Aux, ancho, imgtam);
				/*Comienzo de la iteración*/
				speedcounter(opflow_id);
				/*Inicialización de los punteros de imagenes*/
				if (inicializado==0){
					old_bw=img_bw1;
					new_bw=img_bw2;
					prev=prev1;
					next=next1;
					prev_work=prev2;
					next_work=next2;
					opflow_error=error1;
					error_work=error2;
					opflow_status=status1;
					status_work=status2;
					imgtam.width= SIFNTSC_COLUMNS;
					imgtam.height= SIFNTSC_ROWS;
					if (ippiMinEigenValGetBufferSize_8u32f_C1R(imgtam, 3, 3, &eigen_buff_tam)!=ippStsNoErr){
						fprintf (stderr, "Error calculating size of temp space for calculate eigen values\n");
						exit(-1);
					}
					opflow_img_work=opflow_img_1;
					opflow_img=NULL;
				}

				/*Obtain image*/
				ippiCopy_8u_C3R((Ipp8u*)image, SIFNTSC_COLUMNS*3, (Ipp8u*)export_old_work, SIFNTSC_COLUMNS*3, imgtam);
	//			ippiMulScale_8u_C3R((Ipp8u*)image, SIFNTSC_COLUMNS*3, (Ipp8u *)mask, SIFNTSC_COLUMNS*3,
	//										(Ipp8u*)masked_old, SIFNTSC_COLUMNS*3, imgtam);
				ippiRGBToGray_8u_C3C1R ((Ipp8u*)export_old_work, SIFNTSC_COLUMNS*3, (Ipp8u*)new_bw, SIFNTSC_COLUMNS, imgtam);
				/*Ahora se tiene capturada una imagen, habrá que tener dos para ejecutar
					el algoritmo, se espera a la siguiente iteración si es necesario*/
				if (inicializado){
					int i,j;
					static Ipp8u* eig_buffer=NULL;
					static Ipp32f *eig_val=NULL;
					static int eig_val_s;
	
					int ksize=5, wsize=9;
					float rate=2.0f, alpha=0.375f;
	
					static t_pInteres interes[NUM_FEAT_MAX];
					/*Variable temporal para obtener los puntos de interés*/
	
					if (eig_buffer==NULL){
					//printf ("Reserva memoria para eig_buffer step=%d\n",eig_buff_step);
						eig_buffer=(Ipp8u*)malloc(sizeof (Ipp8u)*eigen_buff_tam);
						if (eig_buffer==NULL)
							printf ("Falla la llamada a malloc %d\n",eigen_buff_tam);
					}
					if (eig_val==NULL){
						eig_val=ippiMalloc_32f_C1 (SIFNTSC_COLUMNS, SIFNTSC_ROWS, &eig_val_s);
						if (eig_val==NULL){
							printf ("Falla la llamada a malloc2\n");
						}
					}
					nfeat_temp=0;
					switch (ippiMinEigenVal_8u32f_C1R((Ipp8u*)old_bw, SIFNTSC_COLUMNS,
																	eig_val, SIFNTSC_COLUMNS*sizeof(Ipp32f),
																	imgtam, ippKernelScharr, 3, 3, eig_buffer))
					{
						case ippStsNoErr:
							for (j=0;j<SIFNTSC_ROWS;j++){
							for (i=0; i<SIFNTSC_COLUMNS; i++){
								int val=i+j*SIFNTSC_COLUMNS;
								Ipp32f valor= eig_val[val];
						//Se utiliza también para reiniciar la matriz resultado
								opflow_img_work[val].calc=0;
						//En este punto hay que umbralizar y ordenar
								if (valor>min_eig && valor<max_eig){
									if (nfeat_temp<NUM_FEAT_MAX){
										int index1;
										int index2;
										if (nfeat_temp==0){
											interes[0].punto.x=i;
											interes[0].punto.y=j;
											interes[0].valor=valor;
										}
										else{
											for (index1=0; index1<nfeat_temp; index1++){
												if (interes[index1].valor<valor){
													/*Desplazar hacia la derecha*/
													for (index2=nfeat_temp; index2>index1; index2--){
														interes[index2]=interes[index2-1];
													}
													interes[index1].punto.x=i;
													interes[index1].punto.y=j;
													interes[index1].valor=valor;
													break;
												}
											}
										}
										nfeat_temp++;
									}
									else{
										if (interes[NUM_FEAT_MAX-1].valor<valor){
										/*Insertar en el lugar adecuado*/
										int index1;
										int index2;
										for (index1=0; index1<nfeat_temp; index1++){
											if (interes[index1].valor<valor){
												/*Desplazar hacia la derecha*/
												for (index2=NUM_FEAT_MAX-1; index2>index1; index2--){
													interes[index2]=interes[index2-1];
												}
												interes[index1].punto.x=i;
												interes[index1].punto.y=j;
												interes[index1].valor=valor;
												break;
											}
										}
										}
									}
								}
							}
							}
							for (i=0;i<nfeat_temp; i++){
								prev_work[i]=interes[i].punto;
							}
	
							break;
						case ippStsNullPtrErr:
							printf ("Puntero nulo.\n");
							if (old_bw==NULL){
							printf ("old_bw nulo\n");
							}
							if (eig_val==NULL){
							printf ("eig_val nulo\n");
							}
							if (eig_buffer==NULL){
							printf ("eig_buffer nulo\n");
							}
							break;
						case ippStsSizeErr:
							printf ("Parámetros erróneos.\n");
							break;
						case ippStsStepErr:
							printf ("Step inválido.\n");
							printf ("width=%d, COLUMS=%d\n", imgtam.width, SIFNTSC_COLUMNS);
							break;
						case ippStsNotEvenStepErr:
							printf ("Step no divisible /4.\n");
							break;
						default:
							printf ("Error desconocido.\n");
							break;
					}

					/*Crea un núcleo para la creación de las pirámides*/
					if (kernel==NULL){
						Ipp32f sum;
						int i;
						kernel=(Ipp32f*)malloc(sizeof(Ipp32f)*ksize);
						kernel_16s=(Ipp16s*)malloc(sizeof(Ipp16s)*ksize);
						switch (ksize) {
							case 3:
							kernel[1] = alpha;
							kernel[0] = kernel[2] = 0.5f*(1.0f - alpha);
							break;
							case 5:
							kernel[2] = alpha;
							kernel[1] = kernel[3] = 0.25f;
							kernel[0] = kernel[4] = 0.5f*(0.5f - alpha);
							break;
							default:
							sum = 0;
							for (i=0; i<ksize; i++) {
								kernel[i] = (Ipp32f)exp(alpha*(ksize/2-i)*(ksize/2-i)*0.5f);
								sum += kernel[i];
							}
							for (i=0; i<ksize; i++) {
								kernel[i] /= sum;
							}
						}
						ippsConvert_32f16s_Sfs(kernel, kernel_16s, ksize, ippRndNear, -15);
					}
					/*Copia de los puntos de interés a next_work*/
					memcpy(next_work, prev_work, nfeat_temp*sizeof(IppiPoint_32f));
					/*Se llama a la función que calcula el flujo*/
					pyroptflow(old_bw, SIFNTSC_COLUMNS, new_bw, SIFNTSC_COLUMNS, imgtam,
						layers, rate, kernel_16s, ksize, prev_work, next_work,
						status_work, error_work, nfeat_temp, wsize, num_iter,
						threshold);
					ippiCopy_8u_C3R((Ipp8u*)original, SIFNTSC_COLUMNS*3, (Ipp8u*)image, SIFNTSC_COLUMNS*3, imgtam);
	
					if (formato==ORIGEN_DESTINO){
						for (i=0;i<nfeat_temp; i++){
							int x=(int)prev_work[i].x;
							int y=(int)prev_work[i].y;
							int valor;
							if (status_work[i]==0 && error_work[i]< max_err && x<SIFNTSC_COLUMNS && y<SIFNTSC_ROWS && y>=0 && x>=0){
								int x1 = (int) prev_work[i].x;
								int y1 = (int) prev_work[i].y;
								int x2 = (int) next_work[i].x;
								int y2 = (int) next_work[i].y;
								valor = x+y*SIFNTSC_COLUMNS;
	
								opflow_img_work[valor].hyp = /*sqrt*/( square(y1 - y2) + square(x1 - x2) );
								if (opflow_img_work[valor].hyp > min_mov){
									opflow_img_work[valor].calc=1;
									opflow_img_work[valor].status=status_work[i];
									opflow_img_work[valor].error=error_work[i];
									opflow_img_work[valor].dest.x=next_work[i].x;
									opflow_img_work[valor].dest.y=next_work[i].y;
									opflow_img_work[valor].angle = atan2( (double) (y1 - y2), (double) (x1 - x2) );
								}
								else{
									opflow_img_work[valor].calc=0;
								}
							}
						}
					}
					else{ /*formato == DESTINO_ORIGEN */
					for (i=0;i<nfeat_temp; i++){
							int x=(int)next_work[i].x;
							int y=(int)next_work[i].y;
							int valor;
							if (status_work[i]==0 && error_work[i]< max_err && x<SIFNTSC_COLUMNS && y<SIFNTSC_ROWS && y>=0 && x>=0){
								int x1 = (int) next_work[i].x;
								int y1 = (int) next_work[i].y;
								int x2 = (int) prev_work[i].x;
								int y2 = (int) prev_work[i].y;
	
								valor= x+y*SIFNTSC_COLUMNS;
	
								opflow_img_work[valor].hyp = /*sqrt*/( square(y1 - y2) + square(x1 - x2) );
								if (opflow_img_work[valor].hyp > min_mov){
									opflow_img_work[valor].calc=1;
									opflow_img_work[valor].status=status_work[i];
									opflow_img_work[valor].error=error_work[i];
									opflow_img_work[valor].dest.x=prev_work[i].x;
									opflow_img_work[valor].dest.y=prev_work[i].y;
									opflow_img_work[valor].angle = atan2( (double) (y1 - y2), (double) (x1 - x2) );
								}
								else{
									opflow_img_work[valor].calc=0;
								}
							}
						}
					}
				}

				inicializado=1;
				/*Cambiar el buffer de las imágenes en 1 canal*/
				if (old_bw==img_bw1){
					old_bw=img_bw2;
					new_bw=img_bw1;
				}
				else{
					old_bw=img_bw1;
					new_bw=img_bw2;
				}
				/*Cambiar el buffer de trabajo de prev y next*/
				if (prev==prev1){
					prev_work=prev1;
					next_work=next1;
					status_work=status1;
					error_work=error1;
					prev=prev2;
					next=next2;
					opflow_status=status2;
					opflow_error=error2;
				} else{
					prev_work=prev2;
					next_work=next2;
					status_work=status2;
					error_work=error2;
					prev=prev1;
					next=next1;
					opflow_status=status1;
					opflow_error=error1;
				}
				nfeat=nfeat_temp;
				if (export_old==export_old1){
					export_old=export_old2;
					export_old_work=export_old1;
				} else{
					export_old=export_old1;
					export_old_work=export_old2;
				}
	
				if (opflow_img_work==opflow_img_1){
					opflow_img=opflow_img_1;
					opflow_img_work=opflow_img_2;
				} else{
					opflow_img=opflow_img_2;
					opflow_img_work=opflow_img_1;
				}
				for (i=0; i<nfeat; i++){
					int valor=((int)prev[i].x+SIFNTSC_COLUMNS*(int)prev[i].y)*3;
					if (valor<max){
						image[valor]=255;
						image[valor+1]=0;
						image[valor+2]=0;
					}
				}
			}
			break;
		}
	}
	pthread_mutex_unlock(&main_mutex);
}

/*Importar símbolos*/
void prueba_ipp_imports(){

}
/*Exportar símbolos*/
void prueba_ipp_exports(){
	myexport("prueba_ipp","cycle",&prueba_ipp_cycle);
	myexport("prueba_ipp","resume",(void *)prueba_ipp_resume);
	myexport("prueba_ipp","suspend",(void *)prueba_ipp_suspend);
}
/*Las inicializaciones van en esta parte*/
void prueba_ipp_init(){
	if (myregister_displaycallback==NULL){
		if ((myregister_displaycallback= (registerdisplay)myimport ("graphics_gtk", "register_displaycallback"))==NULL)
		{
			printf ("I can't fetch register_displaycallback from graphics_gtk\n");
			jdeshutdown(1);
		}
		if ((mydelete_displaycallback= (deletedisplay)myimport ("graphics_gtk", "delete_displaycallback")) ==NULL)
		{
			printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
			jdeshutdown(1);
		}
	}
}
/*Al suspender el esquema*/
void prueba_ipp_end(){

}
void prueba_ipp_stop(){

}
void prueba_ipp_suspend() {
	pthread_mutex_lock(&(all[prueba_ipp_id].mymutex));
	put_state(prueba_ipp_id,slept);
	printf("prueba_ipp: off\n");
	pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
	prueba_ipp_end();
}
void prueba_ipp_resume(int father, int *brothers, arbitration fn) {
	int i;

	/* update the father incorporating this schema as one of its children */
	if (father!=GUIHUMAN && father!=SHELLHUMAN)
	{
		pthread_mutex_lock(&(all[father].mymutex));
		all[father].children[prueba_ipp_id]=TRUE;
		pthread_mutex_unlock(&(all[father].mymutex));
	}

	pthread_mutex_lock(&(all[prueba_ipp_id].mymutex));
	/* this schema resumes its execution with no children at all */
	for(i=0;i<MAX_SCHEMAS;i++) all[prueba_ipp_id].children[i]=FALSE;
	all[prueba_ipp_id].father=father;
	if (brothers!=NULL)
	{
		for(i=0;i<MAX_SCHEMAS;i++) prueba_ipp_brothers[i]=-1;
		i=0;
		while(brothers[i]!=-1) {prueba_ipp_brothers[i]=brothers[i];i++;}
	}
	prueba_ipp_callforarbitration=fn;
	put_state(prueba_ipp_id,notready);
printf("20\n");
	printf("prueba_ipp: on\n");
printf("30\n");
	pthread_cond_signal(&(all[prueba_ipp_id].condition));
printf("40");
	pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
printf("50");
	prueba_ipp_imports();
}
void *prueba_ipp_thread(void *not_used) {
	struct timeval a,b;
	long n=0; /* iteration */
	long next,bb,aa;

	for(;;)
	{
		pthread_mutex_lock(&(all[prueba_ipp_id].mymutex));

		if (all[prueba_ipp_id].state==slept)
		{
			pthread_cond_wait(&(all[prueba_ipp_id].condition),&(all[prueba_ipp_id].mymutex));
			pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
		}
		else
		{
			/* check preconditions. For now, preconditions are always satisfied*/
			if (all[prueba_ipp_id].state==notready)
				put_state(prueba_ipp_id,ready);
			/* check brothers and arbitrate. For now this is the only winner */
			if (all[prueba_ipp_id].state==ready){
				put_state(prueba_ipp_id,winner);
				gettimeofday(&a,NULL);
				aa=a.tv_sec*1000000+a.tv_usec;
				n=0;
			}

			if (all[prueba_ipp_id].state==winner)
				/* I'm the winner and must execute my iteration */
			{
				pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
				/* gettimeofday(&a,NULL);*/
				n++;
				prueba_ipp_iteration();
				gettimeofday(&b,NULL);
				bb=b.tv_sec*1000000+b.tv_usec;
				next=aa+(n+1)*(long)prueba_ipp_cycle*1000-bb;

				if (next>5000){
					usleep(next-5000);
					/* discounts 5ms taken by calling usleep itself, on average */
				}
				else  ;
			}
			else{
				/* just let this iteration go away. overhead time negligible */
				pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
				usleep(prueba_ipp_cycle*1000);
			}
		}
	}
}
void prueba_ipp_startup(char *configfile) {
	pthread_mutex_lock(&(all[prueba_ipp_id].mymutex));
	printf("prueba_ipp schema started up\n");
	prueba_ipp_exports();
	put_state(prueba_ipp_id,slept);
	pthread_create(&(all[prueba_ipp_id].mythread),NULL,prueba_ipp_thread,NULL);
	pthread_mutex_unlock(&(all[prueba_ipp_id].mymutex));
	prueba_ipp_init();
}
void prueba_ipp_guidisplay(){
	pthread_mutex_lock(&main_mutex);
	if (image_selected!=-1){
		GtkImage *img = GTK_IMAGE(glade_xml_get_widget(xml, "image"));

		gdk_threads_enter();
		gtk_widget_queue_draw(GTK_WIDGET(img));
		gdk_threads_leave();
	}
	pthread_mutex_unlock(&main_mutex);
}
void prueba_ipp_guisuspend(void){
	if (win!=NULL){
		gdk_threads_enter();
		gtk_widget_hide(win);
		gdk_threads_leave();
	}
	mydelete_displaycallback(prueba_ipp_guidisplay);
}
void prueba_ipp_guiresume(void){
	static int cargado=0;
	static pthread_mutex_t prueba_ipp_gui_mutex;

	pthread_mutex_lock(&prueba_ipp_gui_mutex);
	if (!cargado){
		loadglade ld_fn;
		cargado=1;
		pthread_mutex_unlock(&prueba_ipp_gui_mutex);
		/*Cargar la ventana desde el archivo xml .glade*/
		gdk_threads_enter();
		if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
			fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
			jdeshutdown(1);
		}
		xml = ld_fn ("prueba_ipp.glade");
		if (xml==NULL){
			fprintf(stderr, "Error al cargar la interfaz gráfica\n");
			jdeshutdown(1);
		}
		win = glade_xml_get_widget(xml, "window");
		/*Conectar los callbacks*/
		{
			GtkComboBoxEntry *sel_ent;
			sel_ent=(GtkComboBoxEntry *)glade_xml_get_widget(xml, "img_sel");
			g_signal_connect(G_OBJECT(sel_ent), "changed", G_CALLBACK(on_img_sel_changed), NULL);
			GtkComboBoxEntry *tipo_ent;
			tipo_ent=(GtkComboBoxEntry *)glade_xml_get_widget(xml, "img_tipo");
			g_signal_connect(G_OBJECT(tipo_ent), "changed", G_CALLBACK(on_img_tipo_changed), NULL);
		}

		{
			GtkHScale *escala1;
			GtkHScale *escala2;
			GtkHScale *escala3;
			GtkHScale *escala4;
			GtkHScale *escala5;
			GtkHScale *escala6;
			escala1=(GtkHScale *)glade_xml_get_widget(xml, "hscale1");
			g_signal_connect(G_OBJECT(escala1), "value_changed", G_CALLBACK(cambiar_imagen_filtror_bajo), NULL);
			escala2=(GtkHScale *)glade_xml_get_widget(xml, "hscale2");
			g_signal_connect(G_OBJECT(escala2), "value_changed", G_CALLBACK(cambiar_imagen_filtrog_bajo), NULL);
			escala3=(GtkHScale *)glade_xml_get_widget(xml, "hscale3");
			g_signal_connect(G_OBJECT(escala3), "value_changed", G_CALLBACK(cambiar_imagen_filtrob_bajo), NULL);
			escala4=(GtkHScale *)glade_xml_get_widget(xml, "hscale4");
			g_signal_connect(G_OBJECT(escala4), "value_changed", G_CALLBACK(cambiar_imagen_filtror_alto), NULL);
			escala5=(GtkHScale *)glade_xml_get_widget(xml, "hscale5");
			g_signal_connect(G_OBJECT(escala5), "value_changed", G_CALLBACK(cambiar_imagen_filtrog_alto), NULL);
			escala6=(GtkHScale *)glade_xml_get_widget(xml, "hscale6");
			g_signal_connect(G_OBJECT(escala6), "value_changed", G_CALLBACK(cambiar_imagen_filtrob_alto), NULL);
		}

		if (win==NULL){
			fprintf(stderr, "Error al cargar la interfaz gráfica\n");
			jdeshutdown(1);
		}
		else{
			gtk_widget_show(win);
			gtk_widget_queue_draw(GTK_WIDGET(win));
		}
		contexto_pila=gtk_statusbar_get_context_id ((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"), "contexto general");
		gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"), contexto_pila, "No hay fuente de imagen seleccionada");
		gdk_threads_leave();
	}
	else{
		pthread_mutex_unlock(&prueba_ipp_gui_mutex);
		gdk_threads_enter();
		gtk_widget_show(win);
		gtk_widget_queue_draw(GTK_WIDGET(win));
		gdk_threads_leave();
	}

	myregister_displaycallback(prueba_ipp_guidisplay);
}
