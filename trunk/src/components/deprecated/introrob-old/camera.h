
/*  
 * Copyright (C) 2008 Roberto Calvo Palomino
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *   
 *   Authors : Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>,
 * 			   Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */
 
#ifndef CAMERA_H_
#define CAMERA_H_

/**
	\class camera
	\brief This class implement a camera instance  
	\autor Roberto Calvo <rocapal@gsyc.es>
	\date  17/05/2008
**/

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>

extern "C"
{
#include "progeo/progeo.h"
}

class camera
{
	public:
		/// \brief Constructor
		camera (char* configFile);
		
		/// \brief Destructor
		~camera();
		
		/// \brief Reaf file config and fill the matrix R and K
		TPinHoleCamera& readConfig();
		
		/// \brief Return a progeo camera with configurated matrix
		TPinHoleCamera& getProgeoCam();
		
		/// \brief Do test of cams
		void test();

	private:
	
		/// \brief Update Matrix
		void updateMatrix();
		
		/// \brief 
		//TPinHoleCamera m_progeoCam;
		TPinHoleCamera cam;
		/// \brief name file
		char* m_nameFile;
		
		/// \brief R Matrix
		gsl_matrix	*m_R; 

		/// \brief aux Matrix
		gsl_matrix	*m_RES; 

		/// \brief RT Matrix
		gsl_matrix	*m_RT;

		/// \brief T Matrix
		gsl_matrix	*m_T;

		/// \brief K Matrix
		gsl_matrix	*m_K;
	
		/// \brief Position vector of cam (x,y,z) 
		gsl_vector *m_pos;
};
#endif /*CAMERA_H_*/
