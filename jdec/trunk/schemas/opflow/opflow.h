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

#ifndef __OPFLOW__
#define __OPFLOW__

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

extern void myschema_startup();
extern void myschema_suspend();
extern void myschema_resume(int father, int *brothers, arbitration fn);
extern void myschema_guiresume();
extern void myschema_guisuspend();

extern int myschema_id; /* schema identifier */
extern int myschema_cycle; /* ms */

/*Own variables*/
extern t_opflow *opflow_img; /*matrix with next points*/
#endif
