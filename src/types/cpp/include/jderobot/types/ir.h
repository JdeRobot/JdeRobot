/*
 *  Copyright (C) 1997-2016 JDE Developers Team
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
 *  Authors :
 *       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
 * Modified by:
 *       Francisco Perez Salgado <f.perez475@gmail.com>
 */

#ifndef JDEROBOTTYPES_IR_H
#define JDEROBOTTYPES_IR_H

namespace JdeRobotTypes {

	class IRData {
	public:
	    int received = 0;
	    double timeStamp = 0; //seconds
	};

} //NS

#endif // JDEROBOTTYPES_SONAR_H
