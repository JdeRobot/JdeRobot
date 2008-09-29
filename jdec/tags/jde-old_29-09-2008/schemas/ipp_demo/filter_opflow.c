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

#include <ipp.h>
#include "jde.h"

#define NUM_FEAT_MAX 300

#define square(a)  ((a)*(a))

static const double pi = 3.14159265358979323846;

typedef struct{
   IppiPoint_32f punto;
   Ipp32f valor;
}t_pInteres;

typedef enum{
   FL_BLACK,
   FL_RED,
   FL_BLUE,
   FL_PALEGREEN,
   FL_WHEAT,
   FL_GREEN,
}tColor;

char *old_img=NULL;
char *new_img=NULL;
IppiSize old_size;

int lineinimage(Ipp8u *img, int xa, int ya, int xb, int yb, tColor thiscolor,
                int width, int height)
{
   float L;
   int i,imax,r,g,b;
   int lastx,lasty,thisx,thisy,lastcount;
   int threshold=1;
   int Xmax,Xmin,Ymax,Ymin;
   int punto;

   Xmin=0; Xmax=width; Ymin=0; Ymax=height;
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
   if ((xa>=Xmin) && (xa<Xmax) && (ya>=Ymin) && (ya<Ymax)){
      img[(width*ya+xa)*3+0]=b;
      img[(width*ya+xa)*3+1]=g;
      img[(width*ya+xa)*3+2]=r;
   }
   if ((xb>=Xmin) && (xb<Xmax) && (yb>=Ymin) && (yb<Ymax)){
      img[(width*yb+xb)*3+0]=b;
      img[(width*yb+xb)*3+1]=g;
      img[(width*yb+xb)*3+2]=r;
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
            if ((lastx>=Xmin)&&(lastx<Xmax)&&(lasty>=Ymin)&&(lasty<Ymax)){
               punto=(width*lasty+lastx)*3;
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

/*Calculates optical flow using IPP library*/
void opflow_IPP(
                const Ipp8u *prevFrame, // previous frame
                int prevStep, // its row step
                const Ipp8u *nextFrame, // next frame
                int nextStep, // its row step
                IppiSize roiSize, // frame size
                int numLevel, // pyramid level number
                float rate, // pyramid rate
                Ipp16s *pKernel, // pyramid kernel
                int kerSize, // pyramid kernel size
                IppiPoint_32f *prevPt, // coordinates on previous frame
                IppiPoint_32f *nextPt, // hint to coordinates on next frame
                Ipp8s *pStatus, // result indicators
                Ipp32f *pError, // differences
                int numFeat, // point number
                int winSize, // search window size
                int numIter, // iteration number
                float threshold) // threshold
{
   IppiPyramid *pPyr1, *pPyr2;
   IppiOptFlowPyrLK *pOF;
   /*Crea las pirÃ¡mides*/
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
      /*Se colocan las imágenes de cada capa en su lugar y se reserva nueva
      memoria para cada capa*/
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
      /*inicializado de las variables temporales necesarias*/
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

int find_relevant(IppiPoint_32f* prev, int nfeat, char *image, IppiSize imgtam,
                 float max_eig, float min_eig){
   int j, i;
   Ipp32f *eig_val=NULL;
   static Ipp8u* eig_buffer=NULL;
   int eig_val_s;
   int eigen_buff_tam;
   int nfeat_temp=0;

   t_pInteres interesting[NUM_FEAT_MAX];

   if (ippiMinEigenValGetBufferSize_8u32f_C1R(imgtam, 3, 3, &eigen_buff_tam)!=
       ippStsNoErr)
   {
      fprintf (stderr, "Error calculating size of temp space for calculate eigen values\n");
      jdeshutdown(-1);
   }

   eig_buffer=(Ipp8u*)malloc(sizeof (Ipp8u)*eigen_buff_tam);
   eig_val=ippiMalloc_32f_C1 (imgtam.width, imgtam.height, &eig_val_s);
   
   switch (ippiMinEigenVal_8u32f_C1R((Ipp8u*)image, imgtam.width,
           eig_val, imgtam.width*sizeof(Ipp32f),
                                           imgtam, ippKernelScharr, 3, 3, eig_buffer))
   {
      case ippStsNoErr:
         for (j=0;j<imgtam.width;j++){
            for (i=0; i<imgtam.width; i++){
               int val=i+j*imgtam.width;
               Ipp32f valor= eig_val[val];
               //En este punto hay que umbralizar y ordenar
               if (valor>min_eig && valor<max_eig){
                  if (nfeat_temp<nfeat){
                     int index1;
                     int index2;
                     if (nfeat_temp==0){
                        interesting[0].punto.x=i;
                        interesting[0].punto.y=j;
                        interesting[0].valor=valor;
                     }
                     else{
                        for (index1=0; index1<nfeat_temp; index1++){
                           if (interesting[index1].valor<valor){
                              /*Desplazar hacia la derecha*/
                              for (index2=nfeat_temp; index2>index1; index2--){
                                 interesting[index2]=interesting[index2-1];
                              }
                              interesting[index1].punto.x=i;
                              interesting[index1].punto.y=j;
                              interesting[index1].valor=valor;
                              break;
                           }
                        }
                     }
                     nfeat_temp++;
                  }
                  else{
                     if (interesting[nfeat-1].valor<valor){
                        /*Insertar en el lugar adecuado*/
                        int index1;
                        int index2;
                        for (index1=0; index1<nfeat_temp; index1++){
                           if (interesting[index1].valor<valor){
                              /*Desplazar hacia la derecha*/
                              for (index2=nfeat-1; index2>index1; index2--){
                                 interesting[index2]=interesting[index2-1];
                              }
                              interesting[index1].punto.x=i;
                              interesting[index1].punto.y=j;
                              interesting[index1].valor=valor;
                              break;
                           }
                        }
                     }
                  }
               }
            }
         }
         for (i=0;i<nfeat_temp; i++){
            prev[i]=interesting[i].punto;
         }

         break;
      case ippStsNullPtrErr:
         printf ("Puntero nulo.\n");
         if (image==NULL){
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
         break;
      case ippStsNotEvenStepErr:
         printf ("Step no divisible /4.\n");
         break;
      default:
         printf ("Error desconocido.\n");
         break;
   }

   ippiFree(eig_val);
   free(eig_buffer);

   return nfeat_temp;
}

void filter_opflow(char* input , char* output, int width, int height, int show_interesting, int show_flow){
   IppiSize new_size;
   new_size.width=width;
   new_size.height=height;

   //Put image at the output then print the opflow lines
   ippiCopy_8u_C3R((Ipp8u *)input, width*3,
                    (Ipp8u *)output, width*3, new_size);

   if (old_img==NULL){
      //Copy image to de output and to de old buffer
      old_size.width=width;
      old_size.height=height;

      old_img=(char *)malloc (sizeof (char) * width*height);
      new_img=(char *)malloc (sizeof (char) * width*height);
      ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3,
                               (Ipp8u *)old_img, width, new_size);
   }
   else if (new_size.width!=old_size.width || new_size.height!=old_size.height){
      //The size of the old image is different, copy it.
      old_size.width=width;
      old_size.height=height;

      free(old_img);
      free(new_img);
      old_img=(char *)malloc (sizeof (char) * width*height);
      new_img=(char *)malloc (sizeof (char) * width*height);
      ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3,
                               (Ipp8u *)old_img, width, new_size);
   }
   else{
      IppiPoint_32f prev[NUM_FEAT_MAX];
      IppiPoint_32f next[NUM_FEAT_MAX];
      Ipp8s status[NUM_FEAT_MAX];
      Ipp32f error[NUM_FEAT_MAX];
      
      int i;
      int nfeat;

      double threshold=0.3;
      int layers=3;
      int num_iter=12;
      float min_eig=1.0;
      float max_eig=2.0;
      float min_mov=2.0;
      float max_err=20;
      float rate=2.0f;
      static Ipp16s *kernel_16s=NULL;
      static Ipp32f *kernel=NULL;
      float alpha=0.375f;
      int ksize=5, wsize=9;
      
      //Calculate optical flow ussing input and old image.
      ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3,
                               (Ipp8u *)new_img, width, new_size);
      
      //First of all fint the relevant points using eigen values;
      nfeat=find_relevant(prev, NUM_FEAT_MAX, old_img, new_size, max_eig, min_eig);

      //Copy interesting points to next
      memcpy(next, prev, nfeat*sizeof(IppiPoint_32f));

      //Create kernel
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
      
      //Calculate optical flow
      opflow_IPP((Ipp8u *)old_img, new_size.width, (Ipp8u *)new_img, new_size.width, new_size,
                 layers, rate, kernel_16s, ksize, prev, next,
                 status, error, nfeat, wsize, num_iter,
                 threshold);

      for (i=0; i<nfeat; i++){
         int x, y, x2, y2;
         int color;
         double hypotenuse;
         double angle;
         
         x=(int)prev[i].x;
         y=(int)prev[i].y;
         x2=(int)next[i].x;
         y2=(int)next[i].y;
         
         if (show_interesting && x<new_size.width && y<new_size.height && x>=0 && y>=0){
            output[(x+y*new_size.width)*3]=255;
            output[(x+y*new_size.width)*3+1]=0;
            output[(x+y*new_size.width)*3+2]=0;
         }
         if (show_flow && x<new_size.width && y<new_size.height && x>=0 && y>=0 &&
                x2<new_size.width && y2<new_size.height && x2>=0 && y2>=0)
         {
            if (status[i]!=0 || error[i] > max_err)
               continue;

            if (error[i] > (max_err/2) ){
               color= FL_RED;
            }
            else{
               color = FL_GREEN;
            }

            hypotenuse = sqrt( square(y - y2) + square(x - x2) );
            angle = atan2( (double) (y - y2), (double) (x - x2) );

            if (hypotenuse<min_mov)
               continue;

            /*Darle longitud al segmento que se pinta*/
            x2 = (int) (x - 2 * hypotenuse * cos(angle));
            y2 = (int) (y - 2 * hypotenuse * sin(angle));
            /*Dibujar la línea principal de la flecha*/
            lineinimage((Ipp8u *)output, x, y, x2, y2,color, width, height);
            /*Ahora las puntas de las flechas*/
            x = (int) (x2 + (hypotenuse/4) * cos(angle + pi / 4));
            y = (int) (y2 + (hypotenuse/4) * sin(angle + pi / 4));
            lineinimage((Ipp8u *)output, x, y, x2, y2, color, width, height);
            x = (int) (x2 + (hypotenuse/4) * cos(angle - pi / 4));
            y = (int) (y2 + (hypotenuse/4) * sin(angle - pi / 4));
            lineinimage((Ipp8u *)output, x, y, x2, y2, color, width, height);
         }
      }

      //Copy the new grayscale image to the old_buffer
      ippiCopy_8u_C1R((Ipp8u *)new_img, width,
                       (Ipp8u *)old_img, width, new_size);
   }

}
