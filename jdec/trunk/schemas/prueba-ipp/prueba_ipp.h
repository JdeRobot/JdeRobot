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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */
typedef struct{
   float x;
   float y;
}floatPoint;
      
typedef struct{
   int calc;
   unsigned char status;
   float error;
   floatPoint dest;
   float hyp;
   float angle;
}t_opflow;

extern void prueba_ipp_startup(char *configfile);
extern void prueba_ipp_suspend();
extern void prueba_ipp_resume(int father, int *brothers, arbitration fn);
extern void prueba_ipp_guiresume();
extern void prueba_ipp_guisuspend();
extern void prueba_ipp_stop();

extern int prueba_ipp_id; /* schema identifier */
extern int prueba_ipp_cycle; /* ms */
