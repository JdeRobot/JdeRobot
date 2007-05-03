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
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include "jde.h"
#include "jdegui.h"
#include "opflowgui.h"
#include "opflow.h"
#include <string.h>

#define OPFLOWver "opflow 1.2" /*Nombre y versión del programa*/

/*Número de puntos de interés*/
#define NUM_FEAT_MAX 300

/*Para los botones*/
#define PUSHED 1
#define RELEASED 0

#define square(a)  ((a)*(a))

typedef struct{
   IppiPoint_32f punto;
   Ipp32f valor;
}t_pInteres;

/*Export variables*/
t_opflow *opflow_img=NULL;

static const double pi = 3.14159265358979323846;

int opflow_id=0; 
int opflow_brothers[MAX_SCHEMAS];
arbitration opflow_callforarbitration;

FD_opflowgui *fd_opflowgui;

/* exported variables */
int opflow_cycle=90; /* ms */

int mysonColorA;
/* imported variables */
char **mycolorA;
resumeFn mycolorAresume;
suspendFn mycolorAsuspend;

/*Necesarias para las Xlib*/
GC opflow_gc;
Window opflow_window;
XImage *new_image;
XImage *old_image;

/*Imágenes para el gui*/
Ipp8u show_old[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];//Para mostrar en el display
Ipp8u show_new[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
Ipp8u *old_img=NULL;//Para que las utilice el gui y las pase al display
Ipp8u *new_img=NULL;

/*Variables del gui*/
double threshold=0.3;
int layers=3;
int num_iter=7;
int show_arrows=0;
int nfeat=0;
float min_eig=1.0;
float max_eig=2.0;
float min_mov=1.5;
float max_err=30;


/*Movimientos encontrados*/
IppiPoint_32f *prev=0; /* coordinates on previous frame*/
IppiPoint_32f *next=0; /* hint to coordinates on next frame*/
Ipp8s *opflow_status=0; /*indicates if opflow have been found*/
Ipp32f *opflow_error=0; /*indicates opflow calc error*/

FD_opflowgui *fd_opflowgui;

/*Calculates optical flow using IPP library*/
void optflow_IPP(
   const Ipp8u *prevFrame, // previous frame
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
      IppiPyramidDownState_8u_C1R **pState1 = (IppiPyramidDownState_8u_C1R**) & (pPyr1->pState);
      IppiPyramidDownState_8u_C1R **pState2 = (IppiPyramidDownState_8u_C1R**) & (pPyr2->pState);
      Ipp8u **pImg1 = pPyr1->pImage;
      Ipp8u **pImg2 = pPyr2->pImage;
      int *pStep1 = pPyr1->pStep;
 
      int *pStep2 = pPyr2->pStep;
 
      IppiSize *pRoi1 = pPyr1->pRoi;
      IppiSize *pRoi2 = pPyr2->pRoi;
      IppHintAlgorithm hint=ippAlgHintFast;
      int i,level = pPyr1->level;
 
      ippiPyramidLayerDownInitAlloc_8u_C1R(pState1,roiSize,rate,
					   pKernel,kerSize,IPPI_INTER_LINEAR);
      ippiPyramidLayerDownInitAlloc_8u_C1R(pState2,roiSize,rate,pKernel,
					   kerSize,IPPI_INTER_LINEAR);
      pImg1[0] = (Ipp8u*)prevFrame;
      pImg2[0] = (Ipp8u*)nextFrame;
      pStep1[0] = prevStep;
      pStep2[0] = nextStep;
      pRoi1[0] = pRoi2[0] = roiSize;
      for (i=1; i<=level; i++) {
	 pPyr1->pImage[i] = ippiMalloc_8u_C1(pRoi1[i].width,
					     pRoi1[i].height,pStep1+i);
	 pPyr2->pImage[i] = ippiMalloc_8u_C1(pRoi2[i].width,pRoi2[i].height,
					     pStep2+i);
	 ippiPyramidLayerDown_8u_C1R(pImg1[i-1],pStep1[i-1],pRoi1[i-1], 
				     pImg1[i],pStep1[i],pRoi1[i],*pState1);
	 ippiPyramidLayerDown_8u_C1R(pImg2[i-1],pStep2[i-1],pRoi2[i-1], 
				     pImg2[i],pStep2[i],pRoi2[i],*pState2);
      }
      ippiOpticalFlowPyrLKInitAlloc_8u_C1R (&pOF,roiSize,winSize,hint);
      switch (ippiOpticalFlowPyrLK_8u_C1R (pPyr1,pPyr2,prevPt,nextPt,pStatus,
	                                   pError,numFeat,winSize,numLevel,
					   numIter,threshold,pOF))
      {
	 case ippStsNoErr:
	    //printf ("Sin error\n");
	    break;
	 case ippStsNullPtrErr:
	    printf ("Puntero a nulo\n");
	    break;
	 case ippStsSizeErr:
	    printf ("Valor de tamaño erroneo\n");
	    break;
	 case ippStsBadArgErr:
	    printf ("Error maxLev, threshold o maxIter\n");
	    break;
	 default:
	    printf ("opción desconocida\n");
      }
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


/*Print lines*/
int lineinimage(Ipp8u *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor){
   float L;
   int i,imax,r,g,b;
   int lastx,lasty,thisx,thisy,lastcount;
   int threshold=1;
   int Xmax,Xmin,Ymax,Ymin;
   int punto;

   Xmin=0; Xmax=SIFNTSC_COLUMNS-1; Ymin=0; Ymax=SIFNTSC_ROWS-1;
   /* In this image always graf coordinates x=horizontal, y=vertical, starting
   at the top left corner of the image. They can't reach 240 or 320, their are
   not valid values for the pixels.  */

   if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
   else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
   else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
   else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==FL_GREEN) {r=0;g=255;b=0;}
   else {r=0;g=0;b=0;}

   /* first, check both points are inside the limits and draw them */
   /* draw both points */
   if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1)){
      img[(SIFNTSC_COLUMNS*ya+xa)*4+0]=b;
      img[(SIFNTSC_COLUMNS*ya+xa)*4+1]=g;
      img[(SIFNTSC_COLUMNS*ya+xa)*4+2]=r;
   }
   if ((xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)){
      img[(SIFNTSC_COLUMNS*yb+xb)*4+0]=b;
      img[(SIFNTSC_COLUMNS*yb+xb)*4+1]=g;
      img[(SIFNTSC_COLUMNS*yb+xb)*4+2]=r;
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
	    if ((lastx>=Xmin)&&(lastx<Xmax+1)&&(lasty>=Ymin)&&(lasty<Ymax+1)){
	       punto=(SIFNTSC_COLUMNS*lasty+lastx)*4;
	       img[punto]=b;
	       img[punto+1]=g;
	       img[punto+2]=r;
	    }
	 }
	 lasty=thisy;
	 lastx=thisx;
	 lastcount=0;
      }
   }
   return 0; 
}

int opflowgui_setupDisplay(void)
      /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/
{
   int vmode;
   static XGCValues gc_values;

   gc_values.graphics_exposures = False;
   opflow_gc = XCreateGC(display,opflow_window, GCGraphicsExposures, &gc_values);
   
   vmode= fl_get_vclass();

   if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
   {
      printf("16 bits mode\n");
      /* Imagen principal */
      old_image = XCreateImage(display,DefaultVisual(display,screen),16,
			       ZPixmap, 0, (char*)show_old, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      /*Imagen filtrada */
      new_image = XCreateImage(display, DefaultVisual(display,screen),16,
			       ZPixmap,0,(char*)show_new,SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);
   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
   {
      printf("24 bits mode\n");
      /* Imagen principal */
      old_image = XCreateImage(display,DefaultVisual(display,screen),24,
			       ZPixmap, 0, (char*)show_old, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      /*Imagen filtrada */
      new_image = XCreateImage(display, DefaultVisual(display,screen),24,
			       ZPixmap,0,(char*)show_new,SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);
   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
   {

      printf("32 bits mode\n");
      /* Imagen principal */
      old_image = XCreateImage(display,DefaultVisual(display,screen),32,
			       ZPixmap, 0, (char*)show_old, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      /*Imagen filtrada */
      new_image = XCreateImage(display, DefaultVisual(display,screen),32,
			       ZPixmap,0,(char*)show_new,SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);
      
   }
   else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
   {
      printf("8 bits mode\n");
      /* Imagen principal */
      old_image = XCreateImage(display,DefaultVisual(display,screen),8,
			       ZPixmap, 0, (char*)show_old, SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

      /*Imagen filtrada */
      new_image = XCreateImage(display, DefaultVisual(display,screen),8,
			       ZPixmap,0,(char*)show_new,SIFNTSC_COLUMNS,
			       SIFNTSC_ROWS,8,0);

   }
   else
   {
      perror("Unsupported color mode in X server");exit(1);
   }
   return 1;
}

void opflow_iteration()
{  
   static Ipp8u img1[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4]; /*Imágenes en color*/
   static Ipp8u img2[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4]; /*con doble buffer*/
   static Ipp8u img3[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4]; /*para cada imagen*/
   static Ipp8u img4[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4]; /*nueva y vieja*/

   static Ipp8u *old_bw=NULL;
   static Ipp8u *new_bw=NULL;

   static Ipp8u img_bw1[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
   static Ipp8u img_bw2[SIFNTSC_COLUMNS*SIFNTSC_ROWS];
   /*   static Ipp8u *old_bw=NULL;
   static Ipp8u *new_bw=NULL;
   */
   static Ipp8u *work_img_new; /*Imagenes de trabajo*/
   static Ipp8u *work_img_old=NULL;

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
  
   static int inicializado=0;
   static IppiSize imgtam;

   /*tamaño del buffer para el cálculo de los puntos de interés*/
   static int eigen_buff_tam;

   int nfeat_temp; /*Número de puntos de interés provisionales*/

   int i,j;
   static Ipp8u* eig_buffer=NULL;
   static int eig_buff_step;
   static Ipp32f *eig_val=NULL;
   static int eig_val_s;
   
   static t_pInteres interes[NUM_FEAT_MAX];

   int ksize=5, wsize=9;
   float rate=2.0f, alpha=0.375f;
   static Ipp32f *kernelX=NULL;
   static Ipp16s *kernel_16s=NULL;
   Ipp32f sum;

   /*Comienzo de la iteración*/
   speedcounter(opflow_id);
   /*Inicialización de los punteros de imagenes*/
   if (inicializado==0){
      work_img_new=img1;
      work_img_old=img2;
      new_img=img3;
      old_img=img4;
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
      if (ippiMinEigenValGetBufferSize_8u32f_C1R(imgtam, 3, 3, &eigen_buff_tam)!=
	  ippStsNoErr){
	 fprintf (stderr, "Error calculating size of temp space for calculate eigen values\n");
	 exit(-1);
      }
      opflow_img_work=opflow_img_1;
      opflow_img=opflow_img_2;
      /*printf("EigenValBufferSize: %d \n",eigen_buff_tam);*/
      /*Inicializar el canal alpha para que no sea transparente*/
   }

   /*Obtain image*/
   ippiCopy_8u_C3AC4R ( (Ipp8u *)(*mycolorA), SIFNTSC_COLUMNS*3,
			(Ipp8u *)work_img_new, SIFNTSC_COLUMNS*4, imgtam);
   ippiRGBToGray_8u_C3C1R ((Ipp8u *)(*mycolorA), SIFNTSC_COLUMNS*3,
			   (Ipp8u *)new_bw, SIFNTSC_COLUMNS, imgtam);
  
   /*Ahora se tiene capturada una imagen, habrá que tener dos para ejecutar
    el algoritmo*/
   if (inicializado){
    
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
      /////////////////////////////////////////////////////////////
      nfeat_temp=0;
      switch (ippiMinEigenVal_8u32f_C1R((Ipp8u*)old_bw, SIFNTSC_COLUMNS,
	                         eig_val, SIFNTSC_COLUMNS*sizeof(Ipp32f),
				 imgtam, ippKernelScharr, 3, 3, eig_buffer))
      {
	 case ippStsNoErr:
	    for (j=0;j<SIFNTSC_ROWS;j++){
	       for (i=0; i<SIFNTSC_COLUMNS; i++){
		  int val=i+j*SIFNTSC_COLUMNS;
		  Ipp32f valorX= eig_val[val];
		  //En este punto hay que umbralizar y ordenar
		  if (valorX>min_eig && valorX<max_eig){
		     if (nfeat_temp<NUM_FEAT_MAX){
			interes[nfeat_temp].punto.x=i;
			interes[nfeat_temp].punto.y=j;
			interes[nfeat_temp].valor=valorX;
			nfeat_temp++;
		     }
		     else{
			if (interes[nfeat_temp].valor<valorX){
			   //Insertar en el lugar adecuado
			   int index1;
			   int index2;
			   for (index1=0; index1<nfeat_temp; index1++){
			      if (interes[index1].valor<valorX){
				 //Desplazar hacia la derecha
				 for (index2=nfeat_temp-1; index2>index1; index2--){
				    interes[index2]=interes[index2-1];
				 }
				 interes[index1].punto.x=i;
				 interes[index1].punto.y=j;
				 interes[index1].valor=valorX;
				 break;
			      }
			   }
			}
		     }
		  }
		  //Aprovechamos el bucle para reiniciar la matriz del resultado
		  opflow_img_work[val].calc=0;
	       }
	    }
            for (i=0;i<nfeat_temp; i++){
	       prev_work[i].x=interes[i].punto.x;
	       prev_work[i].y=interes[i].punto.y;
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
      
      /////////////////////////////////////////////////////////////

      /*Crea un núcleo para la creación de las pirámides*/
      if (kernelX==NULL){
         kernelX=(Ipp32f*)malloc(sizeof(Ipp32f)*ksize);
	 if (kernelX==NULL) printf("falló malloc kernel\n");
         kernel_16s=(Ipp16s*)malloc(sizeof(Ipp16s)*ksize);
	 if (kernel_16s==NULL) printf("falló malloc kernel_16s\n");
	 switch (ksize) {
	    case 3:
	       kernelX[1] = alpha;
	       kernelX[0] = kernelX[2] = 0.5f*(1.0f - alpha);
	       break;
	    case 5:
	       kernelX[2] = alpha;
	       kernelX[1] = kernelX[3] = 0.25f;
	       kernelX[0] = kernelX[4] = 0.5f*(0.5f - alpha);
	       break;
	    default:
	       sum = 0;
	       for (i=0; i<ksize; i++) {
		  kernelX[i] = (Ipp32f)exp(alpha*(ksize/2-i)*(ksize/2-i)*0.5f);
		  sum += kernelX[i];
	       }
	       for (i=0; i<ksize; i++) {
		  kernelX[i] /= sum;
	       }
	 }
	 ippsConvert_32f16s_Sfs(kernelX, kernel_16s, ksize, ippRndNear, -15);
      }

      memcpy(next_work, prev_work, nfeat_temp*sizeof(IppiPoint_32f));
      /*Se llama a la función que calcula el flujo*/
      optflow_IPP(old_bw, SIFNTSC_COLUMNS, new_bw, SIFNTSC_COLUMNS, imgtam,
		  layers, rate, kernel_16s, ksize, prev_work, next_work,
		  status_work, error_work, nfeat_temp, wsize, num_iter,
		  threshold);

      for (i=0;i<nfeat_temp; i++){
	 int valor = prev_work[i].x+prev_work[i].y*SIFNTSC_COLUMNS;
	 if (status_work[i]==0 && error_work[i]< max_err){
	    int x1 = (int) prev_work[i].x;
	    int y1 = (int) prev_work[i].y;
	    int x2 = (int) next_work[i].x;
	    int y2 = (int) next_work[i].y;
	    opflow_img_work[valor].hyp = /*sqrt*/( square(y1 - y2) +
		                               square(x1 - x2) );
	    if (opflow_img_work[valor].hyp > min_mov){
	    
	    opflow_img_work[valor].calc=1;
	    opflow_img_work[valor].status=status_work[i];
	    opflow_img_work[valor].error=error_work[i];
	    opflow_img_work[valor].dest=next_work[i];

	    opflow_img_work[valor].angle = atan2( (double) y1 - y2,
		                                  (double) x1 - x2 );
	    }
	    else{
	       opflow_img_work[valor].calc=0;
	    }
	 }
      }
   }


   inicializado=1;
   /*Mover los punteros de las imágenes*/
   if (work_img_new==img1){
      work_img_new=img4;
      work_img_old=img3;
      old_img=img2;
      new_img=img1;
   }
   else if (work_img_new==img4){
      work_img_new=img2;
      work_img_old=img1;
      old_img=img3;
      new_img=img4;
   }
   else if (work_img_new==img2){
      work_img_new=img3;
      work_img_old=img4;
      old_img=img1;
      new_img=img2;
   }
   else if (work_img_new==img3){
      work_img_new=img1;
      work_img_old=img2;
      old_img=img4;
      new_img=img3;
   }
   /*Cambiar el buffer de las imágenes en 1 canal*/
   if (old_bw==img_bw1){
     /*     printf("cambio %ld %ld\n",(long)old_bw,(long)new_bw);*/
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
   }
   else{
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
   if (opflow_img_work==opflow_img_1){
      opflow_img=opflow_img_1;
      opflow_img_work=opflow_img_2;
   }
   else{
      opflow_img=opflow_img_2;
      opflow_img_work=opflow_img_1;
   }
}


void opflow_suspend()
{
  pthread_mutex_lock(&(all[opflow_id].mymutex));
  put_state(opflow_id,slept);
  mycolorAsuspend();
  printf("opflow: off\n");
  pthread_mutex_unlock(&(all[opflow_id].mymutex));
}

void opflow_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (/*father!=GUIHUMAN*/father>0)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[opflow_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[opflow_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[opflow_id].children[i]=FALSE;
  all[opflow_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) opflow_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {opflow_brothers[i]=brothers[i];i++;}
    }
  opflow_callforarbitration=fn;
  put_state(opflow_id,notready);

  mycolorA=(char **)myimport("colorA","colorA");
  mycolorAresume=(resumeFn)myimport("colorA","resume");
  mycolorAsuspend=(suspendFn *)myimport("colorA","suspend");
  mysonColorA=(*(int *)myimport("colorA","id"));  

  printf("opflow: on\n");
  pthread_cond_signal(&(all[opflow_id].condition));
  pthread_mutex_unlock(&(all[opflow_id].mymutex));
}

void *opflow_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[opflow_id].mymutex));

      if (all[opflow_id].state==slept) 
	{
	  pthread_cond_wait(&(all[opflow_id].condition),&(all[opflow_id].mymutex));
	  pthread_mutex_unlock(&(all[opflow_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[opflow_id].state==notready) put_state(opflow_id,ready);
	  else if (all[opflow_id].state==ready)
	  /* check brothers and arbitrate. For now this is the only winner */
	  {
	     put_state(opflow_id,winner);
	     all[opflow_id].children[mysonColorA]=TRUE;
	     mycolorAresume(opflow_id,NULL,NULL);
	  }
	  else if (all[opflow_id].state==winner);

	  if (all[opflow_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[opflow_id].mymutex));
	      /*printf("opflow: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      opflow_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = opflow_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0)
		 usleep(opflow_cycle*1000-diff);
	      else {
		 printf("time interval violated: opflow\n");
		 usleep(opflow_cycle*1000);
	      }
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[opflow_id].mymutex));
	      usleep(opflow_cycle*1000);
	    }
	}
    }
}

void opflow_startup()
{
   int i;
   pthread_mutex_lock(&(all[opflow_id].mymutex));
   printf("opflow schema started up\n");
   myexport("opflow","cycle",&opflow_cycle);
   myexport("opflow","resume",(void *)opflow_resume);
   myexport("opflow","suspend",(void *)opflow_suspend);
   myexport("opflow","opflow_img", &opflow_img);
   put_state(opflow_id,slept);
   for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
      show_old[i*4+3]=UCHAR_MAX;
      show_new[i*4+3]=UCHAR_MAX;
   }
   pthread_create(&(all[opflow_id].mythread),NULL,opflow_thread,NULL);
   pthread_mutex_unlock(&(all[opflow_id].mymutex));
}

void opflow_guibuttons(FL_OBJECT *obj){
   if (obj == fd_opflowgui->show_arrows){
      if (fl_get_button (obj)==RELEASED){
	 show_arrows=0;
      }
      else {
	 show_arrows=1;
      }
   }
   else if (obj == fd_opflowgui->min_eig){
      min_eig = fl_get_slider_value(obj);
   }
   else if (obj == fd_opflowgui->max_eig){
      max_eig = fl_get_slider_value(obj);
   }
   else if (obj == fd_opflowgui->layers){
      layers = fl_get_slider_value(obj);
   }
   else if (obj == fd_opflowgui->num_iters){
      num_iter = fl_get_slider_value(obj);
   }
   else if (obj == fd_opflowgui->min_mov){
      min_mov = fl_get_slider_value(obj);
   }
}

void opflow_guidisplay(){
   int no_rectas=0, i, j;
   static char text [50];
   static IppiSize imgtam;
   int val=0;
   FL_COLOR color;
   int x1, y1, x2, y2;
   double angle, hypotenuse;
   
   imgtam.height=SIFNTSC_ROWS;
   imgtam.width=SIFNTSC_COLUMNS;
   
   if (new_img!=NULL && old_img!=NULL && prev!=NULL && next!=NULL){
      /*primero se copian las imágenes*/
      ippiCopy_8u_AC4R ((Ipp8u *)old_img, SIFNTSC_COLUMNS*4, (Ipp8u*)show_old,
			 SIFNTSC_COLUMNS*4, imgtam);
      ippiCopy_8u_AC4R ((Ipp8u *)new_img, SIFNTSC_COLUMNS*4, (Ipp8u*)show_new,
			 SIFNTSC_COLUMNS*4, imgtam);
      /*Después en la imagen inicial se pintan las flechas*/
      no_rectas=0;
      for (i=0;i<SIFNTSC_COLUMNS;i++){
	 for (j=0; j<SIFNTSC_ROWS; j++){
	   val=i+j*SIFNTSC_COLUMNS;

	   if (opflow_img[val].status!=0 || opflow_img[val].error > max_err ||
	       opflow_img[val].calc==0)
	       continue;

	    if (opflow_img[val].error >(max_err/2) ){
	       color= FL_RED;
	    }
	    else{
	       color = FL_GREEN;
	    }

	    x1 = (int) i;
	    y1 = (int) j;
	    x2 = (int) opflow_img[val].dest.x;
	    y2 = (int) opflow_img[val].dest.y;

	    hypotenuse = opflow_img[val].hyp;

	    if (hypotenuse<square(min_mov))
	       continue;

	    if (show_arrows){
	       hypotenuse=sqrt(hypotenuse);
	       angle = opflow_img[val].angle;
	       /*Darle longitud al segmento que se pinta*/
	       x2 = (int) (x1 - 2 * hypotenuse * cos(angle));
	       y2 = (int) (y1 - 2 * hypotenuse * sin(angle));
	       /*Dibujar ls línea principal de la flecha*/
	       lineinimage(show_old, x1, y1, x2, y2,color);
	       /*Ahora las puntas de las flechas*/
	       x1 = (int) (x2 + (hypotenuse/4) * cos(angle + pi / 4));
	       y1 = (int) (y2 + (hypotenuse/4) * sin(angle + pi / 4));
	       lineinimage(show_old, x1, y1, x2, y2, color);
	       x1 = (int) (x2 + (hypotenuse/4) * cos(angle - pi / 4));
	       y1 = (int) (y2 + (hypotenuse/4) * sin(angle - pi / 4));
	       lineinimage(show_old, x1, y1, x2, y2, color);
	    }
	    no_rectas++;
	 }
      }
      
      XPutImage(display,opflow_window,opflow_gc,new_image,0,0,
		fd_opflowgui->new_img->x,fd_opflowgui->new_img->y,
		SIFNTSC_COLUMNS, SIFNTSC_ROWS);

      XPutImage(display,opflow_window,opflow_gc,old_image,
		0,0,fd_opflowgui->old_img->x,
		fd_opflowgui->old_img->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
   }

   switch (no_rectas){
      case 0:sprintf(text,"No movements");
      break;
      case 1:sprintf(text,"1 movement");
      break;
      default: sprintf(text,"%d movements",no_rectas);
      break;
   }
  
   fl_set_object_label(fd_opflowgui->no_arrows,text);
}

void opflow_guisuspend(void)
{
  delete_buttonscallback(opflow_guibuttons);
  delete_displaycallback(opflow_guidisplay);
  fl_hide_form(fd_opflowgui->opflowgui);
}

void opflow_guiresume(void)
{
  static int k=0;

  if (k==0){ /* not initialized */
      k++;
      fd_opflowgui = create_form_opflowgui();
      fl_set_form_position(fd_opflowgui->opflowgui,100,200);
      fl_show_form(fd_opflowgui->opflowgui,FL_PLACE_POSITION,FL_FULLBORDER,OPFLOWver);
      opflow_window = FL_ObjWin(fd_opflowgui->old_img);
      opflowgui_setupDisplay();
  }
  else{
     fl_show_form(fd_opflowgui->opflowgui,FL_PLACE_POSITION,FL_FULLBORDER,OPFLOWver);
     opflow_window = FL_ObjWin(fd_opflowgui->old_img);
  }
  
  /*Asignar los valores al gui*/
  fl_set_button (fd_opflowgui->show_arrows, show_arrows);
  fl_set_slider_value (fd_opflowgui->min_eig, min_eig);
  fl_set_slider_value (fd_opflowgui->max_eig, max_eig);
  fl_set_slider_value (fd_opflowgui->layers, layers);
  fl_set_slider_value (fd_opflowgui->num_iters, num_iter);
  fl_set_object_label(fd_opflowgui->no_arrows,"No movements");

  register_buttonscallback(opflow_guibuttons);
  register_displaycallback(opflow_guidisplay);
}
