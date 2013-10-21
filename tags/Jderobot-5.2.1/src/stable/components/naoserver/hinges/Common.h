/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef COMMON_H
#define COMMON_H

const double pi = 3.1415926535897932384626433832795;

/** constant for a 1 degree*/
const double pi_180 = 3.1415926535897932384626433832795/180;

/** Converts angle from degrees to rad.
 * \param degrees angle coded in degrees
 * \return angle coded in rad
 */
inline double toRadians(double degrees){return degrees * pi_180;}

inline double toDegrees(double angle){return angle * 180 / pi;}

#endif
