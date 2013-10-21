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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>,
 *                Jose María Cañas Plaza <jmplaza@gsyc.es>
 */


#ifndef WIIMOTECLIENT_CONTROL_H
#define WIIMOTECLIENT_CONTROL_H


#include <Ice/Application.h>
#include <jderobot/wiimote.h>
#include <Ice/Application.h>

#include "API.h"

namespace wiimoteClient {
    
    class Control {
    public:
        virtual ~Control();

        //FUNCTIONS
        void updateData(Api *api);

        //ICE INTERFACES
        jderobot::wiiMotePrx wiiprx;

    }; //class    
}//End namespace
#endif //WIIMOTECLIENT_CONTROL_H