/*
 *
 *  Copyright (C) 1997-2013 JDE Developers Team
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
 *  Author : Borja Menéndez Moreno <borjamonserrano@gmail.com>
 *
 */

#ifndef NAOMOTIONS_ICE
#define NAOMOTIONS_ICE

#include <jderobot/common.ice>

module jderobot {
    
    enum MotionType { RigthKick, LeftKick, StandupBack, StandupFront, Intercept, ChangeCamera, ResetNaoqi };
    
    /**
     * Interface to the Nao motions
     */
    interface NaoMotions
    {
        int setMotion ( MotionType motion );
    };
};

#endif // NAOMOTIONS_ICE
